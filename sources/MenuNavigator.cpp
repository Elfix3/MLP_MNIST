#include "MenuNavigator.h"

MenuNode::MenuNode(const MenuEntry m) : title(m.first), action(m.second), fetchChildren(nullptr), fetchNeeded(false){}

void MenuNode::addChildren(const MenuList &children, std::function<void()> backAction){
    for(const MenuEntry &m : children){                             //we loop a menu entry over the menu list
        options.push_back(new MenuNode(m));
    }
    options.push_back(new MenuNode({"Back",[backAction](){backAction();}}));           //adds back button to each MenuNode
}

void MenuNode::clearChildren(){
    options.clear();
}


MenuNode::~MenuNode(){
    for(MenuNode* a : options){
        delete a;
    }
}

void MenuNode::setFetchChildren(std::function<void(MenuNode *parent)> fetchMethod){              //case where we need to fetch dynamically children
    fetchNeeded = true;
    fetchChildren = fetchMethod;
}

bool MenuNode::hasChildren(){
    return options.size()>0;
}

MenuNavigator::MenuNavigator(){
    
    manager = new NnManager();
    NeuralNetwork *n1 = new NeuralNetwork(12);
    manager->pushNn(n1);

    NeuralNetwork *n2 = new NeuralNetwork(52);
    manager->pushNn(n2);

    /* NeuralNetwork *n3 = new NeuralNetwork(62);
    manager->pushNn(n3); */

    rootMenu = new MenuNode({"MLP training program",nullptr});
    
    auto myLambda = [this](){ this->unimplemented(); };         //for easy passing an unimplemented method
   
    MenuList menuOptions = {
        {"Create network",[this](){this->createNeuralNetwork();}},
        {"Import network",nullptr},
        {"Training config",nullptr},
        {"Network list",nullptr},
        {"Compare", [this](){this->unimplemented();}}
    };


    MenuList optionsImportExport = {
        {"Import",myLambda},
        {"Export",myLambda},
    };
    
    rootMenu->addChildren(menuOptions,[this](){this->terminate();});
    
    MenuNode* nwListNode = rootMenu->options[3];
    nwListNode->setFetchChildren([this](MenuNode *nwListNode){this->fetchNetworkList(nwListNode);});

    MenuNode* importListNode = rootMenu->options[1];
    importListNode->setFetchChildren([this](MenuNode *importListNode){this->fetchSavedNetworkFiles(importListNode);});

    menuStack.push(rootMenu);
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

void MenuNavigator::trainNetwork(uint32_t nwId)
{
    std::cout<<"training : "<<(int)nwId<<std::endl;
}

void MenuNavigator::evaluateNetwork(uint32_t nwId){
    
}

void MenuNavigator::saveNetwork(uint32_t nwId){
    std::string fileName;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout<<"Please type desired file name : ";
    getline(std::cin, fileName);

    manager->getNetworkFromId(nwId)->save(fileName.c_str());
}

void MenuNavigator::deleteNetwork(uint32_t nwId){
    std::cout<<"am called"<<std::endl;
    manager->showNetWorks();
    manager->deleteNn(nwId);
    manager->showNetWorks();
    this->backToMainMenu();
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
    while ((entry = readdir(dir)) != nullptr) {
        std::string name= entry->d_name;
        if (name== "." || name == "..") continue; // ignorer
        std::cout << name << std::endl;
        MenuNode *mN = new MenuNode({name,[this,name](){this->importNetwork(name);}});        //add the reading function
        node->options.push_back(mN);
    }
    closedir(dir);

    node->options.push_back(new MenuNode({"Back",[this](){this->back();}}));
}

void MenuNavigator::fetchNetworkList(MenuNode *node){
    node->clearChildren();

    //MenuList dynamicOptions;
    
    //dynamic push :)
    for(size_t i = 0; i<manager->get_n_neural_networks(); i++){
        size_t nnId = manager->get_setofNetWorks()[i]->getId();

        MenuList staticOptions = {
            {"Info", [this,nnId](){showInfo(nnId);}},
            {"Train", [this,nnId](){trainNetwork(nnId);}},
            {"Evaluate",[this,nnId](){evaluateNetwork(nnId);}},
            {"Save",[this,nnId](){saveNetwork(nnId);}},
            {"Delete",nullptr}
        };

        MenuList yesNoConfirmation = {
            {"Yes", [this,nnId](){deleteNetwork(nnId);}},
            {"No",  nullptr}
        };
        MenuNode* nwNode = new MenuNode({"Network : "+std::to_string(nnId),nullptr});
        nwNode->addChildren(staticOptions,[this](){this->back();});
        nwNode->options[4]->addChildren(yesNoConfirmation,[this](){this->back();});
        node->options.push_back(nwNode);
    }
    node->options.push_back(new MenuNode({"Back",[this](){this->back();}}));
}



void MenuNavigator::unimplemented(){
    std::cout<<"unimplemented method\n";
}


void MenuNavigator::run(){
    int userInput = -1;
    while(!menuStack.empty()){
        displayMenu();
        if(secureIntInputCheck(userInput,menuStack.top()->options.size())){
           MenuNode *selected = menuStack.top()->options[userInput - 1];
           if(selected->fetchNeeded){                               //makes a fetch if the MenuNode requires it
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
