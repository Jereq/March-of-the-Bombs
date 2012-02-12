#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdio.h>
#include <string>
using std::string;

#include "Camera.h"
#include "AttachmentPoint.h"

#include "Graphics.h"

#include "GUIMain.h"

string windowTitle = "March of the Bombs";
float fpsCap = 60;

bool gamePaused;

//bool relativeMouseCoordinates;
int previousMouseX;
int previousMouseY;

mat4 projectionMatrix;

Camera* camera;

Graphics::ptr graphics;

GUIMain gui;

void displayFunc()
{
	camera->updateViewMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gui.draw(graphics);

	glutSwapBuffers();

	return;
}

void reshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);

	float aspect = static_cast<float>(width) / static_cast<float>(height);
	camera->updateProjectionMatrix(aspect);

	return;
}

void update(float deltaTime)
{
	gui.update();
}

void rotatePlayer(int deltaX, int deltaY)
{
	//vec3 playerRotation = player->getRotation();

	//const static float MOUSE_SESITIVITY = -0.8f;

	//float rotationY = playerRotation.y + deltaX * MOUSE_SESITIVITY;	// x motion rotates around the y axis
	//float rotationX = playerRotation.x + deltaY * MOUSE_SESITIVITY;	// y motion rotates around the x axis

	//// Limit rotation up/down to between straight up and straight down
	//if (rotationX > 90.f)
	//	rotationX = 90.f;
	//else if (rotationX < -90.f)
	//	rotationX = -90.f;

	//playerRotation.y = rotationY;
	//playerRotation.x = rotationX;

	//player->setRotation(playerRotation);
}

void mouseMotionFunc(int x, int y)
{
	int deltaX = x - previousMouseX;
	int deltaY = y - previousMouseY;

	previousMouseX = x;
	previousMouseY = y;

	//if (!gamePaused)
	//{
	//	rotatePlayer(deltaX, deltaY);
	//}
}

//void setCursorVisible(bool cursorVisible)
//{
//	if (cursorVisible)
//	{
//		glutSetCursor(GLUT_CURSOR_INHERIT);
//	}
//	else
//	{
//		glutSetCursor(GLUT_CURSOR_NONE);
//	}
//}

//void setCursorLocked(bool cursorLocked)
//{
//	relativeMouseCoordinates = cursorLocked;
//}

//void centerMouseCursor()
//{
//	int windowCenterX = glutGet(GLUT_WINDOW_WIDTH) / 2;
//	int windowCenterY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
//	
//	glutWarpPointer(windowCenterX, windowCenterY);
//
//	previousMouseX = windowCenterX;
//	previousMouseY = windowCenterY;
//}

int validUpdate = 0;	// Increase to invalidate all queued timer updates
int previousRunTimeMS = 0;
void updateFunc(int value)
{
	if (value != validUpdate)
		return;

	int runTimeMS = glutGet(GLUT_ELAPSED_TIME);
	int deltaTimeMS = runTimeMS - previousRunTimeMS;
	float deltaTime = deltaTimeMS / 1000.f;
	previousRunTimeMS = runTimeMS;

	update(deltaTime);

	glutPostRedisplay();

	int updateTimeMS = glutGet(GLUT_ELAPSED_TIME) - runTimeMS;
	int delayMS = static_cast<int>(1000.f / fpsCap);

	int waitTimeMS;
	if (updateTimeMS > delayMS)
	{
		// TODO: signal slow program execution
		waitTimeMS = 0;
	}
	else
	{
		waitTimeMS = delayMS - updateTimeMS;
	}

	//if (relativeMouseCoordinates)
	//{
	//	centerMouseCursor();
	//}

	glutTimerFunc(waitTimeMS, updateFunc, validUpdate);

	return;
}

void setGamePaused(bool pause)
{
	gamePaused = pause;

	if (pause)
	{
		//setCursorVisible(true);
		//setCursorLocked(false);

		validUpdate++;
	}
	else
	{
		//setCursorVisible(false);
		//setCursorLocked(true);

		//centerMouseCursor();

		previousRunTimeMS = glutGet(GLUT_ELAPSED_TIME);
		glutTimerFunc(0, updateFunc, validUpdate);
	}
}

