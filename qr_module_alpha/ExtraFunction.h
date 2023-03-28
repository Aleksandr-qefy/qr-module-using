#pragma once
#include "QRfunctions.h"
#include <opencv2/highgui.hpp>

inline void get_err_indexes(const bool* arr, const int arr_len, int** err_indexes_arr, int* err_indexes_n)
{
	int index = 0;
	const int _11_bit_string_n = arr_len / 11;
	for (int i = 0; i < _11_bit_string_n; i++)
	{
		int _11_bit_string = 0;
		for (int j = 0; j < 11; j++)
			_11_bit_string += set_bit_to_1_by_indx(10 - j);
		if (_11_bit_string > 2024)
			*err_indexes_arr[index++] = i;

	}
	const int _6_bit_string_indx = _11_bit_string_n * 11;
	int _6_bit_string = 0;
	for (int j = 0; j < 6; j++)
		_6_bit_string += static_cast<int>(set_bit_to_1_by_indx(5 - j));
	if (_6_bit_string > 44)
		*err_indexes_arr[index++] = _11_bit_string_n;
	*err_indexes_n = index;
}

inline int* alphanumeric_correction(const bool* inverted_arr, const int arr_len, int* r_len)
{
	if (arr_len < 1) throw ArgumentError("ћассив inverted_arr должен иметь ненулевую длину arr_len!");
	const int err_indexes_arr_len = (arr_len - 1) / 11 + 1;
	int* err_indexes_arr = new int[err_indexes_arr_len];
	get_err_indexes(inverted_arr, arr_len, &err_indexes_arr, r_len);
	return err_indexes_arr;
}

cv::Mat get_cv2_mat(unsigned __int16** matrix, const int m, const int n)
{
	////https://docs.opencv.org/3.4/d3/d63/classcv_1_1Mat.html#a9fa74fb14362d87cb183453d2441948f
	////https://web-answers.ru/c/opencv-kak-sozdat-mat-iz-ukazatelja-uint8-t.html
	//constexpr int BYTES_PER_TYPE = 2;
	//const size_t step = BYTES_PER_TYPE * n;
	//cv::Mat cv2_mat(m, n, /*CV_16U*//*CV_16UC2*/CV_16UC1, &(matrix[0][0]), cv::Mat::AUTO_STEP/*, step*/);
	cv::Mat cv2_mat(m, n, CV_16U);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			cv2_mat.at<unsigned __int16>(i, j) = matrix[i][j];
	return cv2_mat;
}