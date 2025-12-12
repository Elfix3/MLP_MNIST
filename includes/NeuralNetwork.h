#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include "Layer.h"


class NeuralNetwork{
    private:

        Layer **layers;
        size_t n_layers;
        mutable Matrix Y;

    public:
        
        NeuralNetwork(size_t n_first_input, const std::vector<std::pair<size_t, ActivationType>> &v);   //Sets up a Neural Network with a given conf
        /*################ EXAMPLE OF NETWORK CONFING #########################
        std::vector<std::pair<size_t,ActivationType>> network_config = {
            {32, RELU},
            {16, RELU},
            {10, SOFTMAX}
        };
         ###################################################################*/

        ~NeuralNetwork();

        Matrix forward(const Matrix input) const;                                                        //Forward propagation, stores the result in Matrix Y
        void backward(const Matrix& first_dA);                                                           //Backward propagation
        void update(const double& learning_rate=0.03);                                                   //updates all the weights and biases with a given lr (same for all)
        
        double loss(const Matrix output,size_t correct_digit) const;                                     //Computes Loss on a single data example
        double lossBatch(const Matrix &output, const Matrix &Y) const;                                   //Comptes the Loss on a complete batch
        
        const Layer* getLayer(size_t index) const;                                                       //gets specific layer by index
};

#endif