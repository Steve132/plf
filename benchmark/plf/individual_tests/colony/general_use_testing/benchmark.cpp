#include <iostream>
#include <cstdio> // freopen, sprintf

#include "plf_colony.h"
#include "plf_nanotimer.h"



// Datatypes:

struct small_struct
{
	double *empty_field_1;
	double unused_number;
	unsigned int empty_field2;
	double *empty_field_3;
	double number;
	unsigned int empty_field4;

	small_struct(const unsigned int num): number(num) {};
};



// MATH FUNCTIONS:

// Fast xorshift+128 random number generator function (original: https://codingforspeed.com/using-faster-psudo-random-generator-xorshift/)
unsigned int bench_pcg_rand()
{
	static unsigned int x = 123456789;
	static unsigned int y = 362436069;
	static unsigned int z = 521288629;
	static unsigned int w = 88675123;

	const unsigned int t = x ^ (x << 11);

	// Rotate the static values (w rotation in return statement):
	x = y;
	y = z;
	z = w;

	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}



inline unsigned int fast_mod(const unsigned int input, const unsigned int ceiling) // courtesy of chandler carruth
{
    // apply the modulo operator only when needed
    // (i.e. when the input is greater than the ceiling)
    return (input >= ceiling) ? input % ceiling : input;
}


inline unsigned int rand_within(const unsigned int range)
{
	return fast_mod(bench_pcg_rand(), range);
}






template <class container_type>
void benchmark_general_use_percentage(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const unsigned int erasure_percentage)
{
	assert (number_of_elements > 1);

	const unsigned int total_number_of_insertions = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(erasure_percentage) / 100.0)) + 0.5);
	double total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer full_time;
	full_time.start();

	const unsigned int end = (number_of_runs / 10) + 1;

	// CPU Warmup runs:
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container.insert(bench_pcg_rand() & 255);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= erasure_percentage)
				{
					total += current_element->number;
					++current_element;
				}
				else
				{
					current_element = container.erase(current_element);
				}
			}

			for (unsigned int number_of_insertions = 0; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
			{
				container.insert(bench_pcg_rand() & 255);
			}
		}

		end_memory += container.memory();
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.


	// Actual runs:
	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container.insert(bench_pcg_rand() & 255);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= erasure_percentage)
				{
					total += current_element->number;
					++current_element;
				}
				else
				{
					current_element = container.erase(current_element);
				}
			}

			for (unsigned int number_of_insertions = 0; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
			{
				container.insert(bench_pcg_rand() & 255);
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}




template <class container_type>
void benchmark_range_general_use_percentage(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const unsigned int initial_erasure_percentage, const unsigned int max_erasure_percentage, const unsigned int erasure_addition)
{
	for (unsigned int erasure_percentage = initial_erasure_percentage; erasure_percentage < max_erasure_percentage; erasure_percentage += erasure_addition)
	{
		std::cout << "Erasure percentage: " << erasure_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_general_use_percentage<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, erasure_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}





// Utility functions:

void output_to_csv_file(char *filename)
{
	freopen("errors.log","w", stderr);
	char logfile[512];
	sprintf(logfile, "../../%s.csv", filename);
	std::cout << "Outputting results to logfile " << logfile << "." << std::endl << "Please wait while program completes. This may take a while. Program will close once complete." << std::endl;
	freopen(logfile,"w", stdout);
}





int main(int argc, char **argv)
{
	output_to_csv_file(argv[0]);

	benchmark_range_general_use_percentage< plf::colony<small_struct> >(100, 100000, 10, 3600, 10, 11, 2);

	return 0;
}
