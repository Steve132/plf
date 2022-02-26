#ifndef PLF_BENCH_H
#define PLF_BENCH_H

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <algorithm> // sort, remove_if, etc
#include <cstdio> // freopen, sprintf
#include <limits> // std::numeric_limits

#include "plf_indiesort.h"
#include "plf_colony.h"
#include "plf_rand.h"
#include "plf_stack.h"
#include "plf_queue.h"
#include "plf_list.h"
#include "plf_nanotimer.h"
#include "plf_indexed_vector.h"
#include "plf_pointer_deque.h"
#include "plf_packed_deque.h"
#include "plf_pointer_colony.h"


// Defines:

#if defined(_MSC_VER) && !defined(__clang__) && !defined(__GNUC__)
	#define PLF_BENCH_FORCE_INLINE inline __forceinline

	#if _MSC_VER >= 1600
		#define PLF_BENCH_MOVE_SEMANTICS_SUPPORT
	#endif
	#if _MSC_VER >= 1900
		#define PLF_BENCH_NOEXCEPT noexcept
	#else
		#define PLF_BENCH_NOEXCEPT throw()
	#endif
#elif defined(__cplusplus) && __cplusplus >= 201103L // C++11 support, at least
	#define PLF_BENCH_FORCE_INLINE inline __attribute__((always_inline))

	#if defined(__GNUC__) && defined(__GNUC_MINOR__) && !defined(__clang__) // If compiler is GCC/G++
		#if (__GNUC__ == 4 && __GNUC_MINOR__ >= 3) || __GNUC__ > 4 // 4.2 and below do not support variadic templates
			#define PLF_BENCH_MOVE_SEMANTICS_SUPPORT
		#endif
		#if (__GNUC__ == 4 && __GNUC_MINOR__ >= 6) || __GNUC__ > 4
			#define PLF_BENCH_NOEXCEPT noexcept
		#else
			#define PLF_BENCH_NOEXCEPT throw()
		#endif
	#elif defined(__clang__) && !defined(__GLIBCXX__) && !defined(_LIBCPP_CXX03_LANG)
		#if __clang_major__ >= 3 // clang versions < 3 don't support __has_feature() or traits
			#if __has_feature(cxx_noexcept)
				#define PLF_BENCH_NOEXCEPT noexcept
				#define PLF_BENCH_IS_ALWAYS_EQUAL_SUPPORT
			#else
				#define PLF_BENCH_NOEXCEPT throw()
			#endif
			#if __has_feature(cxx_rvalue_references) && !defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES)
				#define PLF_BENCH_MOVE_SEMANTICS_SUPPORT
			#endif
		#endif
	#elif defined(__GLIBCXX__) // Using another compiler type with libstdc++ - we are assuming full c++11 compliance for compiler - which may not be true
		#if __GLIBCXX__ >= 20080606
			#define PLF_BENCH_MOVE_SEMANTICS_SUPPORT
		#endif
		#if __GLIBCXX__ >= 20120322
			#define PLF_BENCH_NOEXCEPT noexcept
		#else
			#define PLF_BENCH_NOEXCEPT throw()
		#endif
	#elif defined(_LIBCPP_CXX03_LANG) || defined(_LIBCPP_HAS_NO_RVALUE_REFERENCES) // Special case for checking C++11 support with libCPP
		#define PLF_BENCH_NOEXCEPT throw()
	#else // Assume type traits and initializer support for other compilers and standard library implementations
		#define PLF_BENCH_MOVE_SEMANTICS_SUPPORT
		#define PLF_BENCH_NOEXCEPT noexcept
	#endif
#else
	#define PLF_BENCH_NOEXCEPT throw()
#endif



// Datatypes:

struct char_bool
{
	unsigned char number;
	bool erased;
	char_bool(const unsigned char num) PLF_BENCH_NOEXCEPT: number(num), erased(false) {};
};



struct int_bool
{
	unsigned int number;
	bool erased;
	int_bool(const unsigned int num) PLF_BENCH_NOEXCEPT: number(num), erased(false) {};
};



struct double_bool
{
	double number;
	bool erased;
	double_bool(const double  num) PLF_BENCH_NOEXCEPT: number(num), erased(false) {};
};



struct small_struct
{
	double *empty_field_1;
	double unused_number;
	unsigned int empty_field2;
	double *empty_field_3;
	double number;
	unsigned int empty_field4;

	// This function is required for testing std::multiset:
	inline bool operator < (const small_struct &rh) const
	{
		return number < rh.number;
	}

	small_struct(const unsigned int num) PLF_BENCH_NOEXCEPT: number(num) {};
};


unsigned int generic_global =  0;

struct small_struct_non_trivial
{
	double *empty_field_1;
	double unused_number;
	unsigned int empty_field2;
	double *empty_field_3;
	double number;
	unsigned int empty_field4;

	// This function is required for testing std::multiset:
	inline bool operator < (const small_struct_non_trivial &rh) const
	{
		return number < rh.number;
	}

	small_struct_non_trivial(const unsigned int num) PLF_BENCH_NOEXCEPT: number(num) {}

	~small_struct_non_trivial()
	{
		++generic_global;
	}
};



struct small_struct_bool
{
	double *empty_field_1;
	double unused_number;
	unsigned int empty_field2;
	double *empty_field_3;
	double number;
	unsigned int empty_field4;
	bool erased;

	// This function is required for testing std::multiset:
	inline bool operator < (const small_struct &rh) const
	{
		return number < rh.number;
	}

	small_struct_bool(const unsigned int num) PLF_BENCH_NOEXCEPT: number(num), erased(false) {};
};



struct medium1_struct
{
	int numbers[25];
	char a_string[12];
	double unused_number;
	double number;
	double *empty_field_1;
	double *empty_field_2;
	unsigned int empty_field3;
	unsigned int empty_field4;

	// This function is required for testing std::multiset:
	inline bool operator < (const medium1_struct &rh) const
	{
		return number < rh.number;
	}

	medium1_struct(const unsigned int num) PLF_BENCH_NOEXCEPT: number(num) {};
};



struct medium2_struct
{
	int numbers[50];
	char a_string[25];
	double unused_number;
	double number;
	double *empty_field_1;
	double *empty_field_2;
	unsigned int empty_field3;
	unsigned int empty_field4;

	// This function is required for testing std::multiset:
	inline bool operator < (const medium2_struct &rh) const
	{
		return number < rh.number;
	}

	medium2_struct(const unsigned int num) PLF_BENCH_NOEXCEPT: number(num) {};
};



struct large_struct
{
	int numbers[100];
	char a_string[50];
	double unused_number;
	double number;
	double *empty_field_1;
	double *empty_field_2;
	unsigned int empty_field3;
	unsigned int empty_field4;

	// This function is required for testing std::multiset:
	inline bool operator < (const large_struct &rh) const
	{
		return number < rh.number;
	}

	large_struct(const unsigned int num) PLF_BENCH_NOEXCEPT: number(num) {};
};




struct large_struct_bool
{
	int numbers[100];
	char a_string[50];
	double unused_number;
	double number;
	double *empty_field_1;
	double *empty_field_2;
	unsigned int empty_field3;
	unsigned int empty_field4;
	bool erased;

	// This function is required for testing std::multiset:
	inline bool operator < (const large_struct_bool &rh) const
	{
		return number < rh.number;
	}

	large_struct_bool(const unsigned int num) PLF_BENCH_NOEXCEPT: number(num), erased(false) {};
};




// MATH FUNCTIONS:

inline unsigned int fast_mod(const unsigned int input, const unsigned int ceiling) // courtesy of chandler carruth
{
    // apply the modulo operator only when needed
    // (i.e. when the input is greater than the ceiling)
    return (input >= ceiling) ? input % ceiling : input;
}


inline unsigned int rand_within(const unsigned int range)
{
	return fast_mod(plf::rand(), range);
}




// Reserve templates:

template<class container_type>
PLF_BENCH_FORCE_INLINE void container_reserve(container_type &container, unsigned int amount)
{} // General case: does nothing


template<class container_contents>
PLF_BENCH_FORCE_INLINE void container_reserve(std::vector<container_contents> &container, unsigned int amount)
{
	container.reserve(amount);
}


template<class container_contents>
PLF_BENCH_FORCE_INLINE void container_reserve(plf::indexed_vector<container_contents> &container, unsigned int amount)
{
	container.reserve(amount);
}


template<class container_contents>
PLF_BENCH_FORCE_INLINE void container_reserve(plf::stack<container_contents> &container, unsigned int amount)
{
	container.reserve(amount);
}


template<class container_contents>
PLF_BENCH_FORCE_INLINE void container_reserve(plf::queue<container_contents> &container, unsigned int amount)
{
	container.reserve(amount);
}


template<class container_contents>
PLF_BENCH_FORCE_INLINE void container_reserve(plf::colony<container_contents> &container, unsigned int amount)
{
	container.reserve(amount);
}





// INSERT FUNCTIONS:

// Specifically push_back

template <template <typename,typename> class container_type, typename container_contents, typename allocator_type>
PLF_BENCH_FORCE_INLINE void container_push_back(container_type<container_contents, allocator_type> &container, unsigned int number)
{
	container.push_back(container_contents(number)); // using bitwise AND as a faster replacement for modulo, as this is a power of 2 number - 1
}


template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_push_back(plf::colony<container_contents> &container, unsigned int number)
{
	container.insert(container_contents(number));
}




// default:
template <template <typename,typename> class container_type, typename container_contents, typename allocator_type>
PLF_BENCH_FORCE_INLINE void container_insert(container_type<container_contents, allocator_type> &container)
{
	container.push_back(container_contents(plf::rand() & 255)); // using bitwise AND as a faster replacement for modulo, as this is a power of 2 number - 1
}



