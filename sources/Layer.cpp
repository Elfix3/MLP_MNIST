#include "Layer.h"

Layer::Layer(size_t n_input_parameters, size_t n_neural, ActivationType type) :
    W(n_input_parameters, n_neural,NORMALIZED),
    b(1,n_neural,NORMALIZED),
    type(type),
    n_neural(n_neural)
{}

const Matrix &Layer::forward(const Matrix &X){
    assert(X.rows() == 1);

    Z = X*W+b;
    switch(type){
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

const size_t &Layer::get_n_neural() const{
    return n_neural;
}
