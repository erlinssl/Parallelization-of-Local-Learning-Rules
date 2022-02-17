//
// Created by ingebrigt on 27.01.2022.
//

// TODO Refactor usages of 2D arrays


#include "Arrays.h"

#include <utility>

// -----------------------------------------------------------------------
// ----------------------------- CUBE ARRAYS -----------------------------
// -----------------------------------------------------------------------

template <typename T>
CubeArray<T>::CubeArray(bool zero, size_t outer, size_t middle, size_t inner) {
    for (int i = 0; i < outer; ++i) {
        cube.emplace_back(std::vector<std::vector<T>>());
        for (int j = 0; j < middle; ++j) {
            cube[i].emplace_back(std::vector<T>());
            for (int k = 0; k < inner; ++k) {
                if (zero) cube[i][j].emplace_back(0);
                    // todo use c++ 11 instead
                    // todo fix this concerning templates
                else cube[i][j].emplace_back(((T) rand() / RAND_MAX) * (1 - 0));
            }
        }
    }
}

template <typename T>
CubeArray<T>::CubeArray(std::vector<std::vector<std::vector<T>>> cube_) {
    cube = std::move(cube_);
}


template <typename T>
double CubeArray<T>::calc(SquareArray<T> x, size_t outer) {
    double sum = 0;
    for (size_t inner = 0; inner < 5; inner++) {
        for (size_t index = 0; index < 5; index++) {
            double value = x[inner][index] - this->cube[outer][inner][index];
            sum += std::pow(value, 2);
        }
    }
    return -sum;
}

template <typename T>
SquareArray<T> CubeArray<T>::operator[](size_t i) const {
    return SquareArray(cube[i]);
}

template <typename T>
CubeArray<T> operator*(T y, CubeArray<T> x) {
    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j < x[i].size(); ++j) {
            for (int k = 0; k < x[i][j].size(); ++k) {
                x[i][j][k] *= y;
            }
        }
    }
    return x;
}

template <typename T>
size_t CubeArray<T>::size() {
    return cube.size();
}

template <typename T>
CubeArray<T> CubeArray<T>::operator/(T y) {
    auto x = *this;
    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j < x[i].size(); ++j) {
            for (int k = 0; k < x[i][j].size(); ++k) {
                x[i][j][k] /= y;
            }
        }
    }
    return x;
}

template <typename T>
CubeArray<T> CubeArray<T>::operator*(T y) {
    auto x = *this;
    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j < x[i].size(); ++j) {
            for (int k = 0; k < x[i][j].size(); ++k) {
                x[i][j][k] *= y;
            }
        }
    }
    return x;
}

template <typename T>
CubeArray<T> CubeArray<T>::operator+=(CubeArray<T> y) {
    for (int i = 0; i < this->size(); ++i) {
        for (int j = 0; j < (*this)[i].size(); ++j) {
            for (int k = 0; k < (*this)[i][j].size(); ++k) {
                (*this)[i][j][k] += y[i][j][k];
            }
        }
    }
    return *this;
}

