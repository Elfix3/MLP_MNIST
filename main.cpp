#include "Matrix.h"
#include "Layer.h"
#include "MNISTReader.h"


#include <cstdlib>

int main(){
    MNISTReader r("data/train-images.idx3-ubyte","data/train-labels.idx1-ubyte");
    //
    //r.getImage() to fetch image
    //r.getLabel() to fetch label


    Matrix imageInput(28,28,r.getImage(2)); //bof constructeur pour flatten ?



    std::cout<<imageInput.cols()<<std::endl; //784
    std::cout<<imageInput.rows()<<std::endl; //1
    //cout<<IMAGE_GRAY_SCALE
   


    Layer l0(784,256); //relu
    Layer l1(256,128); //relu
    Layer l2(128,10,SOFTMAX); //soft max

    Matrix m(1,6,NORMALIZED);


    
    Matrix normalized = imageInput.Normalize(255);
    
    
    std::cout<<l2.forward(l1.forward(l0.forward(normalized)));
    //cross entrophy

    
    //plot example
    

}