#include <iostream>
#include <fstream>
#include <new>

using namespace std;

#define filename "matrix.txt"

int** create_matrix(int rows, int cols) {
    int** matrix = new int* [rows];
        for(int i = 0; i < cols; i++) {
            matrix[i] = new int [cols];
        }
    return matrix;
}

void delete_matrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete [] matrix[i];
    }
}

int** read_matrix_from_file(int &rows, int &cols) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Неверное имя файла" << endl;
    }
    fin >> rows >> cols;
    int** matrix = create_matrix(rows, cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            fin >> matrix[i][j];
        }
    }
    fin.close();
    return matrix;
}

void print_matrix(int** matrix, int rows, int cols) {
    cout << "Matrix:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << ' ';
        }
        cout << endl;
    }
    cout << "Amount of rows:" << ' ' << rows << endl;
    cout << "Amount of cols:" << ' ' << cols << endl;

}
