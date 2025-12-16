
#include "MNISTReader.h"
#include "NeuralNetwork.h"
#include "cmd_line_tools.h"

#include <cstdlib>

int main(){
    MNISTReader reader; //now only one reader for test and training images
    std::vector<std::pair<size_t,ActivationType>> network_config = {
        {64, RELU},
        {32, RELU},
        {16, RELU},
        {10, SOFTMAX}
    };
    NeuralNetwork nn(784,network_config);


    int usr_choice =-1;
    bool isAlive = true;
    NeuralNetwork **mySetOfNeurals = new NeuralNetwork*[10];
    int train_size = 60000; // typiquement 60000
    int batch_size = 64;
    //int epochs = 10;

    bannerMessage("Hello, welcome to this MLP training program, here is an overview of what you can do with this tool :");

    while(isAlive){
        getSecureIntChoice({"Create network","Train settings","","Load"}, usr_choice);
        switch(usr_choice){
            
            case 1:
            //training ex
            {
            for (int epoch = 0; epoch < 10; epoch++) {
                for (int batch_start = 0;batch_start + batch_size <= train_size; batch_start += batch_size) {
                    Matrix X = reader.X_bach(batch_start,batch_size);
                    Matrix Y = reader.Y_bach(batch_start,batch_size);
                    Matrix A = nn.forward(X.Normalize(255));
                    nn.backward(A-Y);
                    nn.update();
                    std::cout<<nn.lossBatch(A,Y)<<std::endl;
                }
                }
                std::cout<<"done"<<std::endl;
            }

            break;

            case 2:
            {
                size_t n_tests = 10000;
                size_t n_success = 0;
                for(size_t i = 0; i<n_tests;i++){
                    Matrix X_test(1,784,reader.getImage(i,true));
                    Matrix Y_test = nn.forward(X_test);
                    if((int)Y_test.getMaxIndex() == (int)*reader.getLabel(i,true))n_success++;
                }
                double accuracy = (double)n_success/(double)n_tests;
                std::cout<<"Accuracy of the network : "<<accuracy<<std::endl;

                
            }
                
            break;


            case 3:
            {
               nn.save("trained network");
            }


            break;

            case 4 :
            {
                NeuralNetwork nn2("trained network");
                size_t n_tests = 10000;
                size_t n_success = 0;
                for(size_t i = 0; i<n_tests;i++){
                    Matrix X_test(1,784,reader.getImage(i,true));
                    Matrix Y_test = nn2.forward(X_test);
                    if((int)Y_test.getMaxIndex() == (int)*reader.getLabel(i,true))n_success++;
                }
                double accuracy = (double)n_success/(double)n_tests;
                std::cout<<"Accuracy of the network : "<<accuracy<<std::endl;
            }

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