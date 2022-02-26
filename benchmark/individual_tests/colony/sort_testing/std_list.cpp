#include "../../../plf_bench.h"


int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	std::cout << "Char,,,\n";
	benchmark_range_sort< std::list<char> >(100, 1000000, 1.3);
	std::cout << "Int,,,\n";
	benchmark_range_sort< std::list<int> >(100, 1000000, 1.3);
	std::cout << "Double,,,\n";
	benchmark_range_sort< std::list<double> >(100, 1000000, 1.3);
	std::cout << "Small struct,,,\n";
	benchmark_range_sort< std::list<small_struct> >(100, 1000000, 1.3);
	std::cout << "Large struct,,,\n";
	benchmark_range_sort< std::list<large_struct> >(100, 1000000, 1.3);

	std::cout << "Indiesort Char,,,\n";
	benchmark_range_indiesort< std::list<char> >(100, 1000000, 1.3);
	std::cout << "Indiesort Int,,,\n";
	benchmark_range_indiesort< std::list<int> >(100, 1000000, 1.3);
	std::cout << "Indiesort Double,,,\n";
	benchmark_range_indiesort< std::list<double> >(100, 1000000, 1.3);
	std::cout << "Indiesort Small struct,,,\n";
	benchmark_range_indiesort< std::list<small_struct> >(100, 1000000, 1.3);
	std::cout << "Indiesort Large struct,,,\n";
	benchmark_range_indiesort< std::list<large_struct> >(100, 1000000, 1.3);
	return 0;
}
