set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
  add_definitions(-D_BUILD_DIRECTIVE=\\"$(ConfigurationName)\\")
else()
  add_definitions(-D_BUILD_DIRECTIVE=\\"${CMAKE_BUILD_TYPE}\\")
endif()

if(NOT WITH_WARNINGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4996 /wd4355 /wd4244 /wd4985 /wd4267 /wd4619 /wd4512")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /wd4355 /wd4244 /wd4985 /wd4267 /wd4619 /wd4512")
  message(STATUS "MSVC: Disabled generic compiletime warnings")
endif()
