#pragma once

#include <string>
#include "metric.hpp"

/// data entry
template<class Data, class Class>
class DataEntry {
public:
    Data data;
    Class data_class;

    DataEntry(Data data, Class class_);
};


/// KNN classifier
template<class Data, class Class>
class KnnClassifier {
private:
    int k;
    std::vector<DataEntry<Data, Class> *> data_set;
public:
    /// Constructs a classifier with a given k value.
    explicit KnnClassifier(int k);

    /// loads data into the classifier.
    void load_data(const Data &t, const Class &s);

    /// Predicts the class of a given data sample using the KNN algorithm and a given metric.
    Class predict(const Data &t, const Metric<Data>& metric);

    ~KnnClassifier();
};


#include "knn.tpp"