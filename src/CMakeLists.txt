cmake_minimum_required(VERSION 3.9)

set(odbc_wrapper_path "D:/Projects/Import/odbc-cpp-wrapper/src")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${odbc_wrapper_path}")
link_directories("${CMAKE_SOURCE_DIR}/lib/${CMAKE_CFG_INTDIR}")

find_package(ODBC)

add_definitions(/D_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING /DDEBUG_TABLE /DODBC_STATIC)

add_executable(table_gui main.cpp DbTable.cpp table_corr.cpp table_weight.cpp)
target_link_libraries(table_gui odbccpp_static.lib P7.lib nana_v142.lib ODBC::ODBC Ws2_32.lib)
target_compile_features(table_gui PRIVATE cxx_std_17)
set_property(TARGET table_gui PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:Debug>DLL")