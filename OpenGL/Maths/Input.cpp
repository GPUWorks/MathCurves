#include "Input.h"
#include "Scene.h"

Input* Input::currentInstance = nullptr;

void Input::keyboardCallBack(unsigned char touche, int x, int y)
{
	Input::currentInstance->checkKeyboardInputs(touche, x, y);
}

void Input::mouseCallBack(int button, int state, int x, int y)
{
	Input::currentInstance->checkMouseClicks(button, state, x, y);
}

void Input::mouseMoveCallBack(int x, int y)
{
	Input::currentInstance->checkMouseMoves(x, y);
}

Input::Input(Scene *s)
{
	Input::currentInstance = this;
	mouseButtons = new ButtonState[3];
	for (int i = 0; i < 3; i++)
		mouseButtons[i] = NONE;
	scene = s;
}


Input::~Input()
{
}

void Input::init()
{
	glutKeyboardFunc(Input::keyboardCallBack);
	glutMouseFunc(Input::mouseCallBack);
}

bool Input::isLetterPressed(char letter)
{
	int nb = letter - (int)'A';
	if (nb<0 || nb>25)
		return false;
	return (letters[nb] == PRESSED);
}

bool Input::isLetterClicked(char letter)
{
	int nb = letter - (int)'A';
	if (nb<0 || nb>25)
		return false;
	if (letters[nb] == CLICKED)
	{
		letters[nb] = NONE;
		return true;
	}
	return false;
}

bool Input::isMouseButtonPressed(int button)
{
	if (button<0 || button>2)
		return false;
	return (mouseButtons[button] == PRESSED);
}

bool Input::isMouseButtonClicked(int button)
{
	if (button<0 || button>2)
		return false;
	if (mouseButtons[button] == CLICKED)
	{
		mouseButtons[button] = NONE;
		return true;
	}
	return false;
}

void Input::checkKeyboardInputs(unsigned char  touche, int x, int y)
{
	std::cout << "Keypressed : " << (char)touche << std::endl;
	switch (touche)
	{
	case 'a':
		std::cout << "change state : ENTER_POINTS" << std::endl;
		scene->changeState(ENTER_POLYGON);
		glutPostRedisplay();
		break;
	case 'q':
		std::cout << "change state : ENTER_POINTS" << std::endl;
		scene->changeState(ENTER_WINDOW);
		glutPostRedisplay();
		break;
	case 'z':
		std::cout << "change state : DRAW" << std::endl;
		scene->changeState(DRAW);
		glutPostRedisplay();
		break;
	case 'd':
		std::cout << "flush polygons" << std::endl;
		scene->flush();
		break;
	case 'f':
		std::cout << "Lecture point intersection" << std::endl;
		scene->changeState(FILL);
		break;
	case 'p':
		scene->setDrawWindow();
		glutPostRedisplay();
		break;
	case 'k':
		std::cout << "Lecture point intersection" << std::endl;
		scene->changeState(COLOR);
		break;
	default:
		break;
	}
}

void Input::checkMouseClicks(int button, int state, int x, int y)
{
	State sceneState = scene->getState();
	if (sceneState == DRAW && button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
			mouseButtons[0] = PRESSED;
		else
			mouseButtons[0] = CLICKED;

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (sceneState == ENTER_POLYGON || sceneState == ENTER_WINDOW)) {
		maths::Point p;
		p.x = x;
		p.y = y;
		scene->addPoint(p);
		//onMouse = 1;
		glutPostRedisplay();
	}

	/*if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && scene->getState() == DRAW)
	{
	maths::Point p;
	p.x = x;
	p.y = y;

	scene->cursorInPolygon(p);
	}*/
}

bool Input::mouseHasMove()
{
	return mouseMove;
}

void Input::SetMouseMove(bool b)
{
	mouseMove = b;
}

float Input::getMouseX()
{
	return mouseX;
}

float Input::getMouseY()
{
	return mouseY;
}


void Input::checkMouseMoves(int x, int y)
{

	scene->changeState(COLOR);

	mouseMove = true;
	mouseX = x;
	mouseY = y;

}