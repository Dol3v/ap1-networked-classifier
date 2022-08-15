#include <string>
#include <fstream>
#include <unistd.h>
#include <tuple>

#include "server/server.hpp"
#include "common.hpp"
#include "server/classifier/knn.hpp"
#include "server/classifier/csv.hpp"
#include "server/classifier/utils.hpp"

/// Trains the model on a pre-classified dataset.
void train_model(KnnClassifier<std::vector<double>, std::string> &model, std::fstream &data_file) {
    while (data_file) {
        std::string line;
        std::getline(data_file, line);

        if (line.empty()) {
            break;
        }
        std::stringstream ss(line);

        auto values = csv::read_line<double, double, double, double, std::string>(ss);

        auto data_class = std::get<4>(values);
        auto data = utils::tup2vec4<double, std::string>(values);

        model.load_data(data, data_class);
    }
}

/// Classifies the data from a csv input file, and writes the classified data to an output file in csv form.
void classify(KnnClassifier<std::vector<double>, std::string> &model, std::ifstream &unclassified,
              std::ofstream &classified, const Metric<std::vector<double>> &metric) {
    while (unclassified) {
        std::string line;
        std::getline(unclassified, line);

        if (line.empty()) {
            break;
        }
        std::stringstream ss(line);
        auto values = csv::read_line<double, double, double, double, std::string>(ss);

        std::string data_class = model.predict(utils::tup2vec4(values), metric);
        csv::write_line(classified, std::get<0>(values),
                        std::get<1>(values),
                        std::get<2>(values),
                        std::get<3>(values),
                        data_class);
    }
}

int main() {
    KnnClassifier<std::vector<double>, std::string> classifier(3);
    EuclideanMetric metric;

    // training knn model
    std::fstream data;
    data.open("classified.csv", std::ios::in);
    if (!data) {
        throw std::logic_error("Couldn't find training data");
    }
    train_model(classifier, data);
    data.close();

    // listening for incoming connections
    net::Server server(PORT, "0.0.0.0");
    server.listen(3);

    // saving user data to file
    int client_fd = server.accept();
    const std::string user_data_file = "unclassified.txt";
    common::file_recv_to_path(user_data_file, client_fd);

    // classifying data and saving
    const std::string classified_data_file = "classified.txt";
    std::ofstream classified_file(classified_data_file, std::ios::trunc | std::ios::out);
    std::ifstream unclassified_file(user_data_file);
    classify(classifier,unclassified_file, classified_file, metric);

    // send classified data to user
    common::file_upload(classified_data_file, client_fd);
    classified_file.close();
    unclassified_file.close();
    close(client_fd);
}
