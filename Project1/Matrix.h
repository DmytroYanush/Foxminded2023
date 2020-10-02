#include <iostream>
#include <sstream>
#include <vector>
#define elem_type double

class Matrix
{
private:
	std::vector<std::vector<elem_type>> matrix;
	int rows;
	int cols;

public:
	//**********************************CONSTRUCTORS AND DESTRUCTOR*********************
	Matrix() : rows(0), cols(0) { }
	Matrix(int  i) : rows(abs(i)), cols(abs(i)) { if (!create()) bad_allocation(*this); }
	Matrix(int i, int j) : rows(i), cols(j) { if(!create()) bad_allocation(*this); }
	template<size_t ROW, size_t COL>
	Matrix(elem_type (&d_arr)[ROW][COL]);
	Matrix(const char * c_str);
	Matrix(const Matrix& m); //copy constructor
	Matrix(Matrix&& m);      //move constructor
	~Matrix() = default;     //
	//*************************************OVERLOADING**********************************
	
	//*************************************addition*************************************
	Matrix operator+(const Matrix& b)const;
	Matrix& operator+=(const Matrix& b) {
		*this = *this + b; 
		return *this;
	}
	Matrix& operator+(int n) {
		return *this;
	}
	//**********************************substraction***********************************
	Matrix operator-(const Matrix& b)const;
	Matrix& operator-=(const Matrix& b) {
		*this = *this - b;
		return *this;
	}
	Matrix& operator-(int n) {
		return *this;
	}
	//*********************************multiplication**********************************
	Matrix operator*(const Matrix& b)const;
	Matrix& operator*=(const Matrix& b) {
		*this = *this * b;
		return *this;
	}
	Matrix operator*(elem_type x)const;
	Matrix& operator*=(elem_type x) {
		*this = *this * x;
		return *this;
	}
	//************************************division*************************************
	Matrix minor(int not_this_r, int not_this_c) const;
	elem_type determinant()const;
	Matrix invert()const;
	Matrix operator/(const Matrix& b)const;
	Matrix operator/(elem_type x)const;

	//***************************relation operations and equality checks***************
	bool operator>(const Matrix& b)const;
	bool operator>=(const Matrix& b) const;
	bool operator<(const Matrix& b) const;
	bool operator<=(const Matrix& b) const;

	bool operator==(const Matrix& b) const;
	bool operator!=(const Matrix& b) const;


	//***********************************OTHER METHODS*********************************
	double elem_sum() const;
	bool create(elem_type initial = 0);
	void swap(Matrix& b);
	Matrix& operator=(const Matrix& orig);
	Matrix& operator=(Matrix&& orig);
	void fill();
	void display();
	std::string to_string();
	void show();

	void bad_allocation(Matrix & orig) {
		std::cout << "The memory wasn't allocated.\n";
		orig.matrix.resize(0);
		orig.rows = orig.cols = 0;
	}

	friend void swap(Matrix& a, Matrix b) {
		a.swap(b);
	}
};

