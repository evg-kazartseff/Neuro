message("")
if( UNIX )
  message("* Neuro buildtype	 : ${CMAKE_BUILD_TYPE}")
endif()

# output information about installation-directories and locations

message("* Install to		 : ${CMAKE_INSTALL_PREFIX}")
message("")

# Show infomation about the options selected during configuration

if(BUILD_TRAINING)
  message("* Build training         : Yes")
else()
  message("* Build training	 : No (default)")
endif()

message("")
