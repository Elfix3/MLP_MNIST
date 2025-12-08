#ifndef LAYER_H
#define LAYER_H

#include "Matrix.h"
#include "assert.h"

enum ActivationMode{
    RELU,
    SOFTMAX,
};

class Layer{
    private :
        Matrix W; //poids biais 
        Matrix b;
        Matrix Z; //sortie
        Matrix A; //activation après relu/softmax
        ActivationMode mode;

    
    public :
        Layer(size_t n_input_parameters, size_t n_neural, ActivationMode mode=RELU);
        const Matrix& forward(const Matrix &X); //from X and defined W and b, computes Z and A;


        //getters
        const Matrix& getW() const;
        const Matrix& getb() const;
        const Matrix& getZ() const;
        const Matrix& getA() const;
};
#endif