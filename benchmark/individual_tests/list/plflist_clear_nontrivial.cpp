#include "../../../plf_bench.h"


int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	benchmark_range_clear< plf::list<small_struct_non_trivial> >(100, 1000000, 1.2, 5);

	return 0;
}
