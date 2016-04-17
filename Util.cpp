#include "pch.h"
#include "Util.h"


double Util::GetDistance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

float Util::GetDistance(float x1, float y1, float x2, float y2, int px, int py)
{
	/*
	선분 p1->p2 와 점 p 사이의 거리
	A : p1->p, 	B : p1->p2 라고 할 때 base = (A·B) / (B·B)	

	0 > base			: (p1->수선의 발 방향이 선분의 방향과 반대인 경우, p에서 p1까지의 거리가 최종 거리)
	0 < base < 1		: (수선의 발이 정확히 선분 위에 내린 경우, p와 수선의 발 사이의 거리가 최종 거리)
	1 < base			: (p1->수선의 발 크기가 선분의 길이보다 큰 경우, p에서 p2까지의 거리가 최종 거리)
	*/
	float distance;

	std::vector<float> p1_p { px - x1, py - y1 };
	std::vector<float> p1_p2{ x2 - x1, y2 - y1 };

	float base = (std::inner_product(p1_p.begin(), p1_p.end(), p1_p2.begin(), 0) / std::inner_product(p1_p2.begin(), p1_p2.end(), p1_p2.begin(), 0.0f));

	if (0 > base)
	{
		distance = Util::GetDistance(x1, y1, px, py);
	}
	else if (0 < base && base < 1)
	{
		std::vector<float> p1_k{ p1_p2.at(0) * base , p1_p2.at(1) * base };
		distance = Util::GetDistance(p1_k.at(0), p1_k.at(1), px, py);
	}
	else if (1 < base)
	{
		distance = Util::GetDistance(x2, y2, px, py);
	}
	else			// base == 0 || base == 1
	{
		distance = Util::GetMin(Util::GetDistance(x1, y1, px, py), Util::GetDistance(x2, y2, px, py));
	}

	return distance;
}

float Util::GetMin(float p1, float p2)
{
	if (p1 > p2)
		return p2;
	else
		return p1;
}

float Util::GetMax(float p1, float p2)
{
	if (p1 < p2)
		return p2;
	else
		return p1;
}
