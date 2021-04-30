file(GLOB CPP_SOURCES "*.cpp")

add_executable(BudgetRKV2Extractor ${CPP_SOURCES})
target_compile_features(BudgetRKV2Extractor PUBLIC cxx_std_17)
set_target_properties(BudgetRKV2Extractor PROPERTIES CXX_EXTENSIONS OFF
													 RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
target_link_libraries(BudgetRKV2Extractor -static)