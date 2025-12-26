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

//un peu brouillon
using Action = std::function<void()>;
using Fetch = std::function<void(MenuNode*)>;

using MenuList = std::vector<MenuNode*>;

struct MenuNode{
    std::string title;
    Action action;                                                               //action to do
    Fetch fetchChildren;                                                         //fetch method, use for dynamic node fetch
   
    bool fetchNeeded = false;               //still needed ?                                                    //flag to decide if we need to fetch, assumed false per default
    std::vector<MenuNode*> options;                                              //options or children Nodes
    

    MenuNode() = default;
    MenuNode(std::string title,Action actionFn);
    MenuNode(std::string title, Action actionFn, Fetch fetchFn);                                                              //constructor
    ~MenuNode();                                                                                //destructor

    bool hasChildren();

    void addChildren(MenuList children, Action backAction);               //children adder
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

        
        
        void createNeuralNetwork();                                 //not implemented again
        void importNetwork(const std::string &path);                //need to test post refacto, major issue on invalid file, assert on magic number check



        //======= Network direct Actions =======//
        void showInfo(uint32_t nwId);                              //tested ok
        void trainNetwork(uint32_t nwId);                          //not implemented at all
        void evaluateNetwork(uint32_t nwId);                       //somehow works, but only cmd
        void saveNetwork(uint32_t nwId);                           //suspicious
        void deleteNetwork(MenuNode* node, uint32_t nwId);         //seems to work      


       
        //======= Dynamic fetching for MenuNodes =======//
        void fetchNetworkList(MenuNode *node);
        void fetchSavedNetworkFiles(MenuNode *node);
        void fetchConfOptions(MenuNode *node);


    
        /* void sillyDestructiveStuff(); */
        
        
        
        //======= Running flow =======//
        void run();                                                                          //runs the stack menu
        void displayMenu();                                                                  //displays the top of menuStack = the current menu
        void unimplemented();

};
#endif //


