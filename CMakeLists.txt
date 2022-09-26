cmake_minimum_required(VERSION 3.22)
project(DwarfAnalysisTool)

if (UNIX AND NOT APPLE)
    add_subdirectory(lib)

    add_executable(test test.cpp)
    target_compile_options(test PUBLIC -g -Wall)
    target_include_directories(test PUBLIC include)
    target_link_libraries(test DwarfAnalysisTool dwarf z)
    set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
    message(FATAL_ERROR "This project is linux project")
endif()