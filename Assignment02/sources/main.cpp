// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include Mesh Simplification
#include "meshsimplification.hpp"
#include "mesh.hpp"
#include "modelmanager.hpp"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* g_pWindow;
unsigned int g_nWidth = 1024, g_nHeight = 768;

// Include AntTweakBar
#include <AntTweakBar.h>
TwBar *g_pToolBar;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <controls.hpp>
#include <glerror.hpp>


void TW_CALL applyTransformations(void *flag)
{
	int * applyTransformation = (int *) flag;
	(*applyTransformation) = 1;
}

struct Rotation_Point { float angle, X, Y, Z; };
TwStructMember rotationPointMembers[] = {
		{ "angle", TW_TYPE_FLOAT, offsetof(Rotation_Point, angle), " Min=-180 Max=180 Step=1 " },
		{ "X", TW_TYPE_FLOAT, offsetof(Rotation_Point, X), " Min=-100 Max=+100 Step=0.1 " },
		{ "Y", TW_TYPE_FLOAT, offsetof(Rotation_Point, Y), " Min=-100 Max=200 Step=0.1 " },
		{ "Z", TW_TYPE_FLOAT, offsetof(Rotation_Point, Z), " Min=-100 Max=200 Step=0.1 " }
};

struct Translate { float X, Y, Z; };
TwStructMember translateMembers[] = {
		{ "X", TW_TYPE_FLOAT, offsetof(Translate, X), " Min=-100 Max=+100 Step=0.1 " },
		{ "Y", TW_TYPE_FLOAT, offsetof(Translate, Y), " Min=-100 Max=200 Step=0.1 " },
		{ "Z", TW_TYPE_FLOAT, offsetof(Translate, Z), " Min=-100 Max=200 Step=0.1 " }
};

struct Scale { float X, Y, Z; };
TwStructMember scaleMembers[] = {
		{ "X", TW_TYPE_FLOAT, offsetof(Scale, X), " Min=0 Max=+100 Step=0.1 " },
		{ "Y", TW_TYPE_FLOAT, offsetof(Scale, Y), " Min=0 Max=200 Step=0.1 " },
		{ "Z", TW_TYPE_FLOAT, offsetof(Scale, Z), " Min=0 Max=200 Step=0.1 " }
};

struct Shear { float AXIS, X, Y; };
TwStructMember shearMembers[] = {
		{ "AXIS", TW_TYPE_FLOAT, offsetof(Shear, AXIS), " Min=-100 Max=+100 Step=0.1 " },
		{ "X", TW_TYPE_FLOAT, offsetof(Shear, X), " Min=-100 Max=200 Step=0.1 " },
		{ "Y", TW_TYPE_FLOAT, offsetof(Shear, Y), " Min=-100 Max=200 Step=0.1 " }
};

void WindowSizeCallBack(GLFWwindow *pWindow, int nWidth, int nHeight)
{
	g_nWidth = nWidth;
	g_nHeight = nHeight;
	glViewport(0, 0, g_nWidth, g_nHeight);
	TwWindowSize(g_nWidth, g_nHeight);
}

