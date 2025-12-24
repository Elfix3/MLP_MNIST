#ifndef NNMANAGER_H
#define NNMANAGER_H

#include <unordered_set>  
#include <unordered_map>

#include "MNISTReader.h"
#include "NeuralNetwork.h"



class NnManager{
    private:
        MNISTReader *dataReader =nullptr;
        NeuralNetwork **setOfNetworks;
        size_t n_neural_networks;
        size_t n_max_networks = 10;

        TrainingConfig *conf = nullptr;
        std::unordered_map<uint32_t,size_t> existingIds;
    public:
        NnManager();
        ~NnManager();

        bool init_dataReader();


        void train(size_t index);                      //trains a network by its index 
        void compute_accuracy_global(size_t index);
        void compute_accuracy(size_t index);

        bool pushNn(NeuralNetwork *n);                  //true for successful push, false for unsuccessful
        bool deleteNn(const uint32_t &id); 

        void showExistingIds();                         //helper function to delete
        //popNn(); 
        size_t get_n_neural_networks() const;
        NeuralNetwork** get_setofNetWorks() const;
        NeuralNetwork * getNetworkFromId(const uint32_t id) const;

        void showNetWorks() const;

};

void shuffleIndexes(std::vector<size_t>& v, const unsigned int seed);

#endif