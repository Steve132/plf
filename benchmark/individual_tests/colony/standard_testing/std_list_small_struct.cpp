
#include "../../../plf_bench.h"


int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	benchmark_range< std::list<small_struct> >(10, 1000000, 10, 25, true);
	benchmark_range< std::list<small_struct> >(10, 1000000, 1.1, 25, true);

	return 0;
}