template <template <typename,typename,typename> class container_type, typename container_contents, typename allocator_type, typename priority>
PLF_BENCH_FORCE_INLINE void container_insert(container_type<container_contents, allocator_type, priority> &container)
{
	container.push_back(container_contents(plf::rand() & 255)); // using bitwise AND as a faster replacement for modulo, as this is a power of 2 number - 1
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(std::list<container_contents> &container)
{
	container.push_front(container_contents(plf::rand() & 255));
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(plf::list<container_contents> &container)
{
	container.push_back(container_contents(plf::rand() & 255));
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(std::map<unsigned int, container_contents> &container)
{
	container.insert(std::make_pair(static_cast<unsigned int>(container.size()), container_contents(plf::rand() & 255)));
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(plf::packed_deque<container_contents> &container)
{
	container.insert(container_contents(plf::rand() & 255));
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(std::multiset<container_contents> &container)
{
	container.insert(container_contents(plf::rand() & 255));
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(plf::stack<container_contents> &container)
{
	container.push(container_contents(plf::rand() & 255));
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(std::stack<container_contents> &container)
{
	container.push(container_contents(plf::rand() & 255));
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(plf::colony<container_contents> &container)
{
	container.insert(container_contents(plf::rand() & 255));
}


template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(plf::queue<container_contents> &container)
{
	container.push(container_contents(plf::rand() & 255));
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_insert(std::queue<container_contents> &container)
{
	container.push(container_contents(plf::rand() & 255));
}







// The following is only present to cater for older versions of colony for comparison testing
//template <class container_contents>
//PLF_BENCH_FORCE_INLINE void container_insert(plf::colony<container_contents> &container)
//{
//	container.insert(container_contents(plf::rand() & 255));
//}








// Ordered INSERT FUNCTIONS:

// default:
template <template <typename,typename> class container_type, typename container_contents, typename allocator_type, class iterator_type>
PLF_BENCH_FORCE_INLINE void container_ordered_insert(container_type<container_contents, allocator_type> &container, iterator_type &position)
{
	position = container.insert(position, container_contents(plf::rand() & 255));
	++position;
}




template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_ordered_insert(plf::list<container_contents> &container, typename plf::list<container_contents>::iterator position)
{
	container.insert(position, container_contents(plf::rand() & 255));
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_ordered_insert(std::list<container_contents> &container, typename std::list<container_contents>::iterator position)
{
	container.insert(position, container_contents(plf::rand() & 255));
}









// ERASURE FUNCTIONS:

template <class container_type>
PLF_BENCH_FORCE_INLINE void container_erase(container_type &container, typename container_type::iterator &the_iterator)
{
	the_iterator = container.erase(the_iterator);
}


// This was used to check the pointer-erase colony function performance - not used normally:

//template <class container_contents>
//PLF_BENCH_FORCE_INLINE void container_erase(plf::colony<container_contents> &container, typename plf::colony<container_contents>::iterator &the_iterator)
//{
//	the_iterator = container.erase(&(*the_iterator));
//}
//
//



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_erase(std::map<unsigned int, container_contents> &container, typename std::map<unsigned int, container_contents>::iterator &the_iterator)
{
	// This procedure necessary in pre-C++11:
	typename std::map<unsigned int, container_contents>::iterator temp_iterator = the_iterator;
	++the_iterator;
	container.erase(temp_iterator);
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_erase(std::multiset<container_contents> &container, typename std::multiset<container_contents>::iterator &the_iterator)
{
	// This procedure necessary in pre-C++11:
	typename std::multiset<container_contents>::iterator temp_iterator = the_iterator;
	++the_iterator;
	container.erase(temp_iterator);
}



PLF_BENCH_FORCE_INLINE void container_erase(std::vector<int_bool> &container, std::vector<int_bool>::iterator &the_iterator)
{
	the_iterator->erased = true;
	++the_iterator;
}



PLF_BENCH_FORCE_INLINE void container_erase(std::deque<int_bool> &container, std::deque<int_bool>::iterator &the_iterator)
{
	the_iterator->erased = true;
	++the_iterator;
}


PLF_BENCH_FORCE_INLINE void container_erase(std::vector<small_struct_bool> &container, std::vector<small_struct_bool>::iterator &the_iterator)
{
	the_iterator->erased = true;
	++the_iterator;
}



PLF_BENCH_FORCE_INLINE void container_erase(std::deque<small_struct_bool> &container, std::deque<small_struct_bool>::iterator &the_iterator)
{
	the_iterator->erased = true;
	++the_iterator;
}


PLF_BENCH_FORCE_INLINE void container_erase(std::vector<large_struct_bool> &container, std::vector<large_struct_bool>::iterator &the_iterator)
{
	the_iterator->erased = true;
	++the_iterator;
}



PLF_BENCH_FORCE_INLINE void container_erase(std::deque<large_struct_bool> &container, std::deque<large_struct_bool>::iterator &the_iterator)
{
	the_iterator->erased = true;
	++the_iterator;
}



PLF_BENCH_FORCE_INLINE void container_erase(plf::pointer_deque<int_bool> &container, plf::pointer_deque<int_bool>::iterator &the_iterator)
{
	(*the_iterator)->erased = true;
	++the_iterator;
}



PLF_BENCH_FORCE_INLINE void container_erase(plf::indexed_vector<int_bool> &container, plf::indexed_vector<int_bool>::iterator &the_iterator)
{
	(container.set(the_iterator)).erased = true;
	++the_iterator;
}


PLF_BENCH_FORCE_INLINE void container_erase(plf::pointer_deque<small_struct_bool> &container, plf::pointer_deque<small_struct_bool>::iterator &the_iterator)
{
	(*the_iterator)->erased = true;
	++the_iterator;
}



PLF_BENCH_FORCE_INLINE void container_erase(plf::indexed_vector<small_struct_bool> &container, plf::indexed_vector<small_struct_bool>::iterator &the_iterator)
{
	(container.set(the_iterator)).erased = true;
	++the_iterator;
}






// ITERATION FUNCTIONS:

// helper functions:

template <template <typename,typename> class container_type, typename container_contents, typename allocator_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<container_contents, allocator_type> &the_container, const typename container_type<container_contents, allocator_type>::iterator &the_iterator)
{
	return static_cast<unsigned int>(*the_iterator);
}



template <template <typename,typename> class container_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<small_struct_non_trivial, std::allocator<small_struct_non_trivial> > &the_container, const typename container_type<small_struct_non_trivial, std::allocator<small_struct_non_trivial> >::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename> class container_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<small_struct, std::allocator<small_struct> > &the_container, const typename container_type<small_struct, std::allocator<small_struct> >::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename> class container_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<medium1_struct, std::allocator<medium1_struct> > &the_container, const typename container_type<medium1_struct, std::allocator<medium1_struct> >::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename> class container_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<medium2_struct, std::allocator<medium2_struct> > &the_container, const typename container_type<medium2_struct, std::allocator<medium2_struct> >::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename> class container_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<large_struct, std::allocator<large_struct> > &the_container, const typename container_type<large_struct, std::allocator<large_struct> >::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename, typename> class container_type, typename container_contents, typename allocator_type, typename priority>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<container_contents, allocator_type, plf::colony_priority> &the_container, const typename container_type<container_contents, allocator_type, plf::colony_priority>::iterator &the_iterator)
{
	return static_cast<unsigned int>(*the_iterator);
}



template <template <typename,typename, typename> class container_type, typename priority>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<small_struct, std::allocator<small_struct>, plf::colony_priority> &the_container, const typename container_type<small_struct, std::allocator<small_struct>, plf::colony_priority>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename, typename> class container_type, typename priority>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<large_struct, std::allocator<large_struct>, plf::colony_priority> &the_container, const typename container_type<large_struct, std::allocator<large_struct>, plf::colony_priority>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename, typename> class container_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<small_struct, std::allocator<small_struct>, unsigned char> &the_container, const typename container_type<small_struct, std::allocator<small_struct>, unsigned char>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename, typename> class container_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<large_struct, std::allocator<large_struct>, unsigned char> &the_container, const typename container_type<large_struct, std::allocator<large_struct>, unsigned char>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename, typename> class container_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<small_struct, std::allocator<small_struct>, unsigned int> &the_container, const typename container_type<small_struct, std::allocator<small_struct>, unsigned int>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <template <typename,typename, typename> class container_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const container_type<large_struct, std::allocator<large_struct>, unsigned int> &the_container, const typename container_type<large_struct, std::allocator<large_struct>, unsigned int>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::multiset<container_contents> &the_container, const typename std::multiset<container_contents>::iterator &the_iterator)
{
	return static_cast<unsigned int>(*the_iterator);
}



template <>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::multiset<small_struct> &the_container, const typename std::multiset<small_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::multiset<large_struct> &the_container, const typename std::multiset<large_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->number);
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::map<unsigned int, container_contents> &the_container, const typename std::map<unsigned int, container_contents>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->second);
}



template <>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::map<unsigned int, small_struct> &the_container, const std::map<unsigned int, small_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->second.number);
}



template <>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::map<unsigned int, large_struct> &the_container, const std::map<unsigned int, large_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->second.number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::vector<char_bool> &the_container, const std::vector<char_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::deque<char_bool> &the_container, const std::deque<char_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::vector<int_bool> &the_container, const std::vector<int_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::deque<int_bool> &the_container, const std::deque<int_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::vector<double_bool> &the_container, const std::vector<double_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::deque<double_bool> &the_container, const std::deque<double_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::vector<small_struct_bool> &the_container, const std::vector<small_struct_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::deque<small_struct_bool> &the_container, const std::deque<small_struct_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::vector<large_struct_bool> &the_container, const std::vector<large_struct_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const std::deque<large_struct_bool> &the_container, const std::deque<large_struct_bool>::iterator &the_iterator)
{
	return (the_iterator->erased) ? 0 : static_cast<unsigned int>(the_iterator->number);
}



template <typename container_contents, typename allocator_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::pointer_deque<container_contents, allocator_type> &the_container, const typename plf::pointer_deque<container_contents, allocator_type>::iterator &the_iterator)
{
	return static_cast<unsigned int>(**the_iterator);
}


PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::pointer_deque<small_struct> &the_container, const plf::pointer_deque<small_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>((*the_iterator)->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::pointer_deque<int_bool> &the_container, const plf::pointer_deque<int_bool>::iterator &the_iterator)
{
	return static_cast<unsigned int>((*the_iterator)->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::pointer_deque<small_struct_bool> &the_container, const plf::pointer_deque<small_struct_bool>::iterator &the_iterator)
{
	return static_cast<unsigned int>((*the_iterator)->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::pointer_deque<large_struct> &the_container, const plf::pointer_deque<large_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>((*the_iterator)->number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::pointer_deque<large_struct_bool> &the_container, const plf::pointer_deque<large_struct_bool>::iterator &the_iterator)
{
	return static_cast<unsigned int>((*the_iterator)->number);
}



template <typename container_contents, typename allocator_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::pointer_colony<container_contents, allocator_type> &the_container, const typename plf::pointer_colony<container_contents, allocator_type>::iterator &the_iterator)
{
	return static_cast<unsigned int>(**the_iterator);
}


PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::pointer_colony<small_struct> &the_container, const plf::pointer_colony<small_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>((*the_iterator)->number);
}


PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::pointer_colony<large_struct> &the_container, const plf::pointer_colony<large_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>((*the_iterator)->number);
}



template <typename container_contents, typename allocator_type>
PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::indexed_vector<container_contents, allocator_type> &the_container, const typename plf::indexed_vector<container_contents, allocator_type>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_container.get(the_iterator));
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::indexed_vector<small_struct> &the_container, const plf::indexed_vector<small_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>((the_container.get(the_iterator)).number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::indexed_vector<int_bool> &the_container, const plf::indexed_vector<int_bool>::iterator &the_iterator)
{
	return static_cast<unsigned int>((the_container.get(the_iterator)).number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::indexed_vector<small_struct_bool> &the_container, const plf::indexed_vector<small_struct_bool>::iterator &the_iterator)
{
	return static_cast<unsigned int>((the_container.get(the_iterator)).number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::indexed_vector<large_struct> &the_container, const plf::indexed_vector<large_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>((the_container.get(the_iterator)).number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::indexed_vector<large_struct_bool> &the_container, const plf::indexed_vector<large_struct_bool>::iterator &the_iterator)
{
	return static_cast<unsigned int>((the_container.get(the_iterator)).number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::packed_deque<small_struct> &the_container, const plf::packed_deque<small_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->element.number);
}



PLF_BENCH_FORCE_INLINE unsigned int container_iterate(const plf::packed_deque<large_struct> &the_container, const plf::packed_deque<large_struct>::iterator &the_iterator)
{
	return static_cast<unsigned int>(the_iterator->element.number);
}






// SORT:

template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_sort(plf::colony<container_contents> &the_container)
{
	plf::indiesort(the_container);
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_sort(plf::list<container_contents> &the_container)
{
	the_container.sort();
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_sort(std::list<container_contents> &the_container)
{
	the_container.sort();
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_sort(std::deque<container_contents> &the_container)
{
	std::sort(the_container.begin(), the_container.end());
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_sort(std::vector<container_contents> &the_container)
{
	std::sort(the_container.begin(), the_container.end());
}




template <class container_class>
PLF_BENCH_FORCE_INLINE void container_remove_if(container_class &container)
{
	typename container_class::iterator result = container.begin(), it = container.begin(), last = container.end();

	while (it != last)
	{
		if (!(it->erased))
		{
			#ifdef PLF_BENCH_MOVE_SEMANTICS_SUPPORT
				*(result++) = std::move(*it);
			#else
				*(result++) = *it;
			#endif
		}

		++it;
	}

	for (it = result; it != last; ++it)
	{
		container.pop_back();
	}
}


template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_remove_if(plf::pointer_deque<container_contents> &container)
{
	container.remove_if();
}


template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_remove_if(plf::indexed_vector<container_contents> &container)
{
	container.remove_if();
}



template <class container_class>
PLF_BENCH_FORCE_INLINE void container_remove_if2(container_class &container, const unsigned int erasure_percent)
{
	container.erase(std::remove_if(container.begin(), container.end(), [erasure_percent](typename container_class::value_type x){return x < static_cast<typename container_class::value_type>(erasure_percent);}), container.end());
}



PLF_BENCH_FORCE_INLINE void container_remove_if2(std::vector<large_struct> &container, const unsigned int erasure_percent)
{
	container.erase(std::remove_if(container.begin(), container.end(), [erasure_percent](large_struct &x){return x.number < erasure_percent;}), container.end());
}



PLF_BENCH_FORCE_INLINE void container_remove_if2(std::vector<small_struct> &container, const unsigned int erasure_percent)
{
	container.erase(std::remove_if(container.begin(), container.end(), [erasure_percent](small_struct &x){return x.number < erasure_percent;}), container.end());
}



PLF_BENCH_FORCE_INLINE void container_remove_if2(std::deque<large_struct> &container, const unsigned int erasure_percent)
{
	container.erase(std::remove_if(container.begin(), container.end(), [erasure_percent](large_struct &x){return x.number < erasure_percent;}), container.end());
}



PLF_BENCH_FORCE_INLINE void container_remove_if2(std::deque<small_struct> &container, const unsigned int erasure_percent)
{
	container.erase(std::remove_if(container.begin(), container.end(), [erasure_percent](small_struct &x){return x.number < erasure_percent;}), container.end());
}



// For stack testing:
template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_back_pop(plf::stack<container_contents> &container, double &total)
{
	total += container.top();
	container.pop();
}


template <>
PLF_BENCH_FORCE_INLINE void container_back_pop(plf::stack<small_struct> &container, double &total)
{
	total += container.top().number;
	container.pop();
}


template <>
PLF_BENCH_FORCE_INLINE void container_back_pop(plf::stack<large_struct> &container, double &total)
{
	total += container.top().number;
	container.pop();
}


template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_back_pop(std::stack<container_contents> &container, double &total)
{
	total += container.top();
	container.pop();
}


template <>
PLF_BENCH_FORCE_INLINE void container_back_pop(std::stack<small_struct> &container, double &total)
{
	total += container.top().number;
	container.pop();
}


template <>
PLF_BENCH_FORCE_INLINE void container_back_pop(std::stack<large_struct> &container, double &total)
{
	total += container.top().number;
	container.pop();
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_back_pop(std::vector<container_contents> &container, double &total)
{
	total += container.back();
	container.pop_back();
}


template <>
PLF_BENCH_FORCE_INLINE void container_back_pop(std::vector<small_struct> &container, double &total)
{
	total += container.back().number;
	container.pop_back();
}


template <>
PLF_BENCH_FORCE_INLINE void container_back_pop(std::vector<large_struct> &container, double &total)
{
	total += container.back().number;
	container.pop_back();
}



// For queue testing:
template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_front_pop(plf::queue<container_contents> &container, double &total)
{
	total += container.front();
	container.pop();
}


template <>
PLF_BENCH_FORCE_INLINE void container_front_pop(plf::queue<small_struct> &container, double &total)
{
	total += container.front().number;
	container.pop();
}


template <>
PLF_BENCH_FORCE_INLINE void container_front_pop(plf::queue<large_struct> &container, double &total)
{
	total += container.front().number;
	container.pop();
}


template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_front_pop(std::queue<container_contents> &container, double &total)
{
	total += container.front();
	container.pop();
}


template <>
PLF_BENCH_FORCE_INLINE void container_front_pop(std::queue<small_struct> &container, double &total)
{
	total += container.front().number;
	container.pop();
}


template <>
PLF_BENCH_FORCE_INLINE void container_front_pop(std::queue<large_struct> &container, double &total)
{
	total += container.front().number;
	container.pop();
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_front_pop(std::deque<container_contents> &container, double &total)
{
	total += container.front();
	container.pop_front();
}


template <>
PLF_BENCH_FORCE_INLINE void container_front_pop(std::deque<small_struct> &container, double &total)
{
	total += container.front().number;
	container.pop_front();
}


template <>
PLF_BENCH_FORCE_INLINE void container_front_pop(std::deque<large_struct> &container, double &total)
{
	total += container.front().number;
	container.pop_front();
}



// For lesser functions:

template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_reverse(plf::list<container_contents> &the_container)
{
	the_container.reverse();
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_reverse(std::list<container_contents> &the_container)
{
	the_container.reverse();
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_reverse(plf::colony<container_contents> &the_container)
{
	std::reverse(the_container.begin(), the_container.end());
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_reverse(std::deque<container_contents> &the_container)
{
	std::reverse(the_container.begin(), the_container.end());
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_reverse(std::vector<container_contents> &the_container)
{
	std::reverse(the_container.begin(), the_container.end());
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_unique(plf::list<container_contents> &the_container)
{
	the_container.unique();
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_unique(std::list<container_contents> &the_container)
{
	the_container.unique();
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_unique(plf::colony<container_contents> &the_container)
{
	std::unique(the_container.begin(), the_container.end());
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_unique(std::deque<container_contents> &the_container)
{
	std::unique(the_container.begin(), the_container.end());
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_unique(std::vector<container_contents> &the_container)
{
	std::unique(the_container.begin(), the_container.end());
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_remove(plf::list<container_contents> &the_container, const int value)
{
	the_container.remove(value);
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_remove(std::list<container_contents> &the_container, const int value)
{
	the_container.remove(value);
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_remove(plf::colony<container_contents> &the_container, const int value)
{
	std::remove(the_container.begin(), the_container.end(), value);
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_remove(std::deque<container_contents> &the_container, const int value)
{
	std::remove(the_container.begin(), the_container.end(), value);
}



template <class container_contents>
PLF_BENCH_FORCE_INLINE void container_remove(std::vector<container_contents> &the_container, const int value)
{
	std::remove(the_container.begin(), the_container.end(), value);
}



template <class container_type>
PLF_BENCH_FORCE_INLINE unsigned int memory(container_type &container)
{
	return static_cast<unsigned int>(container.memory());
}



template<class container_contents>
PLF_BENCH_FORCE_INLINE unsigned int memory(std::list<container_contents> &list)
{
	return static_cast<unsigned int>(sizeof(list) + (list.size() * (sizeof(container_contents) + (sizeof(void *) * 2))));
}



template<class container_contents>
PLF_BENCH_FORCE_INLINE unsigned int memory(std::vector<container_contents> &vector)
{
	return static_cast<unsigned int>(sizeof(vector) + (vector.capacity() * sizeof(container_contents)));
}



template<class container_contents> // we assume here that the underlying library is libstdc++
PLF_BENCH_FORCE_INLINE unsigned int memory(std::deque<container_contents> &deque)
{
	const unsigned int block_size = (sizeof(container_contents) > 512) ? sizeof(container_contents) : 512;
	const unsigned int elements_per_block = block_size / sizeof(container_contents);
   const unsigned int number_of_blocks = (deque.size() + (elements_per_block - 1) /* round up */) / elements_per_block;
	const unsigned int blocks_plus_block_pointers_size = (block_size + sizeof(void *)) * number_of_blocks;
	return static_cast<unsigned int>(64 + sizeof(deque) + blocks_plus_block_pointers_size);
}



template<class container_contents> // we assume here that the underlying container is deque, and the library is libstdc++
PLF_BENCH_FORCE_INLINE unsigned int memory(std::queue<container_contents> &queue)
{
	const unsigned int block_size = (sizeof(container_contents) > 512) ? sizeof(container_contents) : 512;
	const unsigned int elements_per_block = block_size / sizeof(container_contents);
   const unsigned int number_of_blocks = (queue.size() + (elements_per_block - 1)) / elements_per_block;
	const unsigned int blocks_plus_block_pointers_size = (block_size + sizeof(void *)) * number_of_blocks;
	return static_cast<unsigned int>(64 + sizeof(std::deque<container_contents>) + sizeof(queue) + blocks_plus_block_pointers_size);
}







// Actual tests:

template<class container_type>
PLF_BENCH_FORCE_INLINE void iteration_test(container_type &container, const unsigned int number_of_runs, const bool output_csv = false)
{
	double total_time = 0, total = 0;
	plf::nanotimer timer;

	timer.start();

	const unsigned int end = (number_of_runs / 10) + 1;
	const typename container_type::iterator end_element = container.end();

	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		for (typename container_type::iterator current_element = container.begin(); current_element != end_element; ++current_element)
		{
			total += container_iterate(container, current_element);
		}
	}

	total_time = timer.get_elapsed_us();

	std::cerr << "Dump time and total: " << total_time << ", " << total << std::endl;
	total_time = 0;

	timer.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		for (typename container_type::iterator current_element = container.begin(); current_element != end_element; ++current_element)
		{
			total += container_iterate(container, current_element);
		}
	}

	total_time = timer.get_elapsed_us();

	if (output_csv)
	{
		std::cout << ", " << (total_time / number_of_runs);
	}
	else
	{
		std::cout << "Iterate and sum: " << (total_time / number_of_runs) << "us" << std::endl;
	}

	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or loops will be removed) - no kidding, gcc will actually do this with std::vector.
}










template<class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_stack(const unsigned int number_of_elements, const unsigned int number_of_runs, const bool output_csv = false, const bool reserve = false)
{
	double push_time = 0, pop_back_time = 0, total = 0;
	plf::nanotimer timer, timer2;

	timer2.start();

	// Warm up cache:
	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		timer.start();

		container_type container;

		if (reserve)
		{
			container_reserve(container, number_of_elements);
		}

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		push_time += timer.get_elapsed_us();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_back_pop(container, total);
		}
	}

	// Doing this because pop times are generally too short and lead to chaotic timing results
	// remainder of total time for loop once push and container creation are accounted for must be pop/back time:
	pop_back_time = timer2.get_elapsed_us() - push_time;

	std::cerr << "Dump total and time: " << total << ", " << push_time << ", " << pop_back_time << std::endl;
	push_time = 0;
	pop_back_time = 0;
	total = 0;


	timer2.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		timer.start();

		container_type container;

		if (reserve)
		{
			container_reserve(container, number_of_elements);
		}

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		push_time += timer.get_elapsed_us();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_back_pop(container, total);
		}
	}

	// Doing this because pop times are generally too short and lead to chaotic timing results
	// remainder of total time for loop once push and container creation are accounted for must be pop/back time:
	pop_back_time = timer2.get_elapsed_us() - push_time;


	if (output_csv)
	{
		std::cout << ", " << (push_time / number_of_runs) << ", " << (pop_back_time / number_of_runs) << ", " << ((pop_back_time + push_time) / number_of_runs) << std::endl;
	}
	else
	{
		std::cout << "Push " << number_of_elements << " elements: " << (push_time / number_of_runs) << "us" << std::endl;
		std::cout << "Pop and sum: " << (pop_back_time / number_of_runs) << "us" << std::endl;
		std::cout << "Total time: " << ((pop_back_time + push_time) / number_of_runs) << "us" << "\n\n\n";
	}

	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}





template<class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_queue(const unsigned int number_of_elements, const unsigned int number_of_runs, const bool output_csv = false, const bool reserve = false)
{
	double push_time = 0, pop_back_time = 0, total = 0;
	plf::nanotimer timer, timer2;

	timer2.start();

	// Warm up cache:
	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		timer.start();

		container_type container;

		if (reserve)
		{
			container_reserve(container, number_of_elements);
		}

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		push_time += timer.get_elapsed_us();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_front_pop(container, total);
		}
	}

	// Doing this because pop times are generally too short and lead to chaotic timing results
	// remainder of total time for loop once push and container creation are accounted for must be pop/back time:
	pop_back_time = timer2.get_elapsed_us() - push_time;

	std::cerr << "Dump total and time: " << total << ", " << push_time << ", " << pop_back_time << std::endl;
	push_time = 0;
	pop_back_time = 0;
	total = 0;


	timer2.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		timer.start();

		container_type container;

		if (reserve)
		{
			container_reserve(container, number_of_elements);
		}

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		push_time += timer.get_elapsed_us();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_front_pop(container, total);
		}
	}

	// Doing this because pop times are generally too short and lead to chaotic timing results
	// remainder of total time for loop once push and container creation are accounted for must be pop/back time:
	pop_back_time = timer2.get_elapsed_us() - push_time;


	if (output_csv)
	{
		std::cout << ", " << (push_time / number_of_runs) << ", " << (pop_back_time / number_of_runs) << ", " << ((pop_back_time + push_time) / number_of_runs) << std::endl;
	}
	else
	{
		std::cout << "Push " << number_of_elements << " elements: " << (push_time / number_of_runs) << "us" << std::endl;
		std::cout << "Pop and sum: " << (pop_back_time / number_of_runs) << "us" << std::endl;
		std::cout << "Total time: " << ((pop_back_time + push_time) / number_of_runs) << "us" << "\n\n\n";
	}

	std::cerr << "Dump total: " << total << std::endl;
}




template<class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_queue_pump(const unsigned int number_of_elements, const unsigned int number_of_runs, const float pump_factor, const bool output_csv = false, const bool reserve = false)
{
	double time_taken = 0, total = 0;
	double memory_usage = 0;
	plf::nanotimer timer;

	const unsigned int min_num = static_cast<unsigned int>(static_cast<float>(number_of_elements) / pump_factor);
	const unsigned int max_num = static_cast<unsigned int>(static_cast<float>(number_of_elements) * pump_factor);

	// Warm up cache:
	const unsigned int end = (number_of_runs / 10) + 1;
	timer.start();

	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		if (reserve)
		{
			container_reserve(container, number_of_elements);
		}

		unsigned int container_size = 0;

		for (; container_size != number_of_elements; ++container_size)
		{
			container_insert(container);
		}

		memory_usage = memory(container);

		// Queue and pop elements over time, increasing/decreasing the overall number of elements according to the pump factor, in waves:

		for (unsigned int counter = 0; counter != number_of_elements * 10; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		memory_usage += memory(container);

		// Gradually increase over time:
		while (container_size != max_num)
		{
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			++container_size;
		}


		memory_usage += memory(container);

		for (unsigned int counter = 0; counter != number_of_elements * 2; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		memory_usage += memory(container);

		// Gradually decrease over time:
		while (container_size != number_of_elements)
		{
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			container_front_pop(container, total);
			container_front_pop(container, total);
			--container_size;
		}

		memory_usage += memory(container);

		for (unsigned int counter = 0; counter != number_of_elements * 20; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		memory_usage += memory(container);

		// Quickly decrease over time:
		while (container_size != min_num)
		{
			container_insert(container);
			container_front_pop(container, total);
			container_front_pop(container, total);
			--container_size;
		}

		memory_usage += memory(container);

		for (unsigned int counter = 0; counter != number_of_elements * 20; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		memory_usage += memory(container);

		// Quickly increase over time:
		while (container_size != max_num)
		{
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			++container_size;
		}

		memory_usage += memory(container);

		for (unsigned int counter = 0; counter != number_of_elements * 10; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		memory_usage += memory(container);

		// Very Quickly decrease over time:
		while (container_size != number_of_elements)
		{
			container_front_pop(container, total);
			--container_size;
		}

		memory_usage += memory(container);

		for (unsigned int counter = 0; counter != number_of_elements * 20; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		memory_usage += memory(container);
		memory_usage /= 12;
	}

	// Doing this because pop times are generally too short and lead to chaotic timing results
	// remainder of total time for loop once push and container creation are accounted for must be pop/back time:
	time_taken = timer.get_elapsed_us() / end;

	std::cerr << "Dump total and time: " << total << time_taken << std::endl;

	timer.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		if (reserve)
		{
			container_reserve(container, number_of_elements);
		}

		unsigned int container_size = 0;

		for (; container_size != number_of_elements; ++container_size)
		{
			container_insert(container);
		}

		// Queue and pop elements over time, increasing/decreasing the overall number of elements according to the pump factor, in waves:

		for (unsigned int counter = 0; counter != number_of_elements * 10; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		// Gradually increase over time:
		while (container_size != max_num)
		{
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			++container_size;
		}


		for (unsigned int counter = 0; counter != number_of_elements * 2; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		// Gradually decrease over time:
		while (container_size != number_of_elements)
		{
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			container_front_pop(container, total);
			container_front_pop(container, total);
			--container_size;
		}

		for (unsigned int counter = 0; counter != number_of_elements * 20; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		// Quickly decrease over time:
		while (container_size != min_num)
		{
			container_insert(container);
			container_front_pop(container, total);
			container_front_pop(container, total);
			--container_size;
		}

		for (unsigned int counter = 0; counter != number_of_elements * 20; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		// Quickly increase over time:
		while (container_size != max_num)
		{
			container_insert(container);
			container_front_pop(container, total);
			container_insert(container);
			++container_size;
		}

		for (unsigned int counter = 0; counter != number_of_elements * 10; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}

		// Very Quickly decrease over time:
		while (container_size != number_of_elements)
		{
			container_front_pop(container, total);
			--container_size;
		}

		for (unsigned int counter = 0; counter != number_of_elements * 20; ++counter)
		{
			container_insert(container);
			container_front_pop(container, total);
		}
	}

	time_taken = timer.get_elapsed_us() / number_of_runs;


	if (output_csv)
	{
		std::cout << ", " << time_taken << ", " << memory_usage << std::endl;
	}
	else
	{
		std::cout << "Total time: " << time_taken << std::endl;
		std::cout << "Memory use: " << memory_usage << std::endl;
	}

	std::cerr << "Dump total: " << total << std::endl;
}






// Combinations:

template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage < 100); // Ie. lower than 100%
	assert (number_of_elements > 1);

	const unsigned int erasure_limit = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(erasure_percentage) / 100.0)) + 0.5);
	unsigned int number_of_erasures;
	const unsigned int erasure_percent_expanded = static_cast<unsigned int>((static_cast<double>(erasure_percentage) * 1.28) + 0.5);
	double insert_time = 0, erase_time = 0, total_size = 0;
	plf::nanotimer insert_timer, erase_timer;


	// Dump-runs to get the cache 'warmed up':
	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		number_of_erasures = 0;
		erase_timer.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);

				if (++number_of_erasures == erasure_limit)
				{
					break;
				}
			}
			else
			{
				++current_element;
			}
		}

		if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
		{
			for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}
		}

		erase_time += erase_timer.get_elapsed_us();
		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;
	insert_time = 0; erase_time = 0; total_size = 0;



	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		number_of_erasures = 0;
		erase_timer.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);

				if (++number_of_erasures == erasure_limit)
				{
					break;
				}
			}
			else
			{
				++current_element;
			}
		}

		if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
		{
			for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}
		}

		erase_time += erase_timer.get_elapsed_us();

		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;


	if (output_csv)
	{
		std::cout << ", " << (insert_time / number_of_runs) << ", ";

		if (erasure_percentage != 0)
		{
			std::cout << (erase_time / number_of_runs);
		}
	}
	else
	{
		std::cout << "Insert " << number_of_elements << " elements: " << (insert_time / number_of_runs) << "us" << std::endl;

		if (erasure_percentage != 0)
		{
			std::cout << "Randomly erase " << erasure_percentage << "% of elements: " << (erase_time / number_of_runs) << "us" << std::endl;
		}
	}


	// Do whole process one more time so we have something for iteration test, without copying/moving containers:
	container_type container;

	{
		// Insert test
		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		// Erase test
		if (erasure_percentage != 0)
		{
			number_of_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
				else
				{
					++current_element;
				}
			}

			if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
			{
				for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
				{
					if ((plf::rand() & 127) < erasure_percent_expanded)
					{
						container_erase(container, current_element);

						if (++number_of_erasures == erasure_limit)
						{
							break;
						}
					}
				}
			}
		}
	}


	iteration_test(container, number_of_runs * 1000, output_csv);

	if (output_csv)
	{
		std::cout << "\n";
	}
	else
	{
		std::cout << "\n\n";
	}
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_remove_if(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage < 100); // Ie. lower than 100%
	assert (number_of_elements > 1);


	const unsigned int erasure_limit = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(erasure_percentage) / 100.0)) + 0.5);
	const unsigned int erasure_percent_expanded = static_cast<unsigned int>((static_cast<double>(erasure_percentage) * 1.28) + 0.5);
	unsigned int number_of_erasures;
	double insert_time = 0, erase_time = 0, total_size = 0;
	plf::nanotimer insert_timer, erase_timer;


	// Dump-runs to get the cache 'warmed up':
	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		number_of_erasures = 0;
		erase_timer.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);

				if (++number_of_erasures == erasure_limit)
				{
					break;
				}
			}
			else
			{
				++current_element;
			}
		}

		if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
		{
			for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}
		}

		container_remove_if(container);

		erase_time += erase_timer.get_elapsed_us();
		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;
	insert_time = 0; erase_time = 0; total_size = 0;



	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		number_of_erasures = 0;
		erase_timer.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);

				if (++number_of_erasures == erasure_limit)
				{
					break;
				}
			}
			else
			{
				++current_element;
			}
		}

		if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
		{
			for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}
		}

		container_remove_if(container);

		erase_time += erase_timer.get_elapsed_us();

		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;


	if (output_csv)
	{
		std::cout << ", " << (insert_time / number_of_runs) << ", ";

		if (erasure_percentage != 0)
		{
			std::cout << (erase_time / number_of_runs);
		}
	}
	else
	{
		std::cout << "Insert " << number_of_elements << " elements: " << (insert_time / number_of_runs) << "us" << std::endl;

		if (erasure_percentage != 0)
		{
			std::cout << "Randomly erase " << erasure_percentage << "% of elements: " << (erase_time / number_of_runs) << "us" << std::endl;
		}
	}


	// Do whole process one more time so we have something for iteration test, without copying/moving containers:
	container_type container;

	{
		// Insert test
		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		// Erase test
		if (erasure_percentage != 0)
		{
			number_of_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
				else
				{
					++current_element;
				}
			}

			if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
			{
				for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
				{
					if ((plf::rand() & 127) < erasure_percent_expanded)
					{
						container_erase(container, current_element);

						if (++number_of_erasures == erasure_limit)
						{
							break;
						}
					}
				}
			}
		}

		container_remove_if(container);
	}


	iteration_test(container, number_of_runs * 1000, output_csv);

	if (output_csv)
	{
		std::cout << "\n";
	}
	else
	{
		std::cout << "\n\n";
	}
}



