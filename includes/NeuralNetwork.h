#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include "Layer.h"


class NeuralNetwork{
    private:
        Layer **layers;
        size_t n_layers;

    public:
        NeuralNetwork(size_t n_first_input, const std::vector<std::pair<size_t, ActivationType>> &v, size_t n_final_output);
        ~NeuralNetwork();

        Matrix forward(const Matrix input) const;
            
        void debug();
};

#endif