cmake_minimum_required(VERSION 3.16)

project(new_era_test_server LANGUAGES CXX)

add_executable(new_era_test_server
  new_era_test_server.cpp
)

target_compile_features(new_era_test_server PRIVATE cxx_std_17)

add_executable(ts4_f3_13_both_message_loopback
  ts4_f3_13_both_message_loopback.cpp
)
target_compile_features(ts4_f3_13_both_message_loopback PRIVATE cxx_std_17)

add_executable(ts5_f3_13_scripted_sequence_loopback
  ts5_f3_13_scripted_sequence_loopback.cpp
)
target_compile_features(ts5_f3_13_scripted_sequence_loopback PRIVATE cxx_std_17)

add_executable(ts6_f3_13_multi_accept_loopback
  ts6_f3_13_multi_accept_loopback.cpp
)
target_compile_features(ts6_f3_13_multi_accept_loopback PRIVATE cxx_std_17)

add_executable(ts8_combat_action_skill_rx_loopback
  ../mvp_login/loopback_combat_action_skill_rx/test_combat_action_skill_loopback.cpp
)
target_include_directories(ts8_combat_action_skill_rx_loopback PRIVATE
  ../mvp_login
)
target_compile_features(ts8_combat_action_skill_rx_loopback PRIVATE cxx_std_17)

add_executable(ts9_f3_13_equipment_apply_loopback
  ../mvp_login/loopback_f3_13_equipment_apply/test_f3_13_equipment_apply_loopback.cpp
)
target_include_directories(ts9_f3_13_equipment_apply_loopback PRIVATE
  ../mvp_login
)
target_compile_features(ts9_f3_13_equipment_apply_loopback PRIVATE cxx_std_17)

add_executable(ts10_both_attack_tx_loopback
  ../mvp_login/loopback_both_attack_tx/test_both_attack_tx.cpp
)
target_include_directories(ts10_both_attack_tx_loopback PRIVATE
  ../mvp_login
)
target_compile_features(ts10_both_attack_tx_loopback PRIVATE cxx_std_17)

if (WIN32)
  target_link_libraries(new_era_test_server PRIVATE ws2_32)
  target_link_libraries(ts4_f3_13_both_message_loopback PRIVATE ws2_32)
  target_link_libraries(ts5_f3_13_scripted_sequence_loopback PRIVATE ws2_32)
  target_link_libraries(ts6_f3_13_multi_accept_loopback PRIVATE ws2_32)
  target_link_libraries(ts8_combat_action_skill_rx_loopback PRIVATE ws2_32)
  target_link_libraries(ts9_f3_13_equipment_apply_loopback PRIVATE ws2_32)
  target_link_libraries(ts10_both_attack_tx_loopback PRIVATE ws2_32)
endif()
