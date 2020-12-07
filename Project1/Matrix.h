#include <iostream>
#include <sstream>
#include <vector>
#define elem_type double

class Matrix
{
private:
	elem_type** matrix = nullptr;
	unsigned rows  = 0;
	unsigned cols = 0;
	/*Ці функції використовуюються для 
	оператора ділення матриці на матрицю
	і викликаються тільки з operator/ в іншому
	випадку слід продубльовувати провірки з operator/
	*/
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
	Matrix(const char * c_str);
	Matrix(const Matrix& m);                        //copy constructor
	Matrix(Matrix&& m);                             //move constructor
	~Matrix();                      //destructor
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
	
	Matrix& operator=(const Matrix& orig);
	Matrix& operator=(Matrix&& orig);
	void fill_matrix();
	void display()noexcept;
	std::string to_string();
	void show()noexcept;
	void bad_alloc_fix(Matrix & orig)noexcept;
};

