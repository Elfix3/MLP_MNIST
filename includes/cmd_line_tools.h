#ifndef CMD_LINE_TOOLS_H
#define CMD_LINE_TOOLS_H

#include <iostream>

void secureIntInputCheck(int &user_input); //makes sure the user wont pass non integer values


void bannerMessage(const char* msg); // prints some importants messages in a banner
void choiceMessage(const char** choices, size_t numChoices);


void getSecureIntChoice(std::initializer_list<const char *> choices, int &user_input);


#endif