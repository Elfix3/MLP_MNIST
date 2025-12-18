#ifndef MENU_NAV_H
#define MENU_NAV_H

#include <iostream>
#include <stack>
#include <functional>
#include <string>
#include <vector>

#include "cmd_line_tools.h"
#include "NnManager.h"

struct MenuNode{
    std::string title;
    std::function<void()> action = nullptr;
    std::vector<MenuNode*> options;

    MenuNode(const std::string& title, std::function<void()> action = nullptr);                    //used only to declare main menu root
    MenuNode* addOption(const std::string& optionName, std::function<void()> action);

    ~MenuNode();
};

class MenuNavigator{
    private :
        MenuNode *rootMenu;                                                                       //first menu
        std::stack<MenuNode*> menuStack;                                                          //menuStack.top() is the current Menu
        NnManager *manager;

    public :
        MenuNavigator(MenuNode* root);
        ~MenuNavigator() = default;
        void displayMenu();
        void back();
        void backToMainMenu();


        void run();

};
#endif