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

        TrainingConfig conf;
        std::unordered_map<uint32_t,size_t> existingIds;
    public:
        NnManager();
        ~NnManager();

        void train(const uint32_t& id);                                 //trains a network by its id with the setupTrainingConfig
        void compute_accuracy_global(const uint32_t& id);               //computes accuracy on all tests examples, final ratio
        void compute_accuracy(const uint32_t& id);                      //computes accuracy by digits, one ratio per digit

        bool pushNn(NeuralNetwork *n);                                  //true for successful push, false for unsuccessful
        bool deleteNn(const uint32_t &id);                              //true for successful delete, false for unsuccessful

        void showExistingIds();                                         //helper function to delete

        size_t get_n_neural_networks() const;
        NeuralNetwork** get_setofNetWorks() const;                              //pretty much useless
        NeuralNetwork * getNetworkFromId(const uint32_t id) const;
        TrainingConfig getTrainingConf() const;
        
        void setTrainingConf(const TrainingConfig &newConf);

        void showNetWorks() const;

};

void shuffleIndexes(std::vector<size_t>& v, const unsigned int seed);

#endif