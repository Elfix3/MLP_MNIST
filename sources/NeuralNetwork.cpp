#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(size_t n_first_input,const std::vector<std::pair<size_t, ActivationType>> &v, size_t n_final_final_output){
    if(v.size()<2){
        throw std::invalid_argument("Please add at least 2 layers");
    }
    n_layers = v.size();
    layers = new Layer*[v.size()];
    for(size_t i = 0;i<v.size(); i++){
        size_t input = (i==0 ? n_first_input: v[i-1].first);
        size_t output = (i+1 == v.size()) ?  n_final_final_output : v[i].first;
        ActivationType act  = v[i].second;

        std::cout<<input<<" "<<output<< "\n";
        layers[i] = new Layer(input,output,act);
    }
}
    

NeuralNetwork::~NeuralNetwork(){
    for(size_t i = 0; i< n_layers; i ++){
        delete layers[i];
    }
    delete []layers;
}

Matrix NeuralNetwork::forward(const Matrix input) const
{
    return   layers[2]->forward(layers[1]->forward(layers[0]->forward(input)));
}

void NeuralNetwork::debug(){
    std::cout<<n_layers<<std::endl;
    std::cout<<layers[0]->get_n_neural()<<std::endl;
    std::cout<<layers[1]->get_n_neural()<<std::endl;
}
