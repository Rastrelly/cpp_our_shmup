#include <iostream>
#include <chrono>
#include <ctime>
#include "ourGraphics.h"
#include "speceffect.h"
#include "flyer.h"
#include "enemySpawner.h"

using namespace std::chrono;

struct orthoSpace
{
	float l, r, t, b;
};

OGLManager * gglm;
glm::vec2 inputDir(0.0f);
bool blockX = false, blockY = false;
unsigned int texExpl;

double rot_speed_coeff = 0.1;

int winx = 800, winy = 600; //starting win size
int viewW = 800, viewH = 600; //viewspace
int gameW = 800, gameH = 600; //workspace


steady_clock::time_point lastUpdate = steady_clock::now();

std::vector<speceffect*> effects;
std::vector<flyer*> flyers;

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

void addSpeceffect()
{
	float ex = (float)(rand() % 801 - 400);
	float ey = (float)(rand() % 601 - 300);
	printf("Adding effect at %f; %f\n",ex,ey);
	effects.push_back(new speceffect(gglm, 2, 0.5, glm::vec3(ex,ey,1.0f), texExpl));
}

float getDeltaTime()
{
	auto now = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
	lastUpdate = now;
	return deltaTime;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	if (key == GLFW_KEY_E && action == GLFW_RELEASE)
		addSpeceffect();

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
		ow = 800 * xc;
	}
	else
	{
		yc = ((float)winy / (float)winx);
		viewW = winx;
		viewH = winx * yc;
		ow = 800.0f;
		oh = 600.0f * yc;
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
	axState = cAxState;
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

	unsigned int temptex = makeTexture("spsheet_plane.png");
	unsigned int enemtex = makeTexture("spsheet_enemy_1.png");
	texExpl = makeTexture("spsheet_expl.png");

	enemySpawner eSpawn(&flyers,&oMan);
	eSpawn.addIndexedTexture(enemtex);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	oMan.addShader("shader_light_vert.gls", "shader_light_frag.gls");

	float timer = 0.0f;
	int cframe = 0;

	flyers.push_back(
		new flyer(0, glm::vec2(0.0f), &oMan, 25.0f, temptex, 3, 3)
	);

	while (!glfwWindowShouldClose(oMan.window))
	{
		float deltaTime = getDeltaTime();
		timer += deltaTime;

		if (timer > 0.1) 
		{
			timer = 0.0f;
			cframe++;
			if (cframe > 8)cframe = 0;
		}

		eSpawn.iterate(deltaTime);

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

		//set lighting uniforms for shader
		oMan.useShader(0);
		oMan.getShader(0)->setBool("useColour", true);
		oMan.getShader(0)->setBool("useTexture", true);
		oMan.getShader(0)->setBool("light.use", false);

		oMan.setDefaultProjections();
		oMan.setProjection(mat_proj);
		oMan.setView(mat_view);
		oMan.updateProjectionForShader(0);
		
		//drawSprites(oMan.getShader(0), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(25.0f), glm::vec3(1.0f), temptex, true, 3, 3, cframe);

		for (int i = 0; i < effects.size(); i++)
		{
			effects[i]->processEffect(deltaTime);
		}


		getInputAxisState(oMan.window,inputDir);
		for (int i = 0; i < flyers.size(); i++)
		{
			flyers[i]->processInternals(deltaTime,inputDir);
		}
		oMan.endDraw();

		collectGarbage();

	}
}