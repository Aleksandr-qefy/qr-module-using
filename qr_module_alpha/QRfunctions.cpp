#include "QRfunctions.h"
#include <iomanip>
//using namespace std;

bool* func0(const char* data, const int data_len, int* r_len)
{
    if (!data_len) {
        *r_len = 0;
        return new bool[0];
    }
    // в массиве blocks создаём массивы размером 3 из "строки" data,
    // если остаётся меньше 3, то добавляем туда массив меньше 3
    const int count = (data_len - 1) / 3 + 1;
    char** blocks = new char* [count];
    int data_i = 0;
    int difference = 0;
    for (int i = 0; i < count; i++)
    {
        //const int data_i = i * 3;
        difference = data_len - data_i;
        if (difference < 3)
        {
            blocks[i] = new char[difference + 1];
            for (int k = 0; k < difference; k++)
                blocks[i][k] = data[data_i + k];
            blocks[i][difference] = '\0';
        }
        else
        {
            blocks[i] = new char[4];
            for (int k = 0; k < 3; k++)
                blocks[i][k] = data[data_i + k];
            blocks[i][3] = '\0';
        }
        data_i += 3;
    }
    DEV_MSG("blocks: ")
    PRINT_MATRIX(blocks, count - 1, 3)
    PRINT_ARR(blocks[count - 1], difference);
    //cout << "difference " << difference << endl;
    const int N[3]{ 4, 7, 10 };
    const int n = N[difference - 1];
    unsigned __int16* binary_bits = new unsigned __int16[count];
    for (int i = 0; i < count; i++)
        binary_bits[i] = atoi(blocks[i]);
    DEV_MSG("binary_bits: ")
    PRINT_ARR(binary_bits, count);
    const int res_len = 10 * (count - 1) + n;
    bool* bits = new bool[res_len];
    for (int i = 0; i < count - 1; i++)
        for (int j = 0; j < 10; j++)
            bits[i * 10 + j] = get_bit(binary_bits[i], 9 - j);
    const int p = 10 * (count - 1);
    for (int i = 0; i < n; i++)
        bits[p + i] = get_bit(binary_bits[count - 1], n - i - 1);
    //cout << "bits: " << endl;
    //out::printArr(bits, res_len);
    delete[] binary_bits;
    *r_len = res_len;
    return bits;
}

bool* func1(const char* data, const int data_len, int* r_len)
{
    if (!data_len) {
        *r_len = 0;
        return new bool[0];
    }
    const int count = (data_len - 1) / 2 + 1;
    char** blocks = new char* [count];
    int data_i = 0;
    int difference = 0;
    for (int i = 0; i < count; i++)
    {
        //const int data_i = i * 3;
        difference = data_len - data_i;
        if (difference < 2)
        {
            blocks[i] = new char[difference];
            for (int k = 0; k < difference; k++)
                blocks[i][k] = data[data_i + k];
        }
        else
        {
            blocks[i] = new char[2];
            for (int k = 0; k < 2; k++)
                blocks[i][k] = data[data_i + k];
        }
        data_i += 2;
    }
    DEV_MSG("blocks: ")
    PRINT_MATRIX(blocks, count - 1, 2)
    PRINT_ARR(blocks[count - 1], difference)
    DEV_MSG("difference: " << difference)
    unsigned __int16* binary_bits = new unsigned __int16[count];
    int (*f)(char) = get_alphanumeric_value;
    if (difference % 2)
    {
        for (int i = 0; i < count - 1; i++)
            binary_bits[i] = f(blocks[i][0]) * 45 + f(blocks[i][1]);
        binary_bits[count - 1] = f(blocks[count - 1][0]);
    }
    else
    {
        for (int i = 0; i < count; i++)
            binary_bits[i] = f(blocks[i][0]) * 45 + f(blocks[i][1]);
    }
    PRINT_ARR(binary_bits, count)
    const int res_len = 11 * (count - 1) + (difference % 2 ? 6 : 11);
    bool* bits = new bool[res_len];
    if (difference % 2)
    {
        for (int i = 0; i < count - 1; i++)
            for (int j = 0; j < 11; j++)
                bits[i * 11 + j] = get_bit(binary_bits[i], 10 - j);
        const int p = 11 * (count - 1);
        for (int i = 0; i < 6; i++)
            bits[p + i] = get_bit(binary_bits[count - 1], 5 - i);
    }
    else
    {
        for (int i = 0; i < count; i++)
            for (int j = 0; j < 11; j++)
                bits[i * 11 + j] = get_bit(binary_bits[i], 10 - j);
    }
    //out::printArr(bits, res_len);
    delete[] binary_bits;
    *r_len = res_len;
    return bits;
}

