#pragma once
#include <cmath>
#include <vector>
#include <numeric>
class Util
{
public:
	static double GetDistance(int x1, int y1, int x2, int y2);
	static float GetDistance(float x1, float y1, float x2, float y2, int px, int py);
	static float GetMin(float p1, float p2);
	static float GetMax(float p1, float p2);
};

