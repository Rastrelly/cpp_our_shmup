#include <iostream>
#include <chrono>
#include <ctime>
#include "flyer.h"
#include "enemySpawner.h"
#include "speceffect.h"

using namespace std::chrono;

bool drawOnlyText=false;

struct orthoSpace
{
	float l, r, t, b;
};

int score;

OGLManager * gglm;
glm::vec2 inputDir(0.0f);
glm::vec2 playerPosition;
bool blockX = false, blockY = false;

bool usrWannaShoot = false;

unsigned int texExpl = 0;
unsigned int temptex = 0;
unsigned int fontTexture = 0;

double rot_speed_coeff = 0.1;

int winx = 800, winy = 600; //starting win size
int viewW = 800, viewH = 600; //viewspace
int gameW = 800, gameH = 600; //workspace

bool pause = false;

bool ppress = false;
bool rpress = false;


steady_clock::time_point lastUpdate = steady_clock::now();

std::vector<speceffect*> effects;
std::vector<flyer*> flyers;

void printText(float x, float y, float size, string text, bool useTTF, FontWorker * fw)
{
	if (!useTTF)
	{
		printBitmapText(gglm->getShader(1), x, y, size, text, fontTexture);
	}
	else
	{
		fw->RenderText(gglm->getShader(1), text, x, y, size / 40.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}
}

void collectGarbage()
{
	//collect grabage on effects
	int l = effects.size();
	int i = 0;
	while(i < effects.size())
	{
		if (effects[i]->life <= 0)
		{
			delete(effects[i]);
			effects.erase(effects.begin()+i);
		}
		else i++;
	}

	//collect grabage on flyers
	l = flyers.size();
	i = 0;
	while (i < flyers.size())
	{
		if (flyers[i]->getDed())
		{
			delete(flyers[i]);
			flyers.erase(flyers.begin() + i);
			std::cout << "Flyer garbage entity collected!\n";
		}
		else i++;
	}
}

void addSpeceffect(glm::vec2 efpos)
{
	printf("Adding effect at %f; %f\n", efpos.x, efpos.y);
	effects.push_back(new speceffect(gglm, 2, 0.5, glm::vec3(efpos.x, efpos.y,1.0f), texExpl));
}

float getDeltaTime()
{
	auto now = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
	lastUpdate = now;
	return deltaTime;
}

void restartGame(OGLManager * oglMan)
{
	score = 0;
	for (int i = 0; i < flyers.size(); i++) delete(flyers[i]);
	flyers.clear();
	flyers.push_back(
		new flyer(0, glm::vec2(0.0f), glm::vec2(0.0f), oglMan, 25.0f, temptex, 3, 3)
	);
}

void pauseGame(OGLManager * oglMan)
{
	if (!pause)
		pause = true;
	else
		pause = false;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	if (key == GLFW_KEY_E && action == GLFW_RELEASE)
		addSpeceffect(glm::vec2((float)(rand() % 801 - 400), (float)(rand() % 601 - 300)));

	if (key == GLFW_KEY_R && action == GLFW_RELEASE)
	{
		restartGame(gglm);
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
	{
		pauseGame(gglm);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void window_callback(GLFWwindow* window, int width, int height)
{
	winx = width;
	winy = height;
}

void calcOrthoSizes(orthoSpace &osRes)
{
	float xc = 4.0f;
	float yc = 3.0f;

	float ow=1.0f;
	float oh=1.0f;

	orthoSpace os = {-1.0f, 1.0f, -1.0f, 1.0f};

	if (winx > winy)
	{
		xc = ((float)winx / (float)winy);
		viewH = winy;
		viewW = viewH * xc;
		oh = 600.0f;
		ow = oh * xc;
	}
	else
	{
		yc = ((float)winy / (float)winx);
		viewW = winx;
		viewH = winx * yc;
		ow = 800.0f;
		oh = ow * yc;
	}

	os.l = -ow * 0.5f;
	os.r = ow * 0.5f;
	os.b = -oh * 0.5f;
	os.t = oh * 0.5f;

	osRes = os;
}

void getInputAxisState(GLFWwindow *wnd, glm::vec2 &axState)
{
	glm::vec2 cAxState(0.0f);
	usrWannaShoot = false;

	//for gamepad
	int aj = GLFW_JOYSTICK_1;
	for (int i = 0; i < 12; i++)
	{
		if (glfwJoystickIsGamepad(i))
		{
			aj = i; break;
		}
	}

	std::cout << "Active joystick: " << aj << std::endl;

	if (glfwJoystickIsGamepad(aj))
	{
		GLFWgamepadstate state;
		if (glfwGetGamepadState(aj, &state))
		{
			if (state.buttons[GLFW_GAMEPAD_BUTTON_A]==GLFW_PRESS)
			{
				usrWannaShoot = true;
			}
			if (state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_RELEASE)
			{
				usrWannaShoot = false;
			}

			if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK]==GLFW_PRESS)
			{
				if (!rpress)
				{
					rpress = true;
					restartGame(gglm);
				}
			}

			if (state.buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_RELEASE)
			{
				rpress = false;
			}

			if (state.buttons[GLFW_GAMEPAD_BUTTON_START]==GLFW_PRESS)
			{
				if (!ppress)
				{
					pauseGame(gglm);
					ppress = true;
				}
			}

			if (state.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_RELEASE)
			{
				ppress = false;
			}
		}

		cAxState.x = abs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]) > 0.05 ? state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] : 0.0f;
		cAxState.y = abs(state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]) > 0.05 ? -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] : 0.0f;
		//std::printf("GAMEPAD AX L: %f ; %f\n", cAxState.x, cAxState.y);
	}

	//for keyboard
	int state = glfwGetKey(wnd, GLFW_KEY_UP);
	if (state == GLFW_PRESS)
	{
		cAxState.y = 1.0f;
	}
	state = glfwGetKey(wnd, GLFW_KEY_DOWN);
	if (state == GLFW_PRESS)
	{
		cAxState.y = -1.0f;
	}
	state = glfwGetKey(wnd, GLFW_KEY_LEFT);
	if (state == GLFW_PRESS)
	{
		cAxState.x = -1.0f;
	}
	state = glfwGetKey(wnd, GLFW_KEY_RIGHT);
	if (state == GLFW_PRESS)
	{
		cAxState.x = 1.0f;
	}
	state = glfwGetKey(wnd, GLFW_KEY_LEFT_CONTROL);
	if (state == GLFW_PRESS)
	{
		usrWannaShoot = true;
	}	

	axState = cAxState;
}

