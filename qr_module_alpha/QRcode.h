#pragma once

#include "BoolMatrix.h"
#include "InputOutput.h"
#include "TablesHandlers.h"
#include "QRfunctions.h"
//#include "BigInt.h"
//#include "InputOutput.h"
//#include <vector>

#include <iostream>
#include <opencv2/highgui.hpp>

#include "Matrix.h"
//using std::cout;

int* get_code_words_numbers(int v, ErrCorrLevels errCorrLevel);

class QRcode {
private:
    const int ver, side;
	int mask_pattern_code, border;
	ErrCorrLevels err_corr_level;
	// int mask_pattern_code;
	BoolMatrix pattern;
	BoolMatrix blueprint;
    BoolMatrix blueprint_and_const_DC;
    //unsigned short** pattern_mat;
public:
    QRcode(const int ver, const ErrCorrLevels err_corr_level);
    QRcode(const QRcode* qr_code);
    QRcode(const QRcode& qr_code);
    QRcode& make_QR_code(const int _mask_pattern_code, const bool _border = true);
    QRcode& put_data(const data_el* data, const int data_el_count);
    QRcode& put_data_raw(const data_el* data, const int data_el_count);
    Matrix get_bits_order_matrix(const data_el* data, const int data_el_count, const unsigned short first_mut_bit_val = 0);
	#if defined(DEV)
		void show(const char* wind_name = "QR Code (No base)", const int coeff = 5) const;
	#endif
    int get_side() const;
    int get_border() const;
    //operator unsigned __int16** () const;
    //operator cv::Mat () const;
    operator Matrix() const;
    int obtain_penalty_score() const;
private:
    template<typename T>
    void get_blocks(const T* bytes, const int bytes_len, T*** blocks, int** blocks_sizes, int* blocks_count)
    {
        const int* row = get_code_words_numbers(ver, err_corr_level);
        const int Blocks_Number_in_G_1 = row[2];
        const int DC_Number_in_Blocks_of_G_1 = row[3];
        const int Blocks_Number_in_G_2 = row[4];
        const int DC_Number_in_Blocks_of_G_2 = row[5];
        delete[] row;

        DEV_MSG("Blocks_Number_in_G_1 " << Blocks_Number_in_G_1);
        DEV_MSG("DC_Number_in_Blocks_of_G_1 " << DC_Number_in_Blocks_of_G_1);
        DEV_MSG("Blocks_Number_in_G_2 " << Blocks_Number_in_G_2);
        DEV_MSG("DC_Number_in_Blocks_of_G_2 " << DC_Number_in_Blocks_of_G_2);

        *blocks_count = Blocks_Number_in_G_1 + Blocks_Number_in_G_2;
        *blocks_sizes = new int[*blocks_count];
        *blocks = new T* [*blocks_count];
        LimitedIndex j(0, bytes_len);
        for (int i = 0; i < Blocks_Number_in_G_1; i++)
        {
            (*blocks)[i] = new T[DC_Number_in_Blocks_of_G_1];
            (*blocks_sizes)[i] = DC_Number_in_Blocks_of_G_1;
            for (int k = 0; k < DC_Number_in_Blocks_of_G_1; k++)
            {
                (*blocks)[i][k] = bytes[j++];
                //cout << bytes[j()] << " ";
                //j.inc();
            }
        }
        for (int i = 0; i < Blocks_Number_in_G_2; i++)
        {
            (*blocks)[i + Blocks_Number_in_G_1] = new T[DC_Number_in_Blocks_of_G_2];
            (*blocks_sizes)[i + Blocks_Number_in_G_1] = DC_Number_in_Blocks_of_G_2;
            for (int k = 0; k < DC_Number_in_Blocks_of_G_2; k++)
            {
                (*blocks)[i + Blocks_Number_in_G_1][k] = bytes[j++];
                //cout << bytes[j()] << " ";
                //j.inc();
            }
        }
        //cout << endl;
    }
    bool* get_DC_bits(const data_el* data, const int data_el_count, int* r_len = nullptr) const;
    static unsigned short** init_matrix(const int m, const int n);
    void horizontal_line(BoolMatrix& matrix, int row, int col_b, int col_e, int interval = 1, bool fill = true);
    void vertical_line(BoolMatrix& matrix, int col, int row_b, int row_e, int interval = 1, bool fill = true);
    //def add_alignment_pattern(self, center: Tuple[int, int]) :
    void add_alignment_pattern(const int center_y, const int center_x);

