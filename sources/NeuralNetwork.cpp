#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(){}
NeuralNetwork::NeuralNetwork(uint32_t nwId) : id(nwId), layers(nullptr), n_layers(0){}
NeuralNetwork::NeuralNetwork(const uint32_t &nwId, const size_t &n_first_input, const std::vector<std::pair<size_t, ActivationType>> &v) : id(nwId), inputSize(n_first_input) , isTrained(false)
{
    if(v.size()<2){
        throw std::invalid_argument("Please add at least 2 layers");
    }
    n_layers = v.size();
    layers = new Layer*[v.size()];                                      //allocates the memory needed of the number of layers
    for(size_t i = 0;i<v.size(); i++){
        size_t input = (i==0 ? n_first_input: v[i-1].first);            //if it's the first layer, we create first input -> neural layer
        size_t output = v[i].first; 
        ActivationType act  = v[i].second;

        //std::cout<<input<<" "<<output<< "\n";                         //uncomment to see network structure
        layers[i] = new Layer(input,output,act);
    }
}

NeuralNetwork::NeuralNetwork(const char *filename){                                     //here we should get rid of asserts, and just make sure the 

    std::ifstream file(std::string("saved_networks/")+filename, std::ios::binary);
    if(!file){
        throw std::runtime_error("Error : impossible to open file");
    }
    uint32_t magicNumber;
    uint32_t nnId;
    uint32_t nnlayers;
    uint32_t nnInputSize;

    file.read(reinterpret_cast<char*>(&magicNumber), sizeof(magicNumber));
    file.read(reinterpret_cast<char*>(&nnId), sizeof(nnId));
    file.read(reinterpret_cast<char*>(&nnlayers), sizeof(nnlayers));
    file.read(reinterpret_cast<char*>(&nnInputSize), sizeof(nnInputSize));
    assert((magicNumber == 0x4E4E3031 || magicNumber == 0x4E4E3030) && "Magic number checkup failed");         //0x4E4E3301 for trained network, 0x4E4E3300 for untrained network
    

    id = static_cast<uint32_t>(nnId);                          //already uint32_t
    n_layers = static_cast<size_t>(nnlayers);
    inputSize = static_cast<size_t>(nnInputSize);
    isTrained = static_cast<bool>(magicNumber&0x1);
    layers = new Layer*[n_layers];                             // allocates layers

    for(size_t i = 0; i<n_layers;i++){
        uint32_t layer_index;                                   //if we want to index in a different way than the for loop
        uint32_t input;
        uint32_t output;
        uint32_t act;

        file.read(reinterpret_cast<char*>(&layer_index), sizeof(layer_index));
        file.read(reinterpret_cast<char*>(&act), sizeof(act));
        file.read(reinterpret_cast<char*>(&input), sizeof(input));
        file.read(reinterpret_cast<char*>(&output),sizeof(output));

        layers[i] = new Layer(static_cast<size_t>(input),static_cast<size_t>(output), static_cast<ActivationType>(act));
        
        file.read(reinterpret_cast<char*>(layers[i]->getW().getDatas()), sizeof(double)*input*output);
        file.read(reinterpret_cast<char*>(layers[i]->getb().getDatas()), sizeof(double)*output);
    }

}   


NeuralNetwork::~NeuralNetwork(){
    for(size_t i = 0; i< n_layers; i ++){
        
        delete layers[i];
    }
    delete []layers;
}

Matrix NeuralNetwork::forward(const Matrix input) const{
    Matrix out = input;                             //first input is the image Matrix
    for(size_t i = 0 ; i<n_layers;i++){ 
        out = layers[i]->forward(out);              //newt input becomes the new computated output of the previous layer
        
    }
    //Y = out;                                        //We store final output as the Y prediction
    return out;
}

void NeuralNetwork::backward(const Matrix &first_dA){
    Matrix prop_dA = first_dA;                      //use the first dA computed (usally from logloss)    
    for (int i = n_layers - 1; i >= 0; i--){
        prop_dA = layers[i]->backward(prop_dA);     //propagated dA becomes the new computated one
        
    }
}

void NeuralNetwork::update(const double& learning_rate){
    for(size_t i = 0; i<n_layers;i++){
        layers[i]->update(learning_rate);
    }
}

double NeuralNetwork::loss(const Matrix output, size_t correct_digit) const{
    //single label loss
    assert((correct_digit>=0 && correct_digit<=9)  && "digit must be between 0 and 9");
    double eps = 1e-15;
    return  -std::log(std::max(output(0,correct_digit),eps));
}

