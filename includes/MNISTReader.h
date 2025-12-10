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
        uint8_t *images = nullptr;
        uint8_t *labels = nullptr;
        uint32_t num_images = 0;
        uint32_t rows = 0;
        uint32_t cols = 0;
        

        public :
            MNISTReader(const char* images_file, const char* labels_file);
            //~MNISTReader();
            void debug();

            const uint8_t* getImage(size_t index) const;
            const uint8_t* getLabel(size_t index) const;

            const uint32_t get_num_images() const;
            const uint32_t get_num_rows() const;
            const uint32_t get_num_cols() const;

            Matrix X_bach(size_t batch_start, size_t batch_size);
            Matrix Y_bach(size_t batch_start, size_t batch_size);


            //TO DELETE
            void showImageAndLabel(size_t i); //command line output
            void plot_mnist_direct(size_t imageIndex); //under test
            void plot_mnist_direct(const Matrix &img);




};


#endif //MINST