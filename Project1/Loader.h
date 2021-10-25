#pragma once
#include "PCH.h"
template <typename T>
class Loader {
public:
	virtual T load() = 0;
	virtual bool bad() const = 0;
};

template <typename T>
class ConsoleLoader : public Loader<T> {
public:
	bool bad() const override
	{
		return std::cin.bad();
	}
	T load() override;
};

template <typename T>
class FileLoader : public Loader<T> {
public:
	FileLoader() = default;
	FileLoader (const std::string& pth) : path(pth) 
	{
		open(path);
	}
	T load() override;
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
		close();
	}
	bool bad() const override {
		return (fin.bad());
	}

private:
	std::string path = "";
	std::ifstream fin;
};



