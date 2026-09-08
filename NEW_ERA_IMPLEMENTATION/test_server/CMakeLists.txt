cmake_minimum_required(VERSION 3.16)

project(new_era_test_server LANGUAGES CXX)

add_executable(new_era_test_server
  new_era_test_server.cpp
)

target_compile_features(new_era_test_server PRIVATE cxx_std_17)

if (WIN32)
  target_link_libraries(new_era_test_server PRIVATE ws2_32)
endif()