bool* func2(const char* data, const int data_len, int* r_len)
{
    if (!data_len) {
        *r_len = 0;
        return new bool[0];
    }
    unsigned __int8* binary_bits = new unsigned __int8[data_len];
    //cout << "data_len " << data_len << endl;
    for (int i = 0; i < data_len; i++)
        binary_bits[i] = (int)data[i];
    //out::printArr(binary_bits, data_len);
    //for (int i = 0; i < data_len; i++)
    //    cout << "0x" << hex << binary_bits[i] << " ";
    //cout << endl;
    const int res_len = 8 * data_len;
    //cout << "res_len " << res_len << endl;
    bool* bits = new bool[res_len];
    for (int i = 0; i < data_len; i++)
        for (int j = 0; j < 8; j++)
            bits[i * 8 + j] = get_bit(binary_bits[i], 7 - j);
    delete[] binary_bits;
    *r_len = res_len;
    return bits;
}

bool* func3(const char* data, const int data_len, int* r_len)
{
    *r_len = 0;
    return new bool[0];
}

bool* func4(const char* data, const int data_len, int* r_len)
{
    *r_len = 0;
    return new bool[0];
}

//def int_to_exp_of_2(digit: int) -> int:
int int_to_exp_of_2(int digit)
{
    if (digit < 1) throw ArgumentError("Значение параметра digit не должно быть меньше 1!");
    if (digit == 1) return 0;
    int response = 0;
    while (digit != 1)
    {
        if (digit % 2 == 0)
        {
            digit /= 2;
            //response >>= 1;
            response += 1;
        }
        else
            digit ^= 285;
    }
    return response;
}

int pow_int(const int a, const int b)
{
    if (b < 0) return 0;
    if (b == 0) return 1;
    int res = 1;
    for (int i = 1; i <= b; i++)
    {
        res *= a;
    }
    return res;
}

//def exp_of_2_to_int(exp: int) -> int:
int exp_of_2_to_int(int _exp)
{
    if (_exp < 0) throw ArgumentError("Значение параметра _exp не должно быть меньше 0!");
    if (_exp == 0) return 1;

    int k = 0;
    if (_exp >= 9)
    {
        k = _exp - 9;
        _exp = 9;
    }

    int response = pow_int(2, _exp);
    if (response < 256) return response;
    while ((response ^ 285) > 255)
    {
        if (response % 2 == 0)
        {
            response /= 2;
            //response >>= 1;
            k += 1;
        }
        else
            break;
    }
    response ^= 285;
    for (int i = 0; i < k; i++)
    {
        response *= 2;
        //response <<= 1;
        if (response > 255)
            response ^= 285;
    }
    return response;
}

bool f0(const int i, const int j) { return (i + j) % 2 == 0; }
bool f1(const int i, const int j) { return i % 2 == 0; }
bool f2(const int i, const int j) { return j % 3 == 0; }
bool f3(const int i, const int j) { return (i + j) % 3 == 0; }
bool f4(const int i, const int j) { return (i / 2 + j / 3) % 2 == 0; }
bool f5(const int i, const int j) { return ((i * j) % 2) + ((i * j) % 3) == 0; }
bool f6(const int i, const int j) { return (((i * j) % 2) + ((i * j) % 3)) % 2 == 0; }
bool f7(const int i, const int j) { return (((i + j) % 2) + ((i * j) % 3)) % 2 == 0; }

FuncType1 get_formula(const int code)
{
    const FuncType1 lymbda_funcs[8]
    {
        f0,f1,f2,f3,f4,f5,f6,f7
    };
    return lymbda_funcs[code];
}

int len_of_character_count_indicator(const int mode, const int ver)
{
    int row = 0;
    if (1 <= ver && ver < 10)
        row = 0;
    else if (10 <= ver && ver < 27)
        row = 1;
    else
        row = 2;
    const int values[4][3]
    {
        {10, 12, 14},
        {9, 11, 13},
        {8, 16, 16},
        {8, 10, 12}
    };
    return values[mode][row];
}

unsigned int set_bit_to_1_by_indx(const int idx)
{
    if (idx >= sizeof(int) * CHAR_BIT || idx < 0)
        throw ArgumentError("Некорректный индекс!");
    constexpr unsigned int e = 1;
    return e << idx;
}

