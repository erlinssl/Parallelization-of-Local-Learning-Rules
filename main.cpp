#include <filesystem>
#include <iostream>
#include <fstream>
#include <chrono>

#include "Arrays.h"
#include "Model.h"

#include "dependencies/matplotlibcpp.h"

namespace plt = matplotlibcpp;

double figsize_scale = 0.2;
// TODO Figure out how to set rcParams in matplotlib-cpp


const static int GRID_SIZE = 4;
const static int RESOLUTION = 5;

CubeArray<double> get_data() {
    std::cout << "getting data" << std::endl;

    if (std::filesystem::exists("trainingdata")) {
        std::cout << "found training data" << std::endl;

        std::ifstream f("trainingdata", std::ios::binary | std::ios::in);
        // ignore until image data
        f.ignore(16);

        std::vector<std::vector <std::vector<double>>> multi_pic_vector;
        for(int i = 0; i < 60000; ++i) {
            std::vector<std::vector<double>> single_pic_vector;
            single_pic_vector.reserve(28);
            for(int j = 0; j < 28; ++j) {
                single_pic_vector.emplace_back(std::vector<double>());
            }
            multi_pic_vector.emplace_back(single_pic_vector);
        }

        char b;
        unsigned int rowCounter = 0;
        unsigned int picCounter = 0;
        unsigned int columnCounter = 0;
        while (f.get(b)) {
            multi_pic_vector[picCounter][rowCounter].emplace_back(((double) b) / 255.0);
            columnCounter++;
            if (columnCounter > 27) {
                rowCounter++;
                columnCounter = 0;
                if (rowCounter > 27) {
                    picCounter++;
                    rowCounter = 0;
                }
            }
        }
        std::cout << "number of pictures: " << picCounter << std::endl;
        return CubeArray<double>(multi_pic_vector);
    }
    else {
        std::cerr << "could not find training data, downloading not yet implemented" << std::endl;
        exit(1);
    }
}

// auto data = get_data();

template <typename T>
CubeArray<T> get_batch(size_t batch_size){
    // TODO Uncomment when functionality implemented \
        ALSO make batch resolution dependent on model resolution
    /*
    // batch_indexes = np.floor(np.random.rand(batch_size, 1) * data.shape[0]).astype(int)
    // todo don't know if this needs to be nested, but it is nested in python :)
    std::vector<std::vector<double>> rand_array;
    rand_array.emplace_back(std::vector<double>());
    for (int i = 0; i < batch_size; ++i) {
        rand_array[0].emplace_back((double)rand()/(double)RAND_MAX);
    }
    SquareArray s(rand_array);
    SquareArray<double> multiplied_array = (s * 60000);
    SquareArray<int> batch_indexes(multiplied_array.size(), multiplied_array[0].size());
    for (int i = 0; i < multiplied_array.size(); ++i) {
        for (int j = 0; j < multiplied_array[i].size(); ++j) {
            batch_indexes[i].emplace_back((int) multiplied_array[i][j]);
        }
    }

    // batch_indexes = np.concatenate((batch_indexes, np.floor(2 + np.random.rand(batch_size, 1) * (data.shape[1] - 4)).astype(int)), axis=1)
    auto random_multiplied_square = (SquareArray<double>(batch_size, 1) * (28 - 4)) + 2;
    SquareArray<int> floored(random_multiplied_square.size(), random_multiplied_square[0].size());
    for (int i = 0; i < random_multiplied_square.size(); ++i) {
        for (int j = 0; j < random_multiplied_square[i].size(); ++j) {
            floored[i].emplace_back(random_multiplied_square[i][j]);
        }
    }
    */

    // todo uncomment when operator has been added and proper templates have been made
    // batch_indexes = batch_indexes[0] + floored[0];
    /*
    batch_indexes = np.concatenate((batch_indexes, np.floor(2 + np.random.rand(batch_size, 1) * (data.shape[1] - 4)).astype(int)), axis=1)
    batch_indexes = np.concatenate((batch_indexes, np.floor(2 + np.random.rand(batch_size, 1) * (data.shape[2] - 4)).astype(int)), axis=1)

    batch = np.zeros((batch_size, 5, 5))
    for count, index in enumerate(batch_indexes):
    batch[count] = data[index[0], index[1] - 2:index[1] + 3, index[2] - 2:index[2] + 3]

    return batch

     */

    // TODO Remove placeholder when get_batch functionality finalized
    CubeArray<double> cube(true, batch_size, RESOLUTION, RESOLUTION);
    return cube;
}

template <typename T>
void experiment(const char subfigure, double sigma, double lambda_, size_t nbatches){
    // TODO Set random seed for consistent experiments
    Model<T> model(sigma, lambda_, GRID_SIZE, RESOLUTION);

    size_t batch_size = 100;

    for (size_t i = 0; i < nbatches; i++){
        auto start = std::chrono::high_resolution_clock::now();
        CubeArray<T> batch = get_batch<double>(batch_size);
        for (size_t j = 0; j < batch_size; j++){
            model.update(batch[j]);
        }
        auto stop = std::chrono::high_resolution_clock::now();
        std::cout << "CO3: Completed batch " << i+1 << " @ " << batch_size << " after " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
        << "ms" << std::endl;
    }

    model.save(subfigure);
}

template <typename T>
void figure(const Model<T>& model){
    std::vector<float> z(model.resolution * model.resolution);
    const int nrows = (int) std::sqrt(model.filters), ncols = (int) std::sqrt(model.filters);
    const float* zptr = &(z[0]);
    std::vector<int> ticks = {};
    const int colors = 1;

    for(int row = 0; row < nrows; row++){
        for(int col = 0; col < ncols; col++){
            size_t index = row * nrows + col;
            model.w[index].flat(z);

            plt::subplot2grid(nrows, ncols, row, col, 1, 1);
            plt::imshow(zptr, model.resolution, model.resolution, colors);
            plt::xticks(ticks);
            plt::yticks(ticks);
            plt::plot();
        }
    }
}

template <typename T>
void save_all(){
    std::vector<char> subfigures = {'a'}; //, 'b', 'c', 'd'};
    plt::Plot plot("sub_fig");

    Model<T> model(1.0, 0.5, GRID_SIZE, RESOLUTION);

    for (char fig : subfigures){
        std::cout << "Handling fig " << fig << std::endl;
        model.load(fig);
        figure(model);
        plt::show();
        /*
         std::string path = "../saved/figure2"
         path.push_back(fig);
         path.append(".pgf");
         plt::save(path);
        */
    }
}

int main() {
    //todo what does this do
    srand((unsigned)time(nullptr));
    const double learning_rate = .1;

    /////// EXPERIMENTS
    experiment<double>('a', 1.0, 0.5, 1000);
    // experiment<double>('b', 1.0, 0.5, 10000);
    experiment<double>('c', 0.5, 0.5, 1000);
    // experiment<double>('d', 1.0, 1.0/9.0, 1000);

    /////// SHOWING FIGURES
    save_all<double>();
    // get_data();
    Py_Finalize();
    std::cout << "end of main" << std::endl;
    return 0;
}