template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_remove_if_standard(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage < 100); // Ie. lower than 100%
	assert (number_of_elements > 1);


	const unsigned int erasure_percent_expanded = static_cast<unsigned int>((static_cast<double>(erasure_percentage) * 2.56) + 0.5);
	double insert_time = 0, erase_time = 0, total_size = 0;
	plf::nanotimer insert_timer, erase_timer;


	// Dump-runs to get the cache 'warmed up':
	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		erase_timer.start();

		container_remove_if2(container, erasure_percent_expanded);

		erase_time += erase_timer.get_elapsed_us();
		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;
	insert_time = 0; erase_time = 0; total_size = 0;



	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		erase_timer.start();

		container_remove_if2(container, erasure_percent_expanded);

		erase_time += erase_timer.get_elapsed_us();

		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;


	if (output_csv)
	{
		std::cout << ", " << (insert_time / number_of_runs) << ", ";

		if (erasure_percentage != 0)
		{
			std::cout << (erase_time / number_of_runs);
		}
	}
	else
	{
		std::cout << "Insert " << number_of_elements << " elements: " << (insert_time / number_of_runs) << "us" << std::endl;

		if (erasure_percentage != 0)
		{
			std::cout << "Randomly erase " << erasure_percentage << "% of elements: " << (erase_time / number_of_runs) << "us" << std::endl;
		}
	}


	// Do whole process one more time so we have something for iteration test, without copying/moving containers:
	container_type container;

	// Insert test
	for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
	{
		container_insert(container);
	}

	// Erase test
	if (erasure_percentage != 0)
	{
		container_remove_if2(container, erasure_percent_expanded);
	}

	iteration_test(container, number_of_runs * 1000, output_csv);

	if (output_csv)
	{
		std::cout << "\n";
	}
	else
	{
		std::cout << "\n\n";
	}
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_general_use(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const unsigned int number_of_modifications)
{
	assert (number_of_elements > 1);

	double total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer full_time;
	full_time.start();

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(number_of_elements) >= number_of_modifications)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
				}
			}

			for (unsigned int number_of_insertions = 0; number_of_insertions != number_of_modifications; ++number_of_insertions)
			{
				container_insert(container);
			}
		}

		end_memory += memory(container);
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(number_of_elements) >= number_of_modifications)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
				}
			}

			for (unsigned int number_of_insertions = 0; number_of_insertions != number_of_modifications; ++number_of_insertions)
			{
				container_insert(container);
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}



