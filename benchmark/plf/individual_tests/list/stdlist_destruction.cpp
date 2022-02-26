#include "../../plf_bench.h"


int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	benchmark_range_destruction< std::list<int> >(10, 1000000, 1.1, 5);

	return 0;
}
