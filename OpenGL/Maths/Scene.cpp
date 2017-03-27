#include "Scene.h"

using namespace maths;

Scene* Scene::currentInstance = nullptr;

int colors[9] = {
	255, 0, 0,
	0, 255, 0,
	0, 0, 255 
};

void Scene::drawCallBack()
{
	Scene::currentInstance->mainLoop();
}

void Scene::flush()
{
	if (state != ENTER_POLYGON)
	{
		while (!polygons->empty())
		{
			polygons->pop_back();
		}
		glutPostRedisplay();
	}
}

void Scene::moveSelectedPoint(float x, float y)
{
	if (hasSelectedPoint())
	{
		Point p;
		p.x = x;
		p.y = y;
		polygons->at(polygonSelected).setPoint(p, pointSelected);
		glutPostRedisplay();
	}
}

bool Scene::hasSelectedPoint()
{
	return (polygonSelected != -1 && pointSelected != -1);
}

void Scene::lauchOpenGLLoop()
{
	glutMainLoop();
}

void Scene::initOpenGl(int argc, const char* argv)
{
	glutInit(&argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(width, height);
	windowId = glutCreateWindow("Maths");

	createMenu();

#ifndef NO_GLEW
	glewInit();
#endif

	g_BasicShader.LoadVertexShader("basic.vs");
	g_BasicShader.LoadFragmentShader("basic.fs");
	g_BasicShader.CreateProgram();

	glutDisplayFunc(Scene::drawCallBack);

	//glEnable(GL_CULL_FACE);

	input->init();
	
}

void Scene::createMenu()
{

	// ATTENDS JE SUIS AU TEL AVEC UN COLLEGUE
	mainMenu = glutCreateMenu(Scene::menuCallBack);

	glutAddMenuEntry("Exit", 0);
	glutAddMenuEntry("Add curve         A", 1);
	glutAddMenuEntry("End edition       Z", 2);
	/*glutAddMenuEntry("Cut               C", 3);
	glutAddMenuEntry("Fill polygon(s)   F", 4);
	glutAddMenuEntry("Set window        Q", 5);
	glutAddMenuEntry("Select polygon(s) W", 6);
	glutAddMenuEntry("Hide/Show window  P", 7);*/
	/*
	if (stackPolygonClicked->size() != 0)
	{
	glutAddMenuEntry("Fill           F", 4);
	}
	*/
	/*if (isInPolygon)
	{
		glutAddMenuEntry("Coloring polygon", 5);
	}*/

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// On traite ici le choix de l'utilisateur dans le menu contextuel
void Scene::menu(int num) {
	switch (num)
	{
	case 0:
		glutDestroyWindow(windowId);
		exit(0);
		break;
	case 1:
		input->checkKeyboardInputs('a', 0, 0);
		break;
	case 2:
		input->checkKeyboardInputs('z', 0, 0);
		break;
	case 3:
		input->checkKeyboardInputs('c', 0, 0);
		break;
	case 4:
		input->checkKeyboardInputs('f', 0, 0);
		break;
	case 5:
		input->checkKeyboardInputs('q', 0, 0);
		break;
	case 6:
		input->checkKeyboardInputs('w', 0, 0);
		break;
	case 7:
		input->checkKeyboardInputs('p', 0, 0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}


bool Scene::isPointSelected(float mX, float mY)
{
	if (state == DRAW)
	{

		float nb = 10;
		float nbX = nb /width;
		float nbY = nb / height;

		std::cout << "mx = " << mX <<"  mY=" << mY << std::endl;
		for (int i = 0; i < polygons->size(); i++)
		{
			for (int j = 0; j < polygons->at(i).getPoints()->size(); j++)
			{
				Point p = polygons->at(i).getPoints()->at(j);
				
				std::cout << "x=" << p.x <<"   y=" << p.y << std::endl;
				if (mX > p.x - nbX && mX<p.x + nbX && mY>p.y - nbY && mY < p.y + nbY)
				{
					pointSelected = j;
					polygonSelected = i;
					return true;
				}
			}
		}
	}
	pointSelected = -1;
	polygonSelected = -1;

	return false;
}


void Scene::unselectPoint()
{
	pointSelected = -1;
	polygonSelected = -1;
}

float Scene::getWidth()
{
	return width;
}

float Scene::getHeight()
{
	return height;
}

void Scene::mainLoop()
{
	glViewport(0, 0, width, height);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	
	auto program = g_BasicShader.GetProgram();
	glUseProgram(program);

	auto color_position = glGetAttribLocation(program, "a_Color");
	auto position_location = glGetAttribLocation(program, "a_Position");

	if (state == DRAW)
	{

		if (!polygons->empty())
		{
			for (int i = 0; i < polygons->size(); i++)
			{
				polygons->at(i).recalculateBezierPoints(10);

				const maths::Point *bezierPoints = polygons->at(i).getBezierPoints()->data();
				unsigned int bezierSize = polygons->at(i).getBezierPoints()->size();

				glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, bezierPoints);
				glEnableVertexAttribArray(position_location);

				glPointSize(5);

				glDrawArrays(GL_LINE_STRIP, 0, bezierSize);
				glDisableVertexAttribArray(position_location);
				glDisableVertexAttribArray(color_position);

				const maths::Point *points = polygons->at(i).getPoints()->data();
				unsigned int size = polygons->at(i).getPoints()->size();

				glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, points);
				glEnableVertexAttribArray(position_location);

				glPointSize(5);

				glDrawArrays(GL_POINTS, 0, size);
				glDisableVertexAttribArray(position_location);
				glDisableVertexAttribArray(color_position);
			}
		}

	}
	
	else if (state == ENTER_POLYGON)
	{

		for (int i = 0; i < polygons->size() - 1; i++)
		{
			polygons->at(i).recalculateBezierPoints(10);

			const maths::Point *bezierPoints = polygons->at(i).getBezierPoints()->data();
			unsigned int bezierSize = polygons->at(i).getBezierPoints()->size();

			glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, bezierPoints);
			glEnableVertexAttribArray(position_location);

			glPointSize(5);

			glDrawArrays(GL_LINE_STRIP, 0, bezierSize);
			glDisableVertexAttribArray(position_location);
			glDisableVertexAttribArray(color_position);

			const maths::Point *points = polygons->at(i).getPoints()->data();
			unsigned int size = polygons->at(i).getPoints()->size();

			glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, points);
			glEnableVertexAttribArray(position_location);

			glPointSize(5);

			glDrawArrays(GL_POINTS, 0, size);
			glDisableVertexAttribArray(position_location);
			glDisableVertexAttribArray(color_position);
		}

		const maths::Point *points = polygons->back().getPoints()->data();
		unsigned int size = polygons->back().getPoints()->size();

		for (int i = 0; i < size; i++)
		{
			glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 0, &points[i]);
			glEnableVertexAttribArray(position_location);

			glPointSize(10);

			glDrawArrays(GL_POINTS, 0, 1);
			glDisableVertexAttribArray(position_location);
			glDisableVertexAttribArray(color_position);
		}
	}

	glUseProgram(0);

	glutSwapBuffers();
}

