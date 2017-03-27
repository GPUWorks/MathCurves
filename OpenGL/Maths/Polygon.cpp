#include "Polygon.h"


void maths::Polygon::calculateNormals()
{
	if (normals->empty() && points->size() > 1)
	{
		int size = points->size();
		for (int i = 0; i < size ; i++)
		{
			maths::Point n;
			maths::Point p1= points->at(i);
			maths::Point p2;
			if (i == size - 1)
				p2 = points->at(0);
			else 
				p2 = points->at(i+1);
			if (sensTrigo)
			{
				n.x = (p1.y - p2.y);
				n.y = -(p1.x - p2.x);
			}	
			else
			{
				n.x = -(p1.y - p2.y);
				n.y = (p1.x - p2.x);
			}
			normals->push_back(n);
		}
	}
}

void maths::Polygon::calculateVectors()
{
	if (vectors->empty() && points->size() > 1)
	{
		for (int i = 1; i < points->size(); i++)
		{
			maths::Point v;
			v.x = (points->at(i).x - points->at(i-1).x);
			v.y = (points->at(i).y - points->at(i-1).y);
			vectors->push_back(v);
		}
	}
}

void maths::Polygon::addPoint(maths::Point p)
{
	if(points->size()==1)
	{
		sensTrigo = p.x < points->at(0).x;
	}
	points->push_back(p);
	visibility->push_back(true);
}

void maths::Polygon::addPoint(maths::Point p, int index)
{
	points->insert(points->begin()+index, p);
	visibility->insert(visibility->begin() + index, true);
}

void maths::Polygon::removePoint()
{
	if (!points->empty())
	{
		points->pop_back();
		visibility->pop_back();
	}
		
}

void maths::Polygon::removePoint(int index)
{
	points->erase(points->begin() + index);
	visibility->erase(visibility->begin() + index);
}

void maths::Polygon::setVisibility(int index, bool visible)
{
	visibility->at(index) = visible;
}

bool maths::Polygon::isPointVisible(int index)
{
	return visibility->at(index);
}

std::vector<maths::Point>* maths::Polygon::getPoints()
{
	return points;
}

const std::vector<maths::Point>* maths::Polygon::getNormals()
{
	return normals;
}

void maths::Polygon::setPoint(maths::Point p, int indice)
{
	this->points->at(indice) = p;
}

maths::Polygon::Polygon()
{
	points = new std::vector<maths::Point>();
	normals = new std::vector<maths::Point>();
	vectors = new std::vector<maths::Point>();
	bezierPoints = new std::vector<maths::Point>();
	visibility = new std::vector<bool>();
}

void maths::Polygon::recalculateBezierPoints(int index)
{
	bezierPoints->clear();
	for (int i = 0; i < points->size(); i++)
	{
		bezierPoints->push_back(points->at(i));
	}
	while (index > 0)
	{
		recursiveRecalculateBezierPoints();
		--index;
	}
}

void maths::Polygon::recursiveRecalculateBezierPoints()
{
	std::vector<maths::Point> *tmp = new std::vector<maths::Point>();

	tmp->push_back(bezierPoints->at(0));

	for (int i = 0; i < bezierPoints->size() - 1; i++)
	{
		Point newPoint;
		Point p1 = bezierPoints->at(i);
		Point p2 = bezierPoints->at(i + 1);
		if (p1.x < p2.x)
			newPoint.x = p1.x + (p2.x - p1.x) / 2;
		else newPoint.x = p2.x + (p1.x - p2.x) / 2;

		if (p1.y < p2.y)
			newPoint.y = p1.y + (p2.y - p1.y) / 2;
		else newPoint.y = p2.y + (p1.y - p2.y) / 2;

		tmp->push_back(newPoint);
	}
	tmp->push_back(bezierPoints->at(bezierPoints->size() - 1));

	bezierPoints->clear();
	for (int i = 0; i < tmp->size(); i++)
	{
		bezierPoints->push_back(tmp->at(i));
	}

	delete tmp;
}


std::vector<maths::Point>* maths::Polygon::getBezierPoints()
{
	return bezierPoints;
}


maths::Polygon::~Polygon()
{
}
