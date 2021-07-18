#include "pch.h"
#define elem_type double

class Matrix {
private:
	elem_type** matrix = nullptr;
	unsigned rows  = 0;
	unsigned cols = 0;
	Matrix minor(unsigned not_this_r, unsigned not_this_c) const;
	elem_type determinant()const;
	Matrix invert()const;
public:
	//**********************************CONSTRUCTORS AND DESTRUCTOR*********************
	Matrix() = default;
	Matrix(unsigned  i) : rows(1), cols(1) {
		create();
		matrix[0][0] = i;
	}
	Matrix(unsigned i, unsigned j) : rows(i), cols(j) 
	{ 
		create(); 
	}
	template<size_t ROW, size_t COL>
	Matrix(elem_type(&d_arr)[ROW][COL]);
	Matrix(std::string c_str);
	Matrix(const Matrix& m);                        
	Matrix(Matrix&& m);   
	Matrix& operator=(const Matrix& orig);
	Matrix& operator=(Matrix&& orig);
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
	Matrix operator*(elem_type x)const;
	Matrix& operator*=(elem_type x) {
		*this = *this * x;
		return *this;
	}
	//************************************division*************************************
	
	Matrix operator/(const Matrix& b)const;
	Matrix operator/(elem_type x)const;

	//***************************relation operations and equality checks***************
	bool operator>(const Matrix& b)const {
		return elem_sum() > b.elem_sum();
	}
	bool operator>=(const Matrix& b) const {
		return elem_sum() >= b.elem_sum();
	}
	bool operator<(const Matrix& b) const {
		return elem_sum() < b.elem_sum();
	}
	bool operator<=(const Matrix& b) const {
		return elem_sum() <= b.elem_sum();
	}

	bool operator==(const Matrix& b) const {
		return elem_sum() == b.elem_sum();
	}
	bool operator!=(const Matrix& b) const {
		return elem_sum() != b.elem_sum();
	}


	//***********************************OTHER METHODS*********************************
	double elem_sum() const;
	void create();
	
	void fill_matrix();
	void display()noexcept;
	std::string to_string();
	void show()noexcept;
	void bad_alloc_fix(Matrix & orig)noexcept;

	//********************************* methods for sorting ***************************
	elem_type count_diags() const;
	elem_type count_trace() const;
};

