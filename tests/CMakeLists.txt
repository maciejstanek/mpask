cmake_minimum_required(VERSION 3.8)
project(mpask)

file(GLOB SOURCE_FILES "../src/*.cxx")
file(GLOB TEST_FILES "*_test.cxx")
message("[INFO   ] Adding a main test executable 'mpask_test'.")
add_executable("mpask_test" ${TEST_FILES})
target_link_libraries("mpask_test" mpask GTest::GTest GTest::Main)
target_compile_features("mpask_test" PUBLIC cxx_std_17) # Since CMake 3.8
add_test("mpask_test" "mpask_test")

foreach(SOURCE_FILE ${SOURCE_FILES})
  get_filename_component(SOURCE ${SOURCE_FILE} NAME_WE)
  set(TEST "${SOURCE}_test")
  set(TEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${TEST}.cxx")
  if(NOT ${TEST_FILE} IN_LIST TEST_FILES)
    message("[WARNING] Missing a unit test '${TEST}'.")
  endif()
endforeach(SOURCE_FILE)

# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/in DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
