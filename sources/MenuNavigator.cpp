#include "MenuNavigator.h"


// =========================
// MENU NODES FUNCTIONS
// =========================
MenuNode::MenuNode(std::string title, Action actionFn) : title(title),
action(std::move(actionFn)), fetchNeeded(false) {}                                          //constructor for no fetch needed

MenuNode::MenuNode(std::string title, Action actionFn, Fetch fetchFn) : title(title),
action(std::move(actionFn)), fetchChildren(fetchFn), fetchNeeded(true) {}                   //constructor for fetch needed

MenuNode::~MenuNode(){ clearChildren();}

void MenuNode::addChildren(MenuList children, Action backAction){
    clearChildren();
    options = std::move(children);
    options.push_back(new MenuNode("Back", backAction));
}

void MenuNode::clearChildren(){
    for(MenuNode* node : options){
        delete node;
    }
    options.clear();
}

bool MenuNode::hasChildren(){
    return options.size()>0;
}




MenuNavigator::MenuNavigator(){
    

    manager = new NnManager();
    /* NeuralNetwork *n2 = new NeuralNetwork(52);
    manager->pushNn(n2);

    NeuralNetwork *n3 = new NeuralNetwork(62);
    manager->pushNn(n3); */

    rootMenu = new MenuNode({"MLP training program",nullptr});
    
    Action unimplemented = [this]() {this->unimplemented();};   //for easy passing an unimplemented method
    Action backAction = [this](){back();};
    Action terminateAction = [this](){terminate();};
    Action createNnAction = [this](){createNeuralNetwork();};
    //Fetch fetchNetworks = [this](MenuNode *node){fetchNetworkList(node);};
    MenuList rootMenuOptions = {
        new MenuNode("Create network",createNnAction),
        new MenuNode("Import network",nullptr,[this](MenuNode *node){this->fetchSavedNetworkFiles(node);}),
        new MenuNode("Training config",nullptr, [this](MenuNode *node){this->fetchConfOptions(node);}),
        new MenuNode("Compare", unimplemented),
        new MenuNode("Network list",nullptr,[this](MenuNode *node){this->fetchNetworkList(node);}),
    };
    rootMenu->addChildren(rootMenuOptions,terminateAction);
    menuStack.push(rootMenu);
}

void MenuNavigator::createNeuralNetwork(){
    std::vector<std::pair<size_t, ActivationType>> architecture;
    
    int id = -1;
    int n_layers = -1;
    int n_neurons = -1;
    while(true){
        std::cout<<"Please enter the desired and available network ID : ";
        if(secureIntInputCheck(id,UINT32_MAX>>1)){
            break;
        }
    }

    while(true){
        std::cout<<"Please enter the desired number of layer : ";
        if(secureIntInputCheck(n_layers,10)){                    //we consider max 10 layers
            break;
        }
    }

    for(int i = 0; i<n_layers-1;i++){
        while(true){
            std::cout<<"Please enter num of neurons for layer "<<i+1<<" : ";
            if(secureIntInputCheck(n_neurons,1024)){                            //we consider max 1024 neurons per layer
                architecture.push_back({static_cast<size_t>(n_neurons),RELU});
                break;
            }
        }
    }
    architecture.push_back({10,SOFTMAX});                                        //last activation of digits is 10 with softmax
    


    NeuralNetwork *createdNeuralNw = new NeuralNetwork(static_cast<uint32_t>(id), 784, architecture); //784 because of 28*28 image
    createdNeuralNw->infos();
    manager->pushNn(createdNeuralNw);
}

void MenuNavigator::importNetwork(const std::string &path){
    const char* cpath= path.c_str();
    if(manager->pushNn(new NeuralNetwork(cpath))){
        std::cout<<"Successful import of the file, to use it go to NetworkList"<<std::endl;
        this->backToMainMenu();
    } else {
        std::cout<<"Error during push, file not imported"<<std::endl;
    }
}

// =========================
// ACTION FUNCTIONS
// =========================

void MenuNavigator::showInfo(const uint32_t &nwId){
    manager->getNetworkFromId(nwId)->infos();
}

void MenuNavigator::trainNetwork(const uint32_t &nwId){
    manager->train(nwId);
}

void MenuNavigator::evaluateNetwork(const uint32_t &nwId){
    manager->compute_accuracy_global(nwId);
}

void MenuNavigator::evaluatePerDigit(const uint32_t &nwId){
    manager->compute_accuracy(nwId);
}