double NeuralNetwork::lossBatch(const Matrix &output, const Matrix &Y) const {
    assert(output.rows() == Y.rows() && output.cols() == Y.cols());     //form of cumulative loss
    double eps = 1e-15;
    double total_loss = 0.0;
    size_t batch_size = output.rows();                                  //the size of the batch is given by the number of rows

    for (size_t i = 0; i < batch_size; i++) {
        for (size_t j = 0; j < output.cols(); j++) {                    //gets the signle loss
            if (Y(i,j) == 1.0) {
                total_loss -= std::log(std::max(output(i,j), eps));     //cumulates in the total loss
                break;
            }
        }
    }

    return total_loss / batch_size;                                     //averages the loss
}

const Layer *NeuralNetwork::getLayer(size_t index) const{               //returns layer by index
    if(index >= n_layers) return nullptr;
    return layers[index];
}

const uint32_t NeuralNetwork::getId() const{
    return id;
}

const bool NeuralNetwork::getIsTrained() const{
    return isTrained;
}


void NeuralNetwork::markAsTrained(const TrainingConfig &conf){
    isTrained = true;
    lastTrainingConf = conf;
}

void NeuralNetwork::infos(){
    std::cout<<"=== ID : "<<id<<" ==="<<std::endl;
    
    if(!layers){
        std::cerr<<"Error : no layer initalized in the selected network"<<std::endl;
    } else{
        for(size_t i = 0; i<n_layers;i++){
            std::cout<<"Layer "<<i+1<<" : "<<getLayer(i)->getW().rows()<<" -> "<<
            getLayer(i)->getW().cols()<<(getLayer(i)->getType()==RELU ? "\t\tRELU" : "\t\tSOFTMAX")<<std::endl;
        }
    }
    
    std::cout<<(isTrained?"Already trained":"Never trained")<<std::endl;
}

void NeuralNetwork::save(const char* filename){ //path needed
    //opening of the file
    std::ofstream file(std::string("saved_networks/")+filename, std::ios::binary);
    if(!file){
        std::cerr<<"Error with file opening\n";
        return;
    }
    //network header construction
    uint32_t magicNumber = 0x4E4E3030 | isTrained;
    uint32_t n_layers_32 = static_cast<uint32_t>(n_layers);
    uint32_t inputSize_32 = static_cast<uint32_t>(inputSize);
    file.write(reinterpret_cast<const char*>(&magicNumber), sizeof(magicNumber));                   //magic number in char N N 0 1
    file.write(reinterpret_cast<const char*>(&id), sizeof(id));                                     //neural network id
    file.write(reinterpret_cast<const char*>(&n_layers_32), sizeof(n_layers_32));                   //n_layers in uint32_t
    file.write(reinterpret_cast<const char*>(&inputSize_32), sizeof(inputSize_32));                       //should be 784 for 28*28 picture

    for(uint32_t i = 0; i<n_layers_32; i++){
        const Layer *current = this->getLayer((size_t)i);
        uint32_t type = static_cast<uint32_t>(current->getType());                                  //proper cast from ENUM to uint32_t
        uint32_t input_size_32 = static_cast<uint32_t>(current->getW().rows());                     //gets the input size of current layer
        uint32_t output_size_32 = static_cast<uint32_t>(current->getW().cols());
        

        file.write(reinterpret_cast<const char*>(&i), sizeof(i));                                   //index of the layer
        file.write(reinterpret_cast<const char*>(&type), sizeof(type));                             //activation type, 0 for RELU, 3 for SOFTMAX
        file.write(reinterpret_cast<const char*>(&input_size_32), sizeof(input_size_32));           //input size
        file.write(reinterpret_cast<const char*>(&output_size_32), sizeof(output_size_32));          //to output size (each layer is rows*cols)

        const Matrix& W = current->getW();
        const Matrix& b = current->getb();

        assert(W.rows() == input_size_32);
        assert(W.cols() == output_size_32);

        file.write(reinterpret_cast<const char*>(W.getDatas()), sizeof(double)*input_size_32*output_size_32);       //casts the weights to raw binaries
        file.write(reinterpret_cast<const char*>(b.getDatas()), sizeof(double)*output_size_32);                     //casts the biasis
    }
    file.close();
}

void NeuralNetwork::load(const char *filename){                                                                    //not used anymore, called from constructor
    std::cout<<"try";
}

uint32_t NeuralNetwork::bigToLittleEndian(uint32_t big){                                                            //not used anymore
    return( (big&0xFF) << 24| (big&0xFF00) <<8| (big&0xFF0000)>>8 | (big&0xFF000000)>>24);
}
 //structure of header is :
    //4 bytes magic number
    //4 bytes id
    //4 bytes n_layers
    //4 bytes input size
    
    //and then for each layer :
    //4 bytes layer num (0 for first , n_layers-1 for the last)
    //4 bytes activation type
    //4 bytes input_size
    //4 bytes output_size