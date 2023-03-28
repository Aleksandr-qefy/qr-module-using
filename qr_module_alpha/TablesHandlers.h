#pragma once

#include "Errors.h"
#include "Constants.h"
#include "BoolMatrix.h"
//#include "QRcode.h"
#include "Databases.h"

//def get_code_words_numbers(v: int, mask_code: str) -> List[int]:
int* get_code_words_numbers(int v, ErrCorrLevels errCorrLevel);

//def alignment_pattern_locations(ver: int) -> List[int]:
extern  int* alignment_pattern_locations(int ver, int* arrLen = nullptr);

//def version_patterns_table(ver: int) -> list[list[int]]:
extern  BoolMatrix* version_patterns_table(int ver);

//def get_coefficients_of_the_generator_polynomial(n_of_EC_code_words: int) -> List[int]:
extern int* get_coefficients_of_the_generator_polynomial(int nOfECcodeWords, int* arrLen = nullptr);

//def get_remainder_bits(ver: int, fill = False)->List[int]:
extern int get_remainder_bits_n(const int ver/*, int* arrLen = nullptr, bool fill = false*/);

//def get_character_capacities(ver: int, err_corr_level: str, mode: int) -> int:
extern int get_character_capacities(int ver, ErrCorrLevels errCorrLevel, int mode);

//def get_codewords_number(v: int, mask_code: str) -> int:
extern int get_codewords_number(int ver, ErrCorrLevels errCorrLevel);

extern unsigned int get_format_information(const ErrCorrLevels err_corr_level, const int mask_pattern_code);
