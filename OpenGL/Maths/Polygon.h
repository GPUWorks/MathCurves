#ifndef DEF_MATHS_POLYGON
#define DEF_MATHS_POLYGON

#include <vector>

namespace maths
{
	struct Point
	{
		float x;
		float y;
	};
	typedef struct Point Point;

	struct Color
	{
		float r;
		float g;
		float b;
	};
	typedef struct Color Color;

	class Polygon
	{
		std::vector<maths::Point> *points;
		std::vector<maths::Point> *normals;
		std::vector<maths::Point> *vectors;
		std::vector<bool> *visibility;
		bool sensTrigo;
		void recursiveRecalculateBezierPoints();
		std::vector<maths::Point> *bezierPoints;

	public:
		void calculateNormals();
		void calculateVectors();
		void addPoint(maths::Point p, int index);
		void addPoint(maths::Point p);
		void removePoint();
		void removePoint(int index);
		void setVisibility(int index, bool visible);
		bool isPointVisible(int index);
		void setPoint(maths::Point p, int indice);
		const std::vector<maths::Point>* getNormals();
		Polygon();
		~Polygon();
		std::vector<maths::Point>* getPoints();
		std::vector<maths::Point>* getBezierPoints();
		void recalculateBezierPoints(int index);
	};
}


#endif // !MATHS_POLYGON