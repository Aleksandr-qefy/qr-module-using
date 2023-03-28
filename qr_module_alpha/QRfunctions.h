#pragma once

#include "Errors.h"
#include "InputOutput.h"
#include "BoolMatrix.h"

//def int_to_exp_of_2(digit: int) -> int:
int int_to_exp_of_2(int digit);

//def exp_of_2_to_int(exp: int) -> int:
int exp_of_2_to_int(int _exp);

bool f0(const int i, const int j);
bool f1(const int i, const int j);
bool f2(const int i, const int j);
bool f3(const int i, const int j);
bool f4(const int i, const int j);
bool f5(const int i, const int j);
bool f6(const int i, const int j);
bool f7(const int i, const int j);

typedef bool (*FuncType1)(const int, const int);
FuncType1 get_formula(const int code);

int len_of_character_count_indicator(const int mode, const int ver);

//bool get_bit(const unsigned int digit, const int idx);
template<typename T>
bool get_bit(const T digit, const int idx)
{
    constexpr T e = 1;
    return digit & (e << idx);
}

unsigned int set_bit_to_1_by_indx(const int idx);

bool* func0(const char* data, const int data_len, int* r_len);

int get_alphanumeric_value(const char ch);

bool* func1(const char* data, const int data_len, int* r_len);
bool* func2(const char* data, const int data_len, int* r_len);
bool* func3(const char* data, const int data_len, int* r_len);
bool* func4(const char* data, const int data_len, int* r_len);

class LimitedIndex
{
private:
    const int begin_val;
    int curr_index;
    int limit;
public:
    LimitedIndex(const int begin_val, const int limit);
    void inc(const int delta = 1);
    operator int() const;
    int operator++(int);
};

struct data_el
{
    int mode;
    const char* val;
    int val_len;
};

typedef bool* (*FuncType2)(const char* data, const int data_len, int* r_len);
//def get_data_bits(v, data: List[Union[int, str]], raw = False)->Union[List[int], Tuple[int, int]]:
bool* get_data_bytes(const data_el* data, const int data_el_count,
    const int ver, const int full_bits_count, LimitedIndex** r_len
);


int* bits_to_bytes_arr(const bool* bits, const int bits_len, int* r_len = nullptr);


template<typename T>
T* bytes_to_bits_arr(const int* bytes, const int bytes_len, int* r_len/*= nullptr*/)
{
    const int bits_count = CHAR_BIT * bytes_len;
    T* bits = new T[bits_count];
    for (int i = 0; i < bytes_len; i++)
        for (int j = 0; j < CHAR_BIT; j++)
            bits[i * CHAR_BIT + j] = get_bit(bytes[i], CHAR_BIT - j - 1);
    if (r_len) *r_len = bits_count;
    return bits;
}

int evaluation_condition_1(const BoolMatrix& matrix, const int border);
int evaluation_condition_2(const BoolMatrix& matrix, const int border);
int evaluation_condition_3(const BoolMatrix& matrix, const int border);
int evaluation_condition_4(const BoolMatrix& matrix, const int border);