template <typename T>
void CubeArray<T>::print() {
    for(auto square : cube){
        for(auto vec : square){
            for(auto value : vec){
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

// -----------------------------------------------------------------------
// ---------------------------- SQUARE ARRAYS ----------------------------
// -----------------------------------------------------------------------

template <typename T>
SquareArray<T>::SquareArray(std::vector<T> x) {
    arr = x;
}

template <typename T>
size_t SquareArray<T>::size() {
    return nrows;
}


template <typename T>
SquareArray<T>::SquareArray(size_t nrows_, size_t ncols_) {
    nrows = nrows_;
    ncols = ncols_;
    arr = std::vector<T>();
    for (int i = 0; i < nrows_*ncols_; ++i){
        // todo use c++11 instead
        arr.emplace_back(((T) rand() / RAND_MAX) * (1 - 0));
    }
}

/*
template <typename T>
std::vector<T> SquareArray<T>::operator[](size_t i) {
    return std::vector<T>(arr.begin() + nrows * i, arr.begin() + nrows * i + ncols);
}
 */

template <typename T>
std::vector<T> & SquareArray<T>::operator[](size_t i) {
    return arr[i];
}

template <typename T>
std::vector<T> SquareArray<T>::operator[](size_t i) const {
    return arr[i];
}

template <typename T>
SquareArray<T> operator*(T x, SquareArray<T> y) {
    for (int i = 0; i < y.size(); i++) {
        for (auto &val : y[i]) {
            val *= x;
        }
    }
    return y;
}

template <typename T>
SquareArray<T> SquareArray<T>::operator-(std::vector<std::vector<T>> y) {
    for (int i = 0; i < size(); ++i) {
        for (int j = 0; j < arr[i].size(); ++j) {
            (*this)[i][j] = (*this)[i][j] - y[i][j];
        }
    }
    return *this;
}

template <typename T>
SquareArray<T> SquareArray<T>::operator+=(SquareArray<T> y) {
    for (int i = 0; i < size(); ++i) {
        for (int j = 0; j < y.size(); ++j) {
            arr[i][j] += y[i][j];
        }
    }
    return SquareArray(*this);
}

template <typename T>
SquareArray<T> SquareArray<T>::operator-=(SquareArray<T> y) {
    for (int i = 0; i < size(); ++i) {
        for (int j = 0; j < y.size(); ++j) {
            arr[i][j] -= y[i][j];
        }
    }
    return SquareArray(*this);
}

template <typename T>
SquareArray<T> SquareArray<T>::operator-(SquareArray<T> y) const {
    auto x = *this;
    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j < x[i].size(); ++j) {
            x[i][j] -= y[i][j];
        }
    }
    return x;
}

template <typename T>
SquareArray<T> SquareArray<T>::operator+(T y) {
    auto x = *this;
    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j < x[i].size(); ++j) {
            x[i][j] += y;
        }
    }
    return x;
}

template <typename T>
SquareArray<T> SquareArray<T>::operator*(T y) {
    auto x = *this;
    for (int i = 0; i < x.size(); ++i) {
        for (int j = 0; j < x[i].size(); ++j) {
            x[i][j] *= y;
        }
    }
    return x;
}

template <typename T>
void SquareArray<T>::flat(std::vector<float> &out) {
    for (size_t i = 0; i < size(); i++){
        for (size_t j = 0; j < size(); j++){
            out[size() * i + j] = arr[i][j];
        }
    }
    // return out;
}

template <typename T>
void SquareArray<T>::print(){
    for(const auto& vec : arr){
        for(auto& value : vec){
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

template <typename T>
SquareArray<T> operator+(T x, SquareArray<T> y) {
    for (int i = 0; i < y.size(); ++i) {
        for (int j = 0; j < y[i].size(); ++j) {
            y[i][j] += x;
        }
    }
    return y;
}

template<typename T>
void SquareArray<T>::concatenate(SquareArray<T> x) {
    if (x.size() > 1 || x[0].size() != this->size()) {
        std::cerr << "cannot concatenate, wrong array dimensions" << std::endl;
        // todo better error handling, this is also just a random number
        exit(159);
    }
    for (int i = 0; i < this->size(); ++i) {
        this->arr[i].emplace_back(x[0][i]);
    }
}

template <typename T>
std::vector<std::vector<T>> SquareArray<T>::get_slices(size_t outer_from, size_t outer_to, size_t inner_from, size_t inner_to) {
    std::vector<std::vector<T>> ans;

    for (int i = outer_from; i < outer_to; ++i) {
        ans.emplace_back(std::vector<T>());
        for (int j = inner_from; j < inner_to; ++j) {
            ans[i - outer_from].emplace_back((*this)[i][j]);
        }
    }
    return ans;
}

template <typename T>
size_t SquareArray<T>::index(size_t x, size_t y){
    return x * nrows + y;
}

template class CubeArray<double>;
template class CubeArray<int>;
template class SquareArray<double>;
template class SquareArray<int>;
