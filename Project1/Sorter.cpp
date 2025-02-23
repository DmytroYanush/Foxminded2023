#include "PCH.h"
/*Sorter functions must work with iterators,
do not rely on consistently arranged elements
*/

void QuickSort::matrixSort(Vector<Matrix>& mvec, size_t size) {
	Compare compare;       
	std::sort(mvec.begin(), mvec.end(), compare);
}
void QuickSort::matrixSort(Matrix mptr[], size_t size) {
	qsort(mptr, size, sizeof(Matrix), qsort_compare);
}

void UsualSort::matrixSort(Vector<Matrix>& mvec, size_t size)
{
	Compare compare;
	Matrix pivot = *(mvec.begin() + mvec.size() / 2);    // pivot is matrix object
	int i = 0;
	int j = mvec.size() - 1;
	do {
		while (compare(mvec[i], pivot))
			i++;
		while (desc_compare(mvec[j], pivot))
			j--;
		if (i <= j) {
			Matrix temp = mvec[i];
			mvec[i] = mvec[j];
			mvec[j] = temp;
			i++;
			j--;
		}
	} while (i <= j);
	if (j > 0)
		UsualSort::matrixSort(mvec, j + 1); 
	if (i < mvec.size())
		UsualSort::matrixSort(&mvec[i], mvec.size() - i);
}

void UsualSort::matrixSort(Matrix m_ptr[], size_t size) {
	Compare compare;
	Matrix pivot = m_ptr[size / 2];
	int i = 0;
	int j = size - 1;
	do {
		while (compare(m_ptr[i], pivot))
			i++;
		while (desc_compare(m_ptr[j], pivot))
			j--;
		if (i <= j) {
			Matrix temp = m_ptr[i];
			m_ptr[i] = m_ptr[j];
			m_ptr[j] = temp;
			i++;
			j--;
		}
	} while (i <= j);
	if (j > 0)
		UsualSort::matrixSort(m_ptr, j + 1);
	if (i < size)
		UsualSort::matrixSort(&m_ptr[i], size - i);
}

bool Compare::operator()(const Matrix& m1, const Matrix& m2)
{
	if (m1.count_diags() != m2.count_diags())
		return m1.count_diags() < m2.count_diags();
	else
		return m1.count_trace() < m2.count_trace();
}

bool desc_compare(const Matrix& m1, const Matrix& m2) 
{
	if (m1.count_diags() != m2.count_diags())
		return m1.count_diags() > m2.count_diags();
	else
		return m1.count_trace() > m2.count_trace();
}

int qsort_compare(const void* pm1, const void* pm2)
{  
	Matrix& m1 = *(Matrix*)pm1;
	Matrix& m2 = *(Matrix*)pm2;
	if (m1.count_diags() != m2.count_diags()) {
		if (m1.count_diags() < m2.count_diags())
			return -1;
		else if (m1.count_diags() > m2.count_diags())
			return 1;
	}
	else if (m1.count_trace() < m2.count_trace())
		return -1;
	else if (m1.count_trace() > m2.count_trace())
		return 1;
	else
		return 0;
}
