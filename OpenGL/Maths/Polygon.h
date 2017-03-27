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

	public:
		void calculateNormals();
		void calculateVectors();
		void addPoint(maths::Point p, int index);
		void addPoint(maths::Point p);
		void removePoint();
		void removePoint(int index);
		void setVisibility(int index, bool visible);
		bool isPointVisible(int index);
		std::vector<maths::Point>* getPoints();
		const std::vector<maths::Point>* getNormals();
		Polygon();
		~Polygon();
	};
}


#endif // !MATHS_POLYGON