#include "../../../plf_bench.h"


int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	benchmark_range< std::deque<double> >(10, 1000000, 1.1, 25, true);
	benchmark_range_remove_if_standard< std::deque<double> >(10, 1000000, 1.1, 25, true);

	return 0;
}
