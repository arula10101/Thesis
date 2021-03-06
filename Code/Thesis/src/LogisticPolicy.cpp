#include "thesis/LogisticPolicy.h"
#include <math.h>

LogisticPolicy::LogisticPolicy(size_t dimObservation_,
                               double paramMinValue_,
                               double paramMaxValue_)
    : Policy(dimObservation_, 1ul),
      dimParameters(dimObservation_ + 1),
      parameters(dimObservation_ + 1),
      paramMinValue(paramMinValue_),
      paramMaxValue(paramMaxValue_)
{
    initializeParameters();
}

void LogisticPolicy::initializeParameters()
{
    parameters.randu();
    parameters -= 0.5;
    parameters *= 0.001;
}

void LogisticPolicy::setParameters(arma::vec const & parameters_)
{
    parameters = arma::clamp(parameters_, paramMinValue, paramMaxValue);;
}

arma::vec LogisticPolicy::getAction(arma::vec const & observation_) const
{
    // Compute features
    arma::vec features(dimParameters);
    features(0) = 1.0;
    features.rows(1, dimParameters - 1) = observation_;

    // Compute action
    double activation = arma::dot(parameters, features);
    arma::vec action(1);
    action(0) = std::tanh(activation);
    return action;
}

void LogisticPolicy::reset()
{
    initializeParameters();
}

std::unique_ptr<Policy> LogisticPolicy::cloneImpl() const
{
    return std::unique_ptr<Policy>(new LogisticPolicy(*this));
}
