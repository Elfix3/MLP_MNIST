#ifndef MNIST_READER_H
#define MNIST_READER_H

#include "Matrix.h"

#include <fstream>
#include <iostream>
#include <assert.h>
#include <iomanip>
#include <cstdio>
#include <cstdint>


class MNISTReader{
    private :
        uint8_t *imagesTrain= nullptr;
        uint8_t *labelsTrain= nullptr;


        std::pair<uint8_t*,uint8_t*> trainSet = {nullptr,nullptr}; // images first, labels second,  should be 60k
        std::pair<uint8_t*,uint8_t*> testSet = {nullptr,nullptr};  // images first, labels second, should be 10k

        uint32_t n_imTrain;
        uint32_t n_imTest;
        uint32_t n_rows;
        uint32_t n_cols;
        
        public :
            MNISTReader();
            ~MNISTReader();

            const uint8_t* getImage(size_t index, bool fromTest) const;
            const uint8_t* getLabel(size_t index, bool fromTest) const; 

            const uint32_t get_num_images_Train() const;
            const uint32_t get_num_images_Test() const;

            const uint32_t get_num_rows() const;
            const uint32_t get_num_cols() const;

            Matrix X_bach(size_t batch_start, size_t batch_size);
            Matrix Y_bach(size_t batch_start, size_t batch_size);

            void plot_mnist_direct(size_t imageIndex, bool fromTest);
            void plot_mnist_direct(const Matrix &img);                                      //under test
};


#endif //MINST