const char* alphanumeric_value = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

int get_alphanumeric_value(const char ch)
{
    constexpr int lst_len = 45;
    int i;
    for (i = 0; i < lst_len; i++)
        if (alphanumeric_value[i] == ch) break;
    //delete[] lst;
    if (i == lst_len) throw ArgumentError("Некорректное значение для alphanumeric mode encoding!");
    return i;
}

LimitedIndex::LimitedIndex(const int begin_val, const int limit) : begin_val(begin_val), curr_index(begin_val), limit(limit) {};

void LimitedIndex::inc(const int delta/*= 1*/)
{
    if (curr_index + delta > limit) throw IndexError(curr_index + delta, begin_val, limit);
    curr_index += delta;
}
int LimitedIndex::operator++(int)
{
    inc();
    return curr_index - 1;
}

//int LimitedIndex::operator()() const {
//    return curr_index;
//}
LimitedIndex::operator int() const
{
    return curr_index;
}

typedef bool* (*FuncType2)(const char* data, const int data_len, int* r_len);
//def get_data_bits(v, data: List[Union[int, str]], raw = False)->Union[List[int], Tuple[int, int]]:
bool* get_data_bytes(
    const data_el* data, const int data_el_count,
    const int ver, const int full_bits_count, LimitedIndex** r_len
)
{
    /*
    0 (0001) Numeric Mode
    1 (0010) Alphanumeric Mode
    2 (0100) Byte Mode
    3 (1000) Kanji Mode
    4 (0111) ECI Mode
    */
    constexpr bool mode_indicator[5][4]
    {
        {0,0,0,1},
        {0,0,1,0},
        {0,1,0,0},
        {1,0,0,0},
        {0,1,1,1}
    };
    constexpr FuncType2 funcs[]
    {
        func0,
        func1,
        func2,
        func3,
        func4
    };
    bool* bits = new bool[full_bits_count];
    LimitedIndex bits_len(0, full_bits_count);
    for (int i = 0; i < data_el_count; i++)
    {
        data_el curr_data_el = data[i];
        for (; bits_len < 4; bits_len++)
            bits[bits_len] = mode_indicator[curr_data_el.mode][bits_len];
        //bits_len.inc(4);

        const int character_count_indicator_len =
            len_of_character_count_indicator(curr_data_el.mode, ver);

        bool* data_len_bits = new bool[character_count_indicator_len];
        for (int j = 0; j < character_count_indicator_len; j++)
            data_len_bits[j] = get_bit(curr_data_el.val_len, character_count_indicator_len - j - 1);

        for (int j = 0; j < character_count_indicator_len; j++)
            bits[bits_len++] = data_len_bits[j];
        //bits_len.inc(character_count_indicator_len);
        delete[] data_len_bits;

        int sub_bits_len;
        const FuncType2 func = funcs[curr_data_el.mode];
        const bool* sub_bits = func(curr_data_el.val, curr_data_el.val_len, &sub_bits_len);

        for (int j = 0; j < sub_bits_len; j++)
            bits[bits_len++] = sub_bits[j];
        //bits_len.inc(sub_bits_len);
        delete[] sub_bits;
    }
    *r_len = new LimitedIndex(bits_len, full_bits_count);
    return bits;
}

int* bits_to_bytes_arr(const bool* bits, const int bits_len, int* r_len/*= nullptr*/)
{
    if (bits_len % 8) throw ArgumentError("Число битов должно делиться на 8 без остатка!");
    const int bytes_count = (bits_len - 1) / 8 + 1;
    int* bytes = new int[bytes_count];
    for (int i = 0; i < bytes_count; i++)
    {
        bytes[i] = 0;
        for (int j = 0; j < 8; j++)
            if (bits[i * 8 + j])
                bytes[i] += set_bit_to_1_by_indx(7 - j);
    }
    if (r_len) *r_len = bytes_count;
    return bytes;
}

