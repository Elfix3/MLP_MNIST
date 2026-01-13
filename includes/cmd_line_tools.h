#ifndef CMD_LINE_TOOLS_H
#define CMD_LINE_TOOLS_H

#include <iostream>

bool secureIntInputCheck(int &user_input, size_t max_range); //makes sure the user wont pass non integer values
bool secureDoubleInputCheck(double &user_input,double min, double max); //hmmmm same ? :)
//bool secureIntInputCheck(int &user_input, size_t max_range);

void bannerMessage(const char* msg); // prints some importants messages in a banner
void choiceMessage(const char** choices, size_t numChoices);


void getSecureIntChoice(std::initializer_list<const char *> choices, int &user_input);


#endif