cmake_minimum_required(VERSION 3.8)
project(mpask)

file(GLOB RESOURCES "*.*")
remove_vim_swap_files("${RESOURCES}" PRUNED_RESOURCES)

foreach(RESOURCE ${PRUNED_RESOURCES})
  get_filename_component(RESOURCE_FILE ${RESOURCE} NAME)
  message("[INFO   ] Adding a test resource '${RESOURCE_FILE}'.")
  configure_file(${RESOURCE} ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_FILE})
endforeach(RESOURCE)
