cmake_minimum_required(VERSION 3.14)

project(plf)

add_library(plf INTERFACE)
target_include_directories(plf INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_sources(plf INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_colony.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_indiesort.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_packed_deque.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_queue.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_hive.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_list.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_pointer_colony.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_rand.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_indexed_vector.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_nanotimer.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_pointer_deque.h
	${CMAKE_CURRENT_SOURCE_DIR}/include/plf_stack.h
)

target_compile_features(plf INTERFACE cxx_std_20)

if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	function(plf_add_test SRCNAME SRC)
		message(STATUS "Adding test ${SRCNAME}: ${SRC}")
		add_executable(${SRCNAME} ${SRC})
		target_link_libraries(${SRCNAME} PRIVATE plf)
		add_test(${SRCNAME}_test COMMAND ${SRCNAME})
	endfunction()

	add_subdirectory(benchmark)
	add_subdirectory(test)
endif()