template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_general_use_percentage(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const unsigned int erasure_percentage)
{
	assert (number_of_elements > 1);

	const unsigned int total_number_of_insertions = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(erasure_percentage) / 100.0)) + 0.5);
	double total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer full_time;
	full_time.start();

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= erasure_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
				}
			}

			for (unsigned int number_of_insertions = 0; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
			{
				container_insert(container);
			}
		}

		end_memory += memory(container);
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.


	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= erasure_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
				}
			}

			for (unsigned int number_of_insertions = 0; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
			{
				container_insert(container);
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}




// new type for more realistic test:
template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_general_use_small_percentage(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const double erasure_percentage)
{
	assert (number_of_elements > 1);

	double total = 0;
	unsigned int end_memory = 0, num_erasures;
	const unsigned int comparison_percentage = static_cast<unsigned int>((erasure_percentage * 167772.16) + 0.5), dump_run_end = (number_of_runs / 10) + 1;
	plf::nanotimer full_time;
	full_time.start();

	for (unsigned int run_number = 0; run_number != dump_run_end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
					++num_erasures;
				}
			}

			for (unsigned int counter = 0; counter != num_erasures; ++counter)
			{
				container_insert(container);
			}
		}

		end_memory += memory(container);
	}

	end_memory /= dump_run_end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed)

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
					++num_erasures;
				}
			}

			for (unsigned int counter = 0; counter != num_erasures; ++counter)
			{
				container_insert(container);
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.

}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_general_use_remove_if_small_percentage(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const double erasure_percentage)
{
	assert (number_of_elements > 1);

	double total = 0;
	unsigned int end_memory = 0, num_erasures;
	const unsigned int comparison_percentage = static_cast<unsigned int>(erasure_percentage * 167772.16), dump_run_end = (number_of_runs / 10) + 1;
	plf::nanotimer full_time;
	full_time.start();

	for (unsigned int run_number = 0; run_number != dump_run_end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
					++num_erasures;
				}
			}

			if (num_erasures != 0)
			{
				container_remove_if(container);
			}

			for (unsigned int counter = 0; counter != num_erasures; ++counter)
			{
				container_insert(container);
			}
		}

		end_memory += memory(container);
	}

	end_memory /= dump_run_end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed)

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
					++num_erasures;
				}
			}

			if (num_erasures != 0)
			{
				container_remove_if(container);
			}

			for (unsigned int counter = 0; counter != num_erasures; ++counter)
			{
				container_insert(container);
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}



