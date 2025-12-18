#include "MenuNavigator.h"

MenuNode::MenuNode(const std::string& title, std::function<void()> action) : title(title) {}

MenuNode *MenuNode::addOption(const std::string &optionName, std::function<void()> action)
{
    MenuNode *newOption = new MenuNode(optionName);
    newOption->action = std::move(action);
    options.push_back(newOption);
    return newOption;
}

MenuNode::~MenuNode(){
    for(MenuNode* a : options){
        delete a;
        a = nullptr; 
    }
}

MenuNavigator::MenuNavigator(MenuNode *root){
    rootMenu = root;
    menuStack.push(root);
}


void MenuNavigator::displayMenu(){

    if(menuStack.top()){
        std::cout<<"\n===="<<menuStack.top()->title<<"====\n";
        for(size_t i = 0; i<menuStack.top()->options.size();i++){
            std::cout<<i+1<<". "<<menuStack.top()->options[i]->title<<"\n";
        }
    }
    else {
        std::cout<<"Error : Menu not found\n";
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
    std::cout<<"POPPING NIGGERS";
    while(!menuStack.empty()) {
        menuStack.pop();
    }
    menuStack.push(rootMenu);
}

void MenuNavigator::run(){
    int userInput = -1;
    while(!menuStack.empty()){
        displayMenu();                                      //shows the top menu, top of the stack
        //MenuNode *current = menuStack.top();
        secureIntInputCheck(userInput);

        if(userInput>0 && userInput<=(int)menuStack.top()->options.size()){
            if(menuStack.top()->options[userInput-1]->action){
                menuStack.top()->options[userInput-1]->action();
            } else {
                menuStack.push(menuStack.top()->options[userInput-1]);
            }

            

        } else {
            std::cout<<"Option incorrecte\n";
        }
    }
}
//menuStack.push(menuStack.top()->options[userInput]);