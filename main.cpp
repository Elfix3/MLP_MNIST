
#include "MNISTReader.h"
#include "NeuralNetwork.h"
#include "cmd_line_tools.h"

#include <cstdlib>

int main(){
    MNISTReader reader; //now only one reader for test and training images

    Matrix a(1,784,IMAGE_GRAY_SCALE);
    reader.plot_mnist_direct(a);

    assert(2==0);
    std::vector<std::pair<size_t,ActivationType>> network_config = {
        {26, RELU},
        {26, RELU},
        {10, SOFTMAX}
    };
    NeuralNetwork nn(784,network_config);
    
    
    int usr_choice =-1;
    bool isAlive = true;
    NeuralNetwork **mySetOfNeurals = new NeuralNetwork*[10];
    int batch_size = 30;

    bannerMessage("Hello, welcome to this MLP training program, here is an overview of what you can do with this tool :");

    while(isAlive){
        getSecureIntChoice({"Create neural network","Show infos"}, usr_choice);
        switch(usr_choice){
            
            case 1:
            {
            for (int epoch = 0; epoch < 1; epoch++) {
                for (int batch_start = 0; batch_start < batch_size*500; batch_start += batch_size) {
                    Matrix X = reader.X_bach(batch_start,batch_size);
                    Matrix Y = reader.Y_bach(batch_start,batch_size);
                    Matrix A = nn.forward(X.Normalize(255));
                    nn.backward(A-Y);
                    nn.update();
                    std::cout<<nn.lossBatch(A,Y)<<std::endl;
                }
                }
            }

            break;

            case 2:
                //show info
            break;


            case 3:
                //train NN process, all ? separatly ?
            break;

            case 4 :
                //gives the settings parameters
            break;
            
            case 5:
                //get infos on a neural network
            break;

            case 6 :
                //recap ?
            break;
            
            case 112 :
                isAlive = false;                        //exit code
            break;             
            default:

            break;
        }
            
    }
    delete []mySetOfNeurals;

}