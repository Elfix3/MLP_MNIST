#include "Layer.h"

Layer::Layer(size_t n_input_parameters, size_t n_neural, ActivationMode mode) : W(n_input_parameters, n_neural,NORMALIZED),b(1,n_neural,NORMALIZED),mode(mode) {
}

const Matrix &Layer::forward(const Matrix &X){
    assert(X.rows() == 1);

    Z = X*W+b;
    switch(mode){
        case RELU :
            A = Z.RELU();
        break;
        case SOFTMAX :
            A = Z.SOFTMAX();

        break;
        
        default :

        break;
    }
    return A;
}

const Matrix &Layer::getW() const{
    return W;
}

const Matrix &Layer::getb() const{
    return b;
}

const Matrix &Layer::getZ() const{
    return Z;
}

const Matrix &Layer::getA() const{
    return A;
}
