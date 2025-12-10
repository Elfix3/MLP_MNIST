#ifndef MATRIX_H
#define MATRIX_H


#include <iostream>
#include <assert.h>
#include <cstdlib>  // Pour rand() et srand() // to delete
#include <ctime>    // Pour time() //to delete
#include <random>


enum GENERATION_MODE{
    ZEROS, 
    IMAGE_BIN,
    IMAGE_GRAY_SCALE, //for gray scale image 0-255
    NORMALIZED, //valu-es between -1 and 1 (usefull for weights)
};

class Matrix{
    private :
        size_t n_rows;
        size_t n_cols;
        double *datas;

    public :

        // =========================
        // CONSTRUCTEURS / DESTRUCTEUR
        // =========================
        Matrix();                                   // constructeur par défaut
        Matrix(size_t rows, size_t cols); 
        Matrix(size_t rows, size_t cols, const uint8_t* pixels);  //constructeur pour mes images

        Matrix(size_t rows, size_t cols, GENERATION_MODE G_MODE);          // matrice vide dimensions
        Matrix(size_t rows, size_t cols, double value); // remplie avec une valeur

        Matrix(const Matrix& other);               // constructeur de copie
        Matrix(Matrix&& other) noexcept;           // constructeur de déplacement

        ~Matrix();                                 // destructeur

        // =========================
        // ACCÈS AUX DONNÉES
        // =========================

        double& operator()(size_t i, size_t j);             // accès modifiable
        const double& operator()(size_t i, size_t j) const; // accès lecture seule

        size_t rows() const;
        size_t cols() const;

        Matrix getLine(size_t index) const;
        Matrix getCol(size_t index) const;


        // =========================
        // ASSIGNATION
        // =========================
        Matrix& operator=(const Matrix& other);  // assignation par copie
        Matrix& operator=(Matrix&& other) noexcept; //assignation par déplacement


        // =========================
        // OPÉRATIONS DE BASE
        // =========================

        Matrix operator+(const Matrix& other) const;
        Matrix operator-(const Matrix& other) const;

        Matrix& operator+=(const Matrix& other);
        Matrix& operator-=(const Matrix& other);

        Matrix operator^(const Matrix& other) const; //elements per elements product
        Matrix& operator^=(const Matrix& other);

        Matrix operator*(double scalar) const;
        Matrix& operator*=(double scalar);

        Matrix operator/(double scalar) const;
        Matrix& operator/=(double scalar);

        // =========================
        // PRODUIT MATRICIEL
        // =========================

        Matrix operator*(const Matrix& other) const;

        // =========================
        // OPÉRATIONS CLASSIQUES EN IA
        // =========================

        Matrix transpose() const;
        Matrix sum_columns() const;
        Matrix sum_rows() const;
        Matrix broadcastRows(size_t broad_cast_rows) const;

        Matrix Normalize(double div) const;
        static Matrix identity(size_t n);

        Matrix RELU() const;
        Matrix SOFTMAX() const;
        // =========================
        // OUTILS
        // =========================

        void fill(double value);
        void print() const;
        void printSize() const;
        void showProbability();
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

    };

#endif