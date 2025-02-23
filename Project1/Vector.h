#pragma once
#include "PCH.h"

template<typename T>
class Iterator_I {
public:
	virtual T& first() = 0;
	virtual T& next() = 0;
}; 

template <typename T>
class Vector {
public:
	using smart_p = std::shared_ptr<T>;
	class Iterator : Iterator_I<T>
	{
	public:
		friend class Vector;
		using value_type = T;
		using reference = T& ;
		using pointer = std::shared_ptr<T>* ;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = ptrdiff_t;   // int
	public:
		Iterator() = default;
		Iterator(const pointer ptr, Vector* v)
			: _ptr(ptr), vec(v)
		{	}
		Iterator(const Iterator& other);
		Iterator& operator=(const Iterator& other);
		value_type& first() override;
		value_type& next() override;

		/* return T*,
		required in operations such as: 
		++_ptr < pointer(vec->end())
		*/
		explicit operator pointer() const;
		Iterator& operator++();
		Iterator operator++(int);
		Iterator& operator--();
		Iterator operator--(int); 
		reference operator[](const size_t index);
		pointer operator->();
		reference operator*();
		Iterator operator+(difference_type num);
		Iterator& operator+=(difference_type num);
		Iterator operator-(difference_type num);
		Iterator& operator-=(difference_type num);
		difference_type operator-(const Iterator& other) const;

		// comparison and equality operators
		bool operator<(const Iterator& other) const;
		bool operator>(const Iterator& other) const;
		bool operator<=(const Iterator& other) const;
		bool operator>=(const Iterator& other) const;

		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;
	private:
		pointer _ptr = nullptr;    
		Vector* vec;
	};
	class ActionMap {
	public:
		ActionMap()
		{
			_map["+"] = std::plus<T>();
			_map["-"] = std::minus<T>();
			_map["*"] = std::multiplies<T>();
			_map["/"] = std::divides<T>();
		}
		auto operator[](const std::string oper)
		{
			return _map[oper];
		}
	private:
		std::map<std::string, std::function<T(const T&, const T&)>> _map;
	};


public:
	Vector() : base(nullptr), first_free(nullptr), cap(nullptr)
	{};
	Vector(const Vector& other);
	Vector& operator=(const Vector& other);
	size_t size() const { return first_free - base; }
	size_t capacity() const { return cap - base; }
	Iterator begin();
	Iterator end();
	
	void allocate(size_t cap);
	void push_back(const T& mx);
	void push_back(T&& mx);           // for temporary objects
	void pop_back();
	void erase(const size_t n);
	void erase(Iterator& it);
	template<typename... Args>
	void emplace_back(Args&&...args) {
		chk_n_alloc();
		alloc.construct(first_free++, std::make_shared<T>(std::forward<Args>(args)... )); 
	}
	T& operator[](size_t n);
	void load(Loader<T>& ld);
	void print() const;
	~Vector() { free();}

	/************************************** new for task #9 *************************/
	void load_actions(const std::string& file_name = "MyActions.txt");
	T calculate();
private:
	using p_sort = void(*)(Iterator, Iterator);
	Vector* _this()
	{	
		return this;
	}
	void chk_n_alloc()
	{
		if (size() == capacity())
			reallocate();
	}
	std::pair<smart_p*, smart_p*> alloc_n_copy(const smart_p* b,const smart_p* e);
	void free();
	void reallocate();
	bool is_in_range(size_t n) const
	{
		return (n >= 0 && n <= size());
	}
	/**************************************************************************************/
	std::vector<std::string> actions;
	smart_p* base;
	smart_p* first_free;
	smart_p* cap;
	std::allocator<smart_p> alloc;
};

/********************************************** Methods of Vector & Iterator ***************************************/

//////////////////////////////////// Vector -> private methods ////////////////////////////////
template <typename T>
std::pair<typename Vector<T>::smart_p*, typename Vector<T>::smart_p*>
Vector<T>::alloc_n_copy(const smart_p* b, const smart_p* e)
{
	auto begin = alloc.allocate(e - b);
	auto end = std::uninitialized_copy(b, e, begin);
	return std::make_pair(begin, end);
}

template <typename T>
void Vector<T>::free()
{
	if (base) {
		for (auto p = first_free; p != base; /*empty*/)
			alloc.destroy(--p);
		alloc.deallocate(base, cap - base);
	}
}

template <typename T>
void Vector<T>::reallocate()
{
	auto new_capacity = size() ? 2 * size() : 1;
	auto new_base = alloc.allocate(new_capacity);

	auto dest = new_base;
	auto elem = base;
	for (int i = 0; i < size(); ++i)
		alloc.construct(dest++, *elem++);

	free();
	base = new_base;
	first_free = dest;
	cap = base + new_capacity;
}