int evaluation_condition_1(const BoolMatrix& matrix, const int border)
{
    int score = 0;
    const int side_min_b = matrix.get_m() - border;

    for (int i = border; i < side_min_b; i++)
    {
        int same_color_n = 1;
        bool color = matrix.getElByIds(i, border);
        for (int j = border + 1; j < side_min_b; j++)
            if (matrix.getElByIds(i, j) != color)
            {
                if (same_color_n >= 5)
                    score += same_color_n - 2;
                same_color_n = 0;
                color = !color;
            }
            else
                same_color_n++;

        if (same_color_n >= 5)
            score += same_color_n - 2;
    }

    for (int j = border; j < side_min_b; j++)
    {
        int same_color_n = 1;
        bool color = matrix.getElByIds(border, j);
        for (int i = border + 1; i < side_min_b; i++)
            if (matrix.getElByIds(i, j) != color)
            {
                if (same_color_n >= 5)
                    score += same_color_n - 2;
                same_color_n = 0;
                color = !color;
            }
            else
                same_color_n++;

        if (same_color_n >= 5)
            score += same_color_n - 2;
    }

    return score;
}
int evaluation_condition_2(const BoolMatrix& matrix, const int border)
{
    int score = 0;
    const int side_min_b = matrix.get_m() - border;
    constexpr int block_side = 2;
    constexpr int score_per_block = 3;
    const int i1 = side_min_b - block_side;
    const int j1 = i1;
    for (int i = border; i < i1; i++)
        for (int j = border; j < j1; j++)
        {
            bool color = matrix.getElByIds(i, j);
            bool flag = true;
            for (int delta_i = 1; delta_i < block_side && flag; delta_i++)
                for (int delta_j = 0; delta_j < block_side && flag; delta_j++)
                    if (matrix.getElByIds(i + delta_i, j + delta_j) != color) flag = false;
            if (flag) score += score_per_block;
        }
    return score;
}
int evaluation_condition_3(const BoolMatrix& matrix, const int border)
{
    int score = 0;
    constexpr int score_per_sub_p = 40;
    constexpr int sub_p_len = 11;
    constexpr bool sub_pattern_1[sub_p_len]
    {
        1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0
    };
    constexpr bool sub_pattern_2[sub_p_len]
    {
        0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1
    };
    const int side_min_b = matrix.get_m() - border;

    const int j1 = side_min_b - sub_p_len;
    for (int i = border; i < side_min_b; i++)
        for (int j = border; j < j1; j++)
        {
            int k;
            for (k = 0; k < sub_p_len; k++)
                if (matrix.getElByIds(i, j + k) != sub_pattern_1[k])
                    break;
            if (k == sub_p_len) score += score_per_sub_p;
            for (k = 0; k < sub_p_len; k++)
                if (matrix.getElByIds(i, j + k) != sub_pattern_2[k])
                    break;
            if (k == sub_p_len) score += score_per_sub_p;
        }

    const int i1 = side_min_b - sub_p_len;
    for (int j = border; j < side_min_b; j++)
        for (int i = border; i < i1; i++)
        {
            int k;
            for (k = 0; k < sub_p_len; k++)
                if (matrix.getElByIds(i + k, j) != sub_pattern_1[k])
                    break;
            if (k == sub_p_len) score += score_per_sub_p;
            for (k = 0; k < sub_p_len; k++)
                if (matrix.getElByIds(i + k, j) != sub_pattern_2[k])
                    break;
            if (k == sub_p_len) score += score_per_sub_p;
        }
    return score;
}
int evaluation_condition_4(const BoolMatrix& matrix, const int border)
{
    const int side = matrix.get_m();
    const int side_min_b = side - border;
    //std::cout << "matrix.get_1s_count() " << matrix.get_1s_count() << std::endl;
    //std::cout << "(side * side) " << (side * side) << std::endl;
    const int dark_modules_modules_percent = matrix.get_1s_count() * 100 / (side * side);
    //std::cout << "dark_modules_modules_percent " << dark_modules_modules_percent << std::endl;
    const int _1_multiple_of_5 = dark_modules_modules_percent - dark_modules_modules_percent % 5;
    const int _2_multiple_of_5 = _1_multiple_of_5 + 5;
    //std::cout << "_1_multiple_of_5 " << _1_multiple_of_5 << std::endl;
    //std::cout << "_2_multiple_of_5 " << _2_multiple_of_5 << std::endl;
    const int _1_absolute_value = abs(_1_multiple_of_5 - 50);
    const int _2_absolute_value = abs(_2_multiple_of_5 - 50);
    //std::cout << "_1_absolute_value " << _1_absolute_value << std::endl;
    //std::cout << "_2_absolute_value " << _2_absolute_value << std::endl;
    const int smallest_number =
        std::min(_1_absolute_value / 5, _2_absolute_value / 5);
    //std::cout << "smallest_number " << smallest_number << std::endl;
    return smallest_number * 10;
}