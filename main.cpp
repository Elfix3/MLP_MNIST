
#include "MNISTReader.h"
#include "NeuralNetwork.h"

#include <cstdlib>

int main(){
    MNISTReader r("data/train-images.idx3-ubyte","data/train-labels.idx1-ubyte");

    
    std::vector<std::pair<size_t,ActivationType>> network_config = {
        {256,RELU},
        {128, RELU},
        {10, SOFTMAX} 
    };
    bool isAlive = true;
    int n_epochs = 1;
    int batch_size = 32;
    int usr_choice;
    while(isAlive){
        std::cout<<"Votre choix"<<std::endl;
        std::cin>>usr_choice;

        switch(usr_choice){
            case 1:
            for (int epoch = 0; epoch < n_epochs; epoch++) {
                for (int batch_start = 0; batch_start < 32*1000; batch_start += batch_size) {
                    Matrix X_batch(batch_size,784);
                    //construire X_batch
                }
            }
            break;
            case 2:
                isAlive = false;
            break;
            default:
            break;
        }
            
    }
    NeuralNetwork nn(784,network_config);
    Matrix imageInput(28,28,r.getImage(2));
    Matrix normalized = imageInput.Normalize(255);
    normalized.printSize();
    

    int y = static_cast<int>(*r.getLabel(2)); //correct output
    Matrix Y(1,10);
    Y(0,y-1) = 1.0; //one hot

    
    
    Matrix A = nn.forward(normalized);
    std::cout<<nn.loss(A,y)<<std::endl;
    nn.backward(A-Y);
    nn.update();

    Matrix B = nn.forward(normalized);
    
    std::cout<<nn.loss(B,y)<<std::endl;

    //std::cout<<*r.getLabel(2);
   
    /* Layer l0(784,256); //relu
    Layer l1(256,128); //relu
    Layer l2(128,10,SOFTMAX); //soft max */



    
    
    
    //std::cout<<l2.forward(l1.forward(l0.forward(normalized)));
    //cross entrophy

    
    //plot example
    

}