template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_general_use_remove_if(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const unsigned int number_of_modifications, const bool output_csv = false, const bool reserve = false)
{
	assert (number_of_elements > 1);

	double total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer full_time;
	full_time.start();

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		if (reserve)
		{
			container_reserve(container, number_of_elements);
		}

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(number_of_elements) >= number_of_modifications)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
				}
			}

			container_remove_if(container);

			for (unsigned int number_of_insertions = 0; number_of_insertions != number_of_modifications; ++number_of_insertions)
			{
				container_insert(container);
			}
		}

		end_memory += memory(container);
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		if (reserve)
		{
			container_reserve(container, number_of_elements);
		}

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(number_of_elements) >= number_of_modifications)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
				}
			}

			container_remove_if(container);

			for (unsigned int number_of_insertions = 0; number_of_insertions != number_of_modifications; ++number_of_insertions)
			{
				container_insert(container);
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	if (output_csv)
	{
		std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory;
	}
	else
	{
		std::cout << "Iterate and sum: " << (total_time / static_cast<double>(number_of_runs)) << "us, size = " << end_memory << std::endl;
	}

	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.


	if (output_csv)
	{
		std::cout << "\n";
	}
	else
	{
		std::cout << "\n\n";
	}

}



template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_general_use_remove_if_percentage(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const unsigned int erasure_percentage)
{
	assert (number_of_elements > 1);

	const unsigned int total_number_of_insertions = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(erasure_percentage) / 100.0)) + 0.5);

	double total = 0;
	unsigned int end_memory = 0, num_erasures;
	plf::nanotimer full_time;
	full_time.start();

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= erasure_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
					++num_erasures;
				}
			}

			if (num_erasures != 0)
			{
				container_remove_if(container);
			}

			for (unsigned int number_of_insertions = 0; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
			{
				container_insert(container);
			}
		}

		end_memory += memory(container);
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= erasure_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else
				{
					container_erase(container, current_element);
					++num_erasures;
				}
			}

			if (num_erasures != 0)
			{
				container_remove_if(container);
			}

			for (unsigned int number_of_insertions = 0; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
			{
				container_insert(container);
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_ordered_percentage(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const unsigned int modification_percentage)
{
	assert (number_of_elements > 1);

	const unsigned int total_number_of_insertions = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(modification_percentage) / 200.0)) + 0.5);
	double total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer full_time;
	full_time.start();

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			unsigned int number_of_insertions = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= modification_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
					++number_of_insertions;
				}
			}


			if (container.empty())
			{
				for (; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
				{
					container_insert(container);
				}
			}
		}

		end_memory += memory(container);
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.


	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			unsigned int number_of_insertions = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= modification_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
					++number_of_insertions;
				}
			}


			if (container.empty())
			{
				for (; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
				{
					container_insert(container);
				}
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_ordered_small_percentage(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const double modification_percentage)
{
	assert (number_of_elements > 1);

	double total = 0;
	unsigned int end_memory = 0;
	const unsigned int comparison_percentage = static_cast<unsigned int>((modification_percentage * 167772.16) + 0.5), dump_run_end = (number_of_runs / 10) + 1;
	plf::nanotimer full_time;
	full_time.start();

	for (unsigned int run_number = 0; run_number != dump_run_end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element);
					++current_element;
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
				}
			}

			if (container.empty())
			{
				for (unsigned int element_number = 0; element_number != number_of_elements / 10; ++element_number)
				{
					container_insert(container);
				}
			}
		}

		end_memory += memory(container);
	}

	end_memory /= dump_run_end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed)

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element);
					++current_element;
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
				}
			}

			if (container.empty())
			{
				for (unsigned int element_number = 0; element_number != number_of_elements / 10; ++element_number)
				{
					container_insert(container);
				}
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_ordered_percentage_remove_if(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const unsigned int modification_percentage)
{
	assert (number_of_elements > 1);

	const unsigned int total_number_of_insertions = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(modification_percentage) / 100.0)) + 0.5);
	double total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer full_time;
	full_time.start();

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			unsigned int number_of_insertions = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= modification_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
					++number_of_insertions;
				}
			}

			container_remove_if(container);

			if (container.empty())
			{
				for (; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
				{
					container_insert(container);
				}
			}
		}

		end_memory += memory(container);
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			unsigned int number_of_insertions = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= modification_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
					++number_of_insertions;
				}
			}

			container_remove_if(container);

			if (container.empty())
			{
				for (; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
				{
					container_insert(container);
				}
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_ordered_small_percentage_remove_if(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const double modification_percentage)
{
	assert (number_of_elements > 1);

	double total = 0;
	unsigned int end_memory = 0, num_erasures;
	const unsigned int comparison_percentage = static_cast<unsigned int>((modification_percentage * 167772.16) + 0.5), dump_run_end = (number_of_runs / 10) + 1;
	plf::nanotimer full_time;
	full_time.start();

	for (unsigned int run_number = 0; run_number != dump_run_end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element);
					++current_element;
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
					++num_erasures;
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
				}
			}

			if (num_erasures != 0)
			{
				container_remove_if(container);
			}

			if (container.empty())
			{
				for (unsigned int element_number = 0; element_number != number_of_elements / 10; ++element_number)
				{
					container_insert(container);
				}
			}
		}

		end_memory += memory(container);
	}

	end_memory /= dump_run_end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed)

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element);
					++current_element;
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
					++num_erasures;
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
				}
			}

			if (num_erasures != 0)
			{
				container_remove_if(container);
			}

			if (container.empty())
			{
				for (unsigned int element_number = 0; element_number != number_of_elements / 10; ++element_number)
				{
					container_insert(container);
				}
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_ordered_percentage_remove_if_standard(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const unsigned int modification_percentage)
{
	assert (number_of_elements > 1);

	const unsigned int total_number_of_insertions = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(modification_percentage) / 100.0)) + 0.5);
	double total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer full_time;
	full_time.start();

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			unsigned int number_of_insertions = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= modification_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
					++number_of_insertions;
				}
			}

			container_remove_if2(container);

			if (container.empty())
			{
				for (; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
				{
					container_insert(container);
				}
			}
		}

		end_memory += memory(container);
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			unsigned int number_of_insertions = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if (rand_within(100) >= modification_percentage)
				{
					total += container_iterate(container, current_element++);
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
					++number_of_insertions;
				}
			}

			container_remove_if2(container);

			if (container.empty())
			{
				for (; number_of_insertions != total_number_of_insertions; ++number_of_insertions)
				{
					container_insert(container);
				}
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_ordered_small_percentage_remove_if_standard(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int number_of_cycles, const double modification_percentage)
{
	assert (number_of_elements > 1);

	double total = 0;
	unsigned int end_memory = 0, num_erasures;
	const unsigned int comparison_percentage = static_cast<unsigned int>((modification_percentage * 167772.16) + 0.5), dump_run_end = (number_of_runs / 10) + 1;
	plf::nanotimer full_time;
	full_time.start();

	for (unsigned int run_number = 0; run_number != dump_run_end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element);
					++current_element;
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
					++num_erasures;
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
				}
			}

			if (num_erasures != 0)
			{
				container_remove_if2(container);
			}

			if (container.empty())
			{
				for (unsigned int element_number = 0; element_number != number_of_elements / 10; ++element_number)
				{
					container_insert(container);
				}
			}
		}

		end_memory += memory(container);
	}

	end_memory /= dump_run_end;

	std::cerr << "Dump total and time and memory: " << total << full_time.get_elapsed_us() << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed)

	full_time.start();

	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		for (unsigned int cycle = 0; cycle != number_of_cycles; ++cycle)
		{
			num_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{ // substituting bitwise-and for modulo for speed:
				if ((plf::rand() & 16777215) >= comparison_percentage)
				{
					total += container_iterate(container, current_element);
					++current_element;
				}
				else if ((plf::rand() & 1) == 0)
				{
					container_erase(container, current_element);
					++num_erasures;
				}
				else
				{
					container_ordered_insert(container, current_element);
					++current_element;
				}
			}

			if (num_erasures != 0)
			{
				container_remove_if(container);
			}

			if (container.empty())
			{
				for (unsigned int element_number = 0; element_number != number_of_elements / 10; ++element_number)
				{
					container_insert(container);
				}
			}
		}
	}

	const double total_time = full_time.get_elapsed_us();

	std::cout << ", " << (total_time / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_reinsertion(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage > 0 && erasure_percentage < 100); // Ie. lower than 100%
	assert (number_of_elements > 1);


	const unsigned int erasure_limit = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(erasure_percentage) / 100.0)) + 0.5);
	const unsigned int erasure_percent_expanded = static_cast<unsigned int>((static_cast<double>(erasure_percentage) * 1.28) + 0.5);
	unsigned int number_of_erasures;
	double insert_time = 0, erase_time = 0, total_size = 0;
	plf::nanotimer insert_timer, erase_timer;


	// Dump-runs to get the cache 'warmed up':
	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		number_of_erasures = 0;
		erase_timer.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) >= erasure_percent_expanded)
			{
				++current_element;
			}
			else
			{
				container_erase(container, current_element);

				if (++number_of_erasures == erasure_limit)
				{
					break;
				}
			}
		}

		if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
		{
			for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}
		}

		erase_time += erase_timer.get_elapsed_us();

		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;
	insert_time = 0; erase_time = 0; total_size = 0;



	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		number_of_erasures = 0;
		erase_timer.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) >= erasure_percent_expanded)
			{
				++current_element;
			}
			else
			{
				container_erase(container, current_element);

				if (++number_of_erasures == erasure_limit)
				{
					break;
				}
			}
		}

		if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
		{
			for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}
		}

		erase_time += erase_timer.get_elapsed_us();

		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;


	if (output_csv)
	{
		std::cout << ", " << (insert_time / number_of_runs) << ", ";

		if (erasure_percentage != 0)
		{
			std::cout << (erase_time / number_of_runs);
		}
	}
	else
	{
		std::cout << "Insert " << number_of_elements << " elements: " << (insert_time / number_of_runs) << "us" << std::endl;
		std::cout << "Randomly erase " << erasure_percentage << "% of elements: " << (erase_time / number_of_runs) << "us" << std::endl;
	}


	// Do whole process one more time so we have something for iteration test, without copying/moving containers:
	container_type container;

	{
		// Insert test
		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		// Erase test
		if (erasure_percentage != 0)
		{
			number_of_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if ((plf::rand() & 127) >= erasure_percent_expanded)
				{
					++current_element;
				}
				else
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}

			if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
			{
				for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
				{
					if ((plf::rand() & 127) < erasure_percent_expanded)
					{
						container_erase(container, current_element);

						if (++number_of_erasures == erasure_limit)
						{
							break;
						}
					}
				}
			}
		}
	}


	iteration_test(container, number_of_runs * 1000, output_csv);

	const unsigned int number_of_inserts = static_cast<unsigned int>(static_cast<float>(number_of_elements) * (static_cast<float>(erasure_percentage) / 300));

	for (unsigned int element_number = 0; element_number != number_of_inserts; ++element_number)
	{
		container_insert(container);
	}

	iteration_test(container, number_of_runs * 1000, output_csv);


	if (output_csv)
	{
		std::cout << "\n";
	}
	else
	{
		std::cout << "\n\n";
	}
}



