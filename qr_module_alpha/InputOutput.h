#pragma once

//#define DEV

#ifdef DEV
#define PRINT_ARR(arr, n) out::printArr(arr, n);
#define PRINT_MATRIX(mat, rows, cols) \
	for (int i = 0; i < rows; i++) \
		out::printArr(mat[i], cols);
#define PRINT_BLOCKS(blocks, blocks_sizes, blocks_count) \
    for (int i = 0; i < blocks_count; i++) \
	{ \
		out::printArr(blocks[i], blocks_sizes[i]); \
	}
#define DEV_MSG(str) std::cout << str << std::endl;
#else
#define PRINT_ARR(arr, n)
#define PRINT_MATRIX(mat, rows, cols)
#define PRINT_BLOCKS(blocks, blocks_sizes, blocks_count)
#define DEV_MSG(str)
#endif

//#ifndef INPUT_OUTPUT_H// include guard
//#define INPUT_OUTPUT_H

#include "Constants.h"
#include "Errors.h"
#include "Matrix.h"

#include <opencv2/highgui.hpp>
#include <iomanip>

#if defined(DEV)
namespace out
{
	template <typename T, typename V = T>
	void printArr(T* arr, const int n) {
		for (int i = 0; i < n; i++)
			std::cout << (V)arr[i] << " ";
		std::cout << std:: endl;
	}

	void show_matrix(const Matrix& matrix, const cv::String& wind_name = "Matrix", const int coeff = 10, const int color_map = 0);

	void show_matrix(unsigned short** matrix, const int m, const int n, const cv::String& wind_name = "Matrix", const int coeff = 10, const int color_map = 0);

	void show_matrix(cv::Mat matrix, const cv::String& wind_name = "Matrix", const int coeff = 10, const int color_map = 0);
}
#endif