#include "pch.h"
Matrix ConsoleLoader::load() {
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

Matrix	FileLoader::load() {
	if (fin) 
	{
		std::string temp = "";
		char ch;
		do {
			fin >> ch;
			temp += ch;
		} while (ch != ']');
		return Matrix(temp);
	}
	else
		throw Bad_file();
}