const static char ESC = 0x1B;
const static char W = 'w';
const static char A = 'a';
const static char S = 's';
const static char D = 'd';

const static vec3 FORWARD	= vec3(0, 0, -1);
const static vec3 BACKWARD	= vec3(0, 0, 1);
const static vec3 LEFT		= vec3(-1, 0, 0);
const static vec3 RIGHT		= vec3(1, 0, 0);

//void alterPlayerDirection(vec3 direction)
//{
//	vec3 newDirection = player->getDirection() + direction;
//	player->setDirection(newDirection);
//}

void keyDownFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ESC:
		setGamePaused(!gamePaused);
		break;

	//case W:
	//	alterPlayerDirection(FORWARD);
	//	break;

	//case S:
	//	alterPlayerDirection(BACKWARD);
	//	break;

	//case A:
	//	alterPlayerDirection(LEFT);
	//	break;

	//case D:
	//	alterPlayerDirection(RIGHT);
	//	break;
	}
}

void keyUpFunc(unsigned char key, int x, int y)
{
	//switch (key)
	//{
	//case W:
	//	alterPlayerDirection(-FORWARD);
	//	break;

	//case S:
	//	alterPlayerDirection(-BACKWARD);
	//	break;

	//case A:
	//	alterPlayerDirection(-LEFT);
	//	break;

	//case D:
	//	alterPlayerDirection(-RIGHT);
	//	break;
	//}
}

void initOpenGL(int argc, char** argv)
{
	glutInit(&argc, argv);

	int windowWidth = 800;
	int windowHeight = 600;
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow(windowTitle.c_str());

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutMotionFunc(mouseMotionFunc);
	glutPassiveMotionFunc(mouseMotionFunc);
	glutKeyboardFunc(keyDownFunc);
	glutKeyboardUpFunc(keyUpFunc);

	glutIgnoreKeyRepeat(true);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("OpenGL on %s %s\n", vendor, renderer);
	printf("OpenGL version supported: %s\n", version);
	printf("OpenGL version supported: %i.%i\n", major, minor);
	printf("GLSL version supported: %s\n", glslVersion);

	printf("Will now set GL version to: %i.%i\n", major, minor);
	glutInitContextVersion(major, minor);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing GLEW: %s\n",
			glewGetErrorString(err));
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

int main(int argc, char** argv)
{
	initOpenGL(argc, argv);

	setGamePaused(false);

	//terrain = new Terrain();
	//terrain->loadHeightMap("Images/HelloWorld.tga");
	//terrain->loadBlendMap("Images/BlendMap.tga");
	//terrain->loadBlendTexture("Images/skull.tga", 0);
	//terrain->loadBlendTexture("Images/Blend0.tga", 1);
	//terrain->loadBlendTexture("Images/Blend1.tga", 2);
	
	//vec3 scale = vec3(2000, 100, 2000);

	//vec3 location = -scale / 2.f;
	//location.y = 0;

	//terrain->setScale(scale);
	//terrain->setLocation(location);

	//player = new Player(vec3(), vec3(), 1.7f);
	//player->setTerrain(terrain);

	//camera = new Camera(&player->getEye());
	AttachmentPoint attPoint(vec3(0, 5, -15), vec3());
	camera = new Camera(&attPoint);

	//ModelManager modelManager;
	//model = modelManager.loadFromObj("Objects/bth_fix.obj");
	//model->setPosition(vec3(0.f, 5.f, -10.f));
	//model->setScale(vec3(5/100.f));

	//PointLight pointLight = PointLight(vec4(0.f, 20.f, 20.f, 1.f), vec3(1.f));
	//model->setPointLight(&pointLight);

	graphics = Graphics::ptr(new Graphics());

	glutMainLoop();

	delete camera;
	//delete player;
	//delete terrain;

	return EXIT_SUCCESS;
}