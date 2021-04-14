#include "pch.h"
Matrix ConsoleLoader::load(std::string path) const {
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

Matrix	FileLoader::load(std::string path) const {
	std::ifstream f_in(path);
	if (f_in) {
		std::string temp = "";
		char ch;
		do {
			f_in >> ch;
			temp += ch;
		} while (ch != ']');
		f_in.close();
		return Matrix(temp);
	}
	else
		throw Bad_file();
}