void MenuNavigator::saveNetwork(const uint32_t &nwId){

    //very suspicious


    std::string fileName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout<<"Please type desired file name : ";
    getline(std::cin, fileName);
    manager->getNetworkFromId(nwId)->save(fileName.c_str());
}

void MenuNavigator::deleteNetwork(MenuNode *node,const uint32_t &nwId){
    MenuList yesNoChoice  = {
        new MenuNode("Yes",[this, nwId](){
            if(manager->deleteNn(nwId)){
                std::cout<<"Delete confirmed, network removed"<<std::endl;
            }
            backToMainMenu();
        }),
        new MenuNode("No", [this](){
            std::cout<<"Delete has been canceled"<<std::endl;
            this->menuStack.pop();
        })
    };
    node->addChildren(yesNoChoice,[this](){this->back();});
}



// =========================
// DYNAMIC FETCH OF THE NODE
// =========================

void MenuNavigator::fetchNetworkList(MenuNode *node){                               //fetch the list of networks, and gives to each networks all desired options
    if(!manager){
        std::cerr<<"Critical error : no network manager found"<<std::endl;
    }
    node->clearChildren();                                                          //need to clear all previous children before loading some new
    MenuList nwList;                                                                //dynamic option
    if(manager->get_n_neural_networks()!=0){
        for(size_t i = 0; i<manager->get_n_neural_networks();i++){
            uint32_t nwId = manager->get_setofNetWorks()[i]->getId();

            MenuNode *nwNode = new MenuNode(std::to_string(nwId),nullptr);
            MenuList nwOptions = {                                                  //Static options to capture nwId
                new MenuNode("Info",[this,nwId](){showInfo(nwId);}),  
                new MenuNode("Train",[this,nwId](){trainNetwork(nwId);}),
                new MenuNode("Global accuracy",[this,nwId](){evaluateNetwork(nwId);}),
                new MenuNode("Accuracy per digits",[this,nwId](){evaluatePerDigit(nwId);}),
                new MenuNode("Save",[this,nwId](){saveNetwork(nwId);}),
                new MenuNode("Delete",[this,nwId,nwNode](){this->deleteNetwork(nwNode,nwId);}),   //add confirmation to delete
            };
            
            nwNode->addChildren(nwOptions,[this](){back();});
            nwList.push_back(nwNode);
        }
    } else {
        nwList.push_back(new MenuNode("No networks found",nullptr));
    }
    node->addChildren(nwList,[this](){this->back();});
    
}

void MenuNavigator::fetchSavedNetworkFiles(MenuNode *node){
    const char* modelFolder = "./saved_networks";
    DIR* dir = opendir(modelFolder);
    if(!dir){
        std::cerr<<"Error : saved_networks folder not found"<<std::endl;
        return;
    }
    struct dirent* entry;
    MenuList fileNames;
    while ((entry = readdir(dir)) != nullptr) {
        std::string name= entry->d_name;
        if (name== "." || name == "..") continue; // ignorer
        fileNames.push_back(new MenuNode(name,[this, name](){importNetwork(name);}));
    }
    node->addChildren(fileNames,[this](){this->back();});
    closedir(dir);
}

