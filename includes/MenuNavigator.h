#ifndef MENU_NAV_H
#define MENU_NAV_H

#include <iostream>
#include <stack>
#include <functional>
#include <string>
#include <vector>
#include <dirent.h>

#include "cmd_line_tools.h"
#include "NnManager.h"

struct MenuNode;

//helper alias to INITALISE MenuNode, note that's it's only to initalize
using MenuEntry = std::pair<std::string, std::function<void()>>;                 //maps a string, option name, to an action to do
using MenuList  = std::vector<MenuEntry>;                                                       //list of entry this mapping


/*  using MenuEntry = std::tuple<std::string, std::function<void()>, std::function<void(MenuNode *parent)>>;
 using MenuList  = std::vector<MenuEntry>; 
 */
struct MenuNode{
    std::string title;
    std::function<void()> action;                                                               //action to do
    std::function<void(MenuNode*)> fetchChildren;                                               //fetch method, use for dynamic node fetch
   
    bool fetchNeeded = false;                                                                   //flag to decide if we need to fetch, assumed false per default
    std::vector<MenuNode*> options;                                                             //options or children Nodes
    

    MenuNode(const MenuEntry m);                                                                //constructor
    ~MenuNode();                                                                                //destructor
    
    void setFetchChildren(std::function<void(MenuNode *parent)> fetchMethod);                   //specifies a fetch method to indicate we should fetch children
    bool hasChildren();

    void addChildren(const MenuList &children, std::function<void()> backAction);               //children adder
    void clearChildren();                                                                       //clears all children, for dynamic menu
};
class MenuNavigator{
    private :
        MenuNode *rootMenu;                                                                     //first menu
        std::stack<MenuNode*> menuStack;                                                        //menuStack.top() is the current Menu
        NnManager *manager;

    public :
        MenuNavigator();
        ~MenuNavigator() = default;
        
        
        //action functions :
        void back();                                                                         //pops the stack, should be passed to back action of the add children of the node
        void backToMainMenu();                                                               //returns to the root menu
        void terminate();                                                                    //ends the program

        void createNeuralNetwork();
        void importNetwork(const std::string &path);


        //options on network
        void showInfo(uint32_t nwId);
        void trainNetwork(uint32_t nwId);
        void evaluateNetwork(uint32_t nwId);
        void saveNetwork(uint32_t nwId);
        void deleteNetwork(uint32_t nwId);


       
        //FETCH METHODS, FOR DYNAMIC NODES
        void fetchNetworkList(MenuNode *node);
        void fetchSavedNetworkFiles(MenuNode *node);

        
        
        void unimplemented();
        void sillyDestructiveStuff();
        
        void run();                                                                          //runs the stack menu
        void displayMenu();                                                                  //displays the top of menuStack = the current menu

        bool init_nn_manager();
};
#endif