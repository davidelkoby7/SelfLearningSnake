#pragma once

#include <bits/stdc++.h>

#include "Constants.h"

namespace ActivationFunctions
{
    double ReLU(const double& x)
    {
        return (x > 0 ? x : 0);
    }

    double ReLUDerivative(const double& x)
    {
        return (x > 0 ? 1 : 0);
    }

    double LeakyReLU(const double& x)
    {
        return (x > 0 ? x : 0.1 * x);
    }

    double LeakyReLUDerivative(const double& x)
    {
        return (x > 0 ? 1 : 0.1);
    }

    double Sigmoid(const double& x)
    {
        return 1 / (1 + std::pow(Constants::e, -x));
    }

    double SigmoidDerivative(const double& x)
    {
        double sigmoidValue = Sigmoid(x);
        return sigmoidValue * (1 - sigmoidValue);
    }
}
