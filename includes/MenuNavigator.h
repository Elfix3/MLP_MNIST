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

using Action = std::function<void()>;                                                   //generic action function
using Fetch = std::function<void(MenuNode*)>;                                           //generic fetch function

using MenuList = std::vector<MenuNode*>;                                                //used to add a list of children

struct MenuNode{
    std::string title;
    Action action;                                                                      //action to do
    Fetch fetchChildren;                                                                //fetch method, use for dynamic node fetch
   
    bool fetchNeeded = false;                                                           //flag to decide if we need to fetch, assumed false per default, still needed ?
    std::vector<MenuNode*> options;                                                     //options or children Nodes
    

    // =========================
    // MENU NODES FUNCTIONS
    // =========================
    MenuNode() = default;
    MenuNode(std::string title,Action actionFn);                                        //constructor no fetch
    MenuNode(std::string title, Action actionFn, Fetch fetchFn);                        //constructor with fetch
    ~MenuNode();                                                                        //destructor
    
    bool hasChildren();
    void addChildren(MenuList children, Action backAction);                             //children adder
    void clearChildren();                                                               //clears all children, for dynamic menu
};


class MenuNavigator{
    private :
        MenuNode *rootMenu;                                                             //rootMenu, has to be delete -> recursive delete of all children
        std::stack<MenuNode*> menuStack;                                                //menuStack.top() is the current Menu
        NnManager *manager;                                                             // manager

    public :
        MenuNavigator();
        ~MenuNavigator() = default;
        
        
        // =========================
        // NETWORK DIRECT ACTION
        // =========================
        void showInfo(const uint32_t &nwId);                              //tested ok
        void trainNetwork(const uint32_t &nwId);                          //not implemented at all
        void evaluateNetwork(const uint32_t &nwId);                       //somehow works, but only cmd
        void evaluatePerDigit(const uint32_t &nwId);
        void saveNetwork(const uint32_t &nwId);                           //suspicious
        void deleteNetwork(MenuNode* node, const uint32_t &nwId);         //seems to work      


        // =========================
        // DYNAMIC FETCH OF THE NODE
        // =========================
        void fetchNetworkList(MenuNode *node);                      //add network nodes to the target MenuNode
        void fetchSavedNetworkFiles(MenuNode *node);                //add files nodes to the target MenuNode
        void fetchConfOptions(MenuNode *node);                      //add parameters node to the target MenuNode


        // =========================
        // SPECIAL ACTION
        // =========================
        void createNeuralNetwork();                                 //not implemented again
        void importNetwork(const std::string &path);                //need to test post refacto, major issue on invalid file, assert on magic number check//try to move this in the fetchSavedNwfiles ?
        
        
        // =========================
        // RUNNING FLOW
        // =========================
        void run();                                                 //runs the stack menu
        void displayMenu();                                         //displays the top of menuStack = the current menu
        void back();                                                //pops the stack, should be passed to back action of the add children of the node
        void backToMainMenu();                                      //returns to the root menu
        void terminate();                                           //ends the program
        void unimplemented();                                       //unimplemented util function

};
#endif //


