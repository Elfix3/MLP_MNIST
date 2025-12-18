#ifndef NNMANAGER_H
#define NNMANAGER_H

#include "MNISTReader.h"
#include "NeuralNetwork.h"

struct TrainingConfig{
    double learningRate = 0.02;
    size_t epochs  = 10;
    size_t batchSize = 64;
    bool shuffle = true;
    unsigned int seed = 323;
};


class NnManager{
    private:
        MNISTReader *dataReader =nullptr;
        NeuralNetwork **setOfNetworks;
        size_t n_neural_networks;
        size_t n_max_networks = 10;

        TrainingConfig *conf = nullptr;

    public:
        NnManager();
        ~NnManager();

        bool init_dataReader();


        void train(size_t index);                      //trains a network by its index
        void compute_accuracy(size_t index);

        void pushNn(NeuralNetwork *n);
        bool deleteNn(size_t index); 

        //popNn(); 

};

void shuffleIndexes(std::vector<size_t>& v, const unsigned int seed);

#endif