//#pragma once
#ifndef DATABASES_H// include guard
#define DATABASES_H
namespace Databases {
	//#ifndef DATABASES_H
	//#define DATABASES_H
	namespace AlignmentPatternLocationsTable {
		extern const int rows_count;

		extern const int counts_arr[];

		//unsigned char** init();

		int* get_row(int idx);
	}
	//#endif
	namespace CharacterCapacitiesTable {
		extern const int values_per_row;

		extern const int rows_count;

		//char16_t** init();

		int* get_row(int idx);
	}
	namespace ErrorCorrectionTable {
		extern const int rows_count;

		extern const int counts_arr[];

		//char16_t** init();

		int* get_row(int idx);
	}
	namespace GeneratorPolynomialsTable {
		extern const int rows_count;

		extern const int counts_arr[];

		//char16_t** init();

		int* get_row(int idx);
	}
	namespace RemainderBitsTable {
		extern const int rows_count;

		//char16_t** init();

		int get_row(int idx);
	}
	namespace VersionCodesTable {
		extern const int rows_count;

		//unsigned long int** init();

		unsigned long int get_row(int idx);
	}
	namespace FormatInformationTable
	{
		extern const int rows_count;

		unsigned int get_row(const int idx);
	}
	//namespace LogAntilogTable
	//{
	//	extern const unsigned short values_arr[];

	//	/*unsigned int getRow(int idx);*/
	//}
}
//#include "Databases.cpp"

//#include "AlignmentPatternLocationsTable.cpp"
//#include "CharacterCapacitiesTable.cpp"
//#include "ErrorCorrectionTable.cpp"
//#include "GeneratorPolynomialsTable.cpp"
//#include "RemainderBitsTable.cpp"
//#include "VersionCodesTable.cpp"
//#include "FormatInformationTable.cpp"
#endif 