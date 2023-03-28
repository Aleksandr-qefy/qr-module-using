#include "BoolMatrix.h"

BoolMatrix::BoolMatrix() : m(0), n(0), typeArr(nullptr), typeElCount(0) {};


void BoolMatrix::initArr(const int _m, const int _n, const bool fill/*false*/) {
	const int p = _m * _n;
	const int remainder = p % bitsCountPerType;
	typeElCount = remainder ? p / bitsCountPerType + 1 : p / bitsCountPerType;
	typeArr = new elT[typeElCount];
	if (remainder)
	{
		for (int i = 0; i < typeElCount - 1; i++)
			typeArr[i] = fill ? ~static_cast<elT>(0) : 0;
		//typeArr[typeElCount - 1] = ~static_cast<elT>(~static_cast<elT>(0) << remainder);
		if (fill) typeArr[typeElCount - 1] = ~static_cast<elT>(~static_cast<elT>(0) << remainder);
		else typeArr[typeElCount - 1] = 0;
	}
	else
		for (int i = 0; i < typeElCount; i++)
			typeArr[i] = fill ? ~static_cast<elT>(0) : 0;
}

void BoolMatrix::verify_coords(const int i, const int j) const
{
	if (i < 0 || i >= m)
		throw RowIndexError(i, m - 1);
	if (j < 0 || j >= n)
		throw ColIndexError(j, n - 1);
}

BoolMatrix::BoolMatrix(const int m, const int n, const bool fill/*= false*/) : m(m), n(n) {
	initArr(m, n, fill);
}

BoolMatrix::BoolMatrix(const int* arr, const int arr_len, const int m, const int n, const int b_index/*= 0*/, const bool reversed/*= false*/, int el_count/*= 0*/) : BoolMatrix(m, n) {
	if (b_index < 0 || b_index > arr_len - 1) throw ArgumentError("Значение параметра b_index должно находиться на отрезке 0...(arrLen - 1) !");
	if (!el_count) el_count = arr_len - b_index;
	if (b_index + el_count > arr_len) throw ArgumentError("При таком значении параметра el_count произойдёт выход за границы массива!");
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++) {
			if (!reversed) setElByIds(i, j, arr[b_index + i * n + j]);
			else setElByIds(i, j, arr[b_index + el_count - (i * n + j) - 1]);
			if (i * n + j + 1 == el_count) return;
		}
}

BoolMatrix::BoolMatrix(const BoolMatrix& otherMatrix) : BoolMatrix(otherMatrix.m, otherMatrix.n) {
	if (!otherMatrix.typeArr)
		throw NoSecondMatrixAllocationError();
	//if (typeArr) delete[] typeArr;
	initArr(m, n);
	join(otherMatrix, 0, 0);
}
//BoolMatrix bm = versionPatternsTable(21);