    void add_points(BoolMatrix& matrix, const BoolMatrix& point_obj);

    //def _create_pattern_blueprint(self)->List[List[int]]:
    void create_blueprint();
    static void project_on_matrix(const BoolMatrix& bool_matrix, unsigned short** matrix, const int m, const int  n, unsigned short projecting_val);
    void module_placement(const int* bits, const int len);
    template<typename T>
    void module_placement(unsigned short** matrix, const T* bits, const int len) const
    {
        int k = 0;
        bool upward_move = true;
        int start_j = side - 1;
        for (;;)
        {
            //cout << "k " << k << endl;
            if (upward_move)
            {
                for (int i = side - 1; i >= 0; i--)
                    for (int j = start_j; j >= start_j - 1; j--)
                    {
                        if (!blueprint.getElByIds(i, j))
                        {
                            matrix[i][j] = static_cast<unsigned short>(bits[k]);
                            k += 1;
                        }
                        if (k == len) return;
                    }

            }
            else
            {
                for (int i = 0; i < side; i++)
                    for (int j = start_j; j >= start_j - 1; j--)
                    {
                        if (!blueprint.getElByIds(i, j))
                        {
                            matrix[i][j] = static_cast<unsigned short>(bits[k]);
                            k += 1;
                        }
                        if (k == len) return;
                    }
            }
            start_j -= 2;

            if (start_j == 6) start_j = 5;
            upward_move = !upward_move;

            if (start_j < 0) break;
        }
    }
    template<typename T>
    void module_placement(const Matrix& matrix, const T* bits, const int len) const
    {
        int k = 0;
        bool upward_move = true;
        int start_j = side - 1;
        for (;;)
        {
            //cout << "k " << k << endl;
            if (upward_move)
            {
                for (int i = side - 1; i >= 0; i--)
                    for (int j = start_j; j >= start_j - 1; j--)
                    {
                        if (!blueprint.getElByIds(i, j))
                        {
                            matrix(i, j) = static_cast<unsigned short>(bits[k]);
                            k += 1;
                        }
                        if (k == len) return;
                    }

            }
            else
            {
                for (int i = 0; i < side; i++)
                    for (int j = start_j; j >= start_j - 1; j--)
                    {
                        if (!blueprint.getElByIds(i, j))
                        {
                            matrix(i, j) = static_cast<unsigned short>(bits[k]);
                            k += 1;
                        }
                        if (k == len) return;
                    }
            }
            start_j -= 2;

            if (start_j == 6) start_j = 5;
            upward_move = !upward_move;

            if (start_j < 0) break;
        }
    }
    int* generate_EC_codewords(
        const int* _message_polynomial_coefficients,
        const int _mes_pol_c_len,
        int* r_len = nullptr
    ) const;
    template<typename T>
    static T* interleave_blocks(T** blocks, const int* blocks_sizes, const int blocks_count, int* r_len = nullptr)
	{
        int* blocks_indexes = new int[blocks_count] {};
        int interleaved_blocks_len = 0;
        for (int i = 0; i < blocks_count; i++)
            interleaved_blocks_len += blocks_sizes[i];
        T* interleaved_blocks = new T[interleaved_blocks_len];

        int j = 0;
    	for (int i = 0; i < interleaved_blocks_len;)
    	{
            j %= blocks_count;
            if (blocks_indexes[j] < blocks_sizes[j])
            {
                interleaved_blocks[i] = blocks[j][blocks_indexes[j]++];
            	i++;
            }
            //out::printArr(interleaved_blocks, interleaved_blocks_len);
            //out::printArr(blocks_indexes, blocks_count);
            j++;
    	}
        if (r_len) *r_len = interleaved_blocks_len;
        return interleaved_blocks;
	}
    void create_pattern_base();
    void invert_by_mask_pattern();
    void add_format_info();
};
