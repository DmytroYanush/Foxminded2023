#include "pch.h"
void task_7();
void task_2_3_5();
int main() {
	//task_2_3_5();
	task_7();
	system("pause");
	return 0;
}
////////////////////////////////////////////// tasks 2, 3, 5 //////////////////////////////////////
void task_2_3_5() {
	//bad allocation exception
	Matrix MMM(INT_MAX, INT_MAX);

	double d_arr1[5][5]{ {12, 13, 14, 15, 16},
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
}
//////////////////////////////////////////////////////// task 7 ///////////////////////////////////////////
void fill_arr_of_matrices(Matrix* arr, size_t size) 
{
	for (int i = 0; i < size; i++)
	{
		Matrix temp(8, 8);
		temp.fill_matrix();
		arr[i] = temp;
	}
}
void fill_vec_of_matrices(std::vector<Matrix>& m_vec, size_t size) 
{
	for (int i = 0; i < size; i++)
	{
		Matrix temp(8, 8);
		temp.fill_matrix();
		m_vec.push_back(temp);
	}
}
void show_arr_of_matrices(Matrix* arr, size_t size)
{
	for (int i = 0; i < size; i++) {
		std::cout << "The sum of the elements of the two diagonals of the matrix#"<< i + 1 << ": " << arr[i].count_diags()
			<< "\nTrace of the matrix: " << arr[i].count_trace() << std::endl;
		arr[i].display();
	}
}

void show_vec_of_matrices(std::vector<Matrix>& m_vec)
{
	for (int i = 0; i < m_vec.size(); i++) {
		std::cout << "The sum of the elements of the two diagonals of the matrix#"<< i + 1 << ": "  << m_vec[i].count_diags()
			<< "\nTrace of the matrix: " << m_vec[i].count_trace() << std::endl;
		m_vec[i].display();
	}
}
void task_7() {
	
	/*Loader *source;
	ConsoleLoader cl;
	source = &cl;
	Matrix m1 = source->load();
	m1.display();

	FileLoader fl;
	source = &fl;
	Matrix m2;
	try {
		m2 = source->load("MyMatrices.txt");
	}
	catch (Exception& E) {
		std::cout << E.what();
	}
	m2.display();*/
	/////////////////////////////////////////////sorting////////////////////////////////////
	const size_t SIZE = 10;
	std::cout << "In what matrices will be stored?\n"
		<< "Press 'a' for array, 'v' for std::vector:\n";
	char ch;
	std::cin >> ch;
	if (ch == 'a' || ch == 'A')
	{
		Matrix m_arr[SIZE];
		fill_arr_of_matrices(m_arr, SIZE);

		Sorter* p_sort = nullptr;
		std::cout << "Should I use standard functions of sorting or custom functions?\n"
			<< "Press 's' for standard, 'c' for custom:\n";
		std::cin >> ch;
		if (ch == 's' || ch == 'S')
		{
			p_sort = &QuickSort();
			p_sort->matrixSort(m_arr, SIZE);        //  void QuickSort::matrixSort(Matrix* m_ptr, size_t q_elem)
			std::cout << "RESULT:\n";
			show_arr_of_matrices(m_arr, SIZE);
		}
		else if (ch == 'c' || ch == 'C')
		{
			p_sort = &UsualSort();
			p_sort->matrixSort(m_arr, SIZE);        // void UsualSort::matrixSort(Matrix* m_ptr, size_t size) 
			std::cout << "RESULT:\n";
			show_arr_of_matrices(m_arr, SIZE);
		}
	}
	if (ch == 'v' || ch == 'V')
	{
		std::vector<Matrix> m_vec;
		fill_vec_of_matrices(m_vec, SIZE);

		Sorter* p_sort = nullptr;
		std::cout << "Should I use standard functions of sorting or custom functions?\n"
			<< "Press 's' for standard, 'c' for custom:\n";
		std::cin >> ch;
		if (ch == 's' || ch == 'S')
		{
			p_sort = &QuickSort();
			p_sort->matrixSort(m_vec, SIZE);        // void QuickSort::matrixSort(std::vector<Matrix>& mvec, size_t size) 

			std::cout << "RESULT:\n";
			show_vec_of_matrices(m_vec);
		}
		else if (ch == 'c' || ch == 'C')
		{
			p_sort = &UsualSort();
			p_sort->matrixSort(m_vec, SIZE);        // void UsualSort::matrixSort(std::vector<Matrix>& mvec, size_t size) 

			std::cout << "RESULT:\n";
			show_vec_of_matrices(m_vec);
		}
	}
	
	
	
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





