#include "../../../plf_bench.h"


int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	// Test 1%, 5%, 10% erasure/insertion of container content per frame (assuming 60fps) for half an hour
	benchmark_range_ordered_small_percentage< std::deque<small_struct> >(100, 100000, 1.3, 108000, .00028, 1, 1);
	benchmark_range_ordered_small_percentage< std::deque<small_struct> >(100, 100000, 1.3, 108000, .00139, 1, 1);
	benchmark_range_ordered_small_percentage< std::deque<small_struct> >(100, 100000, 1.3, 108000, .00278, 1, 1);

	// Test 1%, 5%, 10% erasure/insertion of container content per minute of gameplay (assuming 60fps) for half an hour
	benchmark_range_ordered_percentage< std::deque<small_struct> >(100, 100000, 1.3, 3600, 1, 4, 4);
	benchmark_range_ordered_percentage< std::deque<small_struct> >(100, 100000, 1.3, 3600, 5, 7, 4);
	benchmark_range_ordered_percentage< std::deque<small_struct> >(100, 100000, 1.3, 3600, 10, 11, 2);

	// Test 1%, 5%, 10% erasure/insertion of container content per frame (assuming 60fps) for half an hour
	benchmark_range_ordered_small_percentage_remove_if< std::deque<small_struct_bool> >(100, 100000, 1.3, 108000, .00028, 1, 1);
	benchmark_range_ordered_small_percentage_remove_if< std::deque<small_struct_bool> >(100, 100000, 1.3, 108000, .00139, 1, 1);
	benchmark_range_ordered_small_percentage_remove_if< std::deque<small_struct_bool> >(100, 100000, 1.3, 108000, .00278, 1, 1);

	// Test 1%, 5%, 10% erasure/insertion of container content per minute of gameplay (assuming 60fps) for half an hour
	benchmark_range_ordered_percentage_remove_if< std::deque<small_struct_bool> >(100, 100000, 1.3, 3600, 1, 4, 4);
	benchmark_range_ordered_percentage_remove_if< std::deque<small_struct_bool> >(100, 100000, 1.3, 3600, 5, 7, 4);
	benchmark_range_ordered_percentage_remove_if< std::deque<small_struct_bool> >(100, 100000, 1.3, 3600, 10, 11, 2);

	return 0;
}
