cmake_minimum_required(VERSION 3.16)

project(new_era_win_client LANGUAGES CXX)

add_executable(new_era_win_client
  win_client.cpp
)

target_compile_features(new_era_win_client PRIVATE cxx_std_17)

if (WIN32)
  target_link_libraries(new_era_win_client PRIVATE ws2_32)
endif()
