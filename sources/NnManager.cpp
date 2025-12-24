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

void NnManager::compute_accuracy_global(size_t index){
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

void NnManager::compute_accuracy(size_t index){
    std::vector<size_t> occurrences_real(10,0);
    std::vector<size_t> occurrences_found(10,0);
    for(size_t i = 0; i<dataReader->get_num_images_Test();i++){
        Matrix X_test(1,784,dataReader->getImage(i,true));
        Matrix Y_test = setOfNetworks[index]->forward(X_test);
        occurrences_real[(size_t)*dataReader->getLabel(i,true)]++;
        if((size_t)*dataReader->getLabel(i,true) == Y_test.getMaxIndex()){
            occurrences_found[(size_t)*dataReader->getLabel(i,true)]++;
        }
    }
    for(size_t i = 0; i<occurrences_real.size();i++){
        double ratio = ((occurrences_real[i]!=0) ? (100*(double)occurrences_found[i]/(double)occurrences_real[i]) : 0.f);
        std::cout<<i<<": "<<occurrences_found[i]<< "/"<<occurrences_real[i]<<"   "<<ratio<<"%\n";
    }
}

bool NnManager::pushNn(NeuralNetwork *n){
    if(!n) return false;
    if(existingIds.find(n->getId())!= existingIds.end()){
        std::cerr<<"Error : id"<<n->getId()<<" already exists"<<std::endl;
        return false;
    }
    if(n_neural_networks>=n_max_networks){   //needs to realocation
        //realocate
        size_t new_capacity = (n_max_networks == 0) ? 1 : n_max_networks * 2;
        NeuralNetwork **bigger = new NeuralNetwork*[new_capacity]();
        for(size_t i = 0; i<n_neural_networks; i++){
            bigger[i] = setOfNetworks[i]; //copies all previous ptrs
        }
        delete []setOfNetworks;

        setOfNetworks = bigger;
        n_max_networks = new_capacity;
    }
    existingIds[n->getId()] = n_neural_networks;
    std::cout<<"pushing id : "<<n->getId() <<std::endl;
    setOfNetworks[n_neural_networks++] = n;
    return true;
}

bool NnManager::deleteNn(const uint32_t &id){
    if(existingIds.find(id) == existingIds.end()){
        std::cerr<<"Error, id not found in the network manager"<<std::endl;
        return false;
    }
    
    size_t index = existingIds[id];
    

    if (index >= n_neural_networks) {
        return false; 
    }
    if(setOfNetworks[index]){
        std::cout<<index<<std::endl;
        delete setOfNetworks[index];
        for(size_t i = index;i<n_neural_networks-1; i++){
            setOfNetworks[i] = setOfNetworks[i+1];
        }

        setOfNetworks[n_neural_networks-1] = nullptr;
        n_neural_networks--;
        existingIds.erase(id);

        for (auto mapIt = existingIds.begin(); mapIt != existingIds.end(); ++mapIt) {
            if (mapIt->second > index) {
                --mapIt->second;
            }
        }

        return true;
    }
    std::cout<<"unsucessfull delete"<<std::endl;
    return false;
}

void NnManager::showExistingIds(){
    for (const auto& pair : existingIds) {
        std::cout << "id = " << pair.first
                  << ", index = " << pair.second << '\n';
    }
}

size_t NnManager::get_n_neural_networks() const{
    return n_neural_networks;
}

NeuralNetwork **NnManager::get_setofNetWorks() const{
    if(!setOfNetworks){
        std::cerr<<"No set of network initalized"<<std::endl;
        return nullptr;
    }
    return setOfNetworks;
}

NeuralNetwork *NnManager::getNetworkFromId(const uint32_t id) const{
    auto it = existingIds.find(id);
    if(it == existingIds.end()){
        std::cerr<<"Error : no network found with ID : "<<id<<std::endl;
        return nullptr;
    } else {
    }
    return setOfNetworks[it->second];
}

void NnManager::showNetWorks() const{                       //todelete
    for(size_t i = 0; i<n_neural_networks;i++){
        std::cout<<setOfNetworks[i]->getId()<<std::endl;
    }
    std::cout<<"\n\n";
}

bool NnManager::init_dataReader(){
    dataReader = new MNISTReader();
    return (!dataReader ? false : true);
    
}

void shuffleIndexes(std::vector<size_t> &v, const unsigned int seed){
    std::mt19937 rng(seed);
    std::shuffle(v.begin(), v.end(), rng);
}
