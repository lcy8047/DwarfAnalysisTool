add_library(DwarfAnalysisTool DwarfAnalysisTool.cpp GetResult.cpp)

target_compile_options(DwarfAnalysisTool PUBLIC -g -Wall)
target_include_directories(DwarfAnalysisTool PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(DwarfAnalysisTool dwarf z)

if(APPLE)
    target_include_directories(DwarfAnalysisTool PUBLIC /usr/local/include)
endif()