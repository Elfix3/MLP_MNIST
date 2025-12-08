
#include "MNISTReader.h"
#include "NeuralNetwork.h"

#include <cstdlib>

int main(){
    MNISTReader r("data/train-images.idx3-ubyte","data/train-labels.idx1-ubyte");
    Matrix imageInput(28,28,r.getImage(2));
    Matrix normalized = imageInput.Normalize(255);
    //
    //r.getImage() to fetch image *uint8_t
    //r.getLabel() to fetch label uint8_t
    
    int y = static_cast<int>(*r.getLabel(2));

    std::vector<std::pair<size_t,ActivationType>> network_config = {
        {256,RELU},
        {128, SOFTMAX},
    };
    
    NeuralNetwork nn(784,network_config,10);
    //nn.debug();
    Matrix a = nn.forward(normalized);
    a.printSize();
    std::cout << y<<std::endl;
    std::cout<<nn.loss(a,y)<<std::endl;
    
    std::cout<<a;

    //std::cout<<*r.getLabel(2);
   
    /* Layer l0(784,256); //relu
    Layer l1(256,128); //relu
    Layer l2(128,10,SOFTMAX); //soft max */



    
    
    
    //std::cout<<l2.forward(l1.forward(l0.forward(normalized)));
    //cross entrophy

    
    //plot example
    

}