void MenuNavigator::fetchConfOptions(MenuNode *node){
    TrainingConfig c = manager->getTrainingConf();
    //std::cout<<"fetch"<<std::endl;
    auto formatTitle = [](const std::string& title, const std::string& value, size_t width = 30) {
        std::string res = title;
        res+= " :";
        if (title.length() < width) {
            res.append(width - title.length(), ' '); // ajouter des espaces
        }
        res += value;
        return res;
    };

    MenuList confList = {
        new MenuNode(formatTitle("Learning rate",std::to_string(c.learningRate)),[this](){
            double newLR = -2.22;
            std::cout<<"Please enter learning rate: ";
            if(secureDoubleInputCheck(newLR,0,10)){
                TrainingConfig c = manager->getTrainingConf();
                c.learningRate = newLR;
                manager->setTrainingConf(c);
                std::cout<<"Learning rate successfully set to "<<manager->getTrainingConf().learningRate<<std::endl;
            } else {
                std::cout<<"Error, could not change learning rate"<<std::endl;
            }
        }),

        new MenuNode(formatTitle("Number of epoch",std::to_string(c.epochs)),[this](){
            int newEpochNum = -1;
            std::cout<<"Please enter number of epoch : ";
            if(secureIntInputCheck(newEpochNum,10000)){
                TrainingConfig c = manager->getTrainingConf();
                c.epochs = newEpochNum;
                manager->setTrainingConf(c);
                std::cout<<"Num of epoch sucessfully set to : "<<manager->getTrainingConf().epochs<<std::endl; 
            } else {
                std::cout<<"Error, coud not change number of epoch"<<std::endl;
            }
            
        }
        ),
        new MenuNode(formatTitle("Batch size",std::to_string(c.batchSize)),[this](){
            int newBsize = -1;
            std::cout<<"Please enter batch size : ";
            if(secureIntInputCheck(newBsize,10000)){
                TrainingConfig c = manager->getTrainingConf();
                c.batchSize = newBsize;
                manager->setTrainingConf(c);
                std::cout<<"Batch size sucessfully set to : "<<manager->getTrainingConf().epochs<<std::endl; 
            } else {
                std::cout<<"Error, coud not change batch size"<<std::endl;
            }

        }),
        new MenuNode(formatTitle("Shuffle",(c.shuffle ? "Enabled" : "Disabled")),[this](){
            //yes no choice
            int enable = -1;
            std::cout<<"Please enter 1 for yes, 2 for no : ";
            if(secureIntInputCheck(enable,2)){
                TrainingConfig c = manager->getTrainingConf();
                c.shuffle = (enable == 1 ? true : false);
                manager->setTrainingConf(c);
                std::cout<<"Shuffle is now "<<(manager->getTrainingConf().shuffle ? "enabled" : "disabled")<<std::endl;
            } else {
                std::cout<<"Error, incorrect shuffle change"<<std::endl;
            }
        }),
        new MenuNode(formatTitle("Seed",std::to_string(c.seed)),[this](){
            int seed = -1;
            std::cout<<"Please enter seed : ";
            if(secureIntInputCheck(seed,INT32_MAX)){
                TrainingConfig c = manager->getTrainingConf();
                c.seed = seed;
                manager->setTrainingConf(c);
                std::cout<<"Seed succesfully set to : "<<manager->getTrainingConf().seed<<std::endl;
            } else {
                std::cout<<"Error : incorrect seed change"<<std::endl;
            }
        }),
    };

    node->addChildren(confList,[this](){this->back();});
}


// =========================
// RUNNING FLOW
// =========================
void MenuNavigator::run(){ 
    int userInput = -1;
    while(!menuStack.empty()){
        displayMenu();
        if(secureIntInputCheck(userInput,menuStack.top()->options.size())){
           MenuNode *selected = menuStack.top()->options[userInput - 1];
           //std::cout<<"Top title : "<<selected->title<<std::endl;
           //std::cout<<"ACTION :\n"<<std::endl;
           if(selected->action != nullptr){                                   //if we have an action, just do it :) omg nike propaganda ?
                selected->action();                                           //
                if(menuStack.top()->fetchChildren){                            //we suppose that any action can affect the current menu
                    std::cout<<menuStack.top()->title;
                    menuStack.top()->fetchChildren(menuStack.top());
                }
            }
           if(selected->fetchNeeded){                              //makes a fetch if the MenuNode requires it
            selected->fetchChildren(selected);
            }
           if(selected->hasChildren()){                             //push is only valid when we have children (or more options) to push, else, we stay at the same menu level
                menuStack.push(selected);                               //attention ca n'a rien à voir avec si les enfants doivent être générés !, on peut très bien imaginer un cas où on tente
           } 
        }
    }
}

void MenuNavigator::displayMenu(){
    if(menuStack.top()){
        std::cout<<"\n===="<<menuStack.top()->title<<"====\n";
        
        for(size_t i = 0; i<menuStack.top()->options.size();i++){
            std::cout<<i+1<<". "<<menuStack.top()->options[i]->title<<"\n";
        }
    } else {
        std::cerr<<"Error : Menu not found\n";
    }
}

void MenuNavigator::back(){
    if(menuStack.top() != rootMenu){            //accept pop only if it's not the main menu
        menuStack.pop();
    }
    else {
        std::cout<<"Already main root\n";
    }
}

void MenuNavigator::backToMainMenu(){
    while(!menuStack.empty()) {
        menuStack.pop();
    }
    menuStack.push(rootMenu);
}

void MenuNavigator::terminate(){
    while(!menuStack.empty()){
        menuStack.pop();
    }
}

void MenuNavigator::unimplemented(){
    std::cout<<"unimplemented, coming soon"<<std::endl;
}