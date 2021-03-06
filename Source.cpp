#pragma comment(lib, "lib/glew32s.lib")
#pragma comment(lib, "lib/glfw3.lib")
#pragma comment(lib, "opengl32.lib")

using namespace std;
#include <iostream>

#define GLEW_STATIC
#include "include/GL/glew.h"

#define GLFW_USE_DWM_SWAP_INTERVAL
#include "include/GLFW/glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "include/glm/glm.hpp"
#include "include/glm/gtx/transform.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

#include "Model.h"
#include "Camera.h"
#include "LoadShaders.h"

void Init(void);
void Update(void);
void Draw(void);

#define WIDTH 1920
#define HEIGHT 1080

GLFWmonitor *monitor;
GLFWwindow *window;

GLuint program;
GLuint matrix;

//Camera
glm::vec3 camera_pos = glm::vec3(6.0f, 4.0f, 6.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 2.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

Model iron_man("model/Iron_Man.obj");
Camera camera(camera_pos, camera_target, up);

#pragma region Matrices
glm::mat4 model_matrix = glm::mat4(1.0f);

glm::mat4 view_matrix;

//ProjectionMatrix//
float fov = 45; //field of view
float aspect_ratio = (float)WIDTH / (float)HEIGHT; //aspectRatio
float near = 0.1f; //near
float far = 100; //far

glm::mat4 projection_matrix = glm::perspective(glm::radians(fov), aspect_ratio, near, far);
//----------------//


//MVP//
glm::mat4 mvp_matrix;
//---//
#pragma endregion


int main(void) {

	if (!glfwInit()) {
		return -1;
	}

	monitor = glfwGetPrimaryMonitor();

	window = glfwCreateWindow(WIDTH, HEIGHT, "PROJETO P3D", monitor, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 40);
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		return -1;
	}

	Init();

	do {

		Update();
		Draw();

		glfwSwapBuffers(window);

	}while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glfwTerminate();

	return 0;

}

void Init(void) {

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	
	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "shaders/vertexShader.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/fragmentShader.frag" },
		{ GL_NONE, NULL }
	};
	program = LoadShaders(shaders);
	matrix = glGetUniformLocation(program, "MVP");
	
	iron_man.Init(program);
	camera.Init();

	mvp_matrix = projection_matrix * camera.view_matrix * model_matrix;
}

void Update(void) {
	glfwPollEvents();

	camera.Update(window);

	mvp_matrix = projection_matrix * camera.view_matrix * model_matrix;
}

void Draw(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp_matrix[0][0]);

	iron_man.Draw(program);

}

void Cleanup(void) {

	//Delete the buffers

}