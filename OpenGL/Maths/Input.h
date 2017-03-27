#ifndef DEF_INPUT
#define DEF_INPUT


#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#include <Windows.h>
#endif
#include <iostream>
#ifdef NO_GLEW
#include <gl/GL.h>
#include "glext.h"
#else
#include "glew.h"
#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib")
#endif
#endif // !NO_GLEW

#include "freeglut.h"


enum ButtonState {
	PRESSED, CLICKED
};

enum State {
	ENTER_POLYGON, ENTER_WINDOW, DRAW, FILL, COLOR
};

class Scene;

class Input
{

	static Input* currentInstance;
	ButtonState* mouseButtons;
	ButtonState* letters;
	Scene* scene;


	static void keyboardCallBack(unsigned char  touche, int x, int y);
	static void mouseCallBack(int button, int state, int x, int y);

public:
	Input(Scene *scene);
	~Input();
	void init();
	bool isLetterPressed(char letter);
	bool isLetterClicked(char letter);
	bool isMouseButtonPressed(int button);
	bool isMouseButtonClicked(int button);

	void checkKeyboardInputs(unsigned char  touche, int x, int y);
	void checkMouseClicks(int button, int state, int x, int y);
};

#endif




