#include "InputOutput.h"

#if defined(DEV)
void out::show_matrix(const Matrix& matrix, const cv::String& wind_name/* = "Matrix"*/, const int coeff/* = 10*/, const int color_map/* = 0*/)
{

	if (!matrix.is_alloc()) throw NoMatrixAllocationError();

	const int m = matrix.get_m();
	const int n = matrix.get_n();

	cv::Mat img(m * coeff, n * coeff, CV_8UC3);//Создание изображения
	switch (color_map)
	{
	case 1:
		for (int r = 0; r < img.rows; r++) {
			// We obtain a pointer to the beginning of row r
			auto* row = img.ptr<cv::Vec3b>(r);

			for (int c = 0; c < img.cols; c++) {
				const unsigned short el_val = matrix(r / coeff, c / coeff);
				if (el_val != IMMUTABLE_BIT_VAL)
				{
					unsigned char c1 = (unsigned char)el_val;
					unsigned char c2 = (unsigned char)(el_val >> 4);
					unsigned char c3 = (unsigned char)(el_val >> 8);
					row[c] = cv::Vec3b(c3, c1, c2); // matrix[r / coeff][c / coeff] ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
				}
				else
					row[c] = cv::Vec3b(255, 255, 255);
			}
		}
		break;
	case 2:
		//for (int i = 0; i < m; i++)
		//{
		//	for (int j = 0; j < n; j++)
		//		cout << matrix[i][j];
		//	cout << endl;
		//}
		for (int r = 0; r < img.rows; r++) {
			// We obtain a pointer to the beginning of row r
			auto* row = img.ptr<cv::Vec3b>(r);

			for (int c = 0; c < img.cols; c++) {
				const unsigned short el_val = matrix(r / coeff,c / coeff);
				if (!el_val)
				{
					row[c] = cv::Vec3b(0, 0, 0); // matrix[r / coeff][c / coeff] ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
				}
				else
					row[c] = cv::Vec3b(255, 255, 255);
			}
		}
		break;
	default:
		for (int r = 0; r < img.rows; r++) {
			// We obtain a pointer to the beginning of row r
			auto* row = img.ptr<cv::Vec3b>(r);

			for (int c = 0; c < img.cols; c++) {
				// We invert the blue and red values of the pixel
				const int color = 255 * (matrix(r / coeff, c / coeff) % 8) / 7;
				row[c] = cv::Vec3b(color, color, color); // matrix[r / coeff][c / coeff] ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
			}
		}
	}

	imshow(wind_name, img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void out::show_matrix(unsigned short** matrix, const int m, const int n, const cv::String& wind_name/* = "Matrix"*/, const int coeff/* = 10*/, const int color_map/* = 0*/)
{

	if (!matrix) throw NoMatrixAllocationError();

	cv::Mat img(m * coeff, n * coeff, CV_8UC3);//Создание изображения
	switch (color_map)
	{
	case 1:
		for (int r = 0; r < img.rows; r++) {
			// We obtain a pointer to the beginning of row r
			auto* row = img.ptr<cv::Vec3b>(r);

			for (int c = 0; c < img.cols; c++) {
				const unsigned short el_val = matrix[r / coeff][c / coeff];
				if (el_val != IMMUTABLE_BIT_VAL)
				{
					unsigned char c1 = (unsigned char)el_val;
					unsigned char c2 = (unsigned char)(el_val >> 4);
					unsigned char c3 = (unsigned char)(el_val >> 8);
					row[c] = cv::Vec3b(c3, c1, c2); // matrix[r / coeff][c / coeff] ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
				}
				else
					row[c] = cv::Vec3b(255, 255, 255);
			}
		}
		break;
	case 2:
		//for (int i = 0; i < m; i++)
		//{
		//	for (int j = 0; j < n; j++)
		//		cout << matrix[i][j];
		//	cout << endl;
		//}
		for (int r = 0; r < img.rows; r++) {
			// We obtain a pointer to the beginning of row r
			auto* row = img.ptr<cv::Vec3b>(r);

			for (int c = 0; c < img.cols; c++) {
				const unsigned short el_val = matrix[r / coeff][c / coeff];
				if (!el_val)
				{
					row[c] = cv::Vec3b(0, 0, 0); // matrix[r / coeff][c / coeff] ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
				}
				else
					row[c] = cv::Vec3b(255, 255, 255);
			}
		}
		break;
	default:
		for (int r = 0; r < img.rows; r++) {
			// We obtain a pointer to the beginning of row r
			auto* row = img.ptr<cv::Vec3b>(r);

			for (int c = 0; c < img.cols; c++) {
				// We invert the blue and red values of the pixel
				const int color = 255 * (matrix[r / coeff][c / coeff] % 8) / 7;
				row[c] = cv::Vec3b(color, color, color); // matrix[r / coeff][c / coeff] ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
			}
		}
	}

	imshow(wind_name, img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void out::show_matrix(cv::Mat matrix, const cv::String& wind_name/* = "Matrix"*/, const int coeff/* = 10*/, const int color_map/* = 0*/)
{
	cv::Mat img(matrix.rows * coeff, matrix.cols * coeff, CV_8UC3);//Создание изображения
	switch (color_map)
	{
	case 1:
		for (int r = 0; r < img.rows; r++) {
			// We obtain a pointer to the beginning of row r
			auto* row = img.ptr<cv::Vec3b>(r);

			for (int c = 0; c < img.cols; c++) {
				const unsigned short el_val = matrix.at<unsigned __int16>(r / coeff, c / coeff);
				if (el_val != IMMUTABLE_BIT_VAL)
				{
					unsigned char c1 = (unsigned char)el_val;
					unsigned char c2 = (unsigned char)(el_val >> 4);
					unsigned char c3 = (unsigned char)(el_val >> 8);
					row[c] = cv::Vec3b(c3, c1, c2); // matrix[r / coeff][c / coeff] ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
				}
				else
					row[c] = cv::Vec3b(255, 255, 255);
			}
		}
		break;
	case 2:
		//for (int i = 0; i < m; i++)
		//{
		//	for (int j = 0; j < n; j++)
		//		cout << matrix[i][j];
		//	cout << endl;
		//}
		for (int r = 0; r < img.rows; r++) {
			// We obtain a pointer to the beginning of row r
			auto* row = img.ptr<cv::Vec3b>(r);

			for (int c = 0; c < img.cols; c++) {
				const unsigned short el_val = matrix.at<unsigned __int16>(r / coeff, c / coeff);
				if (!el_val)
				{
					row[c] = cv::Vec3b(0, 0, 0); // matrix[r / coeff][c / coeff] ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
				}
				else
					row[c] = cv::Vec3b(255, 255, 255);
			}
		}
		break;
	default:
		for (int r = 0; r < img.rows; r++) {
			// We obtain a pointer to the beginning of row r
			auto* row = img.ptr<cv::Vec3b>(r);

			for (int c = 0; c < img.cols; c++) {
				// We invert the blue and red values of the pixel
				const int color = 255 * (matrix.at<unsigned __int16>(r / coeff, c / coeff) % 8) / 7;
				row[c] = cv::Vec3b(color, color, color); // matrix[r / coeff][c / coeff] ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0); // Vec3b(row[c][2], row[c][1], row[c][0]);
			}
		}
	}

	imshow(wind_name, img);
	cv::waitKey(0);
	cv::destroyAllWindows();
}
#endif
