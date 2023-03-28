/*
https://en.wikipedia.org/wiki/QR_code
https://www.thonky.com/qr-code-tutorial/byte-mode-encoding
http://машинноезрение.рф/i2dip/2018-2019/i2opencv/note_01.html
https://docs.opencv.org/4.x/df/d65/tutorial_table_of_content_introduction.html
Установка OpenCV:
	https://www.codespeedy.com/install-opencv-cpp-with-visual-studio/
	https://opencv.org/releases/
https://docs.python.org/3/extending/newtypes.html
https://stackoverflow.com/questions/30184998/how-to-disable-cout-output-in-the-runtime
https://stackoverflow.com/questions/12957492/writing-python-bindings-for-c-code-that-use-opencv <- реально
крутая статья
https://stackoverflow.com/questions/30388170/sending-a-c-array-to-python-and-back-extending-c-with-numpy
https://wiki.python.org/moin/boost.python/HowTo
https://learn.microsoft.com/ru-RU/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2022
https://docs.python.org/3/extending/extending.html <- 1. Extending Python with C or C++

*/

#include "QRcode.h"
#include "Errors.h"
#include "InputOutput.h"

#include <iomanip>

#include "ExtraFunction.h"

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "RUS");
	try {
		//std::cout.setstate(std::ios_base::failbit);
		const data_el data_el01 = {
			2, "Hello World!", 12
		};
		//const data_el data_el01 = {
		//	2, "30.12.2022 Happy New Year!", 28
		//};
		const data_el data_el02 = {
			1, " $HJHGHBJ", 9
		};
		const data_el data[]
		{
			data_el01,
			data_el02
		};
		const int el_count = 2;

		const int ver = 2;
		const int coeff = 3 + 12 * (40 - ver) / 40;
		QRcode qr_code(ver, M);

		QRcode qr_code_raw(qr_code);
		try
		{
			Matrix mat = qr_code_raw.put_data_raw(data, el_count)/*.show()*/;
			out::show_matrix(mat, "QR (Raw)", coeff, 2);
		}
		catch (NotEnoughSize& err)
		{
			std::cerr << err.getErrMsg() << std::endl;
			return -1;
		}

		QRcode qr_code_bits_order_matrix(qr_code);
		try
		{
			Matrix mat = qr_code_bits_order_matrix.get_bits_order_matrix(data, el_count)/*.show()*/;
			out::show_matrix(mat, "QR (Bits order)", coeff, 1);
		}
		catch (NotEnoughSize& err)
		{
			std::cerr << err.getErrMsg() << std::endl;
			return -1;
		}

		//cv2_mat = qr_code.put_data(data, el_count).make_QR_code(0)/*.show("QR", 3)*/;
		for (int i = 0; i < 8; i++)
		{
			QRcode qr_code_fin(qr_code);
			Matrix mat = qr_code_fin.put_data(data, el_count).make_QR_code(i);
			std::cout << "score[mask = " << i << "]:" << qr_code_fin.obtain_penalty_score() << std::endl;
			out::show_matrix(mat, "QR", coeff, 2);
		}
		//std::cout << "qr_code.get_side() " << qr_code.get_side() << std::endl;
		//std::cout << "qr_code.get_border() " << qr_code.get_border() << std::endl;
		//std::cout << "cv2_mat.rows " << cv2_mat.rows << std::endl;
		//std::cout << "cv2_mat.cols " << cv2_mat.cols << std::endl;
		//out::show_matrix(cv2_mat, "QR Raw", coeff, 2);

		//std::cout.clear();
		return 0;
	}
	catch (Error& err) {
		std::cout.clear();
		const char* err_msg = err.getErrMsg();
		std::cout << err_msg << std::endl;
		delete[] err_msg;
	}
	catch (...) {
		std::cout.clear();
		std::cerr << "Ошибка!" << std::endl;
	}
	return -1;
}
