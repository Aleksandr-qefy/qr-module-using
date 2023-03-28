#pragma once

#include <opencv2/core/cvstd.hpp>

#include "Errors.h"
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
//using namespace cv;

class BoolMatrix {
private:
	typedef unsigned __int8 /*unsigned char*/ elT;
	static constexpr int typeSize = sizeof(elT);
	int m, n;
	static constexpr int bitsCountPerType = typeSize << 3;
	elT* typeArr;
	int typeElCount;
	void initArr(const int _m, const int _n, const bool fill = false);
	void verify_coords(const int i, const int j) const;
public:
	BoolMatrix();
	BoolMatrix(int m, int n, bool fill = false);
	BoolMatrix(const BoolMatrix& otherMatrix);
	BoolMatrix(const int* arr, const int arr_len, const int m, const int n, const int b_index = 0, const bool reversed = false, int el_count= 0);
	BoolMatrix& setElByIds(int i, int j, bool val);
	// const Matrix& otherMatrix
	bool getElByIds(int i, int j) const;
	void show(const cv::String& wind_name = "Bool Matrix", const int coeff = 10) const;
	#if defined(DEV)
		void show_def() const;
	#endif
	int get_m() const;
	int get_n() const;
	int get_1s_count() const;
	void join(const BoolMatrix& otherMatrix, int i0, int j0, bool copy_only_1s = false);
	BoolMatrix copy() const;
	BoolMatrix& operator=(const BoolMatrix& otherMatrix);
	//BoolMatrix unite(const BoolMatrix& otherMatrix);
	~BoolMatrix();
};

BoolMatrix transpose(const BoolMatrix& matrix);