template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_remove_if_reinsertion(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage > 0 && erasure_percentage < 100); // Ie. lower than 100%
	assert (number_of_elements > 1);


	const unsigned int erasure_limit = static_cast<unsigned int>((static_cast<double>(number_of_elements) * (static_cast<double>(erasure_percentage) / 100.0)) + 0.5);
	const unsigned int erasure_percent_expanded = static_cast<unsigned int>((static_cast<double>(erasure_percentage) * 1.28) + 0.5);

	unsigned int number_of_erasures;
	double insert_time = 0, erase_time = 0, total_size = 0;
	plf::nanotimer insert_timer, erase_timer;


	// Dump-runs to get the cache 'warmed up':
	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		number_of_erasures = 0;
		erase_timer.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) >= erasure_percent_expanded)
			{
				++current_element;
			}
			else
			{
				container_erase(container, current_element);

				if (++number_of_erasures == erasure_limit)
				{
					break;
				}
			}
		}

		if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
		{
			for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}
		}

		container_remove_if(container);

		erase_time += erase_timer.get_elapsed_us();

		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;
	insert_time = 0; erase_time = 0; total_size = 0;



	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		// Insert test
		insert_timer.start();

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		insert_time += insert_timer.get_elapsed_us();


		// Erase test
		if (erasure_percentage == 0)
		{
			continue;
		}

		number_of_erasures = 0;
		erase_timer.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) >= erasure_percent_expanded)
			{
				++current_element;
			}
			else
			{
				container_erase(container, current_element);

				if (++number_of_erasures == erasure_limit)
				{
					break;
				}
			}
		}

		if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
		{
			for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
			{
				if ((plf::rand() & 127) < erasure_percent_expanded)
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}
		}


		container_remove_if(container);

		erase_time += erase_timer.get_elapsed_us();

		total_size += container.size();
	}


	// To stop compiler optimising out the above loop
	std::cerr << "Dump times and size: " << insert_time << ", " << erase_time << ", " << total_size << std::endl;


	if (output_csv)
	{
		std::cout << ", " << (insert_time / number_of_runs) << ", ";

		if (erasure_percentage != 0)
		{
			std::cout << (erase_time / number_of_runs);
		}
	}
	else
	{
		std::cout << "Insert " << number_of_elements << " elements: " << (insert_time / number_of_runs) << "us" << std::endl;
		std::cout << "Randomly erase " << erasure_percentage << "% of elements: " << (erase_time / number_of_runs) << "us" << std::endl;
	}


	// Do whole process one more time so we have something for iteration test, without copying/moving containers:
	container_type container;

	{
		// Insert test
		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_insert(container);
		}

		// Erase test
		if (erasure_percentage != 0)
		{
			number_of_erasures = 0;

			for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
			{
				if ((plf::rand() & 127) >= erasure_percent_expanded)
				{
					++current_element;
				}
				else
				{
					container_erase(container, current_element);

					if (++number_of_erasures == erasure_limit)
					{
						break;
					}
				}
			}

			if (number_of_erasures != erasure_limit) // If not enough erasures have occured, reverse_iterate until they have - this prevents differences in container size during iteration
			{
				for (typename container_type::iterator current_element = --(container.end()); current_element != container.begin(); --current_element)
				{
					if ((plf::rand() & 127) < erasure_percent_expanded)
					{
						container_erase(container, current_element);

						if (++number_of_erasures == erasure_limit)
						{
							break;
						}
					}
				}
			}

			container_remove_if(container);
		}
	}


	iteration_test(container, number_of_runs * 1000, output_csv);

	const unsigned int number_of_inserts = static_cast<unsigned int>(static_cast<float>(number_of_elements) * (static_cast<float>(erasure_percentage) / 300));

	for (unsigned int element_number = 0; element_number != number_of_inserts; ++element_number)
	{
		container_insert(container);
	}

	iteration_test(container, number_of_runs * 1000, output_csv);


	if (output_csv)
	{
		std::cout << "\n";
	}
	else
	{
		std::cout << "\n\n";
	}
}





template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_range(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage < 100); // Ie. lower than 100%
	assert (min_number_of_elements > 1);
	assert (min_number_of_elements < max_number_of_elements);

	if (output_csv)
	{
		if (erasure_percentage == 0)
		{
			std::cout << "Number of elements, Insertion,, Iteration," << std::endl;
		}
		else
		{
			std::cout << "Number of elements, Insertion, Erasure, Iteration" << std::endl;
		}
	}


	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		if (output_csv)
		{
			std::cout << number_of_elements;
		}

		benchmark<container_type>(number_of_elements, 10000000 / number_of_elements, erasure_percentage, output_csv, reserve);
	}

	if (output_csv)
	{
		std::cout << "\n,,,\n,,,\n";
	}
}




template <class container_type>
void benchmark_range_remove_if(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage < 100); // Ie. lower than 100%
	assert (min_number_of_elements > 1);
	assert (min_number_of_elements < max_number_of_elements);

	if (output_csv)
	{
		if (erasure_percentage == 0)
		{
			std::cout << "Number of elements, Insertion,, Iteration," << std::endl;
		}
		else
		{
			std::cout << "Number of elements, Insertion, Erasure, Iteration" << std::endl;
		}
	}


	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		if (output_csv)
		{
			std::cout << number_of_elements;
		}

		benchmark_remove_if<container_type>(number_of_elements, 1000000 / number_of_elements, erasure_percentage, output_csv, reserve);
	}

	if (output_csv)
	{
		std::cout << "\n,,,\n,,,\n";
	}
}



