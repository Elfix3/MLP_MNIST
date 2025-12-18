#ifndef TRAINING_H
#define TRAINING_H

#include <iostream>

struct TrainingConfig{
    double learningRate = 0.05;
    size_t epochs  = 10;
    size_t batchSize = 32;
    bool shuffle = true;
    unsigned int seed = 42;
};


class Training{
    private :
        TrainingConfig conf;
    
    public :
        Training() = default;




};

#endif