/////////////////////////////////////// Vector -> public methods ////////////////////////////////
template <typename T>
void Vector<T>::allocate(size_t capacity)
{
	first_free = base = alloc.allocate(capacity);
	cap = base + capacity;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::begin()
{
	return Iterator(base, this);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::end()
{
	return Iterator(first_free, this);
}

template <typename T>
Vector<T>::Vector(const Vector& other)
{
	auto data = alloc_n_copy(other.base, other.first_free);
	base = data.first;
	first_free = cap = data.second;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
	//without comparison
	auto data = alloc_n_copy(other.base, other.first_free);
	free();
	base = data.first;
	first_free = cap = data.second;
	return *this;
}

template <typename T>
void Vector<T>::push_back(const T& mx)
{
	chk_n_alloc();
	alloc.construct(first_free++, std::make_shared<T>(mx));
}

template <typename T>
void Vector<T>::push_back(T&& mx)
{
	chk_n_alloc();
	alloc.construct(first_free++, std::make_shared<T>(mx));
}

template <typename T>
void Vector<T>::pop_back()
{
	if (size() > 1) {
		alloc.destroy(--first_free);
	}
	else
		free();
}

template <typename T>
T& Vector<T>::operator[](size_t n)
{
	if (is_in_range(n))
		return **(base + n);
	else
		throw Exception("Out of range of Vector in operator[]!\n");
}

template <typename T>
void Vector<T>::erase(const size_t position)
{
	if (is_in_range(position))
	{
		std::copy(base + position, first_free, base + position - 1);
		pop_back();
	}
	else
		throw Exception("Out of range of Vector in method erase()!\n");
}

template <typename T>
void Vector<T>::erase(Iterator& it)
{
	if (it >= begin() && it < end())
	{
		std::copy(it + 1, end(), it);
		pop_back();        
	}
	else
		throw Exception("Out of range of Vector!\n");
}
template <typename T>
void Vector<T>::load_actions(const std::string& path)
{
	std::ifstream fin(path);
	std::string temp;
	while (fin)
	{
		getline(fin, temp);
		if (temp == "+" || temp == "-" ||
			temp == "/" || temp == "*")
			actions.push_back(temp);
	}
}

template <typename T>
T Vector<T>::calculate()
{
	try {
		if (size() == 0)
			throw Logic_exc("Vector is empty");
		if (actions.empty())
			throw Logic_exc("No operations on vector elements loaded");
		T result = (*this)[0];
		ActionMap map;
		size_t i = 0;  
		for (size_t j = 1; j < this->size(); ++j) {
			result = map[actions[i]](result, (*this)[j]);
			if (++i == actions.size())   
				i = 0;
			return result;
		}
	}
	catch (Exception& exc)
	{
		std::cout << "Calculate operation was terminated:"
			<< exc.what();
		system("pause");
	}
	return T();
}


template <typename T>
void Vector<T>::load(Loader<T>& ld)
{
	try {
		while (!ld.bad())
			push_back(ld.load());                    //only one Matrix object
	}
	catch (Exception& exc) {
		std::cout << exc.what();
	}
}


template <typename T>
void Vector<T>::print()const
{
	for (auto p = base; p < first_free; p++) {
		std::cout << **p;
		std::cout << std::endl;
	}
}

/*****************************************Methods of Vector<T>::Iterator class***************************/

template <typename T>
Vector<T>::Iterator::Iterator(const Iterator& other)
{
	_ptr = other._ptr;
	vec = other.vec;
}
template <typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(const Iterator& other)
{
	if (*this != other)
	{
		_ptr = other._ptr;
		vec = other.vec;
	}
	return *this;
}

template <typename T>
typename Vector<T>::Iterator::value_type& 
Vector<T>::Iterator::first()
{
	return **(vec->base);
}

template <typename T>      
typename Vector<T>::Iterator::value_type& 
Vector<T>::Iterator::next()
{
	if (*this < vec->end()) {
		auto iter = ++*this;
		return *iter;
	}
	else
		return **this;
}

template <typename T>
Vector<T>::Iterator::operator pointer() const
{
	return _ptr;
}

template <typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
{
	if (++_ptr <= pointer(vec->end()))
		return *this;
	else
		throw Exception("Out of range of Vector!\n");
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
{
	Iterator iterator = *this;
	++(*this);
	return iterator;
}

template <typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
{
	if (--_ptr >= pointer(vec->begin()))
		return *this;
	else
		throw Exception("Out of range of Vector!\n");
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
{
	Iterator iterator = *this;
	--(*this);
	return iterator;
}

template <typename T>
typename Vector<T>::Iterator::reference Vector<T>::Iterator::operator[](const size_t index)
{
	if (index < 0 || index >= vec->size())
		throw Exception("Out of range of Vector!\n");
	return **(_ptr + index);
}

//returns a smart pointer to an object, not a pointer to a smart pointer
template <typename T>
typename Vector<T>::Iterator::pointer Vector<T>::Iterator::operator->()
{
	return *_ptr;
}

template <typename T>
typename Vector<T>::Iterator::reference Vector<T>::Iterator::operator*()
{
	return **_ptr;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator+(difference_type num)
{
	if (num <= pointer(vec->end()) - _ptr) {
		_ptr = _ptr + num;
		return *this;
	}
	else
		throw Exception("Out of range of Vector!\n");
}

template <typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator+=(difference_type num)
{
	return *this + num;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator-(difference_type num)
{
	if (num <= _ptr - pointer(vec->begin()))
		_ptr = _ptr - num;
	else
		throw Exception("Out of range of Vector!\n");
}

template <typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator-=(difference_type num)
{
	return *this - num;
}

template <typename T>
typename Vector<T>::Iterator::difference_type
Vector<T>::Iterator::operator-(const Iterator& other) const
{
	return _ptr - other._ptr;
}

//************************************ comparison and equality operators ******************************
template <typename T>
bool Vector<T>::Iterator::operator<(const Iterator& other) const
{
	return _ptr < other._ptr;
}

template <typename T>
bool Vector<T>::Iterator::operator>(const Iterator& other) const
{
	return _ptr > other._ptr;
}

template <typename T>
bool Vector<T>::Iterator::operator<=(const Iterator& other) const
{
	return _ptr <= other._ptr;
}

template <typename T>
bool Vector<T>::Iterator::operator>=(const Iterator& other) const
{
	return _ptr >= other._ptr;
}

template <typename T>
bool Vector<T>::Iterator::operator==(const Iterator& other) const
{
	return _ptr == other._ptr;
}

template <typename T>
bool Vector<T>::Iterator::operator!=(const Iterator& other) const
{
	return !(*this == other);
}
