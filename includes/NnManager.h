#ifndef NNMANAGER_H
#define NNMANAGER_H

#include "MNISTReader.h"
#include "NeuralNetwork.h"

class NnManager{
    private:
        MNISTReader *dataReader;
        NeuralNetwork **setOfNetworks;
        size_t n_neural_networks;

        public:

};

#endif