#include "Matrix.h"


static std::mt19937 gen(static_cast<unsigned>(time(nullptr)) + __TIME__[0]);


Matrix::Matrix() : n_rows(0),n_cols(0),datas(nullptr){

}

Matrix::Matrix(size_t rows, size_t cols) : n_rows(rows), n_cols(cols){
    datas = new double[rows*cols]();
}

Matrix::Matrix(size_t rows, size_t cols, const uint8_t *pixels) : n_rows(1),n_cols(rows*cols){
    datas = new double[rows*cols]();
    for (size_t i = 0; i < rows * cols; ++i) {
        datas[i] = static_cast<double>(pixels[i]);
    }
    
}

Matrix::Matrix(size_t rows, size_t cols, GENERATION_MODE G_MODE) : n_rows(rows), n_cols(cols){
    datas = new double[rows*cols]();

    std::uniform_int_distribution<int> d0(0,1);
    std::uniform_int_distribution<int> d1(0, 255);
    std::uniform_real_distribution<double> d2(-1.0, 1.0);

    switch(G_MODE){
        case ZEROS :
            break; //already zeroes

        case IMAGE_BIN :
            for (size_t i = 0; i < rows * cols; ++i) {
                datas[i] = d0(gen);  // 0 or 1
            }
            break;

        case IMAGE_GRAY_SCALE :
            for (size_t i = 0; i < rows * cols; ++i) {
                datas[i] = d1(gen);  // 0 to 255
            }
            break;

        case NORMALIZED :
            for (size_t i = 0; i < rows * cols; ++i) {
                datas[i] = d2(gen);  // double -1 to 1
            }
            break;

        default :
            break;
    }

}

Matrix::Matrix(size_t rows, size_t cols, double value): n_rows(rows), n_cols(cols){
    datas = new double[rows*cols];
    for(size_t i = 0; i<rows*cols;i++) datas[i] = value;
}

Matrix::Matrix(const Matrix &other) :  n_rows(other.n_rows), n_cols(other.n_cols){
    datas = new double[ other.n_rows * other.n_cols]();
    for(size_t i = 0; i<other.n_rows*other.n_cols;i++){
        datas[i] = other.datas[i];
    }
}

Matrix::Matrix(Matrix &&other) noexcept :  n_rows(other.n_rows), n_cols(other.n_cols){
    other.n_cols = 0;
    other.n_rows = 0;
    datas = other.datas;
    other.datas = nullptr;
}

Matrix::~Matrix(){
    delete []datas;
}

double &Matrix::operator()(size_t i, size_t j){
    return datas[i*n_cols+j];
}

const double &Matrix::operator()(size_t i, size_t j) const{
    return datas[i*n_cols+j];
}

size_t Matrix::rows() const{
    return this->n_rows;
}

size_t Matrix::cols() const{
    return this->n_cols;
}

Matrix &Matrix::operator=(const Matrix &other){
    if(this != &other){
        delete []datas;
        n_cols = other.n_cols;
        n_rows = other.n_rows;
        datas = new double[n_cols*n_rows];
    
        for(size_t i = 0; i< n_cols*n_rows; i++){
            datas[i] = other.datas[i];
        }
    
    }
    return  *this;
}

