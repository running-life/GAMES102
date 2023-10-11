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

	static void polynomialInterpolation() {
		if (controlPoints.size() < 2) {
			return;
		}
		std::cout << "begin to calculate" << std::endl;
		resultPolynomial.clear();
		float* l = new float[controlPoints.size()];

		for (size_t i = 0; i < controlPoints.size(); ++i) {
			l[i] = controlPoints[i].y;
			for (size_t j = 0; j < controlPoints.size(); ++j) {
				if (j != i)
					l[i] /= (controlPoints[i].x - controlPoints[j].x);
			}
		}

		HM1Point tempPoint;
		tempPoint.r = tempPoint.g = tempPoint.b = 0.6;
		for (float t = -0.99; t < 1.0f; t += 0.01) {
			tempPoint.x = t;
			tempPoint.y = 0;
			for (size_t i = 0; i < controlPoints.size(); ++i) {
				float temp = l[i];
				for (size_t j = 0; j < controlPoints.size(); ++j) {
					if (j != i) {
						temp *= (t - controlPoints[j].x);
					}
				}
				tempPoint.y += temp;
			}
			resultPolynomial.push_back(tempPoint);
		}

		delete []l;
	}

};

