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
    A_prev = X;                                     //we store the previous X
    Z = X*W+b.broadcastRows(X.rows());              //forward prop equation
    
    switch(type){
        case RELU :
            A = Z.RELU();                           //max(Z,0)
        break;
        case SOFTMAX :
            A = Z.SOFTMAX();                        //turns the last n categories into probabilistic distribution
        break;
        default :                                   //no sigmoid or tanh will be implemented
        break;
    }
    
    return A;
}
Matrix Layer::backward(const Matrix &dA){
    int n = dA.cols();                              //batch size
    Matrix dZ;                                      //variation of the activation
    Matrix maskZ = Z;                               //f'Z
    
    switch(type){
        case SOFTMAX :
            dZ = dA;                                //derivative dZ is the same as dA ONLY FOR SOFTMAX

            dW = (A_prev.transpose()*dZ) / n;       //backprop equation
            dB = dZ.sum_rows() / n;
        break;
        case RELU :
            
            for(size_t i = 0 ; i<Z.rows(); i ++){
                for(size_t j = 0; j<Z.cols();j++){
                    if(maskZ(i,j)<0){
                        maskZ(i,j) = 0;
                    } else {
                        maskZ(i,j) = 1;
                    }
                }
            
            }
            
            dZ = dA^maskZ;                          //computation of the derivative dZ
            
            dW = (A_prev.transpose()*dZ) / n;       //backprop equation
            dB = dZ.sum_rows() / n;

            
        break;
        default :
        break;
    }
    
    
    Matrix dA_prev = dZ*W.transpose();              //computes the previous dA   
    return dA_prev;
}

void Layer::update(const double &learning_rate){
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

void Layer::recap() const {
    std::cout<<"Number of neurals : "<<n_neural<<"\n";
    std::cout<<"Weights size :";
    W.printSize();
    std::cout<<"Bias size :";
    b.printSize();
    
    std::cout<<"dW size :";
    dW.printSize();
    std::cout<<"Bias size :";
    dB.printSize();


}
