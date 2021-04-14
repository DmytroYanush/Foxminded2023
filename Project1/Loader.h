#include "pch.h"
class Loader {
public:
	virtual Matrix load(std::string path = "") const = 0;
};
class ConsoleLoader : public Loader {
public:
	Matrix load(std::string path = "") const override;
};
class FileLoader : public Loader {
public:
	Matrix load(std::string path = "") const override;
};