#include "Matrix.h"
int const ROWS = 10;
int const COLS = 10;
int main() {
	double d_arr1 [5][5]{ 
		{12, 13, 14, 15, 16},
		{22, 23, 24, 25, 26},
		{32, 33, 34, 35, 36},
		{42, 43, 44, 45, 46},
		{52, 53, 54, 55, 56} };
	Matrix m1(d_arr1);
	double d_arr2[5][5]{  //this matrix for easy check of determinant
		{1, 2, 3, 4, 5},
		{0, 7, 8, 9, 10},
		{0, 0, 13, 14, 15},
		{0, 0, 0, 19, 20},
		{0, 0, 0, 0, 25} };
	Matrix m2(d_arr2);
	std::cout << "Result of multiplication:\n";
	(m1 * m2).display();

	std::cout << "m2.determinant() = " << m2.determinant() << std::endl;
	Matrix m3 = m1 / m2;
	std::cout << "Result of division:\n";
	m3.display();
	//*************************************
	m3 = m2.invert();
	std::cout << "multiplying the matrix by the inverse of it:\n";
	(m2 * m3).display();   // not too bad

	Matrix m4 = m1 + "222" + m2 + 175 + m3;
	m4.display();
	/*
	double d_arr[3][3] = { {4, -2, 1},
					     {1, 6, -2},
					     {1, 0, 0} };
	Matrix m6(d_arr);
	std::cout << "m6.determinant(): " << m6.determinant() << std::endl;
	Matrix inv6 = m6.invert();
	Matrix m7 = m6 * inv6;
	std::cout << "\n\n\n";
	m7.display(); */

	system("pause");
	return 0;
}








template<size_t ROW, size_t COL>
Matrix::Matrix(double(&d_arr)[ROW][COL]) {
	//std::cout << "I'm in Matrix(double(&d_arr)[ROW][COL])\n";
	rows = ROW;
	cols = COL;
	create();
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			matrix[i][j] = d_arr[i][j];
}

