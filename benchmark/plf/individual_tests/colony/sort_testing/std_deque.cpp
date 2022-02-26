#include "../../../plf_bench.h"


int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	std::cout << "Char,,,\n";
	benchmark_range_sort< std::deque<char> >(100, 1000000, 1.3);
	std::cout << "Int,,,\n";
	benchmark_range_sort< std::deque<int> >(100, 1000000, 1.3);
	std::cout << "Double,,,\n";
	benchmark_range_sort< std::deque<double> >(100, 1000000, 1.3);
	std::cout << "Small struct,,,\n";
	benchmark_range_sort< std::deque<small_struct> >(100, 1000000, 1.3);
	std::cout << "Large struct,,,\n";
	benchmark_range_sort< std::deque<large_struct> >(100, 1000000, 1.3);

	std::cout << "Indiesort Char,,,\n";
	benchmark_range_indiesort< std::deque<char> >(100, 1000000, 1.3);
	std::cout << "Indiesort Int,,,\n";
	benchmark_range_indiesort< std::deque<int> >(100, 1000000, 1.3);
	std::cout << "Indiesort Double,,,\n";
	benchmark_range_indiesort< std::deque<double> >(100, 1000000, 1.3);
	std::cout << "Indiesort Small struct,,,\n";
	benchmark_range_indiesort< std::deque<small_struct> >(100, 1000000, 1.3);
	std::cout << "Indiesort Large struct,,,\n";
	benchmark_range_indiesort< std::deque<large_struct> >(100, 1000000, 1.3);

	return 0;
}
