cmake_minimum_required(VERSION 3.8)
project(mpask)

# find_package(BISON)
# find_package(FLEX)
# BISON_TARGET(mpask_parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cxx)
# FLEX_TARGET(mpask_lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cxx)
# ADD_FLEX_BISON_DEPENDENCY(mpask_lexer mpask_parser)

file(GLOB SOURCES "*.cxx")
# list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cxx") # It is a lib, no nee for a main file
add_library(mpask STATIC ${SOURCES})

find_package(Boost 1.65 REQUIRED)
message("Boost_FOUND = '${Boost_FOUND}'")
message("Boost_LIBRARIES = '${Boost_LIBRARIES}'")
message("Boost_INCLUDE_DIR = '${Boost_INCLUDE_DIR}'")
target_include_directories(mpask PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(mpask LINK_PUBLIC ${Boost_LIBRARIES})

target_include_directories(mpask PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(mpask PUBLIC cxx_std_17)
target_compile_options(mpask PUBLIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)

target_link_libraries(mpask PUBLIC stdc++fs) # experimental/filesystem
target_link_libraries(mpask LINK_PUBLIC ${Boost_LIBRARIES})

# No need for an exec: this is a lib. Moreover, I already have tests.
# add_executable(main "main.cxx")
# target_link_libraries(main PRIVATE mpask)
# target_compile_features(main PUBLIC cxx_std_17)
