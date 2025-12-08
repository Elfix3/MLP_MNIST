#include "MNISTReader.h"
#include <winsock2.h>

MNISTReader::MNISTReader(const char* images_files, const char* labels_files){
    std::ifstream imStream(images_files, std::ios::binary);
    std::ifstream labStream(labels_files, std::ios::binary);
    if (!imStream.is_open() || !labStream.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir les fichiers MNIST");
    }
    uint32_t headerIm[4] = {0x0}; // {magic number, number of images, num rows, num cols
    uint32_t headerLab[2] = {0x0}; // {magic number, number of labels}
    

    for(size_t i = 0 ; i<16; i++){
        int c = imStream.get();
        if (c == EOF) throw std::runtime_error("EOF inattendu dans header images");
        headerIm[i/4] |= static_cast<uint32_t>(c) << (24 - 8*(i % 4));
    }
    
    for (int i = 0; i < 8; i++) {
        int c = labStream.get();
        if (c == EOF) throw std::runtime_error("EOF inattendu dans header labels");
        headerLab[i/4] |= static_cast<uint32_t>(c) << (24 - 8*(i % 4));
    }

    assert(headerIm[0] == 2051 && "image magic number check");
    assert(headerLab[0] == 2049 && "magic number check");
    assert(headerLab[1] == headerIm[1] && "images and label numbers check");
    
    
    num_images = headerIm[1];
    rows = headerIm[2];
    cols = headerIm[3];

    // --- Lecture labels ---
    labels = new uint8_t[num_images];
    labStream.read(reinterpret_cast<char*>(labels), num_images);
    if (labStream.gcount() != static_cast<std::streamsize>(num_images))
        throw std::runtime_error("Erreur lecture labels");

    // --- Lecture images ---
    images = new uint8_t[num_images * rows * cols];
    imStream.read(reinterpret_cast<char*>(images), num_images * rows * cols);
    if (imStream.gcount() != static_cast<std::streamsize>(num_images * rows * cols))
        throw std::runtime_error("Erreur lecture images");

    /* labels = new (std::nothrow) uint8_t[(size_t)headerLab[1]];
    if(!labels){std::cerr<<"Incorrect labels allocation"<<std::endl;};
    
    std::cout << "Position curseur avant read(): " << imStream.tellg() << std::endl;
    std::cout << "Position curseur avant read(): " << labStream.tellg() << std::endl;
    labStream.read(reinterpret_cast<char*>(labels),headerLab[1]);


    images = new (std::nothrow) uint8_t[(size_t)headerIm[1]*headerIm[2]*headerIm[3]];
    if(!images){std::cerr<<"Incorrect labels allocation"<<std::endl;};

    imStream.read(reinterpret_cast<char*>(images),headerIm[1]*headerIm[2]*headerIm[3]); */

    /* std::cout<<headerIm[0]<<std::endl; //debug
    std::cout<<headerIm[1]<<std::endl;
    std::cout<<headerIm[2]<<std::endl;
    std::cout<<headerIm[3]<<std::endl;

    
    std::cout<<headerLab[0]<<std::endl;
    std::cout<<headerLab[1]<<std::endl; */
    
}

/* MNISTReader::~MNISTReader(){
    std::cout<<"this shit called";
    delete []images;
    delete [] labels;
} */

void MNISTReader::debug(){
    for(int i = 0; i<100;i++){
        std::cout<<(int)labels[i]<<std::endl;
    }
}

const uint8_t *MNISTReader::getImage(size_t index)const {
    if(!images){
        std::cerr<<"No images initialized"<<std::endl;
        return nullptr;
    }
    if(index>=num_images){
        std::cerr<<"Invalid index"<<std::endl;
        return nullptr;
    }

    return images+(index*rows*cols);
}

const uint8_t *MNISTReader::getLabel(size_t index) const{
    if(!labels){
        std::cerr<<"No labels initialized"<<std::endl;
        return nullptr;
    }
    if(index>=num_images){
        std::cerr<<"Invalid index"<<std::endl;
        return nullptr;
    }
    return labels+index;

}

const uint32_t MNISTReader::get_num_images() const{
    return num_images;
}

const uint32_t MNISTReader::get_num_rows() const{
    return rows; //should be 28
}

const uint32_t MNISTReader::get_num_cols() const{
    return cols; //should be 28
}

void MNISTReader::showImageAndLabel(size_t i)
{
    using namespace std;
    
    for(size_t j = i*784; j< ((i+1)*784); j++){
        cout << (((int)images[j] > 100) ? '.' : ' ') << " ";

        if((j+1)%28 == 0){cout<<endl;}
    }
    cout<<endl;
    cout<<(int)labels[i]<<endl;

    cout<<endl;
    cout<<endl;
    cout<<endl;
}

void MNISTReader::plot_mnist_direct(size_t imageIndex) {
    std::ofstream dataFile("image.dat");

    for (int y = 0; y<28; y++) {
        for (int x = 0; x<28; x++) {
            dataFile << x << " " << y << " " 
                     << static_cast<int>(images[(imageIndex * 784) + ((28-y) * 28) + x]) 
                     << "\n";
        }
        dataFile << "\n"; // sépare les lignes pour Gnuplot
    }

    dataFile.close(); // fermer avant d'appeler Gnuplot
    system("gnuplot -persistent -e \"set view map; set palette gray; plot 'image.dat' with image\"");
    //std::remove("image.dat");
}

/* void MNISTReader::readData(const char *fileName)
{
    std::ifstream file(fileName,std::ios::binary);
    if(file){
        std::cout<<"Fichier lu\n";
    } else {
        std::cout<<"Erreur lecture fichier\n";
    }
    assert(file);
    file.seekg(16+784*16, std::ios::beg);

    const int rows = 28;
    const int cols = 28;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            unsigned char pixel;
            file.read(reinterpret_cast<char*>(&pixel), 1);
            std::cout << std::setw(3) << std::dec << (int)pixel << " ";
        }
        std::cout << "\n";
    }
}
 */



