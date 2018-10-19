cmake_minimum_required(VERSION 3.8)
project(mibparser)

file(GLOB SOURCES "*.cxx")
# list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cxx") # It is a lib, no nee for a main file
add_library(mibparser STATIC ${SOURCES})

# find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
# target_include_directories(mibparser PUBLIC ${Boost_INCLUDE_DIR})
# target_link_libraries(mibparser LINK_PUBLIC ${Boost_LIBRARIES})

#target_include_directories(mibparser PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(mibparser PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(mibparser PUBLIC cxx_std_17) # Since CMake 3.8
# set(CMAKE_CXX_STANDARD 17) # Workaround
target_compile_options(mibparser PUBLIC -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic)

target_link_libraries(mibparser PUBLIC stdc++fs) # experimental/filesystem

# No need for an exec: this is a lib. Moreover, I already have tests.
# add_executable(main "main.cxx")
# target_link_libraries(main PRIVATE mibparser)
# target_compile_features(main PUBLIC cxx_std_17)
