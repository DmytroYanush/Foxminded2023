#include "Matrix.h"
int main() {
	//bad allocation exception
	Matrix MMM(INT_MAX, INT_MAX);          
	
	double d_arr1 [5][5]{ {12, 13, 14, 15, 16},
						  {22, 23, 24, 25, 26},
						  {32, 33, 34, 35, 36},
						  {42, 43, 44, 45, 46},
						  {52, 53, 54, 55, 56} };
	Matrix m1(d_arr1);

	double d_arr2[5][5]{ {1, 2, 3, 4, 5},
						 {0, 7, 8, 9, 10},
						 {0, 0, 13, 14, 15},
						 {0, 0, 0, 19, 20},
						 {0, 0, 0, 0, 25} };
	Matrix m2(d_arr2);
	
	double d_arr3[3][3] = { {4, -2, 1},
						   {1, 6, -2},
						   {1, 0, 0} };
	Matrix m3(d_arr3); 

	elem_type  low = std::numeric_limits<elem_type>::lowest();
	elem_type d_arr4[2][2]{ {low, low},
							{low, low} };
	Matrix m_low(d_arr4);
	elem_type max = std::numeric_limits<elem_type>::max();
	elem_type d_arr5[2][2]{ {max, max},
							{max, max} };
	Matrix m_max(d_arr5);

	// bad subsrtaction error
	std::cout << "Substraction errors:\n";
	Matrix subst = m3 - m1;  
	// overflow exception (lowest):
	subst = m_low - m_max;
	
	// bad addition error
	std::cout << "Additions errors:\n";
	Matrix add = m1 + m3;
	// overflow exception (max):
	add = m_max + m_max;
	
	// bad multiplication error
	std::cout << "Multiplication error:\n";
	Matrix mult_dev = m1 * m3;
	
	// bad division error
	std::cout << "Division errors:\n";
	Matrix division = m1 / m3;
	double d_arr6[4][5] = { {0, 7, 8, 9, 17},
							{1, 0, 9, 11, 12},
							{0, 0, 1, 7, 8},
							{1, 2, 3, 7, 25} };
	Matrix m4 = d_arr6;
	division = m1 / m4;
	division = m1 / 0;
	
	// non correct string representation of matrix
	Matrix ms("[9.12, 9.13,effff7.19fdffe; fef sef 8.12,8.17,8.10;sdfdffs 9.17, feffg10.30, esfef18.40");
	system("pause");
	return 0;
}





template<size_t ROW, size_t COL>
Matrix::Matrix(elem_type(&d_arr)[ROW][COL]) {
	//std::cout << "I'm in Matrix(double(&d_arr)[ROW][COL])\n";
	rows = ROW;
	cols = COL;
	create();
	for (unsigned i = 0; i < rows; i++)
		for (unsigned j = 0; j < cols; j++)
			matrix[i][j] = d_arr[i][j];
}





