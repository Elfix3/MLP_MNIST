#include "MNISTReader.h"
#include <winsock2.h>

MNISTReader::MNISTReader(){
    //TRAINING SET STEAM
    std::ifstream trainIM("data/train-images.idx3-ubyte", std::ios::binary);
    std::ifstream trainLAB("data/train-labels.idx1-ubyte", std::ios::binary);
    
    //TESTING SET STREAM
    std::ifstream testIM("data/t10k-images.idx3-ubyte", std::ios::binary);
    std::ifstream testLAB("data/t10k-labels.idx1-ubyte", std::ios::binary);
    
    if (!trainIM.is_open() || !trainLAB.is_open() || !testIM.is_open() || !testLAB.is_open()) {
        throw std::runtime_error("Error : impossible to open some MNIST files, DON'T RENAME ORIGINAL MNIST FILES");
    }

    uint32_t headerTrainIM[4] = {0x0};                          // {magic number, number of images, num rows, num cols}
    uint32_t headerTrainLAB[2] = {0x0};                         // {magic number, number of labels}
    uint32_t headerTestIM[4] = {0x0};                           // {magic number, number of images, num rows, num cols}
    uint32_t headerTestLAB[2] = {0x0};                          // {magic number, number of labels}

    for(size_t i = 0 ; i<16; i++){                              //extracts image header
        int train = trainIM.get();
        int test = testIM.get();
        if ((train == EOF) || (train ==EOF)) throw std::runtime_error("EOF inattendu dans header images");
        
        headerTrainIM[i/4] |= static_cast<uint32_t>(train) << (24 - 8*(i % 4));
        headerTestIM[i/4] |= static_cast<uint32_t>(test) << (24 - 8*(i % 4));

    }

    for (int i = 0; i < 8; i++) {                               //extracts label header
        int train = trainLAB.get();
        int test = testLAB.get();
        if ((train == EOF) || (test == EOF)) throw std::runtime_error("EOF inattendu dans header labels");
        headerTrainLAB[i/4] |= static_cast<uint32_t>(train) << (24 - 8*(i % 4));
        headerTestLAB[i/4] |= static_cast<uint32_t>(test) << (24 - 8*(i % 4));
    }

    // --- Header matching check ---
    assert((headerTestIM[0] == 2051 && headerTrainIM[0] == 2051)  &&  "Magic number failed, should be 2051 in MNIST official dataset for images");
    assert((headerTestIM[1] == 10000) &&  "Images should be 10000 in the test set");
    assert((headerTrainIM[1] == 60000) &&  "Images should be 60000 in the train set");

    assert((headerTestLAB[0] == 2049 && headerTrainLAB[0] == 2049) && "Magic number failed, should be 2049 in MNIST official dataset for labels");
    assert(headerTestLAB[1] == headerTestIM[1] && "Labels should be 10000 in the test set");
    assert(headerTrainLAB[1] == headerTrainIM[1] && "Labels should be 60000 in the test set");

    //config initalisation
    n_imTrain = 60000;
    n_imTest = 10000;
    n_rows = 28;
    n_cols = 28;

    //allocation of all my ressources for exploiting the data set
    trainSet.first = new uint8_t[n_imTrain*n_rows*n_cols];                  //each image is  (uint8_t * n_col *n_row) of size
    trainSet.second = new uint8_t[n_imTrain];                               //each label is single uint8_t
    testSet.first = new uint8_t[n_imTest*n_rows*n_cols];
    testSet.second = new uint8_t[n_imTest];
    
    std::streamsize image_bytes = n_imTrain * n_rows * n_cols;
    // --- Images read ---
    trainIM.read(reinterpret_cast<char*>(trainSet.first), image_bytes);
    if (!trainIM) {
        throw std::runtime_error("Lecture images train incomplète");
    }
    
    std::streamsize test_bytes  = n_imTest  * n_rows * n_cols;
    testIM.read(reinterpret_cast<char*>(testSet.first), test_bytes);
    if (!testIM) {
        throw std::runtime_error("Lecture images test incomplète");
    }
    
    // --- Labels read ---
    trainLAB.read(reinterpret_cast<char*>(trainSet.second), n_imTrain);
    if (!trainLAB) throw std::runtime_error("Lecture labels train incomplète");

    testLAB.read(reinterpret_cast<char*>(testSet.second), n_imTest);
    if (!testLAB) throw std::runtime_error("Lecture labels test incomplète");  
}

MNISTReader::~MNISTReader(){
    delete []trainSet.first;
    delete []trainSet.second;
    delete []testSet.first;
    delete []testSet.second;
}

