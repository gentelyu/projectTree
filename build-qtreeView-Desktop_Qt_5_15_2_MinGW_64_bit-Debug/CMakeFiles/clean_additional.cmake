# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\qtreeView_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\qtreeView_autogen.dir\\ParseCache.txt"
  "qtreeView_autogen"
  )
endif()
