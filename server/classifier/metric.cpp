#include <stdexcept>
#include <cmath>
#include "metric.hpp"

template<class T>
double Metric<T>::operator()(const T &t1, const T &t2) const {
    throw std::logic_error("unimplemented function");
}

double EuclideanMetric::operator()(const std::vector<double> &t, const std::vector<double> &t1) const {
    if (t.size() != t1.size()) {
        throw std::runtime_error("operands' sizes don't match");
    }
    double diff = 0;
    for (int i = 0; i < t.size(); i++) {
        diff += std::pow(t.at(i) - t1.at(i), 2);
    }
    return std::pow(diff, 1. / (double) t.size());
}

double ManhattanMetric::operator()(const std::vector<double> &t, const std::vector<double> &t1) const {
    if (t.size() != t1.size()) {
        throw std::runtime_error("operands' sizes don't match");
    }
    double distance = 0;
    for (int i = 0; i < t.size(); i++) {
        distance += std::abs(t.at(i) - t1.at(i));
    }
    return distance;
}

double ChebyshevMetric::operator()(const std::vector<double> &t, const std::vector<double> &t1) const {
    if (t.size() != t1.size()) {
        throw std::runtime_error("operands' sizes don't match");
    }
    double distance = 0;
    for (int i = 0; i < t.size(); i++) {
        distance = std::max(distance, std::abs(t.at(i) - t1.at(i)));
    }
    return distance;
}

// used to prevent linker errors
template class Metric<std::vector<double>>;