#include <iostream>
#include <chrono>
#include <ctime>
#include "ourGraphics.h"
#include "speceffect.h"

using namespace std::chrono;

OGLManager * gglm;
unsigned int texExpl;

double rot_speed_coeff = 0.1;

int winx = 800, winy = 600;

steady_clock::time_point lastUpdate = steady_clock::now();

std::vector<speceffect*> effects;

void addSpeceffect()
{
	float ex = (float)(rand() % 101 - 50) / 50.0f;
	float ey = (float)(rand() % 101 - 50) / 50.0f;
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

int main()
{

	srand(time(NULL));

	OGLManager oMan(800, 600, framebuffer_size_callback);
	gglm = &oMan;
	glfwSetKeyCallback(oMan.window, key_callback); //keyboard events
	glfwSetMouseButtonCallback(oMan.window, mouse_button_callback); //mouse button events
	glfwSetCursorPosCallback(oMan.window, cursor_position_callback); //mouse cursor pos events
	glfwSetScrollCallback(oMan.window, scroll_callback);  //mouse wheel or touchpad scrolling events

	unsigned int temptex = makeTexture("spsheet_plane.png");
	texExpl = makeTexture("spsheet_expl.png");

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	oMan.addShader("shader_light_vert.gls", "shader_light_frag.gls");

	float timer = 0.0f;
	int cframe = 0;

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

		glfwGetWindowSize(oMan.window, &winx, &winy);

		glm::mat4 mat_persp = glm::perspectiveFov(
			(float)winx / (float)winy,
			(float)winx, (float)winy, 0.01f, 1000.0f);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//set lighting uniforms for shader
		oMan.useShader(0);
		oMan.getShader(0)->setBool("useColour", true);
		oMan.getShader(0)->setBool("useTexture", true);
		oMan.getShader(0)->setBool("light.use", false);

		oMan.setDefaultProjections();
		oMan.updateProjectionForShader(0);
		
		drawSprites(oMan.getShader(0), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(.5f), glm::vec3(1.0f), temptex, true, 3, 3, cframe);

		for (int i = 0; i < effects.size(); i++)
		{
			effects[i]->processEffect(deltaTime);
		}

		oMan.endDraw();

	}
}