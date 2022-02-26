add_library(plf_benchmark INTERFACE)
target_include_directories(plf INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

function(plf_add_benchmark SRC)
	get_filename_component(SRCNAME ${SRC} NAME_WE)
	get_filename_component(SRCDIRFULL ${SRC} DIRECTORY)
	get_filename_component(SRCDIR ${SRCDIRFULL} NAME)
	set(FSRCNAME "${SRCDIR}_${SRCNAME}")
	plf_add_test(${FSRCNAME} ${SRC})
	target_link_libraries(${FSRCNAME} INTERFACE plf_benchmark)
endfunction()

file(GLOB_RECURSE PLF_BENCHMARKS
	LIST_DIRECTORIES False
	plf/individual_tests/*.cpp)

foreach(FN ${PLF_BENCHMARKS})
	message(STATUS ${FN})
	plf_add_benchmark(${FN})
endforeach()
