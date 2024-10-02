#include "fitting.hpp"

namespace FITTING {
    Parameter Fitting::calculate()
    {
        // 初始化参数 A, omega, phi, B
        double parameters[4] = {0.785, 1.884, 1.65, 1.305};

        // 构建问题
        ceres::Problem problem;
        for (const auto& point : data) {
            problem.AddResidualBlock(
                new ceres::AutoDiffCostFunction<CosineCostFunctor, 1, 4>(
                    new CosineCostFunctor(point.t, point.y)),
                nullptr,
                parameters
            );
        }

        // 配置求解器
        ceres::Solver::Options options;
        options.linear_solver_type = ceres::DENSE_QR;
        options.minimizer_progress_to_stdout = false;
        options.function_tolerance = 1e-5;

        ceres::Solver::Summary summary;
        ceres::Solve(options, &problem, &summary);
        // std::cout << summary.FullReport() << "\n";

        Parameter a={parameters[0], parameters[1], parameters[2], parameters[3]};
        return a;
    }
}