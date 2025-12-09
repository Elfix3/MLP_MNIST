#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include "Layer.h"


class NeuralNetwork{
    private:

        Layer **layers;
        size_t n_layers;
        mutable Matrix Y;

    public:
        NeuralNetwork(size_t n_first_input, const std::vector<std::pair<size_t, ActivationType>> &v);
        ~NeuralNetwork();

        Matrix forward(const Matrix input) const;
        double loss(const Matrix output,size_t correct_digit) const;
        void backward(const Matrix& first_dA);
        void update();

        const Layer* getLayer(size_t index) const; 

        void debug();
};

#endif