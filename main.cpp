
#include "MNISTReader.h"
#include "NeuralNetwork.h"

#include <cstdlib>

int main(){
    MNISTReader r("data/train-images.idx3-ubyte","data/train-labels.idx1-ubyte");

    MNISTReader test("data/t10k-images.idx3-ubyte","data/t10k-labels.idx1-ubyte");

    std::vector<std::pair<size_t,ActivationType>> network_config = {
        {256,RELU},
        {128, RELU},
        {10, SOFTMAX} 
    };
    NeuralNetwork nn(784,network_config);

    static size_t predictCursor = 0; 

    bool isAlive = true;
    int n_epochs = 10;
    int batch_size = 60;

    /* std::cout<<X;
    X.printSize(); */
    int usr_choice =-1;

    while(isAlive){
        std::cout<<">"<<std::endl;
        std::cin>>usr_choice;

        switch(usr_choice){
            case 1:
            for (int epoch = 0; epoch < n_epochs; epoch++) {
                for (int batch_start = 0; batch_start < 60*100; batch_start += batch_size) {
                    Matrix X = r.X_bach(batch_start,batch_size);
                    Matrix Y = r.Y_bach(batch_start,batch_size);
                    Matrix A = nn.forward(X.Normalize(255));
                    nn.backward(A-Y);
                    nn.update();
                    std::cout<<nn.lossBatch(A,Y)<<std::endl;
                }
                std::cout<<"ONE EPOCH MADE"<<std::endl;
            }
            break;
            case 2 :
            {
                Matrix X_test(1,784,test.getImage(predictCursor++));
                test.plot_mnist_direct(X_test);
                Matrix Y_test = nn.forward(X_test);
                Y_test.printSize();
                std::cout<<Y_test<<std::endl;
                Y_test.showProbability();
            }
            break;
            case 3:
                //
            break;
            default:
            break;
        }
            
    }
    
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
  



    
    
    
    //std::cout<<l2.forward(l1.forward(l0.forward(normalized)));
    //cross entrophy

    
    //plot example
    

}