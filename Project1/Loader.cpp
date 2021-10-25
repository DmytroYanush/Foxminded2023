#include "PCH.h"
/***************************************template spacialization for matrix**********************************/
template<> 
Matrix ConsoleLoader<Matrix>::load() {
	std::cout << "Enter the matrix in the following format:"
		<< "[a,b,c; d, f, e], where a, b, c, d, … – numbers.\n";
	std::string temp = "";
	char ch;
	do {
		std::cin >> ch;
		temp += ch;
	} while (ch != ']');
	return Matrix(temp);
}

template<> 
Matrix	FileLoader<Matrix>::load() {
	if (fin)
	{
		std::string temp = "";
		getline(fin, temp);
		return Matrix(temp);
	}
	else
		throw Bad_file();
}