#include "NnManager.h"

NnManager::NnManager() : n_neural_networks(0), n_max_networks(10), conf(new TrainingConfig()){
    dataReader = new MNISTReader();                 //should be built once
    setOfNetworks = new NeuralNetwork*[10]();       //directly allocates 10 neuralnetwork slots
}

NnManager::~NnManager(){
    for(size_t i = 0; i<n_max_networks;i++){
        delete setOfNetworks[i];
    }
    delete []setOfNetworks;
}


void NnManager::train(size_t index){
    if(!conf){                                              //replace by is valid conf
        std::cerr<<"No conf defined\n"<<std::endl;
        return;
    }
    if(!setOfNetworks[index]){
        std::cerr<<"No network found at this index\n";
        return;
    }
    if(!dataReader){
        std::cerr<<"No data reader defined\n";
        return;
    }
    size_t train_size = static_cast<size_t>(dataReader->get_num_images_Train());
    std::vector<size_t> globalIndexes(train_size);                                                          //vector with numImages indexes
    std::iota(globalIndexes.begin(),globalIndexes.end(), 0);                                                //all index prepared from 0,1,2,... to    dataReader->get_num_images_Train()-1
    
    for(size_t epoch = 0; epoch<conf->epochs; epoch++){
        
        shuffleIndexes(globalIndexes, conf->seed+epoch);                                                    //all indexes are now shuffled
        
        for(size_t batch_start = 0; batch_start<train_size; batch_start+=conf->batchSize){
            
            size_t batch_end = std::min(batch_start + conf->batchSize, train_size);
            
            std::vector<size_t> batchIndex(globalIndexes.begin()+batch_start,globalIndexes.begin()+batch_end);

            Matrix X = dataReader->X_bach(batchIndex);
            Matrix Y = dataReader->Y_bach(batchIndex);

            Matrix A = setOfNetworks[index]->forward(X.Normalize(255));
            setOfNetworks[index]->backward(A-Y);
            setOfNetworks[index]->update(conf->learningRate);
            std::cout<<setOfNetworks[index]->lossBatch(A,Y)<<std::endl;
        }
        std::cout<<"EPOCH MADE"<<std::endl;
    }
    
}

void NnManager::compute_accuracy(size_t index){
    size_t n_tests = 10000;
    size_t n_success = 0;
    for(size_t i = 0; i<n_tests;i++){
                Matrix X_test(1,784,dataReader->getImage(i,true));
                Matrix Y_test = setOfNetworks[index]->forward(X_test);
                if((int)Y_test.getMaxIndex() == (int)*dataReader->getLabel(i,true))n_success++;
    }
    double accuracy = (double)n_success/(double)n_tests;
    std::cout<<"Accuracy of the network : "<<accuracy<<std::endl;
}

void NnManager::pushNn(NeuralNetwork *n)
{
    if(!n) return;

    if(n_neural_networks>=n_max_networks){   //needs to realocation
        //realocate
        size_t new_capacity = (n_max_networks == 0) ? 1 : n_max_networks * 2;
        NeuralNetwork **biggerSetOfNetworks = new NeuralNetwork*[new_capacity];
        for(size_t i = 0; i<n_max_networks; i++){
            biggerSetOfNetworks[i] = setOfNetworks[i]; //copies all previous
        }
        biggerSetOfNetworks[n_neural_networks] = n;
        delete []setOfNetworks;

        setOfNetworks = biggerSetOfNetworks;
        n_max_networks = new_capacity;
    }
    setOfNetworks[n_neural_networks] = n;
    n_neural_networks++;
}

bool NnManager::deleteNn(size_t index){
    if (index >= n_neural_networks) {
        return false; 
    }
    if(setOfNetworks[index]){
        delete setOfNetworks[index];
        for(size_t i = index;i<n_max_networks-1; i++){
            setOfNetworks[index] = setOfNetworks[index+1];
        }
        setOfNetworks[n_max_networks-1] = nullptr;
        n_neural_networks--;
        return true;
    }
    return false;
}

bool NnManager::init_dataReader(){
    dataReader = new MNISTReader();
    return (!dataReader ? false : true);
    
}

void shuffleIndexes(std::vector<size_t> &v, const unsigned int seed){
    std::mt19937 rng(seed);
    std::shuffle(v.begin(), v.end(), rng);
}
