#include <iostream>
#include <sstream>

class Matrix
{
private:
	double **matrix;
	int rows;
	int cols;

public:
	//**********************************CONSTRUCTORS AND DESTRUCTOR*********************
	Matrix() : rows(0), cols(0), matrix(nullptr) { }
	explicit Matrix(int  i) : rows(abs(i)), cols(abs(i)) { create(); }
	Matrix(int i, int j) : rows(i), cols(j) { create(); }
	template<size_t ROW, size_t COL>
	Matrix(double(&d_arr)[ROW][COL]);
	Matrix(const char * c_str);
	Matrix(const Matrix& m); //copy constructor
	Matrix(Matrix&& m);      //move constructor
	~Matrix();
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
	Matrix operator*(double x)const;
	Matrix& operator*=(double x) {
		*this = *this * x;
		return *this;
	}
	//************************************division*************************************
	Matrix minor(int not_this_r, int not_this_c) const;
	double determinant()const;
	Matrix invert()const;
	Matrix operator/(const Matrix& b)const;
	Matrix operator/(double x)const;

	//***************************relation operations and equality checks***************
	bool operator>(const Matrix& b)const;
	bool operator>=(const Matrix& b) const;
	bool operator<(const Matrix& b) const;
	bool operator<=(const Matrix& b) const;

	bool operator==(const Matrix& b) const;
	bool operator!=(const Matrix& b) const;


	//***********************************OTHER METHODS*********************************
	double elem_sum() const;
	void create();
	void operator=(const Matrix& orig);
	void operator=(Matrix&& orig);
	void fill();
	void display();
	std::string to_string();
	void show();
};

