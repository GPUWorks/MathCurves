#ifndef  DEF_SCENE
#define DEF_SCENE

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

#include "CVecteur.h"
#include "../common/EsgiShader.h"
#include "Input.h"

#include "Polygon.h"
#include "Math.h"
#include <map>

class Scene
{
	static Scene* currentInstance;
	maths::Point radiusPoint;
	static void drawCallBack();
	Input *input;
	EsgiShader g_BasicShader;
	int width;
	int height;
	void drawChar(const char c, const maths::Point position, const maths::Color color);
	bool drawWindow;
	void menu(int num);
	int windowId;
	int value;
	int option;
	int mainMenu;
	bool isInPolygon; 
	State state;
	std::vector<maths::Polygon> *polygons; 
	maths::Polygon *window;
	std::vector<std::vector<maths::Point>*> *allIntersection ;
	std::vector<maths::Polygon> *stackPolygonClicked ;

	// TEST ANTHO
	int countPass;
	maths::Polygon savePoly;

public:
	void cut(maths::Polygon pol);
	void cut();
	void flush();
	void lauchOpenGLLoop();
	//static void menuCallBack(int num);
	void initOpenGl(int argc, const char* argv);
	std::vector<maths::Point>* LCARemplissage(maths::Polygon polygon);
	void mainLoop();
	void createMenu();
	maths::Point* isVertexFromPolygon(maths::Polygon polygon, maths::Point pointTest);
	bool isPointInPol(maths::Polygon pol, maths::Point p);
	void cursorInPolygon(maths::Point p);
	maths::Point* ConvertPointPixelToOpenGLUnit(maths::Point point);
	void changeState(State state);
	State getState();
	void addPoint(maths::Point p);
	void setDrawWindow();
	static void menuCallBack(int i);
	maths::Point rotate_point(maths::Polygon poly,float angle,maths::Point p);
	Scene(int w, int h);
	~Scene();
};

#endif // ! SCENE






