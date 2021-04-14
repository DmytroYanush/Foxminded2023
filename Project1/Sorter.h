#include "PCH.h"

int qsort_compare(const void* pm1, const void* pm2);        // utility function
bool desc_compare(const Matrix& m1, const Matrix& m2);		// utility function	for matrix descending order
class Compare {												// utility object for comparision
public:
	bool operator()(const Matrix& m1, const Matrix& m2);
};

class Sorter {
public:
	virtual void matrixSort(std::vector<Matrix>& mvec, size_t size = 0) = 0;
	virtual void matrixSort(Matrix* mptr, size_t size) = 0;
};

class QuickSort : public Sorter {
public:
	void matrixSort(std::vector<Matrix>& mvec, size_t size = 0) override;
	void matrixSort(Matrix* mptr, size_t size) override;

};
class UsualSort : public Sorter {
public:
	void matrixSort(std::vector<Matrix>& mvec, size_t size = 0) override;
	void matrixSort(Matrix* mptr, size_t size) override;
};