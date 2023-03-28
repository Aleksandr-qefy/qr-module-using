#pragma once

#include "Errors.h"
#include "Constants.h"

class Matrix
{
public:
	uint16* matrix;
	const int m;
	const int n;
	const int arr_len;
	Matrix(const int m, const int n);
	Matrix(const Matrix&);
	//uint16& operator()(const int i, const int j);
	uint16& operator()(const int i, const int j) const;
	//void set_by_ids(int i, int j, uint16 val);
	//uint16 get_by_ids(int i, int j) const;
	bool is_alloc() const;
	int get_m() const;
	int get_n() const;
	uint16* get_arr() const;
	int get_arr_len() const;
	~Matrix();
};

//template<typename T>
//class Matrix
//{
//public:
//	T* matrix;
//	const int m;
//	const int n;
//	Matrix(const int m, const int n);
//	Matrix(const Matrix&);
//	T& operator()(const int i, const int j) const;
//	~Matrix();
//};
//
//template<typename T>
//Matrix<T>::Matrix(const int m, const int n) : m(m), n(n)
//{
//	matrix = new T[m * n];
//}
//
//template<typename T>
//Matrix<T>::Matrix(const Matrix<T>& mat_obj) : m(mat_obj.m), n(mat_obj.n)
//{
//	matrix = new T[m * n];
//	for (int i = 0; i < m; i++)
//		for (int j = 0; j < n; j++)
//			matrix[i * n + j] = mat_obj(i, j);
//}
//
//template<typename T>
//T& Matrix<T>::operator()(const int i, const int j) const
//{
//	if (i < 0 || i >= m) throw RowIndexError(i, m);
//	if (j < 0 || j >= n) throw ColIndexError(j, n);
//	return matrix[i * n + j];
//}
//
//template<typename T>
//Matrix<T>::~Matrix()
//{
//	delete[] matrix;
//}

