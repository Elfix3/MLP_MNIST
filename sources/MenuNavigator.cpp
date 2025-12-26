#include "MenuNavigator.h"



MenuNode::MenuNode(std::string title, Action actionFn) : title(title),
action(std::move(actionFn)), fetchNeeded(false) {}                                          //constructor for no fetch needed

MenuNode::MenuNode(std::string title, Action actionFn, Fetch fetchFn) : title(title),
action(std::move(actionFn)), fetchChildren(fetchFn), fetchNeeded(true) {}                   //constructor for fetch needed

MenuNode::~MenuNode(){
    clearChildren();
}

/* void MenuNode::addChildren(const MenuList &children, std::function<void()> backAction){
    for(const MenuEntry &m : children){                             //we loop a menu entry over the menu list
        //options.push_back(new MenuNode(m));
    }
    options.push_back(new MenuNode({"Back",[backAction](){backAction();}}));           //adds back button to each MenuNode
} */

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
    NeuralNetwork *n1 = new NeuralNetwork(32);
    manager->pushNn(n1);

    NeuralNetwork *n2 = new NeuralNetwork(52);
    manager->pushNn(n2);

    NeuralNetwork *n3 = new NeuralNetwork(62);
    manager->pushNn(n3);

    rootMenu = new MenuNode({"MLP training program",nullptr});
    
    //auto myLambda = [this](){ this->unimplemented(); };         //for easy passing an unimplemented method
    Action backAction = [this](){back();};
    Action terminateAction = [this](){terminate();};
    

    //Fetch fetchNetworks = [this](MenuNode *node){fetchNetworkList(node);};
    MenuList rootMenuOptions = {
        new MenuNode("Create network",nullptr),
        new MenuNode("Import network",nullptr,[this](MenuNode *node){this->fetchSavedNetworkFiles(node);}),
        new MenuNode("Training config",nullptr, [this](MenuNode *node){this->fetchConfOptions(node);}),
        new MenuNode("Compare", nullptr),
        new MenuNode("Network list",nullptr,[this](MenuNode *node){this->fetchNetworkList(node);}),
    };
    rootMenu->addChildren(rootMenuOptions,terminateAction);

    /* MenuList optionsImportExport = {
        {"Import",myLambda},
        {"Export",myLambda},
    };
     */
    /* rootMenu->addChildren(menuOptions,[this](){this->terminate();});
    
    MenuNode* nwListNode = rootMenu->options[3];
    nwListNode->setFetchChildren([this](MenuNode *nwListNode){this->fetchNetworkList(nwListNode);});

    MenuNode* importListNode = rootMenu->options[1];
    importListNode->setFetchChildren([this](MenuNode *importListNode){this->fetchSavedNetworkFiles(importListNode);});

    MenuNode* trainingConfNode = rootMenu->options[2];
    trainingConfNode->setFetchChildren([this](MenuNode *trainingConfNode){this->fetchConfOptions(trainingConfNode);}); */
    
    menuStack.push(rootMenu);
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

    //push
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

void MenuNavigator::showInfo(uint32_t nwId){
    manager->getNetworkFromId(nwId)->infos();
}

void MenuNavigator::trainNetwork(uint32_t nwId){
    //not yet implemented
}

void MenuNavigator::evaluateNetwork(uint32_t nwId){
    manager->compute_accuracy_global(nwId);
}

void MenuNavigator::saveNetwork(uint32_t nwId){

    //very suspicious


    std::string fileName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout<<"Please type desired file name : ";
    getline(std::cin, fileName);
    manager->getNetworkFromId(nwId)->save(fileName.c_str());
}

void MenuNavigator::deleteNetwork(MenuNode *node, uint32_t nwId){
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

void MenuNavigator::displayMenu(){
    if(menuStack.top()){
        std::cout<<"\n===="<<menuStack.top()->title<<"====\n";
        for(size_t i = 0; i<menuStack.top()->options.size();i++){
            std::cout<<i+1<<". "<<menuStack.top()->options[i]->title<<"\n";
        }
    }
    else {
        std::cerr<<"Error : Menu not found\n";
    }
}

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
                new MenuNode("Train",[this,nwId](){unimplemented();}),
                new MenuNode("Evaluate",[this,nwId](){evaluateNetwork(nwId);}),
                new MenuNode("Save",[this,nwId](){unimplemented();}),
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
    node->clearChildren();
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
        new MenuNode(formatTitle("Learning rate",std::to_string(c.learningRate)),nullptr),
        new MenuNode(formatTitle("Number of epoch",std::to_string(c.epochs)),nullptr),
        new MenuNode(formatTitle("Batch size",std::to_string(c.batchSize)),nullptr),
        new MenuNode(formatTitle("Shuffle",(c.shuffle ? "Enabled" : "Disabled")),nullptr),
        new MenuNode(formatTitle("Seed",std::to_string(c.seed)),nullptr),
    };
    node->addChildren(confList,[this](){this->back();});
}

void MenuNavigator::unimplemented(){
    std::cout<<"unimplemented, coming soon"<<std::endl;
}

void MenuNavigator::run()
{
    int userInput = -1;
    while(!menuStack.empty()){
        displayMenu();
        if(secureIntInputCheck(userInput,menuStack.top()->options.size())){
           MenuNode *selected = menuStack.top()->options[userInput - 1];
           if(selected->fetchNeeded){                              //makes a fetch if the MenuNode requires it
            selected->fetchChildren(selected);
            }
           if(selected->action != nullptr){                                    //if we have an action, just do it :)
                selected->action();
           }
           if(selected->hasChildren()){                             //push is only valid when we have children (or more options) to push, else, we stay at the same menu level
            menuStack.push(selected);                               //attention ca n'a rien à voir avec si les enfants doivent être générés !, on peut très bien imaginer un cas où on tente
           }                                                        //de générer des enfants sans succès, auquel cas, il n'y a rien à afficher 
        }
    }
}




