#include "PCH.h"
//////////////////////////////////// Vector -> private methods ////////////////////////////////
std::pair<Matrix*, Matrix*> Vector::alloc_n_copy(const Matrix* b, const Matrix* e)
{
	auto begin = alloc.allocate(e - b);           
	auto end = std::uninitialized_copy(b, e, begin);
	return std::make_pair(begin, end);
}

void Vector::free()
{
	if (base) {
		for (auto p = first_free; p != base; /*empty*/)
			alloc.destroy(--p);                 
		alloc.deallocate(base, cap - base);
	} 
}

void Vector::reallocate()
{
	auto new_capacity = size() ? 2 * size() : 1;
	auto new_base = alloc.allocate(new_capacity);

	auto dest = new_base;
	auto elem = base;    
	for (int i = 0; i < size(); ++i)
		alloc.construct(dest++, std::move(*elem++));

	//free();
	base = new_base;
	first_free = dest;
	cap = base + new_capacity;
}

/////////////////////////////////////// Vector -> public methods ////////////////////////////////
void Vector::allocate(size_t capacity)
{
	first_free = base = alloc.allocate(capacity);
	cap = base + capacity;
}

Vector::Iterator Vector::begin()
{
	return Iterator(base, this);
}

Vector::Iterator Vector::end() 
{ 
	return Iterator(first_free, this);
}

Vector::Vector(const Vector& other) 
{
	auto data = alloc_n_copy(other.base, other.first_free);
	base = data.first;
	first_free = cap = data.second;
}

Vector& Vector::operator=(const Vector& other)
{
	auto data = alloc_n_copy(other.base, other.first_free);
	free();
	base = data.first;
	first_free = cap = data.second;
	return *this;
}

void Vector::push_back(const Matrix& mx)
{
	chk_n_alloc();
	alloc.construct(first_free++, mx);
}

void Vector::push_back(Matrix&& mx) 
{
	chk_n_alloc();
	alloc.construct(first_free++, std::move(mx));
}

void Vector::pop_back()
{
	if (size() > 1)
		alloc.destroy(--first_free);
	else 
		free();
}

Matrix& Vector::operator[](size_t n) 
{
	if (is_in_range(n))
		return *(base + n);
	else
		throw Exception("Out of range of Vector!\n");
}

void Vector::erase(const size_t position)
{
	if (is_in_range(position))
	{   
		std::copy(base + position, first_free, base + position - 1);  
		pop_back();
	} 
	else
		throw Exception("Out of range of Vector!\n");
}

void Vector::erase(Iterator& it)
{
	if (it >= begin() && it < end())
	{
		std::copy(it + 1, end(), it);
		pop_back();
	}
	else
		throw Exception("Out of range of Vector!\n");
}

void Vector::sort(Sorter& srt)
{
	srt.matrixSort(base, size());
}

void Vector::load(Loader& ld, size_t quantity)
{
	for (size_t i = 0; i < quantity; i++)
	   push_back(ld.load());          //only one Matrix object
}

void Vector::print()const
{
	for (auto p = base; p < first_free; p++) {
//		std::cout << "The sum of the elements of the two diagonals of the matrix#" << p - base + 1 << ": " << p->count_diags()
//			<< "\nTrace of the matrix: " << p->count_trace() << std::endl;
		p->display();
		std::cout << std::endl;
	}
}

/*****************************************Methods of Vector::Iterator class***************************/
using Iterator = Vector::Iterator;
Iterator::Iterator(const Iterator& other)
{
	_ptr = other._ptr;
	vec = other.vec;
}
Iterator& Iterator::operator=(const Iterator& other)
{
	if (*this != other)
	{
		_ptr = other._ptr;
		vec = other.vec;
	}
	return *this;
}

Iterator::value_type& Iterator::first()
{
	return *(vec->base);
}

Iterator::value_type& Iterator::next() 
{
	return *(++(*this));              // range checking in the operator++
}

Iterator::operator pointer() const
{
	return _ptr;
}

Iterator& Iterator::operator++()
{
	if (++_ptr <= pointer(vec->end()))
		return *this;
	else
		throw Exception("Out of range of Vector!\n");
}

Iterator Iterator::operator++(int)
{
	Iterator iterator = *this;
	++(*this);
	return iterator;
}

Iterator& Iterator::operator--()
{
	if (--_ptr >= pointer(vec->begin()))
		return *this;
	else
		throw Exception("Out of range of Vector!\n");
}

Iterator Iterator::operator--(int)
{
	Iterator iterator = *this;
	--(*this);
	return iterator;
}

Iterator::reference Iterator::operator[](const size_t index)
{
	if (index < 0 || index >= vec->size())
		throw Exception("Out of range of Vector!\n");
	return *(_ptr + index);
}

Iterator::pointer Iterator::operator->()
{
	return _ptr;
}

Iterator::reference Iterator::operator*()
{
	return *_ptr;
}

Iterator& Iterator::operator+(difference_type num)
{
	if (num <= pointer(vec->end()) - _ptr) {
		_ptr = _ptr + num;
		return *this;
	}
	else
		throw Exception("Out of range of Vector!\n");
}
Iterator& Iterator::operator+=(difference_type num)
{
	return *this + num;
}

Iterator& Iterator::operator-(difference_type num)
{
	if (num <= _ptr - pointer(vec->begin()))
		_ptr = _ptr - num;
	else
		throw Exception("Out of range of Vector!\n");
}

Iterator& Iterator::operator-=(difference_type num)
{
	return *this - num;
}

Iterator::difference_type Iterator::operator-(const Iterator& other) const
{
	return _ptr - other._ptr;
}

// comparison and equality operators
bool Iterator::operator<(const Iterator& other) const
{
	return _ptr < other._ptr;
}

bool Iterator::operator>(const Iterator& other) const
{
	return _ptr > other._ptr;
}

bool Iterator::operator<=(const Iterator& other) const
{
	return _ptr <= other._ptr;
}

bool Iterator::operator>=(const Iterator& other) const
{
	return _ptr >= other._ptr;
}

bool Iterator::operator==(const Iterator& other) const
{
	return _ptr == other._ptr;
}

bool Iterator::operator!=(const Iterator& other) const
{
	return !(*this == other);
}