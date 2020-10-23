#include "Matrix.h"
#include <algorithm>
#include <ctime>
#include <climits>

#define OUT(x) std::cout << x << std::endl
inline void error() {
	std::cout << "Invalid string!\nIt must be a string representation of a matematical object(matrix)!\n";
}
//******************************************** constructors and destructor ********************
// constructor for conversion string char* to object
Matrix::Matrix(const char * c_str) {
	/*here there are checks for non-empty string,
	for valid characters, for the correct sequence of characters,
	for the equality of elements in the rows*/
	std::cout << "I'm in Matrix(const char* c_str)\n";
	std::istringstream istr(c_str);
	// counting rows and cols
	int total = 0, x_cols = 0;
	unsigned begin = 0;   // for controlling beginning of string - '['
	unsigned end = 0;	  // for controlling ending of string - ']'
	long num;
	char ch;
	while (istr >> ch) {
		//character validation
		if ((!isdigit(ch)) && (ch != '[') && (ch != ']') && (ch != '-') && (ch != ',') && (ch != ';')) {
			error();
			cols = 0;
			return;
		}
		//syblol '[' must be the first and only 1 time
		if (ch == '[') {
			begin += 1;
			if (begin > 1) {
				error();
				cols = 0;
				return;
			}
		}
		//symbol ']' must be the last
		else if (ch == ']') {
			if (end == 1 || begin == 0) {   // always checking begin == 0, because '[' only can be the first
				error();
				cols = 0;
				return;
			}
			end += 1;
		}
		else if (isdigit(ch) || ch == '-') {
			if (begin == 0) {
				error();
				cols = 0;
				return;
			}
			istr.unget();
			istr >> num;
			total++;
			x_cols++;
			istr >> ch;
			// after the number can be only the following characters: ','  ';'  ']'
			if (ch != ',' && ch != ';' && ch != ']') {
				error();
				cols = 0;
				return;
			}
			istr.unget();
		}
		// after punctuation symbols can be only numbers or symbol '-' 
		else if (ch == ',') {
			istr >> ch;
			if (ch == ',') {
				error();
				cols = 0;
				return;
			}
			istr.unget();
		}
		else if (ch == ';') {
			istr >> ch;
			//checking if the string is empty (total == 0)
			if (total == 0 || begin == 0 || ch == ',') {
				error();
				cols = 0;
				return;
			}
			//check the equality of the number of elements in the rows
			if (cols != 0 && cols != x_cols) {
				error();
				cols = 0;
				return;
			}
			istr.unget();
			cols = x_cols;
			x_cols = 0;
		}
	}
	rows = total / cols;
	// create and fill array
	if (!create()) bad_allocation(*this);
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
	OUT("---->Copy Constructor<----");

	rows = orig.rows;
	cols = orig.cols;
	if (!create()) bad_allocation(*this);

	//copying all elements
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			matrix[i][j] = orig.matrix[i][j];
}

// move constructor
Matrix::Matrix(Matrix&& orig) : rows(orig.rows), cols(orig.cols), matrix(orig.matrix) {
	OUT("---->Move Constructor<----");
	orig.rows = 0;
	orig.cols = 0;
	orig.matrix = nullptr;
}

Matrix::~Matrix()
{
	if (matrix != nullptr) {
		for (unsigned i = 0; i < rows; i++)
			delete[] matrix[i];
		delete[] matrix;
	}
}


//*************************************************** methods *****************************
bool Matrix::create() {
	matrix = new(std::nothrow) elem_type *[rows];
	if (matrix == nullptr)
		return false;
	for (unsigned i = 0; i < rows; i++) {
		matrix[i] = new(std::nothrow) elem_type[cols];
		if (matrix[i] == nullptr)
			return false;
	}
	return true;
}
void Matrix::bad_allocation(Matrix & orig) {
	std::cout << "The memory wasn't allocated.\n";
	if (matrix != nullptr) {
		size_t i = 0;
		while (matrix[i] != nullptr)
			delete[] matrix[i++];
		delete[] matrix;
	}
	rows = cols = 0;
}


Matrix& Matrix::operator=(const Matrix& orig) {
	OUT("---->Copy Assignment<----\n");
	if (matrix != orig.matrix) {
		this->~Matrix();
		rows = orig.rows;
		cols = orig.cols;
		if(!create()) bad_allocation(*this);
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				matrix[i][j] = orig.matrix[i][j];
			}
		}
	}
	return *this;
}

Matrix& Matrix::operator=(Matrix&& orig) {
	//std::cout << "---->Move Assignment<----\n";
	(*this).~Matrix();
	//shallow copy
	rows = orig.rows;
	cols = orig.cols;
	matrix = orig.matrix;   //
	// make safe orig
	orig.rows = 0;
	orig.cols = 0;
	orig.matrix = nullptr;
	return *this;
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
	if (cols != b.rows) {
		std::cerr << "Error! column of first matrix not equal to row of second.\n";
		return *this;
	}
	Matrix mult{ rows, b.cols };
			
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < b.cols; j++) {
			mult.matrix[i][j] = 0;
			for (int k = 0; k < cols; k++)
				mult.matrix[i][j] += matrix[i][k] * b.matrix[k][j];
		}

	return mult;
}

Matrix Matrix::operator*(elem_type x)const {
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

elem_type Matrix::determinant()const {
	if (rows == 1)
		return matrix[0][0];
	if (rows == 2)
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		
	int row = 0;
	elem_type det = 0;
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
	Matrix adjugate(rows, cols);
	int sign = 1;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			adjugate.matrix[i][j] = sign * invert.minor(i, j).determinant();
			sign *= -1;
		}
	std::cout << "Adjugate matrix:\n";
	adjugate.display();
	elem_type det = determinant();
	invert = adjugate / int(det);
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
Matrix Matrix::operator/(elem_type x)const {
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

