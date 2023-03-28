#pragma once

#include <iostream>
//using namespace std;

class Error /*: public std::exception*/ {
public:
	/*Error() : std::exception(getErrMsg()) {}*/
	virtual char* getErrMsg() {
		return (char*)"Ошибка! (больше информации нет)";
	}
};

class ArgumentError : public Error {
private:
	const char* errMsg;
public:
	ArgumentError(const char* errMsg) : errMsg(errMsg) {}
	char* getErrMsg() override {
		char* msg = new char[100];
		sprintf_s(msg, 100, "%s", errMsg);
		return msg;
	}
};

class MatrixesSizesError : public Error {
private:
	int m1, n1, m2, n2;
public:
	MatrixesSizesError(int m1, int n1, int m2, int n2) : m1(m1), n1(n1), m2(m2), n2(n2) {}
	char* getErrMsg() override {
		char* msg = new char[100];
		sprintf_s(msg, 100, "1ая матрица %dX%d, а 2ая - %dX%d. 2ую матрицу нельзя вписать в 1ую!", m1, n1, m2, n2);
		return msg;
	}
};

// NO_SECOND_MATRIX_ALLOCATION
class NoMatrixAllocationError : public Error {
	char* getErrMsg() override {
		return (char*)"Эта матрица не была определена в памяти!";
	}
};

// NO_SECOND_MATRIX_ALLOCATION
class NoSecondMatrixAllocationError : public Error {
	char* getErrMsg() override {
		return (char*)"Второй операнд (матрица) не был определён в памяти!";
	}
};

class IndexErrorBase : public Error {
protected:
	int errIndex;
	int maxAllowedIndex;
public:
	IndexErrorBase(int errIndex, int maxAllowedIndex) : errIndex(errIndex), maxAllowedIndex(maxAllowedIndex) {};
	//	virtual char* getErrMsg() = 0;
};

class IndexError : public IndexErrorBase {
private:
	int minIndx;
public:
	IndexError(int indx, int minIndx, int maxIndx) : IndexErrorBase(indx, maxIndx), minIndx(minIndx) {}
	char* getErrMsg() override {
		char* msg = new char[100];
		sprintf_s(msg, 100, "Неправильный индекс %d! Индекс должен находится в диапазоне %d...%d", errIndex, minIndx, maxAllowedIndex);
		return msg;
	}
};

class RowIndexError : public IndexErrorBase {
public:
	RowIndexError(int indx, int maxIndx) : IndexErrorBase(indx, maxIndx) {}
	char* getErrMsg() override {
		char* msg = new char[100];
		sprintf_s(msg, 100, "Неправильный индекс строки %d! Индекс должен находится в диапазоне 0...%d", errIndex, maxAllowedIndex);
		return msg;
	}
};

class ColIndexError : public IndexErrorBase {
public:
	ColIndexError(int indx, int maxIndx) : IndexErrorBase(indx, maxIndx) {}
	char* getErrMsg() override {
		char* msg = new char[100];
		sprintf_s(msg, 100, "Неправильный индекс столбца %d! Индекс должен находится в диапазоне 0...%d", errIndex, maxAllowedIndex);
		return msg;
	}
};

class NotEnoughSize : public Error {
public:
	char* getErrMsg() override {
		return (char*)"Не хватает размера QR кода, чтобы при данной версии и уровне коррекции ошибок отобразить всю информацию!";
	}
};