template <class container_type>
void benchmark_range_remove_if_standard(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage < 100); // Ie. lower than 100%
	assert (min_number_of_elements > 1);
	assert (min_number_of_elements < max_number_of_elements);

	if (output_csv)
	{
		if (erasure_percentage == 0)
		{
			std::cout << "Number of elements, Insertion,, Iteration," << std::endl;
		}
		else
		{
			std::cout << "Number of elements, Insertion, Erasure, Iteration" << std::endl;
		}
	}


	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		if (output_csv)
		{
			std::cout << number_of_elements;
		}

		benchmark_remove_if_standard<container_type>(number_of_elements, 1000000 / number_of_elements, erasure_percentage, output_csv, reserve);
	}

	if (output_csv)
	{
		std::cout << "\n,,,\n,,,\n";
	}
}





template <class container_type>
void benchmark_range_remove_if_reinsertion(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage < 100); // Ie. lower than 100%
	assert (min_number_of_elements > 1);
	assert (min_number_of_elements < max_number_of_elements);

	if (output_csv)
	{
		if (erasure_percentage == 0)
		{
			std::cout << "Number of elements, Insertion,, Iteration," << std::endl;
		}
		else
		{
			std::cout << "Number of elements, Insertion, Erasure, Iteration" << std::endl;
		}
	}


	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		if (output_csv)
		{
			std::cout << number_of_elements;
		}

		benchmark_remove_if_reinsertion<container_type>(number_of_elements, 1000000 / number_of_elements, erasure_percentage, output_csv, reserve);
	}

	if (output_csv)
	{
		std::cout << "\n,,,\n,,,\n";
	}
}


template <class container_type>
void benchmark_range_reinsertion(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (erasure_percentage < 100); // Ie. lower than 100%
	assert (min_number_of_elements > 1);
	assert (min_number_of_elements < max_number_of_elements);

	if (output_csv)
	{
		std::cout << "Number of elements, Insertion, Erase " << erasure_percentage << "%, Iteration, Iteration after reinsert " << static_cast<unsigned int>(static_cast<float>(erasure_percentage) / 3.0f) << "%\n";
	}

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		if (output_csv)
		{
			std::cout << number_of_elements;
		}

		benchmark_reinsertion<container_type>(number_of_elements, 1000000 / number_of_elements, erasure_percentage, output_csv, reserve);
	}

	if (output_csv)
	{
		std::cout << "\n,,,\n,,,\n";
	}
}






template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_erasure_range(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int initial_erasure_percentage, const unsigned int erasure_addition, const unsigned int max_erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (initial_erasure_percentage < 100); // Ie. lower than 100%
	assert (min_number_of_elements > 1);
	assert (min_number_of_elements < max_number_of_elements);

	for (unsigned int erasure_percentage = initial_erasure_percentage; erasure_percentage < max_erasure_percentage; erasure_percentage += erasure_addition)
	{
		benchmark_range<container_type>(min_number_of_elements, max_number_of_elements, multiply_factor, erasure_percentage, output_csv, reserve);
		std::cout << "\n,,,\n,,,\n";
	}
}


template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_erasure_if_range(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int initial_erasure_percentage, const unsigned int erasure_addition, const unsigned int max_erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (initial_erasure_percentage < 100); // Ie. lower than 100%
	assert (min_number_of_elements > 1);
	assert (min_number_of_elements < max_number_of_elements);

	for (unsigned int erasure_percentage = initial_erasure_percentage; erasure_percentage < max_erasure_percentage; erasure_percentage += erasure_addition)
	{
		benchmark_range_remove_if<container_type>(min_number_of_elements, max_number_of_elements, multiply_factor, erasure_percentage, output_csv, reserve);
		std::cout << "\n,,,\n,,,\n";
	}
}


template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_erasure_if_range_reinsertion(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int initial_erasure_percentage, const unsigned int erasure_addition, const unsigned int max_erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (initial_erasure_percentage < 100); // Ie. lower than 100%
	assert (min_number_of_elements > 1);
	assert (min_number_of_elements < max_number_of_elements);

	for (unsigned int erasure_percentage = initial_erasure_percentage; erasure_percentage < max_erasure_percentage; erasure_percentage += erasure_addition)
	{
		benchmark_range_remove_if_reinsertion<container_type>(min_number_of_elements, max_number_of_elements, multiply_factor, erasure_percentage, output_csv, reserve);
		std::cout << "\n,,,\n,,,\n";
	}
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_erasure_range_reinsertion(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int initial_erasure_percentage, const unsigned int erasure_addition, const unsigned int max_erasure_percentage, const bool output_csv = false, const bool reserve = false)
{
	assert (initial_erasure_percentage < 100); // Ie. lower than 100%
	assert (min_number_of_elements > 1);
	assert (min_number_of_elements < max_number_of_elements);

	for (unsigned int erasure_percentage = initial_erasure_percentage; erasure_percentage < max_erasure_percentage; erasure_percentage += erasure_addition)
	{
		benchmark_range_reinsertion<container_type>(min_number_of_elements, max_number_of_elements, multiply_factor, erasure_percentage, output_csv, reserve);
		std::cout << "\n,,,\n,,,\n";
	}
}




template <class container_type>
void benchmark_range_stack(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const bool output_csv = false, const bool reserve = false)
{
	if (output_csv)
	{
		std::cout << "Number of elements, Push, Back/Pop, Total time" << std::endl;
	}

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		if (output_csv)
		{
			std::cout << number_of_elements;
		}

		benchmark_stack<container_type>(number_of_elements, 100000000 / number_of_elements, output_csv, reserve);
	}

	if (output_csv)
	{
		std::cout << "\n,,,\n,,,\n";
	}
}





template <class container_type>
void benchmark_range_queue(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const bool output_csv = false, const bool reserve = false)
{
	if (output_csv)
	{
		std::cout << "Number of elements, Push, Back/Pop, Total time" << std::endl;
	}

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		if (output_csv)
		{
			std::cout << number_of_elements;
		}

		benchmark_queue<container_type>(number_of_elements, 100000000 / number_of_elements, output_csv, reserve);
	}

	if (output_csv)
	{
		std::cout << "\n,,,\n,,,\n";
	}
}





template <class container_type>
void benchmark_range_queue_pump(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const float pump_factor, const double multiply_factor, const bool output_csv = false, const bool reserve = false)
{
	if (output_csv)
	{
		std::cout << "Number of elements, Total time, Memory" << std::endl;
	}

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		if (output_csv)
		{
			std::cout << number_of_elements;
		}

		benchmark_queue_pump<container_type>(number_of_elements, 10000000 / number_of_elements, pump_factor, output_csv, reserve);
	}

	if (output_csv)
	{
		std::cout << "\n,,\n,,\n";
	}
}





template <class container_type>
void benchmark_range_general_use(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const unsigned int initial_number_of_modifications, const unsigned int max_number_of_modifications, const unsigned int number_of_modification_addition_amount)
{
	for (double number_of_modifications = initial_number_of_modifications; number_of_modifications <= max_number_of_modifications; number_of_modifications += number_of_modification_addition_amount)
	{
		std::cout << "Number of modifications during 1 frame: " << number_of_modifications << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_general_use<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_modifications);
		}

		std::cout << "\n,,,\n,,,\n";
	}
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



template <class container_type>
void benchmark_range_general_use_small_percentage(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const double initial_erasure_percentage, const double max_erasure_percentage, const double erasure_addition)
{
	for (double erasure_percentage = initial_erasure_percentage; erasure_percentage < max_erasure_percentage; erasure_percentage += erasure_addition)
	{
		std::cout << "Erasure percentage: " << erasure_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_general_use_small_percentage<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, erasure_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}




template <class container_type>
void benchmark_range_general_use_remove_if_small_percentage(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const double initial_erasure_percentage, const double max_erasure_percentage, const double erasure_addition)
{
	for (double erasure_percentage = initial_erasure_percentage; erasure_percentage < max_erasure_percentage; erasure_percentage += erasure_addition)
	{
		std::cout << "Erasure percentage (remove_if): " << erasure_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_general_use_remove_if_small_percentage<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, erasure_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}



template <class container_type>
void benchmark_range_general_use_remove_if(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const unsigned int initial_number_of_modifications, const unsigned int max_number_of_modifications, const unsigned int number_of_modification_addition_amount)
{
	for (double number_of_modifications = initial_number_of_modifications; number_of_modifications <= max_number_of_modifications; number_of_modifications += number_of_modification_addition_amount)
	{
		std::cout << "Number of modifications during 1 frame: " << number_of_modifications << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_general_use_remove_if<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, number_of_modifications);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}




template <class container_type>
void benchmark_range_general_use_remove_if_percentage(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const unsigned int initial_erasure_percentage, const unsigned int max_erasure_percentage, const unsigned int erasure_addition)
{
	for (unsigned int erasure_percentage = initial_erasure_percentage; erasure_percentage < max_erasure_percentage; erasure_percentage += erasure_addition)
	{
		std::cout << "Erasure percentage (remove_if): " << erasure_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_general_use_remove_if_percentage<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, erasure_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}




template <class container_type>
void benchmark_range_ordered_small_percentage(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const double initial_modification_percentage, const double max_modification_percentage, const double modification_addition)
{
	for (double modification_percentage = initial_modification_percentage; modification_percentage < max_modification_percentage; modification_percentage += modification_addition)
	{
		std::cout << "Modification percentage: " << modification_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_ordered_small_percentage<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, modification_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}




template <class container_type>
void benchmark_range_ordered_percentage(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const unsigned int initial_modification_percentage, const unsigned int max_modification_percentage, const unsigned int modification_addition)
{
	for (unsigned int modification_percentage = initial_modification_percentage; modification_percentage < max_modification_percentage; modification_percentage += modification_addition)
	{
		std::cout << "Modification percentage: " << modification_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_ordered_percentage<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, modification_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}



template <class container_type>
void benchmark_range_ordered_small_percentage_remove_if(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const double initial_modification_percentage, const double max_modification_percentage, const double modification_addition)
{
	for (double modification_percentage = initial_modification_percentage; modification_percentage < max_modification_percentage; modification_percentage += modification_addition)
	{
		std::cout << "Modification percentage: " << modification_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_ordered_small_percentage_remove_if<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, modification_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}




template <class container_type>
void benchmark_range_ordered_percentage_remove_if(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const unsigned int initial_modification_percentage, const unsigned int max_modification_percentage, const unsigned int modification_addition)
{
	for (unsigned int modification_percentage = initial_modification_percentage; modification_percentage < max_modification_percentage; modification_percentage += modification_addition)
	{
		std::cout << "Modification percentage: " << modification_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_ordered_percentage_remove_if<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, modification_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}



template <class container_type>
void benchmark_range_ordered_small_percentage_remove_if_standard(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const double initial_modification_percentage, const double max_modification_percentage, const double modification_addition)
{
	for (double modification_percentage = initial_modification_percentage; modification_percentage < max_modification_percentage; modification_percentage += modification_addition)
	{
		std::cout << "Modification percentage: " << modification_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_ordered_small_percentage_remove_if_standard<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, modification_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}




template <class container_type>
void benchmark_range_ordered_percentage_remove_if_standard(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int number_of_cycles, const unsigned int initial_modification_percentage, const unsigned int max_modification_percentage, const unsigned int modification_addition)
{
	for (unsigned int modification_percentage = initial_modification_percentage; modification_percentage < max_modification_percentage; modification_percentage += modification_addition)
	{
		std::cout << "Modification percentage: " << modification_percentage << "\n\nNumber of elements, Total time, Memory Usage\n";

		for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
		{
			std::cout << number_of_elements;

			benchmark_ordered_percentage_remove_if_standard<container_type>(number_of_elements, (1000 / number_of_elements) + 1, number_of_cycles, modification_percentage);
		}

		std::cout << "\n,,,\n,,,\n";
	}
}



template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_sort(const unsigned int number_of_elements, const unsigned int number_of_runs)
{
	assert (number_of_elements > 1);

	double total = 0, iterate_time_total = 0, sort_time_total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer iterate_time, sort_time;

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 65535);
		}

		end_memory += memory(container);

		sort_time.start();
		container_sort(container);
		sort_time_total += sort_time.get_elapsed_ms();

		iterate_time.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		iterate_time_total += iterate_time.get_elapsed_ms();
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << sort_time_total << iterate_time_total << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.

	iterate_time_total = 0;
	sort_time_total = 0;


	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 65535);
		}

		sort_time.start();
		container_sort(container);
		sort_time_total += sort_time.get_elapsed_ms();

		iterate_time.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		iterate_time_total += iterate_time.get_elapsed_ms();
	}

	std::cout << ", " << (sort_time_total / static_cast<double>(number_of_runs)) << ", " << (iterate_time_total / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}



template <class container_type>
void benchmark_range_sort(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor)
{
	std::cout << "\n\nNumber of elements, Sort time, Post-sort iteration time, Memory Usage\n";

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		std::cout << number_of_elements;

		benchmark_sort<container_type>(number_of_elements, (10000000 / number_of_elements) + 1);
	}
}






template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_indiesort(const unsigned int number_of_elements, const unsigned int number_of_runs)
{
	assert (number_of_elements > 1);

	double total = 0, iterate_time_total = 0, sort_time_total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer iterate_time, sort_time;

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 65535);
		}

		end_memory += memory(container);

		sort_time.start();
		plf::indiesort(container);
		sort_time_total += sort_time.get_elapsed_ms();

		iterate_time.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		iterate_time_total += iterate_time.get_elapsed_ms();
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << sort_time_total << iterate_time_total << end_memory << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.

	iterate_time_total = 0;
	sort_time_total = 0;


	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 65535);
		}

		sort_time.start();
		plf::indiesort(container);
		sort_time_total += sort_time.get_elapsed_ms();

		iterate_time.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		iterate_time_total += iterate_time.get_elapsed_ms();
	}

	std::cout << ", " << (sort_time_total / static_cast<double>(number_of_runs)) << ", " << (iterate_time_total / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl; // To prevent compiler from optimizing out both inner loops (ie. total must have a side effect or it'll be removed) - no kidding, gcc will actually do this with std::vector.
}