BoolMatrix& BoolMatrix::setElByIds(const int i, const int j, const bool val) {
	if (!typeArr) throw NoMatrixAllocationError();
	verify_coords(i, j);
	const int index = i * n + j;
	const int count = index / bitsCountPerType;
	const int remain = index % bitsCountPerType;
	if (val)
		typeArr[count] |= (1 << remain);
	else
		typeArr[count] = ~(~(typeArr[count]) | (1 << remain));
	return *this;
}
// const Matrix& otherMatrix
bool BoolMatrix::getElByIds(const int i, const int j) const {
	if (!typeArr) NoMatrixAllocationError();
	const int index = i * n + j;
	const int count = index / bitsCountPerType;
	const int remain = index % bitsCountPerType;
	return typeArr[count] & (1 << remain);
}
#if defined(DEV)
void BoolMatrix::show(const cv::String& wind_name/*="Bool Matrix"*/, const int coeff/*= 10*/) const {
	if (!typeArr) throw NoMatrixAllocationError();
	cv::Mat img(m * coeff, n * coeff, CV_8UC3);
	//for (Pixel& p : Mat_<Pixel>(img)) {
	//	p.x = 255;
	//}
	for (int r = 0; r < img.rows; r++) {
		// We obtain a pointer to the beginning of row r
		auto* row = img.ptr<cv::Vec3b>(r);

		for (int c = 0; c < img.cols; c++) {
			// We invert the blue and red values of the pixel
			row[c] = getElByIds(r / coeff, c / coeff) ? cv::Vec3b(255, 255, 255): cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
		}
	}
	/** @brief Runs the given functor over all matrix elements in parallel.

	The operation passed as argument has to be a function pointer, a function object or a lambda(C++11).

	Example 1. All of the operations below put 0xFF the first channel of all matrix elements:
	@code
	Mat image(1920, 1080, CV_8UC3);
	typedef cv::Point3_<uint8_t> Pixel;

	// first. raw pointer access.
	for (int r = 0; r < image.rows; ++r) {
		Pixel* ptr = image.ptr<Pixel>(r, 0);
		const Pixel* ptr_end = ptr + image.cols;
		for (; ptr != ptr_end; ++ptr) {
			ptr->x = 255;
		}
	}

	// Using MatIterator. (Simple but there are a Iterator's overhead)
	for (Pixel &p : cv::Mat_<Pixel>(image)) {
		p.x = 255;
	}

	// Parallel execution with function object.
	struct Operator {
		void operator ()(Pixel &pixel, const int * position) {
			pixel.x = 255;
		}
	};
	image.forEach<Pixel>(Operator());

	// Parallel execution using C++11 lambda.
	image.forEach<Pixel>([](Pixel &p, const int * position) -> void {
		p.x = 255;
	});
	*/
	//Mat img_resized;
	//cv::resizeWindow(img, img_resized, cv::Size(m * 25, n * 25));

	imshow(wind_name, img);
	cv::waitKey(0);
	cv::destroyAllWindows();

	////setConsoleColor(0, 9);
	//for (int i = 0; i < m; i++) {
	//	for (int j = 0; j < n; j++)
	//		//if (getElByIds(i, j))
	//		//	cout << "\u25A0";
	//		//else
	//		//	cout << "\u25A1";
	//		//if (getElByIds(i, j))
	//		//	cout << (char)254u;
	//		//else
	//		//	cout << (char)255u;
	//		if (getElByIds(i, j))
	//			cout << 1;
	//		else
	//			cout << 0;
	//	cout << endl;
	//}
	////setConsoleColor(0, 0);
	//for (int k = 0; k < n; k++)
	//	cout << '-';
	//cout << endl;
}
#include <iomanip>
void BoolMatrix::show_def() const
{
	if (!typeArr) {
		std::cout << "Нет выделения памяти для матрицы!" << std::endl;
		return;
	}
	//setConsoleColor(0, 9);
	for (int i = 0; i < typeElCount; i++)
	{
		for (int j = 0; j < bitsCountPerType; j++)
			if (typeArr[i] & static_cast<elT>(1) << j)
				std::cout << 1;
			else
				std::cout << 0;
		std::cout << std::endl;
	}
	constexpr std::streamsize center = bitsCountPerType / 2 + 2;
	std::cout << std::setw(center) << "def!" << std::endl;
}
#endif
int BoolMatrix::get_m() const
{
	return m;
}
int BoolMatrix::get_n() const
{
	return n;
}
int BoolMatrix::get_1s_count() const
{
	int res = 0;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			res += getElByIds(i, j);
	return res;
}
void BoolMatrix::join(const BoolMatrix& otherMatrix, const int i0, int const j0, const bool copy_only_1s/*= false*/) {
	if (otherMatrix.m > m || otherMatrix.n > n) 
		throw MatrixesSizesError(m, n, otherMatrix.m, otherMatrix.n);
	verify_coords(i0, j0);

	const int i1 = i0 + otherMatrix.m - 1;
	const int j1 = j0 + otherMatrix.n - 1;
	if (i1 < 0 || i1 >= m)
		throw RowIndexError(i1, m - 1);
	if (j1 < 0 || j1 >= n)
		throw ColIndexError(j1, n - 1);

	if (!copy_only_1s)
		for (int i = 0; i < otherMatrix.m; i++)
			for (int j = 0; j < otherMatrix.n; j++)
				setElByIds(i + i0, j + j0, otherMatrix.getElByIds(i, j));
	else
		for (int i = 0; i < otherMatrix.m; i++)
			for (int j = 0; j < otherMatrix.n; j++)
				if (otherMatrix.getElByIds(i, j))
				setElByIds(i + i0, j + j0, true);
}
BoolMatrix BoolMatrix::copy() const {
	if (!typeArr) throw NoMatrixAllocationError();
	//BoolMatrix* newMatrixP = new BoolMatrix(m, n);
	//for (int i = 0; i < m; i++)
	//	for (int j = 0; j < n; j++)
	//		newMatrixP->setElByIds(i, j, getElByIds(i, j));
	//return *newMatrixP;
	BoolMatrix newMatrix(m, n);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			newMatrix.setElByIds(i, j, getElByIds(i, j));
	return newMatrix;
}
//BoolMatrix BoolMatrix::unite(const BoolMatrix& otherMatrix) {
//	if (!typeArr) throw NoMatrixAllocationError();
//	////BoolMatrix* newMatrixP = new BoolMatrix(m, n);
//	////*newMatrixP = copy();
//	////newMatrixP->join(otherMatrix, i, j);
//	////return *newMatrixP;
//	//BoolMatrix newMatrix(m, n);
//	BoolMatrix newMatrix = copy();
//	for (int i = 0; i < m; i++)
//		for (int j = 0; j < n; j++)
//			newMatrix.setElByIds(i, j, getElByIds(i, j));
//	return newMatrix;
//}
BoolMatrix& BoolMatrix::operator=(const BoolMatrix& otherMatrix) {
	if (this == &otherMatrix) return *this;
	// Эта ошибка проверяется именно в 1ой матрице, 
	// т.к. собственно 2ая матрица не обязана быть определена в памяти,
	// ведь мы не извлекаем или добавляем в неё элементы, значит мы можем определить её в памяти позже
	if (!otherMatrix.typeArr)
		throw NoSecondMatrixAllocationError();
	if (typeArr) {
		delete[] typeArr;
		typeArr = nullptr;
	}
	m = otherMatrix.m;
	n = otherMatrix.n;
	initArr(m, n);
	join(otherMatrix, 0, 0);
	return *this;
}
//BoolMatrix bm;
//bm = versionPatternsTable(21);

BoolMatrix::~BoolMatrix() {
	//cout << typeid(typeArr).name() << endl;
	//if (typeArr) cout << (int) typeArr[6] << endl;
	if (typeArr) delete[] typeArr;
}

/*inline*/ BoolMatrix transpose(const BoolMatrix& matrix)
{
	//const auto matrix_t_p = new BoolMatrix(matrix.get_n(), matrix.get_m());
	//for (int i = 0; i < matrix_t_p->get_m(); i++)
	//	for (int j = 0; j < matrix_t_p->get_n(); j++)
	//		matrix_t_p->setElByIds(i, j, matrix.getElByIds(j, i));
	//return matrix_t_p;
	BoolMatrix matrix_t(matrix.get_n(), matrix.get_m());
	for (int i = 0; i < matrix_t.get_m(); i++)
		for (int j = 0; j < matrix_t.get_n(); j++)
			matrix_t.setElByIds(i, j, matrix.getElByIds(j, i));
	return matrix_t;
}