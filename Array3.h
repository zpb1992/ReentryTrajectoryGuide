#pragma once

// 没有bool的都是浅拷贝 我感觉数据量有些大 25000*300个double

template<typename T>
class Array3
{
public:
	Array3(T **array, unsigned *column, unsigned row, bool deepCopy = false);
	Array3(T **array, unsigned column, unsigned row, bool deepCopy = false);
	Array3(const Array3 &a);
	Array3(Array3 &&a) noexcept;
	Array3 &operator =(const Array3 &a);
	Array3 &operator =(Array3 &&a);
	~Array3();
public:
	T **_array;
	unsigned *_column;
	unsigned _row;

private:
	bool _deepCopy;
private:
	bool create(T **array, unsigned *column, unsigned row, bool deepCopy = false);

};

template<typename T>
inline Array3<T>::Array3(T ** array, unsigned * column, unsigned row, bool deepCopy)
{
	create(array, column, row, deepCopy);
}

template<typename T>
inline Array3<T>::Array3(T ** array, unsigned column, unsigned row, bool deepCopy)
{
	unsigned *col = new unsigned[row];
	for (unsigned i = 0; i < row; ++i)
	{
		col[i] = column;
	}
	create(array, col, row, deepCopy);
	//delete col;
}



template<typename T>
inline Array3<T>::~Array3()
{
	if (_deepCopy)
	{
		delete _column;
		for (unsigned i = 0; i < _row; ++i)
		{
			delete _array[i];
		}
		delete _array;
	}
}

template<typename T>
inline Array3<T>::Array3(const Array3 & a) :Array3(a._array, a._column, a._row, false)
{
}

template<typename T>
inline Array3<T>::Array3(Array3 && a) noexcept
{
	_deepCopy = true;
	_array = a._array;
	_column = a._column;
	_row = a._row;
	a._array = nullptr;
	a._column = a._column;
	a._row = 0;
}

template<typename T>
inline Array3<T> & Array3<T>::operator=(const Array3 & a)
{
	// TODO: insert return statement here
	create(a._array, a._column, a._row, false);
	return *this;
}

template<typename T>
inline Array3<T> & Array3<T>::operator=(Array3 && a)
{
	// TODO: insert return statement here
	_deepCopy = true;
	_array = a._array;
	_column = a._column;
	_row = a._row;
	a._array = nullptr;
	a._column = a._column;
	a._row = 0;
}

template<typename T>
inline bool Array3<T>::create(T ** array, unsigned * column, unsigned row, bool deepCopy)
{
	_deepCopy = deepCopy;

	if (deepCopy)
	{
		_row = row;
		_column = new unsigned[row];
		_array = new T *[row];

		for (unsigned i = 0; i < row; ++i)
		{
			_column[i] = column[i];
			_array[i] = new T[_column[i]];
			for (unsigned j = 0; j < _column[i]; ++j)
			{
				_array[i][j] = array[i][j];
			}
		}
	}
	else
	{
		_row = row;
		_column = column;
		_array = array;
	}

	return true;
}
