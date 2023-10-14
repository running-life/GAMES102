#pragma once


#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>


struct HM1Point {
	float x, y, r, g, b;
	HM1Point(float x_ = 0, float y_ = 0, float r_ = 0, float g_ = 0, float b_ = 0):x(x_), y(y_), r(r_), g(g_), b(b_) {}
};

class HM1 {
public:
	static bool polynomialInterpolationFlag;
	static bool RBFInterpolationFlag;
	static bool leastSquareFittingFlag;
	static bool ridgeFittingFlag;
	static std::vector<HM1Point> controlPoints;
	static std::vector<HM1Point> resultPolynomial;
	static std::vector<HM1Point> resultGauss;
	static std::vector<HM1Point> resultLeastSquare;
	static std::vector<HM1Point> resultRidge;

	static void polynomialInterpolation() {
		if (controlPoints.size() < 2) {
			return;
		}
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

	static void gaussInterpolation() {
		float sigma = 0.01;
		float step = 0.01;
		size_t n = controlPoints.size();
		if (n < 2)
			return;
		resultGauss.clear();

		auto g = [&](size_t i, float x) {
			if (i == 0) return 1.0f;
			float xi = controlPoints[i - 1].x;
			return std::exp(-(x - xi) * (x - xi) / (2 * sigma * sigma));
			};

		// add another point, because we have n + 1 variable but n control points
		float newPointX = 0, newPointY = 0;
		for (size_t i = 0; i < n; ++i) {
			newPointX += controlPoints[i].x;
			newPointY += controlPoints[i].y;
		}
		newPointX /= n;
		newPointY /= n;

		// init gMatirx
		Eigen::MatrixXf gMatrix(n + 1, n + 1);
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j <= n; ++j) {
				gMatrix(i, j) = g(j, controlPoints[i].x);
			}
		}
		for (size_t j = 0; j <= n; ++j) {
			gMatrix(n, j) = g(j, newPointX);
		}

		//std::cout << gMatrix << std::endl;

		// inti Y;
		Eigen::VectorXf Y(n + 1);
		for (size_t i = 0; i < n; ++i) {
			Y(i) = controlPoints[i].y;
		}
		Y(n) = newPointY;

		std::cout << Y << std::endl;

		Eigen::VectorXf B = gMatrix.colPivHouseholderQr().solve(Y);

		auto f = [&](float x) {
			size_t n = controlPoints.size();
			float ans = 0;
			for (size_t i = 0; i <= n; ++i) {
				ans += B[i] * g(i, x);
			}
			return ans;
		};

		std::cout << gMatrix * B << std::endl;

		for (float t = -0.99f; t < 1.0f; t += step) {
			resultGauss.push_back({ t, f(t), 0.2f, 0.2f, 0.2f });
		}


	}

	static void leastSquareFitting() {
		size_t n = controlPoints.size();
		size_t m = 4;	// highest degree of polynomial
		float step = 0.01;
		if (n <= m)
			return;
		resultLeastSquare.clear();

		Eigen::MatrixXf M(n, m);

		auto b = [&](size_t i, float x) -> float {
			if (i == 0)
				return 1.0;
			float ans = x;
			for (int j = 1; j < i; ++j) {
				ans *= x;
			}
			return ans;
			};

		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < m; ++j) {
				M(i, j) = b(j, controlPoints[i].x);
			}
		}

		Eigen::VectorXf Y(n);
		for (size_t i = 0; i < n; ++i) {
			Y(i) = controlPoints[i].y;
		}
		Y = M.transpose() * Y;
		M = M.transpose() * M;
		Eigen::VectorXf lambda = M.colPivHouseholderQr().solve(Y);

		auto f = [&](float x) -> float {
			float ans = 0;
			for (size_t j = 0; j < m; ++j) {
				ans += lambda(j) * b(j, x);
			}
			return ans;
			};

		for (float t = -0.99; t < 1.0f; t += step) {
			resultLeastSquare.push_back({ t, f(t), 0.1f, 0.3f, 0.9f });
		}
	}


	static void ridgeFitting() {
		size_t n = controlPoints.size();
		size_t m = 4;	// highest degree of polynomial
		float step = 0.01;
		float alpha = 0.0005;
		if (n <= m)
			return;
		resultRidge.clear();

		Eigen::MatrixXf M(n, m);

		auto b = [&](size_t i, float x) -> float {
			if (i == 0)
				return 1.0;
			float ans = x;
			for (int j = 1; j < i; ++j) {
				ans *= x;
			}
			return ans;
			};

		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < m; ++j) {
				M(i, j) = b(j, controlPoints[i].x);
			}
		}

		Eigen::VectorXf Y(n);
		for (size_t i = 0; i < n; ++i) {
			Y(i) = controlPoints[i].y;
		}

		Eigen::MatrixXf newM = M.transpose() * M + alpha * Eigen::MatrixXf::Identity(m, m);

		Eigen::VectorXf lambda = newM.inverse() * M.transpose() * Y;

		auto f = [&](float x) -> float {
			float ans = 0;
			for (size_t j = 0; j < m; ++j) {
				ans += lambda(j) * b(j, x);
			}
			return ans;
			};

		for (float t = -0.99; t < 1.0f; t += step) {
			resultRidge.push_back({ t, f(t), 0.7f, 0.5f, 0.9f });
		}


	}



};

