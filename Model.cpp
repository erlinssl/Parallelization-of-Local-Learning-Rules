#include "Model.h"

#include <iostream>
#include <iterator>
#include <vector>

template <typename T>
std::vector<std::vector<T>> operator-=(af::array &x, af::array &y) {
    x -= y;
    return x;
}

template <typename T>
double Model<T>::f(af::array const &i, af::array const &x, af::seq & pos) {
    return af::exp(-af::sum(af::sum(af::moddims(af::pow(x-i, 2), resolution, resolution, filters)))/sigma).scalar<float>();
}

template <typename T>
void Model<T>::update(af::array const &x) {
    af::array nums = af::seq(filters);
    try {
        af::array diff = af::constant(0, resolution, resolution, filters);
        gfor(af::seq i1, filters) {
            af::array mu_copy = -mu;
            mu_copy(af::span, af::span, i1) += x;

            af::array exp = moddims(af::exp(-af::sum(af::sum(af::pow(mu_copy, 2))) / sigma), 1, 1, filters);
            diff += mu_copy * exp;

            for(size_t i2 = 0; i2 < filters; i2++) {
                af::array mu2_copy = -mu;
                mu2_copy(af::span, af::span, i1) += mu(af::span, af::span, i2);
                diff -= (mu2_copy * (2.0 * lambda * af::exp(-af::sum(af::sum(af::pow(mu2_copy, 2))) / sigma)));
            }
        }
        mu += ((diff * learning_rate) / sigma);
    } catch (af::exception & e) {
        std::cout << "\033[1;31m" << e.what() << "\033[0m" << std::endl;
        exit(1);
    }
}

template <typename T>
void Model<T>::save(const char subfigure) {
    std::string path = "../saved/figure2";
    path.push_back(subfigure);
    path.append(".fig");

    std::cout << "Saving figure to " << path << std::endl;
    auto temp = af::saveArray(&subfigure, mu, &path[0], false);
    std::cout << "index of array '" << &subfigure << "' is " << temp << std::endl;
}

template <typename T>
bool Model<T>::load(const char subfigure) {
    std::string path = "../saved/figure2";
    path.push_back(subfigure);
    path.append(".fig");
    if(!std::filesystem::exists(path)){
        std::cout << "Figure " << subfigure << " not found." << std::endl;
        return false;
    }

    std::cout << "Reading array with key '" << &subfigure << "' from " << path << std::endl;
    auto index = af::readArrayCheck(&path[0], &subfigure);
    if(index >= 0) {
        mu = af::readArray(&path[0], index);
    } else {
        std::cout << "Invalid key" << std::endl;
        return false;
    }
    return true;
}

template class Model<int>;
template class Model<float>;
template class Model<double>;
