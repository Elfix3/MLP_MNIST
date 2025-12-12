#ifndef LAYER_H
#define LAYER_H

#include "Matrix.h"
#include "assert.h"

enum ActivationType {
    RELU,
    SIGMOID,
    TANH,
    SOFTMAX
};
class Layer{
    private :
        Matrix A_prev;
        Matrix W; //poids biais 
        Matrix b;
        Matrix Z; //sortie
        Matrix A; //activation après relu/softmax
        ActivationType type;

        Matrix dW;
        Matrix dB;

        size_t n_neural;//nombre de neurones-
    
    public :
        Layer(size_t n_input_parameters, size_t n_neural, ActivationType type=RELU);
        const Matrix& forward(const Matrix &X); //from X and defined W and b, computes Z and A;
        Matrix backward(const Matrix &dA);

        void update(const double& learning_rate);

        //getters
        const Matrix& getW() const;
        const Matrix& getb() const;
        const Matrix& getZ() const;
        const Matrix& getA() const;
        const Matrix& getdW() const;
        const Matrix& getdB() const;
        
        const size_t& get_n_neural() const; 

        void recap() const ;
};
#endif