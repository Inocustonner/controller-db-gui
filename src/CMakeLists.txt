cmake_minimum_required(VERSION 3.9)
include_directories("${CMAKE_SOURCE_DIR}/include")
link_directories("${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}")


add_executable(table_gui main.cpp)
find_package(ODBC)
target_link_libraries(table_gui PUBLIC nana_v142.lib ODBC::ODBC odbc_lib.lib p7.lib Ws2_32.lib)
set_property(TARGET table_gui PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:Debug>DLL")

#force c++latest on msvc  
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")