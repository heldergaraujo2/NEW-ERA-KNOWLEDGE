# 1.0-E — F1 RX handshake validation + login result parser

**STATUS: EXECUTED / DELIVERED / PASS**

## Objective
Close the remaining client-side RX items in the normative MVP login trace: validate the F1:00 handshake version encoding and parse/map the S→C F1:01 login result.

## Evidence basis
NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md §4.1, §4.3 and §6 establish the fixed 12-byte F1:00 handshake, Version[i]-(i+1) comparison, F1:01 result as C1 Size>=5 with Value at byte 4, core values 0/1/2/3/4/6, compatibility success 0x20, and generic default.

## Delivered
- NEW_ERA_IMPLEMENTATION/mvp_login/f1_login_rx.h
- NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_login_rx/test_f1_login_rx_loopback.cpp
- CMake target ts16_f1_login_rx_loopback
- negative tests for version mismatch and truncated result
- real TCP loopback

## Golden vectors
F1:00: c10cf100011234342e2f3d3d
F1:01 success: c105f10101
F1:01 compatibility success: c105f10120

## Validation
C++17 with g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread; real TCP loopback on 127.0.0.1 with ephemeral port.
Observed: TS-16 F1:00 validation + F1:01 result RX TCP loopback: PASS
Exit status: 0.

## Boundaries
No claim of original Windows/MU runtime integration. Values whose emitters were not recovered remain compatibility/default behavior only. F1:00 server-version storage representation remains the documented hypothesis; this validates the client-side comparison rule only.
