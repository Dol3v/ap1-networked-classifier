#pragma once
#include <vector>

/// Measures distance between two data points.
template <class T>
class Metric {
public:
    virtual double operator()(const T&, const T&) const;
};

/// The usual Euclidean metric.
class EuclideanMetric : public Metric<std::vector<double>> {
public:
    virtual double operator()(const std::vector<double> &t, const std::vector<double> &t1) const;
};

/// The Manhattan distance function.
class ManhattanMetric : public Metric<std::vector<double>> {
public:
    virtual double operator()(const std::vector<double> &t, const std::vector<double> &t1) const;
};

/// The Chebyshev distance function.
class ChebyshevMetric : public Metric<std::vector<double>> {
public:
    double operator()(const std::vector<double> &t, const std::vector<double> &t1) const override;
};