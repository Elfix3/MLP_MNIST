#include "Layer.h"

Layer::Layer(size_t n_input_parameters, size_t n_neural, ActivationType type) :
    W(n_input_parameters, n_neural,NORMALIZED),
    b(1,n_neural,NORMALIZED),
    type(type),
    dW(n_input_parameters, n_neural,static_cast<double>(0)),
    dB(1,n_neural,static_cast<double>(0)),
    n_neural(n_neural)
{}

const Matrix &Layer::forward(const Matrix &X){
    assert(X.rows() == 1);
    A_prev = X;
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

Matrix Layer::backward(const Matrix &dA){
    int n = dA.cols(); //batch size
    Matrix dZ;
    Matrix maskZ = Z; //f'Z
    switch(type){
        case SOFTMAX :
            dZ = dA; //for softmax only
            dW = (dZ*A_prev.transpose()) / n;
            dB = dZ.sum_columns() / n;
        break;
        case RELU :
            for(size_t i = 0 ; i<Z.rows(); i ++){
                for(size_t j = 0; j<Z.cols();j++){
                    if(maskZ(i,j)<0){
                        maskZ(i,j) = 0;
                    } else {
                        maskZ(i,j) = 0;
                    }
                }
            }
            dZ = dA^maskZ;
            dW = (dZ * A_prev.transpose()) / n;
            dB = dZ.sum_columns() / n;
            
        break;
        default :
        break;
    }
    Matrix dA_prev = W.transpose() * dZ;
    return dA_prev;
}

void Layer::update(double learning_rate){
    W-= dW*learning_rate;
    b-= dB*learning_rate;
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

const Matrix &Layer::getdW() const{
    return dW;
}

const Matrix &Layer::getdB() const{
    return dB;
}

const size_t &Layer::get_n_neural() const{
    return n_neural;
}
