# 1.0-F3 — F1 end-to-end login flow

STATUS: EXECUTED / DELIVERED / PASS (test-key scope)

This objective composes the completed F1 RX session state machine with the existing F1:01 C3 request builder and proves the ordered transaction over a real local TCP connection.

Sequence: server F1:00 -> client validation/capture HeroKey -> client F1:01 C3 request -> server acceptance -> server F1:01 success -> client LoggedIn.

Golden inputs retained from prior objectives: F1:00 c10cf100011234342e2f3d3d; F1:01 success c105f10101. Request length is 79 bytes with C3 framing under the deterministic test-key fixture already used by TS-13.

Validation: C++17, -Wall -Wextra -Wpedantic -pthread, real TCP loopback on 127.0.0.1 with ephemeral port. Observed: TS-18 F1 end-to-end flow loopback: PASS. Exit 0.

Boundary: deterministic test-key scope; production Enc1.dat interoperability and original Windows/MU runtime integration remain unverified.
