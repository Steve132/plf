#include "../../../plf_bench.h"


int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	benchmark_range_unique< std::list<int> >(100, 1000000, 1.2, 2);

	return 0;
}
