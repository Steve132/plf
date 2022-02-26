#include "../../plf_bench.h"


int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	benchmark_range_remove< plf::list<int> >(1000, 1000000, 1.2, 2);

	return 0;
}
