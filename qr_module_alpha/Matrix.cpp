#include "Matrix.h"

Matrix::Matrix(const int m, const int n) : m(m), n(n), arr_len(m * n)
{
	matrix = new uint16[arr_len];
}

Matrix::Matrix(const Matrix& mat_obj) : m(mat_obj.m), n(mat_obj.n), arr_len(mat_obj.m * mat_obj.n)
{
	matrix = new uint16[arr_len];
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			matrix[i * n + j] = mat_obj(i, j);
}

//uint16& Matrix::operator()(const int i, const int j)
//{
//	if (i < 0 || i >= m) throw RowIndexError(i, m);
//	if (j < 0 || j >= n) throw ColIndexError(j, n);
//	return matrix[i * n + j];
//}
//
uint16& Matrix::operator()(const int i, const int j) const
{
	if (i < 0 || i >= m) throw RowIndexError(i, m);
	if (j < 0 || j >= n) throw ColIndexError(j, n);
	return matrix[i * n + j];
}

//void Matrix::set_by_ids(int i, int j, uint16 val)
//{
//	if (i < 0 || i >= m) throw RowIndexError(i, m);
//	if (j < 0 || j >= n) throw ColIndexError(j, n);
//	matrix[i * n + j] = val;
//}
//uint16 Matrix::get_by_ids(int i, int j) const
//{
//	if (i < 0 || i >= m) throw RowIndexError(i, m);
//	if (j < 0 || j >= n) throw ColIndexError(j, n);
//	return matrix[i * n + j];
//}

bool Matrix::is_alloc() const
{
	return matrix;
}

int Matrix::get_m() const
{
	return m;
}

int Matrix::get_n() const
{
	return n;
}

uint16* Matrix::get_arr() const
{
	return matrix;
}

int Matrix::get_arr_len() const
{
	return arr_len;
}

Matrix::~Matrix()
{
	delete[] matrix;
}