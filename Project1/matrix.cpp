#include "Matrix.h"
#include <algorithm>
#include <ctime>
#include <climits>

//******************************************** constructors and destructor ********************
// constructor for conversion string char* to object
Matrix::Matrix(const char * c_str) {
	//std::cout << "---->Matrix(const char* c_str)<----\n";
	std::istringstream istr(c_str);
	// counting rows and cols
	int total = 0, x_cols = 0;
	double num;
	char ch;
	while (istr >> ch) {
		if (isdigit(ch) || ch == '-') {
			istr.unget();
			istr >> num;
			total++;
			x_cols++;
		}
		if (ch == ';') {
			cols = x_cols;
			x_cols = 0;
		}
	} 
	if (x_cols != 0) 
		cols = x_cols;
	rows = total / cols;
	// create and fill array
	create();
	std::istringstream istr1(c_str);
	int i = 0, j = 0;
	while (istr1 >> ch) {
		if (isdigit(ch) || ch == '-') {
			istr1.unget();
			istr1 >> num;
			matrix[i][j] = num;
			j++;
		}
		if (ch == ';' && i < rows) {
			i++; j = 0;
		}
	}
}

// copy constructor
Matrix::Matrix(const Matrix& orig) {
	//std::cout << "---->Copy Constructor<----\n";

	rows = orig.rows;
	cols = orig.cols;
	create();

	//copying all elements
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			matrix[i][j] = orig.matrix[i][j];
}

// move constructor
Matrix::Matrix(Matrix&& m) : rows(m.rows), cols(m.cols), matrix(m.matrix) {
	//std::cout << "---->Move Constructor<----\n";

	m.rows = m.cols = 0;
	m.matrix = nullptr;
}

Matrix::~Matrix()
{
	for (int i = 0; i < rows; i++)
		delete[] matrix[i];
	delete[] matrix;
}

//*************************************************** methods *****************************
void Matrix::create() {
	matrix = new double *[rows];               //allocating memory for an array of pointers
	for (int i = 0; i < rows; i++)
		matrix[i] = new double[cols];	        //allocating memory for all others arrays
}

void Matrix::operator=(const Matrix& orig) {
	//std::cout << "---->Copy Assignment<----\n";
	if (this != &orig) {   //self-assignment check
		//free the memory of old matrix
		for (int i = 0; i < rows; i++)
			delete[] matrix[i];
		delete[] matrix;
		//allocating the same size of memory as another matrix
		rows = orig.rows;
		cols = orig.cols;
		create();
		//copying all elements
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				matrix[i][j] = orig.matrix[i][j];
	}
}

void Matrix::operator=(Matrix&& orig) {
	//std::cout << "---->Move Assignment<----\n";
	(*this).~Matrix();
	//shallow copy
	rows = orig.rows;
	cols = orig.cols;
	matrix = orig.matrix;
	// make safe orig
	orig.rows = orig.cols = 0;
	orig.matrix = nullptr;
}
void Matrix::fill() {
	srand(time(NULL));
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			matrix[i][j] = rand() % 10000 - 5000;
}

void Matrix::display() {
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout.width(12);
			std::cout << matrix[i][j];
		}
		std::cout << std::endl;
	}
}

std::string Matrix::to_string() {
	std::ostringstream os;
	os << '[';
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			os << matrix[i][j];
			if (j < cols - 1)
				os << ", ";
		}
		if (i < rows - 1)
			os << "; ";
	}
	os << ']';
	return os.str();   // convert type ostringstream to string
}

void Matrix::show() {
	std::cout << '[';
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			std::cout << matrix[i][j];
			if (j < cols - 1)
				std::cout << ", ";
		}
		if (i < rows - 1)
			std::cout << "; ";
	}
	std::cout << ']' << std::endl;
}

double Matrix::elem_sum() const {
	double sum = 0;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			sum += matrix[i][j];
	return sum;
}

//*************************************************OVERLOADING********************************

//**************************************************addition**********************************
Matrix Matrix::operator+(const Matrix& b)const {
	if (cols != b.cols || rows != b.rows) {
		std::cerr << "It is impossible to perform the addition operation, "
			<< "the matrices do not meet the conditions.\n";
		return *this;
	}
	else {
		Matrix matx(rows, cols);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++) {

				if (b.matrix[i][j] > 0 && matrix[i][j] > LLONG_MAX - b.matrix[i][j] ||        //overflow diagnostic
					b.matrix[i][j] < 0 && matrix[i][j] < LLONG_MIN - b.matrix[i][j]) {
					std::cerr << "It is impossible to perform the addition operation, because we have overflow of range.\n";
					return *this;
				}

				matx.matrix[i][j] = matrix[i][j] + b.matrix[i][j];
			}
		return matx;
	}
}