void Scene::changeState(State s)
{
	if (state == s)
		return;
	state = s;
	switch (state)
	{
	case ENTER_POLYGON:
		polygons->push_back(*(new maths::Polygon()));
		break;
	case ENTER_WINDOW:
		window->getPoints()->clear();
		break;
	case DRAW:
		break;
	case FILL:
		allIntersection->clear();
		for (int i = 0; i < polygons->size(); i++)
		{
			allIntersection->push_back(LCARemplissage(polygons->at(i)));
		}
		break;
	default:
		break;
	}
}

State Scene::getState()
{
	return state;
}

void Scene::addPoint(maths::Point p)
{
	switch (state)
	{
	case ENTER_POLYGON:
		if (!polygons->empty())
		{
			//std::cout << "point added x=" << p.x << " y=" << p.y << std::endl;
			p.x -= width / 2;
			p.x /= width / 2;
			p.y = height - p.y;
			p.y -= height / 2;
			p.y /= height / 2;
			p.x = Math::round(p.x);
			p.y = Math::round(p.y);
			//std::cout << "point normalized x=" << p.x << " y=" << p.y << std::endl;

			polygons->back().addPoint(p);
		}
		break;
	case ENTER_WINDOW:
		//std::cout << "point added x=" << p.x << " y=" << p.y << std::endl;
		p.x -= width / 2;
		p.x /= width / 2;
		p.y = height - p.y;
		p.y -= height / 2;
		p.y /= height / 2;
		p.x = Math::round(p.x);
		p.y = Math::round(p.y);
		//std::cout << "point normalized x=" << p.x << " y=" << p.y << std::endl;

		window->addPoint(p);
		break;
	case DRAW:
		break;
	case FILL:
		break;
	case COLOR:
		break;
	default:
		break;
	}
	
}

