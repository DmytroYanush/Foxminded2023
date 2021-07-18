#include "pch.h"
class Iterator_I {
public:
	virtual Matrix& first() = 0;
	virtual Matrix& next() = 0;
};

class Vector {
public:
	class Iterator : Iterator_I
	{
	public:
		friend class Vector;
	public:
		using value_type = Matrix;
		using reference = Matrix & ;
		using pointer = Matrix * ;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = ptrdiff_t;
	public:
		Iterator() = default;
		Iterator(const pointer ptr, Vector* v)
			: _ptr(ptr), vec(v)
		{	}
		Iterator(const Iterator& other);
		Iterator& operator=(const Iterator& other);
		value_type& first() override;
		value_type& next() override;

		/* return Matrix*,
		required in operations such as: ++_ptr < pointer(p_to_vec->end())
		*/
		explicit operator pointer() const;

		Iterator& operator++();
		Iterator operator++(int);
		Iterator& operator--();
		Iterator operator--(int);
		reference operator[](const size_t index);
		pointer operator->();
		reference operator*();
		Iterator& operator+(difference_type num);
		Iterator& operator+=(difference_type num);
		Iterator& operator-(difference_type num);
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
		pointer _ptr = nullptr;    // FAIL!
		Vector* vec;
	};
private:
	Matrix* base;
	Matrix* first_free;
	Matrix* cap;
	std::allocator<Matrix> alloc;
public:
	friend class Iterator;
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
	void push_back(const Matrix& mx);
	void push_back(Matrix&& mx);           // for temporary objects
	void pop_back();
	void erase(const size_t n);
	void erase(Iterator& it);
	Matrix& operator[](size_t n);
	
	void sort(Sorter& srt);
	void load(Loader& ld, size_t quantity = 1);

	void print() const;
	~Vector() { free();}
private:
	//utility methods
	Vector* _this()
	{
		return this;
	}
	void chk_n_alloc()
	{
		if (size() == capacity())
			reallocate();
	}
	std::pair<Matrix*, Matrix*> alloc_n_copy(const Matrix* b,const Matrix* e);
	void free();
	void reallocate();
	bool is_in_range(size_t n) const
	{
		return (n > 0 && n <= size());
	}
};