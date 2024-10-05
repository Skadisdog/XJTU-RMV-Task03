#ifndef FITTING_HPP
#define FITTING_HPP
#include <stdlib.h>
#include <vector>
#include <ceres/ceres.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>

struct Parameter
{
    double A;
    double omega;
    double phi;
    double B;
};
namespace FITTING
{
    class Fitting
    {
    private:
        struct DataPoint {
            double t;
            double y;
        };
        std::vector<DataPoint> data;
        struct CosineCostFunctor { // 代价函数
            CosineCostFunctor(double t, double y) : t_(t), y_(y) {}

            template <typename T>
            bool operator()(const T* const parameters, T* residual) const {
                const T& A = parameters[0];
                const T& omega = parameters[1];
                const T& phi = parameters[2];
                const T& b = parameters[3];

                residual[0] = T(y_) - (A * ceres::cos(omega * T(t_) + phi) + b);
                return true;
            }

        private:
            const double t_;
            const double y_;
        };
    public:
        void addData(DataPoint input)
        {
            data.push_back(input);
        }
        void clearData()
        {
            data.clear();
        }
        struct Parameter calculate();
    };
}
#endif