void Scene::setDrawWindow()
{
	drawWindow = !drawWindow;
}

void Scene::menuCallBack(int i)
{
	Scene::currentInstance->menu(i);
}

void Scene::drawChar(const char c, const maths::Point position, const maths::Color color)
{
	//gluOrtho2D(0.0, width, 0.0, height);
	glColor3f(color.r, color.g, color.b);
	maths::Point p;
	p.x = (position.x*(width / 2)) + (width / 2);
	p.y= height -(position.y*(height/ 2)) + (height/ 2);
	
	glRasterPos2i(20, 20);
	void * font = GLUT_BITMAP_9_BY_15;
	glutBitmapCharacter(font, c);
}

// Paramètre : polygon que l'on veut colorier
// Retour : tableau de points (traités comme des segments) qui doivent être colorié dans la mainLoop
// But :  Détecter quelles zones sont à colorier
std::vector<maths::Point>* Scene::LCARemplissage(maths::Polygon polygon)
{
	std::vector<maths::Point>* allPointToDraw = new std::vector<maths::Point>();

	for (int y = 1; y < height; y++)
	{
		int bitParite = 0;

		// On créé notre ligne de balayage
		std::vector<maths::Point>* ligneBallayage = new std::vector<maths::Point>();

		maths::Point posXLigneBalayage;
		posXLigneBalayage.x = 0;
		posXLigneBalayage.y = y;

	maths:Point posYLigneBalayage;
		posYLigneBalayage.x = width;
		posYLigneBalayage.y = y;

		ligneBallayage->push_back(posXLigneBalayage);
		ligneBallayage->push_back(posYLigneBalayage);

		// On récupère les points du polygon
		std::vector<maths::Point>* pointsFromPolygon = polygon.getPoints();
		int nbPoint = polygon.getPoints()->size();

		// On test les intersections entre la ligne de balayage et tous les côtés du polygon
		std::vector<maths::Point>* pointsIntersection = new std::vector<maths::Point>();
		for (int i = 0; i < nbPoint; i++)
		{
			maths::Point pointIntersection;

			// Attention ici on converti les points du repère pixel de l'écran au repère OpenGL
			maths::Point* pointXLigneBalayage = ConvertPointPixelToOpenGLUnit(ligneBallayage->at(0));
			maths::Point* pointYLigneBalayage = ConvertPointPixelToOpenGLUnit(ligneBallayage->at(1));

			// Petit cas particulier pour le dernier point que l'on associe au premier point pour tester le côté qui ferme le polygon
			// TODO : Trier intersection avec les segments ayant la norme la plus petite par rapport au bord gauche de la fenêtre
			if (i == nbPoint - 1)
			{
				pointIntersection = CVecteur::Intersection(*pointXLigneBalayage, *pointYLigneBalayage, pointsFromPolygon->at(i), pointsFromPolygon->at(0));
			}
			else
			{
				pointIntersection = CVecteur::Intersection(*pointXLigneBalayage, *pointYLigneBalayage, pointsFromPolygon->at(i), pointsFromPolygon->at(i + 1));
			}

			if (pointIntersection.x != -1 && pointIntersection.y != -1)
			{
				// TODO: Faire le tri ici
				//Fait ! :)
				int i = 0;
				while (i < pointsIntersection->size())
				{
					if (pointsIntersection->at(i).x > pointIntersection.x)
						break;
					i++;
				}
				if (i == pointsIntersection->size())
					pointsIntersection->push_back(pointIntersection);
				else
					pointsIntersection->insert(pointsIntersection->begin() + i, pointIntersection);
			}
		}

		if (pointsIntersection->size() != 0)
		{
			// On dessine entre chaque intersection avec la bonne couleur
			for (int i = 0; i < pointsIntersection->size(); i++)
			{
				// On teste le cas particulier du passage de la ligne de balayage sur un sommet du polygon
				maths::Point* peakDetect = isVertexFromPolygon(polygon, pointsIntersection->at(i));
				if (peakDetect != nullptr)
				{
					// On ajoute 2 fois car on traitra les points contenus dans le tableau 2 à 2 (segement)
					allPointToDraw->push_back(*peakDetect);
					allPointToDraw->push_back(*peakDetect);
				}
				else
				{
					allPointToDraw->push_back(pointsIntersection->at(i));
				}
			}
		}
	}

	return allPointToDraw;
}

