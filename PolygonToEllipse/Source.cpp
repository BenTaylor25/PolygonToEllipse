#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <vector>
#include <cstdlib>
#include <ctime>

int WIDTH = 800;
int HEIGHT = 450;

// Maths Vector
class M_Vect
{
public:
	int x, y;

	M_Vect(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};


class Screen : public olc::PixelGameEngine
{
	std::vector<M_Vect> points;
	std::vector<M_Vect> midPoints;

	/////////////////////////////////////////////////////
	// 0 : draw                                        //
	// 1 : view midpoints                              //
	// 2 : switch to midpoints   (switches back to 1)  //
	/////////////////////////////////////////////////////
	int stage = 0;

public:
	Screen() { sAppName = "PolygonToEllipse"; }

	bool OnUserCreate() override { return true; }

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (!stage)
		{
			CheckClick();
		}

		CheckContinue();

		FillRect(0, 0, WIDTH, HEIGHT, olc::BLACK);
		DrawPoints();

		return true;
	}

	void CheckClick()
	{
		if (GetMouse(0).bReleased)
		{
			M_Vect newPoint(GetMouseX(), GetMouseY());

			points.push_back(newPoint);
		}
	}
	
	void CheckContinue()
	{
		if (GetMouse(1).bReleased)
		{
			stage++;
			if (stage == 3) { stage = 1; }
			
			switch (stage)
			{
			case 1:
				for (int i = 0; i < points.size(); i++)
				{
					midPoints.push_back(GetMidPoint(i, (i + 1) % points.size()));
				}
				break;
			case 2:
				points = midPoints;
				midPoints.clear();
			}
		}
	}

	M_Vect GetMidPoint(int p1, int p2)
	{
		M_Vect midPoint
		(
			(points[p2].x + points[p1].x) / 2,
			(points[p2].y + points[p1].y) / 2
		);

		return midPoint;
	}
	
	void DrawPoints()
	{
		for (int i = 0; i < points.size(); i++)
		{
			FillCircle(points[i].x, points[i].y, 2, olc::WHITE);

			if (stage == 1)
			{
				FillCircle(midPoints[i].x, midPoints[i].y, 2, olc::RED);
			}

			int next = (i + 1) % points.size();
			DrawLine(points[i].x, points[i].y, points[next].x, points[next].y, olc::WHITE);
		}
	}
};


int main()
{
	Screen s;
	if (s.Construct(WIDTH, HEIGHT, 1, 1)) 
	{ 
		s.Start(); 
	}
	return 0;
}
