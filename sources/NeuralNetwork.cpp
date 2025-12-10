#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(size_t n_first_input,const std::vector<std::pair<size_t, ActivationType>> &v){
    if(v.size()<2){
        throw std::invalid_argument("Please add at least 2 layers");
    }
    n_layers = v.size();
    layers = new Layer*[v.size()];
    for(size_t i = 0;i<v.size(); i++){
        size_t input = (i==0 ? n_first_input: v[i-1].first);
        size_t output = v[i].first;
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

Matrix NeuralNetwork::forward(const Matrix input) const{
    Matrix out = input;
    for(size_t i = 0 ; i<n_layers;i++){ 
        out = layers[i]->forward(out);
        
    }
    Y = out;
    return out;
}

void NeuralNetwork::backward(const Matrix &first_dA){
    Matrix prop_dA = first_dA;
    
    for (int i = n_layers - 1; i >= 0; i--){ 
        prop_dA = layers[i]->backward(prop_dA);
        
    }
}

void NeuralNetwork::update(){
    for(size_t i = 0; i<n_layers;i++){
        layers[i]->update();
    }
}

double NeuralNetwork::loss(const Matrix output, size_t correct_digit) const{
    //1 label
    assert((correct_digit>=0 && correct_digit<=9)  && "digit must be between 0 and 9");
    double eps = 1e-15;
    return  -std::log(std::max(output(0,correct_digit),eps));
}

double NeuralNetwork::lossBatch(const Matrix &output, const Matrix &Y) const {
    assert(output.rows() == Y.rows() && output.cols() == Y.cols());
    double eps = 1e-15;
    double total_loss = 0.0;
    size_t batch_size = output.rows();

    for (size_t i = 0; i < batch_size; i++) {
        for (size_t j = 0; j < output.cols(); j++) {
            if (Y(i,j) == 1.0) {
                total_loss -= std::log(std::max(output(i,j), eps));
                break; // une seule classe à 1
            }
        }
    }

    return total_loss / batch_size; // loss moyenne
}

const Layer *NeuralNetwork::getLayer(size_t index) const{
    if(index >= n_layers) return nullptr;
    return layers[index];
}

void NeuralNetwork::debug(){
    
}