//**************************************************substraction*******************************
Matrix Matrix::operator-(const Matrix& b)const {
	if (cols != b.cols || rows != b.rows) {
		std::cerr << "It's impossible to perform the substraction operation, "
			<< "the matrices do not meet the conditions.\n";
		return *this;
	}
	else{
		Matrix matx(rows, cols);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++) {

				if (b.matrix[i][j] > 0 && matrix[i][j] < LLONG_MIN + b.matrix[i][j] ||        //overflow diagnostic
					b.matrix[i][j] < 0 && matrix[i][j] > LLONG_MAX + b.matrix[i][j]) {
					std::cerr << "It is impossible to perform the substraction operation, because we have overflow of range.\n";
					return *this;
				}
			
				matx.matrix[i][j] = matrix[i][j] - b.matrix[i][j];
			}
		return matx;
	}
}

//********************************************************multiplication*********************************
Matrix Matrix::operator*(const Matrix& b) const {
	if ((*this).cols != b.rows) {
		std::cerr << "Error! column of first matrix not equal to row of second.\n";
		return *this;
	}
	Matrix mult{ (*this).rows, b.cols };
	for (int i = 0; i < mult.rows; i++)
		for (int j = 0; j < mult.cols; j++)
			mult.matrix[i][j] = 0;
		
	for (int i = 0; i < (*this).rows; i++)
		for (int j = 0; j < b.cols; j++)
			for (int k = 0; k < (*this).cols; k++)
				mult.matrix[i][j] += (*this).matrix[i][k] * b.matrix[k][j];

	return mult;
}

Matrix Matrix::operator*(double x)const {
	Matrix mult(rows, cols);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			mult.matrix[i][j] = matrix[i][j] * x;
	return mult;
}

//**************************************************dividing**********************************
Matrix Matrix::minor(int not_this_r, int not_this_c)const {
	Matrix minor(rows - 1, cols - 1);
	int i = 0;
	int j = 0;
	// Looping for each element of the matrix 
	for (int row = 0; row < rows; row++)
	{
		for (int col = 0; col < cols; col++)
		{
			//  Copying into temporary matrix only those element 
			//  which are not in given row and column 
			if (row != not_this_r && col != not_this_c)
			{
				minor.matrix[i][j++] = matrix[row][col];

				// Row is filled, so increase row index and 
				// reset col index 
				if (j == cols - 1)
				{
					j = 0;
					i++;
				}
			}
		}
	}
	return minor;
}

double Matrix::determinant()const {
	if (rows == 1)
		return matrix[0][0];
	if (rows == 2)
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		
	int row = 0;
	double det = 0;
	int sign = 1;
	for (int j = 0; j < cols; j++) {
		if (matrix[row][j] == 0) {
			det += 0;
			sign *= -1;
		}
		else {
			det += sign * matrix[row][j] * minor(row, j).determinant();  // recursion	
			sign *= -1;
		}
	}
	return det;
}
Matrix Matrix::invert()const {
	Matrix invert(rows, cols);
	// making transpose matrix 
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			invert.matrix[i][j] = matrix[j][i];
	std::cout << "Transpose matrix:\n";
	invert.display();
	// Making A* (Adjugate matrix)
	Matrix un(rows, cols);
	int sign = 1;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			un.matrix[i][j] = sign * invert.minor(i, j).determinant();
			sign *= -1;
		}
	std::cout << "Adjugate matrix:\n";
	un.display();
	double det = determinant();
	invert = un / int(det);
	std::cout << "Invert matrix:\n";
	invert.display();
	return invert;
}

Matrix Matrix::operator/(const Matrix& b)const {
	if ((*this).cols != b.rows) {
		std::cerr << "Error! column of first matrix not equal to row of second.\n";
		return *this;
	}
	if (b.rows != b.cols) {
		std::cerr << "Error! Determinant in a non-square matrix cannot be determined\n";
		return *this;
	}
	if (b.determinant() == 0) {
		std::cerr << "Error! Determinant = 0, it is impossible to count the inverse matrix\n";
		return *this;
	}
	return *this * b.invert();
}
Matrix Matrix::operator/(double x)const {
	if (x == 0) {
		std::cerr << "Error! Dividing by zero!";
		return *this;
	}
	Matrix div(rows, cols);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			div.matrix[i][j] = matrix[i][j] / x;
	return div;
}

//***************************relation operations and equality checks*******************
bool Matrix::operator>(const Matrix& b) const {
	if (elem_sum() > b.elem_sum())
		return true;
	else
		return false;
}

bool Matrix::operator>=(const Matrix& b) const {
	if (elem_sum() >= b.elem_sum())
		return true;
	else
		return false;
}

bool Matrix::operator<(const Matrix& b) const {
	if (elem_sum() < b.elem_sum())
		return true;
	else
		return false;
}

bool Matrix::operator<=(const Matrix& b) const {
	if (elem_sum() > b.elem_sum())
		return true;
	else
		return false;
}

bool Matrix::operator==(const Matrix& b) const {
	if (elem_sum() == b.elem_sum())
		return true;
	else
		return false;
}
bool Matrix::operator!=(const Matrix& b) const {
	if (elem_sum() != b.elem_sum())
		return true;
	else
		return false;
}
