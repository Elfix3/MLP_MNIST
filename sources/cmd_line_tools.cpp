#include "cmd_line_tools.h"



void secureIntInputCheck(int &user_input){
    if(!(std::cin>>user_input)){
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        std::cout<<"Incorrect input, please enter a number"<<std::endl;
    }
}

void bannerMessage(const char *msg){
    std::cout<<"\n###########################################################################################################"<<std::endl;
    std::cout<<msg<<std::endl;
    std::cout<<"###########################################################################################################\n"<<std::endl;
}

void choiceMessage(const char **choices, size_t numChoices){
    for(size_t i = 0; i<numChoices;i++){
        std::cout<<i<<" : "<<choices[i]<<std::endl;
    }
    std::cout<<std::endl;
}

void getSecureIntChoice(std::initializer_list<char *> choices)
{
}

void getSecureIntChoice(std::initializer_list<const char *> choices, int &user_input){
    int index = 0;
    for(const auto& s : choices){

        std::cout<<++index<<" : "<<s<<std::endl;
    }
    std::cout<<std::endl;
    if(!(std::cin>>user_input)){
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        std::cout<<"Incorrect input, please enter a number"<<std::endl;
    }
}
