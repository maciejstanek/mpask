cmake_minimum_required(VERSION 3.8)
project(mpask)

file(GLOB SOURCES "*.cxx")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cxx")
add_library(mpask STATIC ${SOURCES})

find_package(Boost 1.65 COMPONENTS program_options REQUIRED)
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

### EXECUTABLE ###

add_executable(mpask_exe "main.cxx")
target_compile_features(mpask_exe PUBLIC cxx_std_17)
target_include_directories(mpask_exe PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(mpask_exe LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(mpask_exe PRIVATE mpask)
