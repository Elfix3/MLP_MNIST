#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include "Layer.h"
#include <fstream>

class NeuralNetwork{
    private:
        uint32_t id = 12; //default ID for test purposes
        size_t inputSize = 784; //is the size of the image/data processed
        Layer **layers;
        size_t n_layers;
        mutable Matrix Y;

    public:
        NeuralNetwork();
        NeuralNetwork(size_t n_first_input, const std::vector<std::pair<size_t, ActivationType>> &v);   //Sets up a Neural Network with a given conf
        /*################ EXAMPLE OF NETWORK CONFING #########################
        std::vector<std::pair<size_t,ActivationType>> network_config = {
            {32, RELU},
            {16, RELU},
            {10, SOFTMAX}
        };
         ###################################################################*/
        NeuralNetwork(const char* filename);

        ~NeuralNetwork();

        Matrix forward(const Matrix input) const;                                                        //Forward propagation, stores the result in Matrix Y
        void backward(const Matrix& first_dA);                                                           //Backward propagation
        void update(const double& learning_rate=0.05);                                                   //updates all the weights and biases with a given lr (same for all)
        
        double loss(const Matrix output,size_t correct_digit) const;                                     //Computes Loss on a single data example
        double lossBatch(const Matrix &output, const Matrix &Y) const;                                   //Comptes the Loss on a complete batch
        
        const Layer* getLayer(size_t index) const;                                                       //gets specific layer by index
        
        void save(const char*);                                                                                     //
        void load(const char* filename);

        static uint32_t bigToLittleEndian(uint32_t little);
};

#endif