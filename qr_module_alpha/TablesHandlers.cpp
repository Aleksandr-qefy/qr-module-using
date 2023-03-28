#include "TablesHandlers.h"

//def get_code_words_numbers(v: int, mask_code: str) -> List[int]:
int* get_code_words_numbers(int v, ErrCorrLevels errCorrLevel) {
    /*
        0: Total_Number_of_Data_Codewords: int \n
        1: ErrCorr_Codewords_Per_Block: int \n
        2: Number_of_Blocks_in_Group_1: int \n
        3: Number_of_Data_Codewords_in_Blocks_of_Group_1: int \n
        4: Number_of_Blocks_in_Group_2: int \n
        5: Number_of_Data_Codewords_in_Blocks_of_Group_2: int \n
        // Ддина возвращаемого массива всегда 6
    */
    if (v < MIN_QR_CODE_VERSION || v > MAX_QR_CODE_VERSION) throw IndexError(v, 1, MAX_QR_CODE_VERSION);
    namespace Table = Databases::ErrorCorrectionTable;
    int indx = (v - 1) * 4;
    switch (errCorrLevel)
    {
        //case QRcode::L:
        //    break;
    case M:
        indx += 1;
        break;
    case Q:
        indx += 2;
        break;
    case H:
        indx += 3;
        break;
    default:
        break;
    }
    int* row = Table::get_row(indx);
    //for (int j = 0; j < Table::countsArr[indx]; j++)
    //    cout << (int)row[j] << " ";
    //cout << endl;
    int* res = new int[6];
    res[4] = res[5] = 0;
    for (int i = 0; i < Table::counts_arr[indx]; i++)
        res[i] = row[i];
    delete[] row;
    return res;
    //if (Table::countsArr[indx] < 6) {
    //    int* res = new int[6];
    //    for (int i = 0; i < 4; i++)
    //        res[i] = row[i];
    //    res[4] = res[5] = 0;
    //    delete[] row;
    //    return res;
    //}
    //return row;
}
//int* row = getCodeWordsNumbers(11, QRcode::M);
//...
//delete[] row;

//def alignment_pattern_locations(ver: int) -> List[int]:
int* alignment_pattern_locations(int ver, int* arrLen/* = nullptr*/) {
    if (ver < 2 || ver > MAX_QR_CODE_VERSION) throw IndexError(ver, 2, MAX_QR_CODE_VERSION);
    namespace Table = Databases::AlignmentPatternLocationsTable;
    if (arrLen) *arrLen = Table::counts_arr[ver - 2];
    return Table::get_row(ver - 2);
}

//def version_patterns_table(ver: int) -> list[list[int]]:
BoolMatrix* version_patterns_table(int ver) {
    if (ver < 7 || ver > MAX_QR_CODE_VERSION) throw IndexError(ver, 7, MAX_QR_CODE_VERSION);
    auto* pattern = new BoolMatrix(3, 6);
    namespace Table = Databases::VersionCodesTable;
    //unsigned long int* row = Table::getRow(ver - 7);
    //unsigned long int decimal = row[0];
    //delete[] row;
    unsigned long int decimal = Table::get_row(ver - 7);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 6; j++) {
            bool bit = decimal & (1UL << (17 - (i * 6 + j)));
            // bool bit = decimal & (1UL << (i * 6 + j));
            pattern->setElByIds(i, j, bit);
        }
    return pattern;
}
//BoolMatrix bm = versionPatternsTable(7);
//bm.show();

//def get_coefficients_of_the_generator_polynomial(n_of_EC_code_words: int) -> List[int]:
int* get_coefficients_of_the_generator_polynomial(int nOfECcodeWords, int* arrLen/* = nullptr*/) {
    namespace Table = Databases::GeneratorPolynomialsTable;
    int indx;
    for (indx = 0; indx < 13; indx++)
        if (Table::counts_arr[indx] == nOfECcodeWords) break;
    if (indx == 13) throw ArgumentError("Нет строки в таблице с таким количеством слов-кодов коррекции ошибок!");
    const int len = Table::counts_arr[indx];
    const auto row = Table::get_row(indx);
    const auto res = new int[len + 1];
    res[0] = 0;
    for (int i = 0; i < len; i++)
        res[i + 1] = row[i];
    if (arrLen) *arrLen = len + 1;
    return res;
}
//int len;
//int* row = getCoefficientsOfTheGeneratorPolynomial(24, &len);
//out::printArr<>(row, len);
//delete[] row;

//def get_remainder_bits(ver: int, fill = False)->List[int]:
int get_remainder_bits_n(const int ver/*, int* arrLen = nullptr, bool fill = false*/) {
    if (ver < MIN_QR_CODE_VERSION || ver > MAX_QR_CODE_VERSION) throw IndexError(ver, 1, MAX_QR_CODE_VERSION);
    namespace Table = Databases::RemainderBitsTable;
    //const int* row = Table::getRow(ver - 1);
    //const int count = row[0];
    //delete[] row;
    ////int* res = new int[count];
    ////for (int i = 0; i < count; i++)
    ////    res[i] = fill;
    ////if (arrLen) *arrLen = count;
    ////return res;
    //return count;
    return Table::get_row(ver - 1);
}
//int len;
//int* row = getRemainderBits(5, &len);
//out::printArr<>(row, len);
//delete[] row;


//def get_character_capacities(ver: int, err_corr_level: str, mode: int) -> int:
int get_character_capacities(int ver, ErrCorrLevels errCorrLevel, int mode) {
    /* mode
        0: 'numeric'
        1: 'alphanumeric'
        2: 'byte'
        3: 'kanji'
    */
    if (ver < MIN_QR_CODE_VERSION || ver > MAX_QR_CODE_VERSION) throw IndexError(ver, 1, MAX_QR_CODE_VERSION);
    if (mode < 0 || mode > 3) throw ArgumentError("Значение параметра mode должно находится на отрезке 0...3 !");
    namespace Table = Databases::CharacterCapacitiesTable;
    int indx = (ver - 1) * 4;
    int* row = Table::get_row(indx);
    int res = row[mode];
    delete[] row;
    return res;
}

//def get_codewords_number(v: int, mask_code: str) -> int:
int get_codewords_number(int ver, ErrCorrLevels errCorrLevel) {
    int* row = get_code_words_numbers(ver, errCorrLevel);
    int res = row[0];
    delete[] row;
    return res;
}

unsigned int get_format_information(const ErrCorrLevels err_corr_level, const int mask_pattern_code)
{
    namespace Table = Databases::FormatInformationTable;
    int indx = mask_pattern_code;
    switch (err_corr_level)
    {
        //case QRcode::L:
        //    break;
    case M:
        indx += 8 * 1;
        break;
    case Q:
        indx += 8 * 2;
        break;
    case H:
        indx += 8 * 3;
        break;
    default:
        break;
    }
    if (indx > Table::rows_count - 1)
        throw ArgumentError("Параметры функции заданы неверно!");
    return Table::get_row(indx);
}