void runTimer(float &timer, float dt, float timerLimit)
{
	timer += dt;

	if (timer > timerLimit)
	{
		timer = 0.0f;
	}
}


int main()
{

	srand(time(NULL));

	OGLManager oMan(800, 600, framebuffer_size_callback);
	gglm = &oMan;
	glfwSetKeyCallback(oMan.window, key_callback); //keyboard events
	glfwSetMouseButtonCallback(oMan.window, mouse_button_callback); //mouse button events
	glfwSetCursorPosCallback(oMan.window, cursor_position_callback); //mouse cursor pos events
	glfwSetScrollCallback(oMan.window, scroll_callback);  //mouse wheel or touchpad scrolling events
	glfwSetWindowSizeCallback(oMan.window, window_callback);

    temptex = makeTexture("spsheet_plane.png");
	unsigned int enemtex = makeTexture("spsheet_enemy_1.png");
	unsigned int enemtex2 = makeTexture("spsheet_enemy_2.png");
	unsigned int enemtex3 = makeTexture("spsheet_enemy_3.png");
	unsigned int boss1tex = makeTexture("spsheet_boss_1.png");
	unsigned int pup1 = makeTexture("spsheet_heal.png");
	unsigned int pup2 = makeTexture("spsheet_gun.png");
	unsigned int bulletTex = makeTexture("bullet01.png");
	unsigned int bulletTex2 = makeTexture("bullet02.png");
	unsigned int uihp = makeTexture("hp_mark.png");
	texExpl = makeTexture("spsheet_expl.png");
	fontTexture = makeTexture("font_map.png");

	enemySpawner eSpawn(&flyers,&oMan,&score);
	eSpawn.addIndexedTexture(enemtex);
	eSpawn.addIndexedTexture(bulletTex);
	eSpawn.addIndexedTexture(bulletTex2);
	eSpawn.addIndexedTexture(enemtex2);
	eSpawn.addIndexedTexture(pup1);
	eSpawn.addIndexedTexture(boss1tex);
	eSpawn.addIndexedTexture(enemtex3);
	eSpawn.addIndexedTexture(pup2);
	
	eSpawn.addGroundTexture(makeTexture("ground1.png"));
	eSpawn.addGroundTexture(makeTexture("ground2.png"));
	eSpawn.addGroundTexture(makeTexture("ground3.png"));

	FontWorker fWork("C:\\Windows\\Fonts\\arial.ttf");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	oMan.addShader("shader_light_vert.gls", "shader_light_frag.gls");
	oMan.addShader("shader_ttf_font_vert.gls", "shader_ttf_font_frag.gls");

	float timer = 0.0f;
	int cframe = 0;

	restartGame(&oMan);

	float terra_shift = 0.0f;
	float terra_timer = 0.0f;

	while (!glfwWindowShouldClose(oMan.window))
	{
		float deltaTime = getDeltaTime();

		if (pause) deltaTime = 0.0f;

		glfwGetWindowSize(oMan.window, &winx, &winy);

		//ortho matrix

		orthoSpace ourOrthoSpace;
		calcOrthoSizes(ourOrthoSpace);

		glm::mat4 mat_proj = glm::ortho(
			ourOrthoSpace.l, ourOrthoSpace.r,
			ourOrthoSpace.b, ourOrthoSpace.t,
			-100.0f,100.0f);

		glm::mat4 mat_view = glm::mat4(1.0);

		//view matrix
		
		glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//set lighting uniforms for shader 0
		if (!drawOnlyText)
		{
			oMan.useShader(0);
			oMan.getShader(0)->setBool("useColour", true);
			oMan.getShader(0)->setBool("useTexture", true);
			oMan.getShader(0)->setBool("light.use", false);

			oMan.setDefaultProjections();
			oMan.setProjection(mat_proj);
			oMan.setView(mat_view);
			oMan.updateProjectionForShader(0);

			
			//draw terrain
			if (!pause)
			runTimer(terra_timer, deltaTime, 10.0f);
			terra_shift = terra_timer * 80.0f;
			drawSprites(oMan.getShader(0), glm::vec3(0.0f, 300.0f - (terra_shift - 800.0f), -1.0f), glm::vec3(400.0f), glm::vec3(1.0f), eSpawn.groundTextures[eSpawn.cGroundTex], true, 1, 1, 0);
			drawSprites(oMan.getShader(0), glm::vec3(0.0f, 300.0f - terra_shift, -1.0f), glm::vec3(400.0f), glm::vec3(1.0f), eSpawn.groundTextures[eSpawn.cGroundTex], true, 1, 1, 0);
			drawSprites(oMan.getShader(0), glm::vec3(0.0f, 300.0f - (terra_shift + 800.0f), -1.0f), glm::vec3(400.0f), glm::vec3(1.0f), eSpawn.groundTextures[eSpawn.cGroundTex], true, 1, 1, 0);

			//process flyers
			if (!pause)
			eSpawn.iterate(deltaTime);					//run spawner
			getInputAxisState(oMan.window, inputDir);    //get current user axis and button inputs
			playerPosition = flyers[0]->getPos();       //extract player position 
			flyers[0]->setUserShootCall(usrWannaShoot); //transfer user's will to use violence

			if (!pause)
			for (int i = 0; i < flyers.size(); i++)
			{
				flyers[i]->setPlrPos(playerPosition); //transfer player position
				flyers[i]->processInternals(deltaTime, inputDir); //work on internal logic
			}

			if (!pause)
			{
				eSpawn.runFiring();
				eSpawn.checkBulletCollisions();
			}

			//process speceffects		

			//spawn explosions
			for (int i = 0; i < flyers.size(); i++)
				if (flyers[i]->getDed() && flyers[i]->getNeedExplode())
				{
					if (i != 0) score+= flyers[i]->getScoreValue();
					addSpeceffect(flyers[i]->getPos());
				}

			for (int i = 0; i < effects.size(); i++)
			{
				effects[i]->processEffect(deltaTime);
			}
		}

		//draw hp
		if (flyers.size() > 0 && flyers[0] != NULL)
			if (flyers[0]->getTeam() == 0)
			{
				int plHp = flyers[0]->getHp();
				for (int i = 0; i < plHp; i++)
				{
					float hpX = 380.0f - ((plHp-i-1) * 32);
					float hpY = 280.0f;
					drawPlane(oMan.getShader(0), glm::vec3(hpX, hpY, 0.4f), glm::vec3(16.0f), glm::vec3(1.0f), uihp, true);
				}
			}


		//set uniforms for shader 0
		oMan.useShader(1);

		oMan.setDefaultProjections();
		oMan.setProjection(mat_proj);
		oMan.updateProjectionForShader(1);

		//print score
		printText(-380.0f, 265.0f, 20.0f, to_string(score).c_str(), true, &fWork);
		
		if (pause) printText(-100.0f, 0.0f, 20.0f, "PAUSE", true, &fWork);
		
		//render stuff
		oMan.endDraw();

		//grabage collection
		// (remove elements that are no longer active)
		collectGarbage();
	}
}