cmake_minimum_required(VERSION 3.12)

include(CMakePackageConfigHelpers)

SET(curr_version 1.0)

project(BudgetRKV2Extractor VERSION ${curr_version} 
				DESCRIPTION "A shitty RKV2 extractor"
				LANGUAGES CXX)
				
add_subdirectory(src)