maths::Point* Scene::isVertexFromPolygon(maths::Polygon polygon, maths::Point pointTest)
{
	for (int i = 0; i < polygon.getPoints()->size(); i++)
	{
		if (pointTest.x == polygon.getPoints()->at(i).x && pointTest.y == polygon.getPoints()->at(i).y)
		{
			return &(polygon.getPoints()->at(i));
		}
	}

	return nullptr;
}

maths::Point* Scene::ConvertPointPixelToOpenGLUnit(maths::Point point)
{
	maths::Point* pointOpenGL = new maths::Point();

	float v = 2.0 / (double)width;
	float w = 2.0 / (double)height;
	pointOpenGL->x = v * (double)point.x - 1.0;
	pointOpenGL->y = w * (double)point.y - 1.0;

	return pointOpenGL;
}

bool Scene::isPointInPol(maths::Polygon pol, maths::Point p)
{
	pol.calculateNormals();
	int nbPointWin = pol.getPoints()->size();

	for (int j = 0; j < nbPointWin; j++)
	{
		maths::Point p1 = pol.getPoints()->at(j);

		if (!Math::isPointVisible(p, p1, pol.getNormals()->at(j)))
			return false;
	}
	return true;
}

void Scene::cursorInPolygon(maths::Point p)
{
	for (int i = 0; i < polygons->size(); i++)
	{
		bool dog = isPointInPol(polygons->at(i), p);
		
		if (dog == true)
		{
			stackPolygonClicked->push_back(polygons->at(i));
		}
	}
}

maths::Point Scene::scalePoint(maths::Point p, float ratio)
{
	// TODO : Test ratio

	maths::Point resPoint;

	resPoint.x = p.x * ratio;
	resPoint.y = p.y * ratio;

	return resPoint;
}

maths::Point Scene::translatePoint(maths::Point p, float translateX, float translateY)
{
	maths::Point resPoint;

	resPoint.x = p.x + translateX;
	resPoint.y = p.y + translateY;

	return resPoint;
}

maths::Point Scene::rotate_point(maths::Polygon poly,float angle,maths::Point p)
{
  float s = sin(angle);
  float c = cos(angle);

  // On calcul la moyenne des coordonnées des sommets du polygon
  float pivotX = 0;
  float pivotY = 0;

  int nbPoints = poly.getPoints()->size();
  
  for (int i = 0; i < nbPoints ; i++)
  {
	  pivotX += poly.getPoints()->at(i).x;
	  pivotY += poly.getPoints()->at(i).y;
  }

  pivotX = pivotX / nbPoints;
  pivotY = pivotY / nbPoints;

  // application formule
  p.x -= pivotX;
  p.y -= pivotY;

  // rotation du point
  float xnew = p.x * c - p.y * s;
  float ynew = p.x * s + p.y * c;

  p.x = xnew + pivotX;
  p.y = ynew + pivotY;

  return p;
}

Scene::Scene(int w, int h)
{
	window = new maths::Polygon();
	state = DRAW;
	height = h;
	width = w;
	value = 0;
	drawWindow = true;
	isInPolygon = true;
	Scene::currentInstance = this;
	input = new Input(this);
	polygons = new std::vector<maths::Polygon>();
	allIntersection = new std::vector<std::vector<maths::Point>*>();
	stackPolygonClicked = new std::vector<maths::Polygon>();
	countPass = 0;
	radiusPoint.x = 10.0f/ width;
	radiusPoint.y = 10.0f /height;
	pointSelected = -1;
	polygonSelected = -1;
}


Scene::~Scene()
{
	g_BasicShader.DestroyProgram();
}
