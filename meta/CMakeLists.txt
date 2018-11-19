cmake_minimum_required(VERSION 3.8)
project(mpask)

find_program(BASH_PROGRAM bash)
if(BASH_PROGRAM)
  list(APPEND CALLBACKS "dot_unflatten" "dot_draw")
  foreach(CALLBACK ${CALLBACKS})
    message("[INFO   ] Adding a test callback '${CALLBACK}'.")
    add_test(${CALLBACK} ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/${CALLBACK}.sh)
  endforeach(CALLBACK)
endif(BASH_PROGRAM)

# foreach(CALLBACK ${CALLBACKS})
#   message("[INFO   ] Adding a test callback '${CALLBACK}'.")
#   configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${CALLBACK}.sh" "${CMAKE_CURRENT_BINARY_DIR}/${CALLBACK}.sh")
#   add_test(NAME ${CALLBACK} COMMAND "./${CALLBACK}.sh") # WORKING_DIRECTORY
# endforeach(CALLBACK)