Matrix& Matrix::operator=(Matrix &&other) noexcept {
    if (this != &other) { 
        delete[] datas;             

        // Transférer les membres
        n_rows = other.n_rows;
        n_cols = other.n_cols;
        datas = other.datas;

        other.n_rows = 0;
        other.n_cols = 0;
        other.datas = nullptr;
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &other) const{
    assert(n_rows == other.n_rows && n_cols == other.n_cols);
    Matrix sumMatrix(n_rows, n_cols);
    for(size_t i = 0; i<n_cols*n_rows;i++){
        sumMatrix.datas[i] = datas[i] + other.datas[i];
    }
    return sumMatrix;
}

Matrix Matrix::operator-(const Matrix &other) const{
    assert(n_rows == other.n_rows && n_cols == other.n_cols);
    Matrix diffMatrix(n_rows, n_cols);
    for(size_t i = 0; i<n_cols*n_rows;i++){
        diffMatrix.datas[i] = datas[i] - other.datas[i];
    }
    return diffMatrix;
}

Matrix &Matrix::operator+=(const Matrix &other){
    assert(n_rows == other.n_rows && n_cols == other.n_cols);
    for(size_t i = 0; i<n_cols*n_rows;i++){
        datas[i] = datas[i] + other.datas[i];
    }
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &other){
    assert(n_rows == other.n_rows && n_cols == other.n_cols);
    for(size_t i = 0; i<n_cols*n_rows;i++){
        datas[i] = datas[i] - other.datas[i];
    }
    
    return *this;
}

Matrix Matrix::operator*(double scalar) const{
    Matrix scaledMatrix(n_rows, n_cols);
    for(size_t i = 0; i<n_cols*n_rows;i++){
        scaledMatrix.datas[i] = datas[i]*scalar;
    }
    
    return scaledMatrix;
}

Matrix &Matrix::operator*=(double scalar){
    for(size_t i = 0; i<n_cols*n_rows;i++){
        datas[i]*=scalar;
    }
    
    return *this;
}

Matrix Matrix::operator*(const Matrix &other) const{ //this * other
    assert(n_cols == other.n_rows); //result is  n_rows* other.n_cols
    Matrix productMatrix(n_rows,other.n_cols);
    for(size_t i = 0; i < productMatrix.n_cols*productMatrix.n_rows; i++){
        size_t indexCol = i%productMatrix.n_cols;
        size_t indexRow = i/productMatrix.n_cols;
        for(size_t j = 0 ; j <n_cols ; j++){
           productMatrix.datas[indexRow*productMatrix.n_cols + indexCol]+= datas[indexRow*n_cols + j]*other.datas[j*other.n_cols + indexCol];
        }
    }


    return productMatrix;
}




Matrix Matrix::transpose() const {
    Matrix transposed(n_cols, n_rows);  //transposed dims
    for(size_t i = 0; i < n_rows; i++) {      
        for(size_t j = 0; j < n_cols; j++) {   
            transposed(j, i) = (*this)(i, j);  // index swaps
        }
    }

    return transposed;
}

Matrix Matrix::Normalize(double div) const{
    Matrix normalized(n_rows,n_cols);
    for(size_t i =0; i<n_cols*n_rows;i++){
        normalized.datas[i] = datas[i]/div;
    }
    return normalized;
}

Matrix Matrix::identity(size_t n){
    Matrix id(n,n);
    for(size_t i = 0; i< n; i++){
        id(i,i) = 1;
    }
    return id;
}

Matrix Matrix::RELU()const{
    assert(n_rows == 1 &&"Nombre de lignes != de 1");
    Matrix copy(*this);
    for(size_t i = 0 ; i<n_cols*n_rows;i++){
        copy.datas[i] = (copy.datas[i] > 0) ? copy.datas[i] : 0;
    }
    return copy;
}

Matrix Matrix::SOFTMAX() const{
    assert(n_rows == 1 &&"Nombre de lignes != de 1");
    Matrix copy(*this);

    double max_val = datas[0];

    //finds the translation
    for(size_t i = 1; i<n_cols;i++){
        if(datas[i]>max_val) max_val = datas[i];
    }

    double sumExp = 0;
    const double e = 1e-10;  // seuil
    for(size_t i = 0; i<n_cols; i++){
        copy.datas[i] = std::exp(datas[i]-max_val);
        sumExp+= copy.datas[i];
    }
    
    for(size_t i = 0; i<n_cols; i++){
        copy.datas[i] /= sumExp;
        if(copy.datas[i] < e){
            copy.datas[i] = 0.0;
        }
    }
    return copy;
}



    

void Matrix::fill(double value){
    for(size_t i = 0; i<n_cols*n_rows;i++){
        datas[i] = value;
    }
}


void Matrix::print() const
{
    for(size_t i = 0; i<n_rows;i++){
        for(size_t j = 0; j<n_cols; j++){
            std::cout<<datas[i*n_cols + j]<<" ";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";
}
void Matrix::printSize() const{
    std::cout<<"Size : "<<n_rows<<" x "<<n_cols<<std::endl;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    for (size_t i = 0; i < m.n_rows; i++) {
        for (size_t j = 0; j < m.n_cols; j++) {
            os << m.datas[i * m.n_cols + j] << " ";
        }
        os << "\n";
    }
    return os;
}