const uint8_t *MNISTReader::getImage(size_t index, bool fromTest)const {
    const std::pair<uint8_t*,uint8_t*> &set = fromTest ? testSet : trainSet;                //selects the test or training set depending on fromTest
    const auto  n_elements = fromTest ? n_imTest : n_imTrain;


    if(!set.first){                                                                        //checks allocation
        std::cerr<<"No images initialized"<<std::endl;
        return nullptr;
    }
    if(index>=n_elements){                                                                  //checks out of bond
            std::cerr<<"Invalid index"<<std::endl;
            return nullptr;
    }
    return set.first+(index*n_rows*n_cols);
}

const uint8_t *MNISTReader::getLabel(size_t index, bool fromTest) const{ 
    
    const std::pair<uint8_t*,uint8_t*> &set = fromTest ? testSet : trainSet;                //selects the test or training set depending on fromTest
    const auto  n_elements = fromTest ? n_imTest : n_imTrain;
 
    if(!set.second){                                                                        //checks allocation
        std::cerr<<"No labels initialized"<<std::endl;
        return nullptr;
    }
    if(index>=n_elements){                                                                  //checks out of bond
        std::cerr<<"Invalid index"<<std::endl;
        return nullptr;
    }
    return set.second+index;
}

const uint32_t MNISTReader::get_num_images_Train() const{
    return n_imTrain;
}

const uint32_t MNISTReader::get_num_images_Test() const{
    return n_imTest;
}



const uint32_t MNISTReader::get_num_rows() const{
    return n_rows; //should be 28
}

const uint32_t MNISTReader::get_num_cols() const{
    return n_cols; //should be 28
}

Matrix MNISTReader::X_bach(size_t batch_start, size_t batch_size){
    Matrix X(batch_size,n_rows*n_cols);                                                     // X is a (batch_size × 784) matrix: one flattened MNIST image per row.
    for(size_t i = 0; i < batch_size; i++){                                                 // Row i corresponds to image (batch_start + i).                                       
        for(size_t j = 0; j<n_rows*n_cols;j++){
            X(i,j) = static_cast<double>(*(getImage(batch_start + i,false) +j) );            //since we create batch from training images, we pass true parameter to getImage()
        }
    }
    return X;
}

Matrix MNISTReader::Y_bach(size_t batch_start, size_t batch_size){
    Matrix Y(batch_size, 10);                                                               //Y is a (batch_size × 10) matrix: giving one hot vector for correct output
    for (size_t i = 0; i < batch_size; i++) {
        uint8_t label = *getLabel(batch_start + i,false);                                    //since we create batch from training images, we pass true parameter to getLabel()
        Y(i, label) = 1.0;
    }
    return Y;

}


void MNISTReader::plot_mnist_direct(size_t imageIndex, bool fromTest) {                     //true if from test set, false if comes from training set
    const std::pair<uint8_t*,uint8_t*> &set = fromTest ? testSet : trainSet;                //selects the test or training set depending on fromTest
    std::ofstream dataFile("image.dat");
    for (int y = 0; y<28; y++) {
        for (int x = 0; x<28; x++) {
            dataFile << x << " " << y << " " 
            << static_cast<int>(set.first[(imageIndex * 784) + ((27-y) * 28) + x])          //filps the image verticaly
            << "\n";
            dataFile << "\n"; 
        }
    }
    dataFile.close();
    system("gnuplot -persistent -e \"set view map; set palette gray; plot 'image.dat' with image\"");
    std::remove("image.dat");
}

void MNISTReader::plot_mnist_direct(const Matrix &img) {
    assert(img.rows() == 1 && img.cols() == 784);

    std::ofstream dataFile("image.dat");
    if (!dataFile.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir image.dat\n";
        return;
    }

    for (size_t y = 0; y < 28; ++y) {
        for (size_t x = 0; x < 28; ++x) {
            size_t index = (27 - y) * 28 + x;
            dataFile << x << " " << y << " "
                     << static_cast<int>(img(0, index))
                     << "\n";
        }
        dataFile << "\n";
    }
    dataFile.close();

    int ret = system("gnuplot -persistent -e \"set view map; set palette gray; plot 'image.dat' with image\"");
    if (ret != 0) {
        std::cerr << "Erreur : appel gnuplot échoué\n";
    }
}






/* std::cout<<headerTestIM[0]<<std::endl;
    std::cout<<headerTestIM[1]<<std::endl;
    std::cout<<headerTestIM[2]<<std::endl;
    std::cout<<headerTestIM[3]<<std::endl;

    std::cout<<headerTrainIM[0]<<std::endl;
    std::cout<<headerTrainIM[1]<<std::endl;
    std::cout<<headerTrainIM[2]<<std::endl;
    std::cout<<headerTrainIM[3]<<std::endl;
    std::cout<<std::endl;


    std::cout<<headerTestLAB[0]<<std::endl;
    std::cout<<headerTestLAB[1]<<std::endl;

    std::cout<<headerTrainLAB[0]<<std::endl;
    std::cout<<headerTrainLAB[1]<<std::endl; */