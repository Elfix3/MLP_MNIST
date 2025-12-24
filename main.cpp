
#include "MNISTReader.h"
#include "cmd_line_tools.h"
#include <cstdlib>

#include "MenuNavigator.h"
#include "NnManager.h"


int main(){
    //MenuNode *main = new MenuNode("MLP trainer");
    /* 
    main->addOption("Neural networks",nullptr); //option 0
    main->addOption("Train network",nullptr);
    main->addOption("Evaluate",nullptr);
    main->addOption("Save",nullptr);
    
    main->options[0]->addOption("Show my networks", [](){});
    main->options[0]->addOption("Create network", [](){});
    main->options[0]->addOption("Back", [nav](){ nav->back();});
    
    main->options[1]->addOption("Edit training configuration", [](){});
    main->options[1]->addOption("Train all", [](){std::cout<<"train sequence start"<<std::endl;});
    main->options[1]->addOption("Back", [nav](){ nav->back();});
    
    main->options[2]->addOption("Evaluate",[](){});
    main->options[2]->addOption("Evaluate all",[](){});
    main->options[2]->addOption("Back",[nav](){ nav->back();}); */
    
    
    
    
    /*  std::vector<std::pair<size_t,ActivationType>> network_config = {
        {16, RELU},
        {16, RELU},
        {10, SOFTMAX}
        };
        NeuralNetwork nn(784,network_config); */
        
    //NnManager manager;
    //manager.pushNn(&nn);
    /* manager.train(0);
    manager.compute_accuracy(0);
    manager.compute_accuracy_global(0); */

    MenuNavigator *nav= new MenuNavigator();
    nav->run();
    
    return 0;
}