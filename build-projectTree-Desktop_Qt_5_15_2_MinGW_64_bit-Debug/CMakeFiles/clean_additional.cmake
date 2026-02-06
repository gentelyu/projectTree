# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\projectTree_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\projectTree_autogen.dir\\ParseCache.txt"
  "projectTree_autogen"
  )
endif()
