#include "QRcode.h"

QRcode::QRcode(const int ver, const ErrCorrLevels err_corr_level) :
    ver(ver),
    side(21 + (ver - 1) * 4),
    mask_pattern_code(NO_PATTERN_BASE), border(0),
    err_corr_level(err_corr_level)
{
    if (ver < MIN_QR_CODE_VERSION || ver > MAX_QR_CODE_VERSION)
        throw ArgumentError("Параметр ver должен находится на отрезке 1...40 !");
    pattern = BoolMatrix(side, side);

    create_blueprint();
}
QRcode::QRcode(const QRcode* qr_code) : ver(qr_code->ver), side(qr_code->side),
    mask_pattern_code(qr_code->mask_pattern_code), border(qr_code->border), err_corr_level(qr_code->err_corr_level),
    pattern(qr_code->pattern),
    blueprint(qr_code->blueprint) {}
QRcode::QRcode(const QRcode& qr_code) : ver(qr_code.ver), side(qr_code.side),
    mask_pattern_code(qr_code.mask_pattern_code), border(qr_code.border),
    err_corr_level(qr_code.err_corr_level), pattern(qr_code.pattern),
    blueprint(qr_code.blueprint) {}
QRcode& QRcode::make_QR_code(const int _mask_pattern_code, const bool _border /*= true*/)
{
    if (_mask_pattern_code < MIN_MASK_PATTERN_CODE || _mask_pattern_code > MAX_MASK_PATTERN_CODE)
        throw ArgumentError("Параметр mask_pattern_code должен находится на отрезке 0...7 !");

    //QRcode* qr_finished = new QRcode(this);
    if (mask_pattern_code == NO_PATTERN_BASE)
    {
        mask_pattern_code = _mask_pattern_code;
        create_pattern_base();
        invert_by_mask_pattern();
    }
    if (_border)
    {
        border = 4 + (ver - 1) / 2 + 1;
        DEV_MSG("qr_base->border " << border)
        const int new_side = side + 2 * border;
        BoolMatrix pattern_with_borders(new_side, new_side);
        pattern_with_borders.join(pattern, border, border);
        pattern = pattern_with_borders;
    }

    return *this;
}
QRcode& QRcode::put_data(const data_el* data, const int data_el_count)
{
    int DC_bits_0_len;
    bool* DC_bits_0 = get_DC_bits(data, data_el_count, &DC_bits_0_len);

    DEV_MSG("$4")
	PRINT_ARR(DC_bits_0, DC_bits_0_len)

    int DC_bytes_0_len;
    int* DC_bytes_0 = bits_to_bytes_arr(DC_bits_0, DC_bits_0_len, &DC_bytes_0_len);
    delete[] DC_bits_0;

    DEV_MSG("$5")
	PRINT_ARR(DC_bytes_0, DC_bytes_0_len)

    int** DC_blocks;
    int* DC_blocks_sizes;
    int blocks_count;
    get_blocks<int>(DC_bytes_0, DC_bytes_0_len, &DC_blocks, &DC_blocks_sizes, &blocks_count);
    DEV_MSG("$6")
    PRINT_BLOCKS(DC_blocks, DC_blocks_sizes, blocks_count)
    delete[] DC_bytes_0;

    //const int* row = get_code_words_numbers(ver, err_corr_level);
    ////const int ErrCorr_Codewords_Per_Block = row[1];
    //delete[] row;

    DEV_MSG("$6-7.1")

    int** EC_blocks = new int* [blocks_count];
    int* EC_blocks_sizes = new int[blocks_count];
    //const int EC_blocks_count = EC_blocks_count;
    for (int i = 0; i < blocks_count; i++)
        EC_blocks[i] =
			generate_EC_codewords(DC_blocks[i], DC_blocks_sizes[i], &(EC_blocks_sizes[i]));

    DEV_MSG("$7")
    PRINT_BLOCKS(EC_blocks, EC_blocks_sizes, blocks_count)

    int DC_bytes_1_len;
    const int* DC_bytes_1 = interleave_blocks(DC_blocks, DC_blocks_sizes, blocks_count, &DC_bytes_1_len);
    for (int i = 0; i < blocks_count; i++)
        delete[] DC_blocks[i];
    delete[] DC_blocks_sizes;
    int EC_bytes_len;
    const int* EC_bytes = interleave_blocks(EC_blocks, EC_blocks_sizes, blocks_count, &EC_bytes_len);
    for (int i = 0; i < blocks_count; i++)
        delete[] EC_blocks[i];
    delete[] EC_blocks_sizes;

    const int code_words_len = DC_bytes_1_len + EC_bytes_len;
    int* code_words = new int[code_words_len];

    for (int i = 0; i < DC_bytes_1_len; i++)
        code_words[i] = DC_bytes_1[i];
    delete[] DC_bytes_1;
    for (int i = 0; i < EC_bytes_len; i++)
        code_words[DC_bytes_1_len + i] = EC_bytes[i];
    delete[] EC_bytes;

    DEV_MSG("code_words")
    PRINT_ARR(code_words, code_words_len)

    int pre_bits_len;
    const int* pre_bits = bytes_to_bits_arr<int>(code_words, code_words_len, &pre_bits_len);
    delete[] code_words;

    const int remainder_bits_n = get_remainder_bits_n(ver);
    const int bits_len = pre_bits_len + remainder_bits_n;
    int* bits = new int[bits_len];
    for (int i = 0; i < pre_bits_len; i++)
        bits[i] = pre_bits[i];
    delete[] pre_bits;
    for (int i = 0; i < remainder_bits_n; i++)
        bits[pre_bits_len + i] = 0;

    DEV_MSG("bits")
    PRINT_ARR(bits, bits_len)
    module_placement(bits, bits_len);
    delete[] bits;

    return *this;
}
QRcode& QRcode::put_data_raw(const data_el* data, const int data_el_count)
{
    const int* row = get_code_words_numbers(ver, err_corr_level);
    const int DC_n = row[0];
    const int EC_n = row[1];
    delete[] row;

    DEV_MSG("DC_n " << DC_n)
	DEV_MSG("EC_n " << EC_n)

    LimitedIndex* len_p = nullptr;
    const int DC_bits_len = DC_n * CHAR_BIT;
    /*bool* bool_row =*/
    try
    {
        delete[] get_data_bytes(data, data_el_count, ver, DC_bits_len, &len_p);
    }
    catch (IndexError&)
    {
        throw NotEnoughSize();
    }
    const LimitedIndex data_len = *len_p;
    const int terminator_len = (DC_bits_len - data_len) % 11 % 6;
    //constexpr int terminator_len = 4;

    DEV_MSG("data_len " << data_len)
	DEV_MSG("terminator_len " << terminator_len)

    //const int remainder_bits_n = getRemainderBitsN(ver);
    //const int  DC_bits_len = DC_n * CHAR_BIT;// + remainder_bits_n;
    DEV_MSG("DC_bits_len " << DC_bits_len)
    bool* DC_bits = new bool[DC_bits_len];
    for (int i = 0; i < data_len; i++)
        DC_bits[i] = true;
    for (int i = data_len; i < DC_bits_len - terminator_len; i++)
        DC_bits[i] = false;
    for (int i = DC_bits_len - terminator_len; i < DC_bits_len; i++)
        DC_bits[i] = true;

    DEV_MSG("DC_bits ")
    PRINT_ARR(DC_bits, DC_bits_len)

    int bytes_count;
    const int* bytes = bits_to_bytes_arr(DC_bits, DC_bits_len, &bytes_count);
    delete[] DC_bits;

    int** DC_blocks;
    int* DC_blocks_sizes;
    int blocks_count;
    get_blocks<int>(bytes, bytes_count, &DC_blocks, &DC_blocks_sizes, &blocks_count);
    delete[] bytes;

    int DC_bytes_len;
    const int* DC_bytes = interleave_blocks(DC_blocks, DC_blocks_sizes, blocks_count, &DC_bytes_len);
    for (int i = 0; i < blocks_count; i++)
        delete[] DC_blocks[i];
    delete[] DC_blocks;
    delete[] DC_blocks_sizes;
    const int EC_bytes_len = EC_n * blocks_count;
    int* EC_bytes = new int[EC_bytes_len];
    for (int i = 0; i < EC_bytes_len; i++)
        EC_bytes[i] = 0b11111111;
    //  Тут выше в строке нет ошибки

    const int code_words_len = DC_bytes_len + EC_bytes_len;
    int* code_words = new int[code_words_len];

    for (int i = 0; i < DC_bytes_len; i++)
        code_words[i] = DC_bytes[i];
    delete[] DC_bytes;
    for (int i = 0; i < EC_bytes_len; i++)
        code_words[DC_bytes_len + i] = EC_bytes[i];
    delete[] EC_bytes;

    DEV_MSG("code_words ")
    PRINT_ARR(code_words, code_words_len)

    int pre_bits_len;
    const int* pre_bits = bytes_to_bits_arr<int>(code_words, code_words_len, &pre_bits_len);
    delete[] code_words;

    const int remainder_bits_n = get_remainder_bits_n(ver);
    const int bits_len = pre_bits_len + remainder_bits_n;
    int* bits = new int[bits_len];
    for (int i = 0; i < pre_bits_len; i++)
        bits[i] = pre_bits[i];
    delete[] pre_bits;
    for (int i = 0; i < remainder_bits_n; i++)
        bits[pre_bits_len + i] = 1;

    DEV_MSG("bits_len " << bits_len)
	DEV_MSG("bits ")
    PRINT_ARR(bits, bits_len)
    module_placement(bits, bits_len);
    pattern.join(blueprint, 0, 0, true);
    delete[] bits;

    return *this;
}
Matrix QRcode::get_bits_order_matrix(const data_el* data, const int data_el_count, const unsigned short first_mut_bit_val/* = 0*/)
{
    const int* row = get_code_words_numbers(ver, err_corr_level);
    const int DC_n = row[0];
    const int EC_n = row[1];
    delete[] row;

    DEV_MSG("DC_n " << DC_n)
	DEV_MSG("EC_n " << EC_n)

    LimitedIndex* len_p = nullptr;
    const int DC_bits_len = DC_n * CHAR_BIT;
    /*bool* bool_row =*/
    try
    {
        delete[] get_data_bytes(data, data_el_count, ver, DC_bits_len, &len_p);;
    }
    catch (IndexError&)
    {
        throw NotEnoughSize();
    }
    const LimitedIndex data_len = *len_p;
    const int terminator_len = (DC_bits_len - data_len) % 11 % 6;
    //constexpr int terminator_len = 4;

    DEV_MSG("data_len " << data_len)
	DEV_MSG("terminator_len " << terminator_len)

    //const int remainder_bits_n = getRemainderBitsN(ver);
    //const int  DC_bits_len = DC_n * CHAR_BIT;// + remainder_bits_n;
    int* DC_bits = new int[DC_bits_len];
    for (int i = 0; i < data_len; i++)
        DC_bits[i] = IMMUTABLE_BIT_VAL;
    int k = first_mut_bit_val;
    for (int i = data_len; i < DC_bits_len - terminator_len; i++)
    {
        DC_bits[i] = k;
        k++;
    }
    for (int i = DC_bits_len - terminator_len; i < DC_bits_len; i++)
        DC_bits[i] = IMMUTABLE_BIT_VAL;

    DEV_MSG("DC_bits_len " << DC_bits_len)
	DEV_MSG("DC_bits ")
    PRINT_ARR(DC_bits, DC_bits_len)

    int** DC_bytes_0 = new int* [DC_n];
    for (int i = 0; i < DC_n; i++)
    {
        DC_bytes_0[i] = new int[CHAR_BIT];
        for (int j = 0; j < CHAR_BIT; j++)
            DC_bytes_0[i][j] = DC_bits[i * CHAR_BIT + j];
    }
    delete[] DC_bits;

    DEV_MSG("DC_bytes_0 ")
    PRINT_MATRIX(DC_bytes_0, DC_n, CHAR_BIT)

    int*** DC_blocks;
    int* DC_blocks_sizes;
    int blocks_count;
    get_blocks<int*>(DC_bytes_0, DC_n, &DC_blocks, &DC_blocks_sizes, &blocks_count);

    //std::cout << "DC_blocks " << std::endl;
    //for (int i = 0; i < blocks_count; i++)
    //{
	   // std::cout << "block " << i + 1 << " size " << DC_blocks_sizes[i] << std::endl;
    //    for (int j = 0; j < DC_blocks_sizes[i]; j++)
    //        PRINT_ARR(DC_blocks[i][j], CHAR_BIT)
    //}

    int DC_bytes_len;
    int** DC_bytes = interleave_blocks(DC_blocks, DC_blocks_sizes, blocks_count, &DC_bytes_len);

    DEV_MSG("DC_bytes ")
    PRINT_MATRIX(DC_bytes, DC_bytes_len, CHAR_BIT)

    const int EC_bytes_len = EC_n * blocks_count;
    int** EC_bytes = new int* [EC_bytes_len];
    for (int i = 0; i < EC_bytes_len; i++)
    {
        EC_bytes[i] = new int[CHAR_BIT];
        for (int j = 0; j < CHAR_BIT; j++)
            EC_bytes[i][j] = IMMUTABLE_BIT_VAL;
    }

    DEV_MSG("EC_bytes ")
	PRINT_MATRIX(EC_bytes, EC_bytes_len, CHAR_BIT)
    //---------------------------------------------------------------------------

    const int code_words_len = DC_bytes_len + EC_bytes_len;
    int** code_words = new int* [code_words_len];
    for (int i = 0; i < code_words_len; i++)
        code_words[i] = new int[CHAR_BIT];

    for (int i = 0; i < DC_bytes_len; i++)
        code_words[i] = DC_bytes[i];

    for (int i = 0; i < EC_bytes_len; i++)
        code_words[DC_bytes_len + i] = EC_bytes[i];

    DEV_MSG("code_words ")
    PRINT_MATRIX(code_words, code_words_len, CHAR_BIT)

    //int pre_bits_len;
    //int** pre_bits = bytes_to_bits_arr<int*>(code_words, code_words_len, &pre_bits_len);

    const int remainder_bits_n = get_remainder_bits_n(ver);
    //const int bits_len = pre_bits_len + remainder_bits_n;
    //int* bits = new int[bits_len];
    //for (int i = 0; i < pre_bits_len; i++)
    //    bits[i] = pre_bits[i];
    //for (int i = 0; i < remainder_bits_n; i++)
    //    bits[pre_bits_len + i] = 1;

    const int code_words_bits_len = code_words_len * CHAR_BIT;
    const int bits_len = code_words_bits_len + remainder_bits_n;
    int* bits = new int[bits_len];
    for (int i = 0; i < code_words_len; i++)
        for (int j = 0; j < CHAR_BIT; j++)
            bits[i * CHAR_BIT + j] = code_words[i][j];
    for (int i = 0; i < code_words_len; i++)
        delete[] code_words[i];
	delete[] code_words;
    for (int i = 0; i < remainder_bits_n; i++)
        bits[code_words_bits_len + i] = IMMUTABLE_BIT_VAL;
    DEV_MSG("bits_len " << bits_len)
    DEV_MSG("bits ")
    PRINT_ARR(bits, bits_len)

    //unsigned __int16** matrix = new unsigned __int16* [side];
    //for (int i = 0; i < side; i++)
    //    matrix[i] = new unsigned __int16[side];


    //cv::Mat cv2_mat(side, side, CV_16U);
    Matrix matrix(side, side);
    module_placement(matrix, bits, bits_len);
    delete[] bits;
    for (int i = 0; i < side; i++)
        for (int j = 0; j < side; j++)
            if (blueprint.getElByIds(i, j))
                matrix(i, j) = IMMUTABLE_BIT_VAL;
    //pattern.join(blueprint, 0, 0, true);

    //for (int i = 0; i < DC_n; i++)
    //    delete[] DC_bytes_0[i];
    delete[] DC_bytes_0;
    //for (int i = 0; i < blocks_count; i++)
    //    for (int j = 0; j < DC_blocks_sizes[i]; j++)
    //        delete[] DC_blocks[i][j];
    delete[] DC_blocks;
    delete[] DC_blocks_sizes;
    delete[] DC_bytes;
    //for (int i = 0; i < EC_bytes_len; i++)
    //    delete[] EC_bytes[i];
    delete[] EC_bytes;

    return matrix;
}
#if defined(DEV)
void QRcode::show(const char* wind_name/* = "QR Code (No base)"*/, const int coeff/* = 5*/) const
{
    pattern.show(wind_name, coeff);
}
#endif
int QRcode::get_side() const
{
    return side;
}
int QRcode::get_border() const
{
    return border;
}
//QRcode::operator unsigned __int16** () const
//{
//    const int m = pattern.get_m() + border * 2;
//    const int n = pattern.get_n() + border * 2;
//    unsigned __int16** matrix = new unsigned __int16* [m];
//    for (int i = 0; i < m; i++)
//    {
//        matrix[i] = new unsigned short[n];
//        for (int j = 0; j < n; j++)
//            if (pattern.getElByIds(i, j))
//                matrix[i][j] = IMMUTABLE_BIT_VAL;
//            else
//                matrix[i][j] = 0;
//    }
//    return matrix;
//}
//QRcode::operator cv::Mat() const
//{
//    const int m = pattern.get_m();
//    const int n = pattern.get_n();
//    cv::Mat cv2_mat(m, n, CV_16U);
//    for (int i = 0; i < m; i++)
//        for (int j = 0; j < n; j++)
//            if (pattern.getElByIds(i, j))
//                cv2_mat.at<unsigned __int16>(i, j) = IMMUTABLE_BIT_VAL;
//            else
//                cv2_mat.at<unsigned __int16>(i, j) = 0;
//    return cv2_mat;
//}
QRcode::operator Matrix() const
{
	const int m = pattern.get_m();
    const int n = pattern.get_n();
    Matrix matrix(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (pattern.getElByIds(i, j))
                matrix(i, j) = IMMUTABLE_BIT_VAL;
            else
                matrix(i, j) = 0;
    return matrix;
}
int QRcode::obtain_penalty_score() const
{
    int score = 0;
    score += evaluation_condition_1(pattern, border);
    score += evaluation_condition_2(pattern, border);
    score += evaluation_condition_3(pattern, border);
    score += evaluation_condition_4(pattern, border);
    return score;
}

bool* QRcode::get_DC_bits(const data_el* data, const int data_el_count, int* r_len/* = nullptr*/) const
{
    const int* row = get_code_words_numbers(ver, err_corr_level);
    const int DC_n = row[0];
    //const int EC_n = row[1];
    delete[] row;

    LimitedIndex* len_p = nullptr;
    const int DC_bits_n = DC_n * 8;
    bool* bool_row;
	try
    {
        bool_row = get_data_bytes(data, data_el_count, ver, DC_bits_n, &len_p);
    }
    catch (IndexError&)
    {
        throw NotEnoughSize();
    }
    LimitedIndex bool_row_len = *len_p;
    DEV_MSG("$1 ")
    PRINT_ARR(bool_row, bool_row_len)

    const int _4_bits_n = (DC_bits_n - bool_row_len < 4) ? DC_bits_n - bool_row_len: 4;
    for (int i = 0; i < _4_bits_n; i++)
        bool_row[bool_row_len++] = false;
    //bool_row_len.inc(_4_bits_n);
    DEV_MSG("$2 ")
    PRINT_ARR(bool_row, bool_row_len)

    while (bool_row_len % 8)
    {
        bool_row[bool_row_len++] = false;
    }
    DEV_MSG("$3 ")
    PRINT_ARR(bool_row, bool_row_len)

    const int k = (DC_bits_n - bool_row_len - 1) / 8 + 1;
    DEV_MSG("DC_bits_n " << DC_bits_n)
    constexpr bool _236_bits[8]{ 1,1,1,0,1,1,0,0 };
    constexpr bool  _17_bits[8]{ 0,0,0,1,0,0,0,1 };
    for (int i = 0; i < k; i++)
    {
        if (i % 2)
        {
            for (int j = 0; j < 8; j++)
                bool_row[bool_row_len++] = _17_bits[j];
            //bool_row_len.inc(8);
        }
        else
        {
            for (int j = 0; j < 8; j++)
                bool_row[bool_row_len++] = _236_bits[j];
            //bool_row_len.inc(8);
        }
    }
    //DEV_MSG("bool_row_len " << bool_row_len)
    DEV_MSG("bool_row")
    PRINT_ARR(bool_row, DC_bits_n);
    if (r_len) *r_len = DC_bits_n;
    return bool_row;
    //delete[] bool_row;
}
//unsigned short** QRcode::init_matrix(const int m, const int n)
//{
//    unsigned short** matrix = new unsigned short* [m];
//    for (int i = 0; i < m; i++)
//        matrix[i] = new unsigned short[n] {};
//    return matrix;
//}
void QRcode::horizontal_line(BoolMatrix& matrix, int row, int col_b, int col_e, int interval/* = 1*/, bool fill/* = true*/)
{
    if (interval < 1) throw ArgumentError("Параметр interval должен быть больше 0!");
    if (col_b > col_b) throw ArgumentError("Параметр col_b больше col_b, должно быть наоборот!");
    const int m = matrix.get_m();
    const int n = matrix.get_n();
    if (row < 0 || row >= m)
        throw RowIndexError(row, m - 1);
    if (col_b < 0 || col_b >= n)
        throw ColIndexError(col_b, n - 1);
    if (col_b < 0 || col_b >= n)
        throw ColIndexError(col_b, n - 1);
    for (int col = col_b; col <= col_e; col += interval)
        matrix.setElByIds(row, col, fill);
}

void QRcode::vertical_line(BoolMatrix& matrix, int col, int row_b, int row_e, int interval/* = 1*/, bool fill/* = true*/)
{
    if (interval < 1) throw ArgumentError("Параметр interval должен быть больше 0!");
    if (row_b > row_e) throw ArgumentError("Параметр row_b больше row_e, должно быть наоборот!");
    const int m = matrix.get_m();
    const int n = matrix.get_n();
    if (col < 0 || col >= n)
        throw ColIndexError(col, n - 1);
    if (row_b < 0 || row_b >= n)
        throw RowIndexError(row_b, m - 1);
    if (row_e < 0 || row_e >= n)
        throw RowIndexError(row_e, m - 1);
    for (int row = row_b; row <= row_e; row += interval)
        matrix.setElByIds(row, col, fill);
}
//def add_alignment_pattern(self, center: Tuple[int, int]) :
void QRcode::add_alignment_pattern(const int center_y, const int center_x)
{
    const int x0 = center_x - 3;
    const int y0 = center_y - 3;
    //const auto arr = new int[] {
    //    1, 1, 1, 1, 1,
    //	1, 0, 0, 0, 1,
    //	1, 0, 1, 0, 1,
    //	1, 0, 0, 0, 1,
    //	1, 1, 1, 1, 1
    //};
    //const BoolMatrix alignment_pattern(arr, 25, 5, 5);
    BoolMatrix alignment_pattern(7, 7, true);
    alignment_pattern.join(BoolMatrix(5, 5), 1, 1);
    alignment_pattern.join(BoolMatrix(3, 3, true), 2, 2);
    pattern.join(alignment_pattern, x0, y0);
}

void QRcode::add_points(BoolMatrix& matrix, const BoolMatrix& point_obj)
{
    int len;
    const int* pattern_locations = alignment_pattern_locations(ver, &len);
    const int max_coord = len - 1;
    for (int i = 0; i < len; i++)
        for (int j = 0; j < len; j++)
        {
            const int x = pattern_locations[i];
            const int y = pattern_locations[j];
            if (
                (y == pattern_locations[0] && x == pattern_locations[0]) ||
                (y == pattern_locations[0] && x == pattern_locations[max_coord]) ||
                (y == pattern_locations[max_coord] && x == pattern_locations[0])
                ) continue;
            matrix.join(point_obj, y - 2, x - 2);
        }
}

//def _create_pattern_blueprint(self)->List[List[int]]:
void QRcode::create_blueprint() {
    blueprint = BoolMatrix(side, side);
    if (ver > 1) {
        add_points(blueprint, BoolMatrix(5, 5, true));
    }
    blueprint.join(BoolMatrix(9, 9, true), 0, 0);
    blueprint.join(BoolMatrix(9, 8, true), 0, side - 8);
    blueprint.join(BoolMatrix(8, 9, true), side - 8, 0);
    horizontal_line(blueprint, 6, 8, side - 8);
    vertical_line(blueprint, 6, 8, side - 8);

    if (ver < 7) return;

    blueprint.join(BoolMatrix(3, 6, true), side - 11, 0);
    blueprint.join(BoolMatrix(6, 3, true), 0, side - 11);
}
void QRcode::project_on_matrix(const BoolMatrix& bool_matrix, unsigned short** matrix, const int m, const int  n, unsigned short projecting_val)
{
    if (bool_matrix.get_m() != m || bool_matrix.get_n() != n)
        throw ArgumentError("Несовпадение размеров bool_matrix и matrix!");

    for (int i = 0; i < bool_matrix.get_m(); i++)
        for (int j = 0; j < bool_matrix.get_n(); j++)
            if (bool_matrix.getElByIds(i, j))
                matrix[i][j] = projecting_val;
}
void QRcode::module_placement(const int* bits, const int len)
{
    int k = 0;
    bool upward_move = true;
    int start_j = side - 1;
    for (;;)
    {
        if (upward_move)
        {
            for (int i = side - 1; i >= 0; i--)
                for (int j = start_j; j >= start_j - 1; j--)
                {
                    if (!blueprint.getElByIds(i, j))
                    {
                        pattern.setElByIds(i, j, bits[k]);
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
                        pattern.setElByIds(i, j, bits[k]);
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
int* QRcode::generate_EC_codewords(
    const int* _message_polynomial_coefficients,
    const int _mes_pol_c_len,
    int* r_len/* = nullptr*/
) const
{
    const int* row = get_code_words_numbers(ver, err_corr_level);
    const int EC_n = row[1];
    //cout << "EC_n " << EC_n << endl;
    delete[] row;

    int generator_len;
    const int* generator_polynomial_coefficients_c =
        get_coefficients_of_the_generator_polynomial(EC_n, &generator_len);

    //const int* generator_polynomial_coefficients1 = 
    //    getCoefficientsOfTheGeneratorPolynomial(EC_n, &generator_len);
    //out::printArr(generator_polynomial_coefficients1, generator_len);
    //int* generator_polynomial_coefficients_c = new int[generator_len + 1];
    //generator_polynomial_coefficients_c[0] = 0;
    //for (int i = 1; i < _mes_pol_c_len; i++)
    //    generator_polynomial_coefficients_c[i] = generator_polynomial_coefficients1[i - 1];

    const int message_len = _mes_pol_c_len + EC_n;
    int* message_polynomial_coefficients = new int[message_len];

    for (int i = 0; i < _mes_pol_c_len; i++)
        message_polynomial_coefficients[i] = _message_polynomial_coefficients[i];
    for (int i = _mes_pol_c_len; i < message_len; i++)
        message_polynomial_coefficients[i] = 0;

    //out::printArr(generator_polynomial_coefficients_c, generator_len);
    for (int lead_term_idx = 0; lead_term_idx < _mes_pol_c_len; lead_term_idx++)
    {
        //cout << lead_term_idx << "-$1 ";
        //out::printArr(message_polynomial_coefficients, message_len);
        if (!message_polynomial_coefficients[lead_term_idx])
            continue;
        const int lead_term_of_the_message_polynomial = int_to_exp_of_2(message_polynomial_coefficients[lead_term_idx]);
        //cout << lead_term_idx << "-$2 " << lead_term_of_the_message_polynomial << endl;
        int* generator_polynomial_coefficients = new int[generator_len];
        for (int idx = 0; idx < generator_len; idx++)
        {
            int byte = generator_polynomial_coefficients_c[idx];
            byte += lead_term_of_the_message_polynomial;
            byte %= 255;
            generator_polynomial_coefficients[idx] = byte;
        }
        //cout << lead_term_idx << "-$3 ";
        //out::printArr(generator_polynomial_coefficients, generator_len);

        for (int idx = 0; idx < generator_len; idx++)
        {
            message_polynomial_coefficients[lead_term_idx + idx] ^= exp_of_2_to_int(generator_polynomial_coefficients[idx]);
        }
        delete[] generator_polynomial_coefficients;
    }
    //message_polynomial_coefficients = message_polynomial_coefficients[-EC_n:];
    const auto res = new int[EC_n];
    for (int i = 0; i < EC_n; i++)
        res[i] = message_polynomial_coefficients[_mes_pol_c_len + i];

    if (r_len)
        *r_len = EC_n;

    //delete[] generator_polynomial_coefficients1;
    delete[] generator_polynomial_coefficients_c;
    delete[] message_polynomial_coefficients;

    return res;
}

void QRcode::create_pattern_base()
{
    add_alignment_pattern(3, 3);
    add_alignment_pattern(side - 4, 3);
    add_alignment_pattern(3, side - 4);
    //const auto arr = new int[] {
    //    1, 1, 1, 1, 1,
    //    1, 0, 0, 0, 1,
    //    1, 0, 1, 0, 1,
    //    1, 0, 0, 0, 1,
    //    1, 1, 1, 1, 1
    //};
    //add_points(pattern, BoolMatrix(arr, 25, 5, 5));
    BoolMatrix point_pattern(5, 5, true);
    point_pattern.join(BoolMatrix(3, 3), 1, 1);
    point_pattern.setElByIds(2, 2, true);
    if (ver > 1) add_points(pattern, point_pattern);
    horizontal_line(pattern, 6, 8, side - 8, 2);
    vertical_line(pattern, 6, 8, side - 8, 2);
    pattern.setElByIds(side - 8, 8, true);
    add_format_info();
    if (ver >= 7)
    {
        const BoolMatrix version_pattern = *version_patterns_table(ver);
        pattern.join(version_pattern, side - 11, 0);
        pattern.join(transpose(version_pattern), 0, side - 11);
    }
}
void QRcode::invert_by_mask_pattern()
{
    const FuncType1 func = get_formula(mask_pattern_code);
    for (int i = 0; i < side; i++)
        for (int j = 0; j < side; j++)
            if (func(i, j) && !blueprint.getElByIds(i, j))
                pattern.setElByIds(i, j, !pattern.getElByIds(i, j));
}
void QRcode::add_format_info()
{
    const auto bits_arr = new int[15];
    const unsigned int type_information = get_format_information(err_corr_level, mask_pattern_code);
    constexpr unsigned int e = 1;

    for (int i = 0; i < 15; i++)
        bits_arr[i] = (type_information & (e << i)) > 0;
    //bits_arr[i] = i % 2;
//out::printArr(bits_arr, 15);

// Left Top Point
    pattern.join(BoolMatrix(bits_arr, 15, 6, 1, 0, false, 6), 0, 8);
    pattern.join(BoolMatrix(bits_arr, 15, 2, 1, 6, false, 2), 7, 8);
    pattern.setElByIds(8, 7, bits_arr[8]);
    //transpose(BoolMatrix(9, 9)BoolMatrix(bits_arr, 15, 6, 1, 9))
    //BoolMatrix(bits_arr, 15, 1, 6, 9, true).show();
    pattern.join(BoolMatrix(bits_arr, 15, 1, 6, 9, true), 8, 0);
    // Right Top Point
    //BoolMatrix(bits_arr, 15, 1, 8, 0, true, 8).show();
    pattern.join(BoolMatrix(bits_arr, 15, 1, 8, 0, true, 8), 8, side - 8);
    // Left Bottom Point
    pattern.join(BoolMatrix(bits_arr, 15, 7, 1, 8), side - 7, 8);
}

