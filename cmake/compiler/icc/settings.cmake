add_definitions(-D_BUILD_DIRECTIVE="'${CMAKE_BUILD_TYPE}'")

if( WITH_WARNINGS )
  add_definitions(-w1)
  message(STATUS "ICC: All warnings enabled")
endif()
