#include "pch.h"
class Loader {
public:
	virtual Matrix load() = 0;
};

class ConsoleLoader : public Loader {
public:
	Matrix load() override;
};

class FileLoader : public Loader {
public:
	FileLoader() = default;
	FileLoader (const std::string& pth) : path(pth) 
	{
		fin.open(path);
	}
	Matrix load() override;
	void open(const std::string& pth)
	{
		fin.close();
		fin.open(pth);
	}
	void close()
	{
		fin.close();
	}
	~FileLoader()
	{
		fin.close();
	}
private:
	std::string path = "";
	std::ifstream fin;
};