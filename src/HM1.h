#pragma once


#include <vector>


struct HM1Point {
	float x, y, r, g, b;
	HM1Point(float x_ = 0, float y_ = 0, float r_ = 0, float g_ = 0, float b_ = 0):x(x_), y(y_), r(r_), g(g_), b(b_) {}
};

class HM1 {
public:
	static std::vector<HM1Point> controlPoints;
	static std::vector<HM1Point> resultPolynomial;
	std::vector<float> test() {
		std::vector<float> temp;
		for (float i = -0.80f; i < 1.0f; i += 0.04f) {
			temp.push_back(i);
			temp.push_back(i*i);
			temp.push_back(0.5f);
			temp.push_back(0.5f);
			temp.push_back(0.5f);
		}
		return temp;
	}

	void polynomialInterpolation() {
		if (controlPoints.size() < 2) {
			return;
		}

		

	}

};

