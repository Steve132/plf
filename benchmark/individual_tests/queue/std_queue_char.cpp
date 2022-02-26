#include "../../plf_bench.h"



int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	benchmark_range_queue< std::queue<unsigned char> >(10, 1000000, 1.1, true);
	benchmark_range_queue_pump< std::queue<unsigned char> >(10, 1000000, 1.75, 1.1, true);

	return 0;
}
