#include "pch.h"

//******************************************** constructors and destructor ********************
// constructor for conversion string char* to object
Matrix::Matrix(std::string c_str) {
	/*here there are checks for non-empty string,
	for valid characters, for the correct sequence of characters,
	for the equality of elements in the rows*/
	std::istringstream istr(c_str);
	// counting rows and cols
	int total = 0, x_cols = 0;
	unsigned begin = 0;   // for controlling beginning of string - '['
	unsigned end = 0;	  // for controlling ending of string - ']'
	elem_type num;
	char ch;
	try {
		while (istr >> ch) {
			//character validation
			if ((!isdigit(ch)) && (ch != '[') && (ch != ']') && (ch != '-') && (ch != ',') && (ch != ';') && (ch != '.')) {
				throw Bad_string("Only such characters are valid: '['\t'-'\t','\t';'\t']'\t'.'.\n");
			}

			//syblol '[' must be the first and only 1 time
			if (ch == '[') {
				begin += 1;
				if (begin > 1) {
					throw Bad_string("Syblol '[' must be the first and only 1 time.\n");
				}
			}

			//symbol ']' must be the last
			else if (ch == ']') {
				if (end == 1 || begin == 0) {   // always checking begin == 0, because '[' only can be the first
					throw Bad_string("Symbol ']' must be the last.\n");
				}
				end += 1;
			}

			else if (isdigit(ch) || ch == '-') {
				if (begin == 0) {
					throw Bad_string("Syblol '[' must be the first.\n");
				}
				istr.unget();
				istr >> num;
				total++;
				x_cols++;
				istr >> ch;
				// after the number can be only the following characters: ','  ';'  ']'
				if (ch != ',' && ch != ';' && ch != ']' && ch != '.') {
					throw Bad_string("After the number can be only the following characters: ','  ';'  ']' '.' .\n");
				}
				istr.unget();
			}
			// after punctuation symbols can be only numbers or symbol '-' 
			else if (ch == ',') {
				istr >> ch;
				if (ch == ',') {
					throw Bad_string("After punctuation symbols can be only numbers or symbol '-'.\n");
				}
				istr.unget();
			}
			else if (ch == ';') {
				istr >> ch;
				//checking if the element position is empty (total == 0)
				if (total == 0 || begin == 0 || ch == ',') {
					throw Bad_string("The element position is empty.\n");
				}
				//check the equality of the number of elements in the rows
				if (cols != 0 && cols != x_cols) {
					throw Bad_string("The length of the rows of the matrix does not match.\n");
				}
				istr.unget();
				cols = x_cols;
				x_cols = 0;
			}
		}
		if (cols == 0)
			throw Bad_string("The string is empty.\n");
		rows = total / cols;
		// create and fill array
		create();
		std::istringstream istr1(c_str);
		unsigned i = 0, j = 0;
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
	catch (Bad_string& bs) {
		std::cout << bs.what();
	}
	catch (Exception& bs) {
		std::cout << bs.what();
	}
	catch (...) {
		std::cout << "Some error occured!\n";
		throw;
	}
}


// copy constructor
Matrix::Matrix(const Matrix& orig) {
	rows = orig.rows;
	cols = orig.cols;
	create();

	//copying all elements
	for (unsigned i = 0; i < rows; i++)
		for (unsigned j = 0; j < cols; j++)
			matrix[i][j] = orig.matrix[i][j];
}

// move constructor
Matrix::Matrix(Matrix&& orig) : rows(orig.rows), cols(orig.cols), matrix(orig.matrix) {
	orig.rows = 0;
	orig.cols = 0;
	orig.matrix = nullptr;
}

Matrix::~Matrix() 
{
	if (matrix != nullptr) {
		for (unsigned i = 0; i < rows; i++) {
			if (matrix[i] != nullptr)			
				delete[] matrix[i];
		}
			delete[] matrix;
	}
}


//*************************************************** methods *****************************
void Matrix::create() {                                 
	try {
		/*allocating memory for pointers
		and assigning them nullptr
		*/
		matrix = new elem_type* [rows];      
		for (unsigned i = 0; i < rows; i++)
		{
			matrix[i] = nullptr;
		}

		/*allocating memory for all arrays
		and assigning zero to all element
		of the matrix 
		*/
		for (unsigned i = 0; i < rows; i++)
		{
			matrix[i] = new elem_type[cols];
		}
		for (unsigned i = 0; i < rows; i++)
			for (unsigned j = 0; j < cols; j++)
				matrix[i][j] = 0;
		}

	catch(std::bad_alloc & ba) {
		bad_alloc_fix(*this);
		std::cout << ba.what() << std::endl;
		throw;
	}
}

void Matrix::bad_alloc_fix(Matrix & orig)noexcept {
	std::cout << "The memory wasn't allocated.\n";
	if (matrix != nullptr) {
		unsigned i = 0;
		while (matrix[i] != nullptr)
			delete[] matrix[i++];
		delete[] matrix;
	}
	rows = cols = 0;
}

Matrix& Matrix::operator=(const Matrix& orig) {
	if (matrix != orig.matrix) {
		this->~Matrix();
		rows = orig.rows;
		cols = orig.cols;
		create();     
		/*if the function create() does not allocate memory
		the loop below 	will not be executed (except condition checks)
		*/
		for (unsigned i = 0; i < rows; i++) {
			for (unsigned j = 0; j < cols; j++) {
				matrix[i][j] = orig.matrix[i][j];
			}
		}
	}
	return *this;
}

Matrix& Matrix::operator=(Matrix&& orig) {
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

void Matrix::fill_matrix() {
	std::random_device rd;
	std::mt19937::result_type seed = rd() ^ (
		(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()
			).count() +
			(std::mt19937::result_type)
		std::chrono::duration_cast<std::chrono::microseconds>(
			std::chrono::high_resolution_clock::now().time_since_epoch()
			).count());

	std::mt19937 gen(seed);
	std::uniform_int_distribution<unsigned> distrib(1, 99);
		
	for (unsigned i = 0; i < rows; i++)
		for (unsigned j = 0; j < cols; j++)
			matrix[i][j] = distrib(gen);
	
}

void Matrix::display()noexcept {
	for (unsigned i = 0; i < rows; i++)
	{
		for (unsigned j = 0; j < cols; j++)
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
	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
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

std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
	for (unsigned i = 0; i < m.rows; i++)
	{
		for (unsigned j = 0; j < m.cols; j++)
		{
			os.width(12);
			os << m.matrix[i][j];
		}
		os << '\n';
	}
	return os;
}
void Matrix::show() noexcept 
{
	std::cout << '[';
	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
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
	for (unsigned i = 0; i < rows; i++)
		for (unsigned j = 0; j < cols; j++)
			sum += matrix[i][j];
	return sum;
}

//*************************************************OVERLOADING********************************
//**************************************************addition**********************************
Matrix Matrix::operator+(const Matrix& b)const {
	try {
		if (cols != b.cols || rows != b.rows) {
			throw Bad_arith_arg(std::string("It's impossible to perform the addition operation, ") +
				"The number of rows (" + std::to_string(rows) + " and" + std::to_string(b.rows) +
				") or/and columns (" + std::to_string(cols) + " and " + std::to_string(b.cols) +
				") in two matrices is not equal.\n");
		}
		Matrix matx(rows, cols);
		for (unsigned i = 0; i < rows; i++)
			for (unsigned j = 0; j < cols; j++) {
				//overflow diagnostic
				if (b.matrix[i][j] >= 0) {
					if (matrix[i][j] > std::numeric_limits<elem_type>::max() - b.matrix[i][j]) {
						throw Elem_overflow("It is impossible to perform the addition operation!\n");
					}
				}
				else {
					if (matrix[i][j] < std::numeric_limits<elem_type>::lowest() - b.matrix[i][j])
					{
						throw Elem_overflow("It is impossible to perform the addition operation!\n");
					}
				}
				matx.matrix[i][j] = matrix[i][j] + b.matrix[i][j];
			}
		return matx;
	}
	/*we are going from more specialized
      exceptions to the generalized
	*/
	catch (Elem_overflow& eo) {
		std::cout << eo.what();
	}
	catch (Bad_arith_arg& baa) {
		std::cout << baa.what();
	}
	catch (Exception& bs) {
		std::cout << bs.what();
	}
	catch (...) {
		std::cout << "Some error occured!\n";
		throw;
	}
	return Matrix();
}


//**************************************************substraction*******************************
Matrix Matrix::operator-(const Matrix& b)const {
	try {
		if (cols != b.cols || rows != b.rows) {
			throw Bad_arith_arg(std::string("It's impossible to perform the substraction operation, ") +
				"The number of rows (" + std::to_string(rows) + " and " + std::to_string(b.rows) +
				") or/and columns (" + std::to_string(cols) + " and " + std::to_string(b.cols) +
				") in two matrices is not equal.\n");
		}
		else {
			Matrix matx(rows, cols);
			for (unsigned i = 0; i < rows; i++)
				for (unsigned j = 0; j < cols; j++) {
					//overflow diagnostic
					if (b.matrix[i][j] <= 0) {
						if (matrix[i][j] > std::numeric_limits<elem_type>::max() + b.matrix[i][j]) {
							throw Elem_overflow("It is impossible to perform the substraction operation!\n");
						}
					}
					else {
						if (matrix[i][j] < std::numeric_limits<elem_type>::lowest() + b.matrix[i][j])
						{
							throw Elem_overflow("It is impossible to perform the substraction operation!\n");
						}
					}
					matx.matrix[i][j] = matrix[i][j] - b.matrix[i][j];
				}
			return matx;
		}
	}
	/*class Exception catches
	 Bad_arith_arg and Elem_overflow 
	 then uses the correct function what()
	 for classes Bad_arith_arg and elem_overflow
	*/
	catch (Exception& ex) {   
		std::cout << ex.what();
	}
	catch (...) {
		std::cout << "Some error occured!\n";
		throw;
	}
	return Matrix();
}

//********************************************************multiplication*********************************
Matrix Matrix::operator*(const Matrix& b) const {
	try {
		if (cols != b.rows) {
			throw Bad_arith_arg(std::string("It's impossible to perform the multiplication operation, ") +
				"The number of cols (" + std::to_string(cols) + ") in first matrix is'n equal to number of rows ("
				+ std::to_string(b.rows) +
				") in second matrix.\n");
		}
		Matrix mult(rows, b.cols);
		for (unsigned i = 0; i < rows; i++)
			for (unsigned j = 0; j < b.cols; j++) {
				mult.matrix[i][j] = 0;
				for (unsigned k = 0; k < cols; k++)
					mult.matrix[i][j] += matrix[i][k] * b.matrix[k][j];
			}
		return mult;
	}
	catch (Exception& baa) {
		std::cout << baa.what();
		throw;
	}
	return Matrix();
}

Matrix Matrix::operator*(elem_type x)const {
	Matrix mult(rows, cols);
	for (unsigned i = 0; i < rows; i++)
		for (unsigned j = 0; j < cols; j++)
			mult.matrix[i][j] = matrix[i][j] * x;
	return mult;
}

//**************************************************dividing**********************************
Matrix Matrix::minor(unsigned not_this_r, unsigned not_this_c)const {
	Matrix minor(rows - 1, cols - 1);
	int i = 0;
	int j = 0;
	// Looping for each element of the matrix 
	for (unsigned row = 0; row < rows; row++)
	{
		for (unsigned col = 0; col < cols; col++)
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
		
	unsigned row = 0;
	elem_type det = 0;
	int sign = 1;
	for (unsigned j = 0; j < cols; j++) {
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
	for (unsigned i = 0; i < rows; i++)
		for (unsigned j = 0; j < cols; j++)
			invert.matrix[i][j] = matrix[j][i];
	// Making A* (Adjugate matrix)
	Matrix adjugate(rows, cols);
	int sign = 1;
	for (unsigned i = 0; i < rows; i++)
		for (unsigned j = 0; j < cols; j++) {
			adjugate.matrix[i][j] = sign * invert.minor(i, j).determinant();
			sign *= -1;
		}
	elem_type det = determinant();
	invert = adjugate / int(det);
	return invert;
}

Matrix Matrix::operator/(const Matrix& b)const {
	try {
		if (b.rows != b.cols) {
			throw Bad_arith_arg(std::string("Determinant in a non-square matrix cannot be determined\n"));
		}
		if ((*this).cols != b.rows) {
			throw Bad_arith_arg(std::string("It's impossible to perform the dividing operation, ") +
				"The number of cols (" + std::to_string(cols) + ") in first matrix is'n equal to number of rows ("
				+ std::to_string(b.rows) +
				") in second matrix.\n");
		}
		if (b.determinant() == 0) {
			throw Bad_arith_arg("Determinant = 0, it is impossible to count the inverse matrix\n");
		}
		//main logic of method operator/()
		return *this * b.invert();
	}
	catch (Exception& baa) {
		std::cout << baa.what();
		throw;
	}
	return Matrix();
}
Matrix Matrix::operator/(elem_type x)const {
	try {
		if (x == 0) {
			throw Div_by_zero("Dividing by zero.\n");
		}
		Matrix div(rows, cols);
		for (unsigned i = 0; i < rows; i++)
			for (unsigned j = 0; j < cols; j++)
				div.matrix[i][j] = matrix[i][j] / x;
		return div;
	}
	catch (Exception& e) {
		std::cout << e.what();
		throw;
	}
	return Matrix();
}

//*********************************************sorting********************************************
elem_type Matrix::count_diags() const
{
	if (rows != cols)
		throw Exception("It is impossible to calculate the sum of the elements of the diagonal in a non-square matrix.\n");
	elem_type sum = count_trace();
	for (int i = rows - 1; i >= 0; i--)
		for (int j = 0; j < cols; j++)
			sum += matrix[i][j];
	return sum;
}

elem_type Matrix::count_trace() const
{
	if (rows != cols)
		throw Exception("It is impossible to calculate the trace in a non - square matrix.\n");
	elem_type sum = 0;
	for (int i = 0; i < rows; i++)
		sum += matrix[i][i];
	return sum;
}

