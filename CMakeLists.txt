cmake_minimum_required (VERSION 3.16)
project (CoreHD CXX)

find_package (Boost CONFIG REQUIRED)
find_package (data CONFIG REQUIRED)
find_package (pegtl CONFIG REQUIRED)
find_package (argh CONFIG REQUIRED)
find_package (nlohmann_json CONFIG REQUIRED)
find_package (gigamonkey CONFIG REQUIRED)

add_executable (CoreHD core_hd.cpp)

target_link_libraries (CoreHD PUBLIC
  argh
  gigamonkey::gigamonkey
  data::data
  boost::boost)

target_include_directories (CoreHD PUBLIC include)

target_compile_features (CoreHD PUBLIC cxx_std_20)
set_target_properties (CoreHD PROPERTIES CXX_EXTENSIONS OFF)

option (PACKAGE_TESTS "Build the tests" ON)

add_definitions ("-DHAS_BOOST")