int main(void)
{
	int nUseMouse = 0;

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	g_pWindow = glfwCreateWindow(g_nWidth, g_nHeight, "Transformations", NULL, NULL);
	if (g_pWindow == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(g_pWindow);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	check_gl_error();//OpenGL error from GLEW

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(g_nWidth, g_nHeight);

	// Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
	glfwSetMouseButtonCallback(g_pWindow, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetCursorPosCallback(g_pWindow, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetScrollCallback(g_pWindow, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetKeyCallback(g_pWindow, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
	glfwSetCharCallback(g_pWindow, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar
	glfwSetWindowSizeCallback(g_pWindow, WindowSizeCallBack);


	bool transformations[] = {0, 0, 0, 0};

	Rotation_Point g_RotatePoint;
	g_RotatePoint.angle = 0.0;
	g_RotatePoint.X = 0.0;
	g_RotatePoint.Y = 0.0;
	g_RotatePoint.Z = 0.0;

	Translate g_TranslationVector;
	g_TranslationVector.X = 0.0;
	g_TranslationVector.Y = 0.0;
	g_TranslationVector.Z = 0.0;

	Scale g_ScaleVector;
	g_ScaleVector.X = 0.0;
	g_ScaleVector.Y = 0.0;
	g_ScaleVector.Z = 0.0;

	Shear g_ShearVector;
	g_ShearVector.AXIS = 0.0;
	g_ShearVector.X = 0.0;
	g_ShearVector.Y = 0.0;


	//Separator.
	TwAddSeparator(g_pToolBar, NULL, NULL);


	//create the toolbar
	g_pToolBar = TwNewBar("Graphic ToolBar");

	// Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
	double speed = 0.0;
	TwAddVarRW(g_pToolBar, "speed", TW_TYPE_DOUBLE, &speed, " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");

	unsigned int model = 0;
	TwAddVarRW(g_pToolBar, "model", TW_TYPE_UINT32, &model, " label='Selected Model'");

	// Add 'bgColor' to 'bar': it is a modifable variable of type TW_TYPE_COLOR3F (3 floats color)
	vec3 oColor(0.0f);
	TwAddVarRW(g_pToolBar, "bgColor", TW_TYPE_COLOR3F, &oColor[0], " label='Background color' ");

	TwType rotationPointType = TwDefineStruct("ROTATION_POINT", rotationPointMembers, 4, sizeof(Rotation_Point), NULL, NULL);
	TwAddVarRW(g_pToolBar, "boolTransformationsRotationPoint", TW_TYPE_BOOLCPP, &transformations[1], " Group='RotationPoint' Label='Use Rotation on Point?' ");
	TwAddVarRW(g_pToolBar, "ROTATION_POINT", rotationPointType, &g_RotatePoint, " Group='RotationPoint' Label='Rotate angle, Point' ");

	TwAddSeparator(g_pToolBar, NULL, NULL);

	TwType scaleType = TwDefineStruct("SCALE", scaleMembers, 3, sizeof(Scale), NULL, NULL);
	TwAddVarRW(g_pToolBar, "boolTransformationsScale", TW_TYPE_BOOLCPP, &transformations[2], " Group='Scale' Label='Use Scale?' ");
	TwAddVarRW(g_pToolBar, "SCALE", scaleType, &g_ScaleVector, " Group='Scale' Label='Scale vector' ");

	TwAddSeparator(g_pToolBar, NULL, NULL);

	TwType shearType = TwDefineStruct("SHEAR", shearMembers, 3, sizeof(Shear), NULL, NULL);
	TwAddVarRW(g_pToolBar, "boolTransformationShear", TW_TYPE_BOOLCPP, &transformations[3], " Group='Shear' Label='Use Shear?' ");
	TwAddVarRW(g_pToolBar, "SHEAR", shearType, &g_ShearVector, " Group='Shear' Label='Shear Axis, Point' ");

	TwAddSeparator(g_pToolBar, NULL, NULL);

	TwType translateType = TwDefineStruct("TRANSLATE", translateMembers, 3, sizeof(Translate), NULL, NULL);
	TwAddVarRW(g_pToolBar, "boolTransformationsTranslate", TW_TYPE_BOOLCPP, &transformations[0], " Group='Translate' Label='Use Translation?' ");
	TwAddVarRW(g_pToolBar, "TRANSLATE", translateType, &g_TranslationVector, " Group='Translate' Label='Translate vector, coordinates' ");

	TwAddSeparator(g_pToolBar, NULL, NULL);

	double time = 0.0;
	TwAddVarRW(g_pToolBar, "Time Animating", TW_TYPE_DOUBLE, &time, " label='Animation Time' min=0 max=20 step=1' ");


	int applyTransformation;
	TwAddButton(g_pToolBar, "ApplyTransformations", applyTransformations, &applyTransformation, "Label='Apply the checked transformations'");

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(g_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(g_pWindow, g_nWidth / 2, g_nHeight / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);


	modelManager manager("shaders/StandardShading.vertexshader",
	                     "shaders/StandardShading.fragmentshader",
	                     "LightPosition_worldspace");

	///MeshSimplification MS;
	///Create and compile our GLSL program from the shaders


	// Read our .obj file
	manager.loadMesh("mesh/suzanne.obj");
	manager.createModel(manager.getMeshes()->front(), "mesh/uvmap.DDS", "myTextureSampler");

	glm::mat4 transformationMatrix = glm::mat4(1.0);

// Load it into a VBO

	glUseProgram(manager.getProgramID());


	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames    = 0;

	glDisable(GL_CULL_FACE);

	do {
		check_gl_error();


		if (applyTransformation)
		{
			applyTransformation = 0;
			if (transformations[0])
			{
				transformationMatrix = glm::translate(transformationMatrix, glm::vec3(g_TranslationVector.X, g_TranslationVector.Y, g_TranslationVector.Z));
			}
			if (transformations[1])
			{
				transformationMatrix = glm::rotate(transformationMatrix, g_RotatePoint.angle, glm::vec3(g_RotatePoint.X, g_RotatePoint.Y, g_RotatePoint.Z));
			}
			if (transformations[2])
			{
				transformationMatrix = glm::scale(transformationMatrix, glm::vec3(g_ScaleVector.X, g_ScaleVector.Y, g_ScaleVector.Z));
			}
			if (transformations[3]) {
				switch((int)g_ShearVector.AXIS) {
					case 0: //XY
						transformationMatrix *= glm::mat4(
								1, 0, 0, 0,
								0, 1, 0, 0,
								g_ShearVector.X, g_ShearVector.Y, 1, 0,
								0, 0, 0, 1
						);
						break;
					case 1: //XZ
						transformationMatrix *= glm::mat4(
								1, 0, 0, 0,
								g_ShearVector.X, 1, g_ShearVector.Y, 0,
								0, 0, 1, 0,
								0, 0, 0, 1
						);
						break;
					case 2: //YZ
						transformationMatrix *= glm::mat4(
								1, g_ShearVector.X, g_ShearVector.Y, 0,
								0, 1, 0, 0,
								0, 0, 0, 1,
								0, 0, 0, 1
						);
						break;
					default:
						break;
				}
			}
			if(transformationMatrix != glm::mat4(1.0))
			{
				manager.queueTransformation(model, transformationMatrix);
				transformationMatrix = glm::mat4(1.0);
			}
		}

		//use the control key to free the mouse
		if (glfwGetKey(g_pWindow, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS)
			nUseMouse = 0;
		else
			nUseMouse = 1;

		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames  = 0;
			lastTime += 1.0;
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(manager.getProgramID());




		// Compute the MVP matrix from keyboard and mouse input

		computeMatricesFromInputs(nUseMouse, g_nWidth, g_nHeight);

		manager.setLightPosition(glm::vec3(4, 4, 4));


		manager.draw();

		// Draw tweak bars
		TwDraw();

		// Swap buffers
		glfwSwapBuffers(g_pWindow);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(g_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	       glfwWindowShouldClose(g_pWindow) == 0);

	// Cleanup VBO and shader


	// Terminate AntTweakBar and GLFW
	TwTerminate();
	glfwTerminate();

	return 0;
}