template <class container_type>
void benchmark_range_indiesort(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor)
{
	std::cout << "\n\nNumber of elements, Sort time, Post-sort iteration time, Memory Usage\n";

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		std::cout << number_of_elements;

		benchmark_indiesort<container_type>(number_of_elements, (10000000 / number_of_elements) + 1);
	}
}






template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_reverse(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage, const bool do_sort)
{
	assert (number_of_elements > 1);

	const unsigned int erasure_percent_expanded = static_cast<unsigned int>((static_cast<double>(erasure_percentage) * 1.28) + 0.5);
	double total = 0, iterate_time_total = 0, reverse_time_total = 0;
	plf::nanotimer iterate_time, reverse_time;

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 65535);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);
			}
			else
			{
				++current_element;
			}
		}

		if (do_sort)
		{
			container_sort(container);
		}

		reverse_time.start();
		container_reverse(container);
		reverse_time_total += reverse_time.get_elapsed_ms();

		iterate_time.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		iterate_time_total += iterate_time.get_elapsed_ms();
	}

	std::cerr << "Dump total and time and memory: " << total << reverse_time_total << iterate_time_total << std::endl;

	iterate_time_total = 0;
	reverse_time_total = 0;


	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 65535);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);
			}
			else
			{
				++current_element;
			}

		}

		if (do_sort)
		{
			container_sort(container);
		}

		reverse_time.start();
		container_reverse(container);
		reverse_time_total += reverse_time.get_elapsed_ms();

		iterate_time.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		iterate_time_total += iterate_time.get_elapsed_ms();
	}


	std::cout << ", " << (reverse_time_total / static_cast<double>(number_of_runs)) << ", " << (iterate_time_total / static_cast<double>(number_of_runs)) << "\n";
	std::cerr << "Dump total: " << total << std::endl;
}



template <class container_type>
void benchmark_range_reverse(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage, const bool do_sort = false)
{
	std::cout << "\n\nNumber of elements, reverse time, Post-reverse iteration time\n";

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		std::cout << number_of_elements;

		benchmark_reverse<container_type>(number_of_elements, (10000000 / number_of_elements) + 1, erasure_percentage, do_sort);
	}
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_remove(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage)
{
	assert (number_of_elements > 1);

	const unsigned int erasure_percent_expanded = static_cast<unsigned int>((static_cast<double>(erasure_percentage) * 1.28) + 0.5);
	double total = 0, remove_time_total = 0;
	plf::nanotimer remove_time;

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 31);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);
			}
			else
			{
				++current_element;
			}

		}

		remove_time.start();
		container_remove(container, 30);
		remove_time_total += remove_time.get_elapsed_ms();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}
	}

	std::cerr << "Dump total and time and memory: " << total << remove_time_total << std::endl;

	remove_time_total = 0;


	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 31);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);
			}
			else
			{
				++current_element;
			}

		}

		remove_time.start();
		container_remove(container, 30);
		remove_time_total += remove_time.get_elapsed_ms();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}
	}

	std::cout << ", " << (remove_time_total / static_cast<double>(number_of_runs)) << "\n";
	std::cerr << "Dump total: " << total << std::endl;
}



template <class container_type>
void benchmark_range_remove(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage)
{
	std::cout << "\n\nNumber of elements, remove time\n";

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		std::cout << number_of_elements;

		benchmark_remove<container_type>(number_of_elements, (10000000 / number_of_elements) + 1, erasure_percentage);
	}
}





template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_unique(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage)
{
	assert (number_of_elements > 1);

	const unsigned int erasure_percent_expanded = static_cast<unsigned int>((static_cast<double>(erasure_percentage) * 1.28) + 0.5);
	double total = 0, unique_time_total = 0;
	plf::nanotimer unique_time;

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 15);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);
			}
			else
			{
				++current_element;
			}

		}

		unique_time.start();
		container_unique(container);
		unique_time_total += unique_time.get_elapsed_ms();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}
	}

	std::cerr << "Dump total and time: " << total << unique_time_total << std::endl;

	unique_time_total = 0;


	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 15);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);
			}
			else
			{
				++current_element;
			}

		}

		unique_time.start();
		container_unique(container);
		unique_time_total += unique_time.get_elapsed_ms();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}
	}

	std::cout << ", " << (unique_time_total / static_cast<double>(number_of_runs)) << "\n";
	std::cerr << "Dump total: " << total << std::endl;
}



template <class container_type>
void benchmark_range_unique(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage)
{
	std::cout << "\n\nNumber of elements, unique time\n";

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		std::cout << number_of_elements;

		benchmark_unique<container_type>(number_of_elements, (10000000 / number_of_elements) + 1, erasure_percentage);
	}
}




template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_clear(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage)
{
	assert (number_of_elements > 1);

	const unsigned int erasure_percent_expanded = static_cast<unsigned int>((static_cast<double>(erasure_percentage) * 1.28) + 0.5);
	double total = 0, iterate_time_total = 0, clear_time_total = 0;
	unsigned int end_memory = 0;
	plf::nanotimer iterate_time, clear_time;

	const unsigned int end = (number_of_runs / 10) + 1;
	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 15);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);
			}
			else
			{
				++current_element;
			}

		}

		end_memory += memory(container);

		iterate_time.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		iterate_time_total += iterate_time.get_elapsed_ms();

		clear_time.start();
		container.clear();
		clear_time_total += clear_time.get_elapsed_ms();
	}

	end_memory /= end;

	std::cerr << "Dump total and time and memory: " << total << clear_time_total << iterate_time_total << end_memory << std::endl;

	iterate_time_total = 0;
	clear_time_total = 0;


	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 15);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end();)
		{
			if ((plf::rand() & 127) < erasure_percent_expanded)
			{
				container_erase(container, current_element);
			}
			else
			{
				++current_element;
			}

		}

		iterate_time.start();

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		iterate_time_total += iterate_time.get_elapsed_ms();

		clear_time.start();
		container.clear();
		clear_time_total += clear_time.get_elapsed_ms();
	}

	std::cout << ", " << (clear_time_total / static_cast<double>(number_of_runs)) << ", " << (iterate_time_total / static_cast<double>(number_of_runs)) << ", " << end_memory << "\n";
	std::cerr << "Dump total: " << total << std::endl;
}



template <class container_type>
void benchmark_range_clear(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage)
{
	std::cout << "\n\nNumber of elements, clear time\n";

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		std::cout << number_of_elements;

		benchmark_clear<container_type>(number_of_elements, (10000000 / number_of_elements) + 1, erasure_percentage);
	}
}



template <class container_type>
PLF_BENCH_FORCE_INLINE void benchmark_destruction(const unsigned int number_of_elements, const unsigned int number_of_runs, const unsigned int erasure_percentage, const bool do_sort)
{
	assert (number_of_elements > 1);

	double total = 0, destruction_time_total = 0;
	plf::nanotimer destruction_time;

	const unsigned int end = (number_of_runs / 10) + 1;
	destruction_time.start();

	for (unsigned int run_number = 0; run_number != end; ++run_number)
	{
		destruction_time_total += destruction_time.get_elapsed_ms();
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 65535);
		}

		if (do_sort)
		{
			container_sort(container);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		destruction_time.start();
	}

	destruction_time_total += destruction_time.get_elapsed_ms();

	std::cerr << "Dump total and time: " << total << destruction_time_total << std::endl;

	destruction_time_total = 0;
	destruction_time.start();


	for (unsigned int run_number = 0; run_number != number_of_runs; ++run_number)
	{
		destruction_time_total += destruction_time.get_elapsed_ms();
		container_type container;

		for (unsigned int element_number = 0; element_number != number_of_elements; ++element_number)
		{
			container_push_back(container, plf::rand() & 65535);
		}

		if (do_sort)
		{
			container_sort(container);
		}

		for (typename container_type::iterator current_element = container.begin(); current_element != container.end(); ++current_element)
		{
			total += container_iterate(container, current_element);
		}

		destruction_time.start();
	}

	destruction_time_total += destruction_time.get_elapsed_ms();

	std::cout << ", " << (destruction_time_total / static_cast<double>(number_of_runs)) << ", " << "\n";
	std::cerr << "Dump total: " << total << std::endl;
}



template <class container_type>
void benchmark_range_destruction(const unsigned int min_number_of_elements, const unsigned int max_number_of_elements, const double multiply_factor, const unsigned int erasure_percentage, const bool do_sort = false)
{
	std::cout << "\n\nNumber of elements, destruction time\n";

	for (unsigned int number_of_elements = min_number_of_elements; number_of_elements <= max_number_of_elements; number_of_elements = static_cast<unsigned int>(static_cast<double>(number_of_elements) * multiply_factor))
	{
		std::cout << number_of_elements;

		benchmark_destruction<container_type>(number_of_elements, (10000000 / number_of_elements) + 1, erasure_percentage, do_sort);
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


#endif // PLF_BENCH_H
