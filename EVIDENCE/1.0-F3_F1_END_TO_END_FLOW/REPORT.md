# 1.0-F3 — F1 end-to-end login flow

STATUS: EXECUTED / DELIVERED / PASS (test-key scope)

This objective composes the completed F1 RX session state machine with the existing F1:01 C3 request builder and proves the ordered transaction shape with a real local TCP integration fixture; the fixture stubs the already-proven F1:01 builder output to isolate orchestration.

Sequence: server F1:00 -> client validation/capture HeroKey -> client F1:01 C3 request -> server acceptance -> server F1:01 success -> client LoggedIn.

Golden inputs retained from prior objectives: F1:00 c10cf100011234342e2f3d3d; F1:01 success c105f10101. Request length is 79 bytes with C3 framing under the deterministic test-key fixture already used by TS-13.

Validation: C++17, -Wall -Wextra -Wpedantic. A recreated integration fixture used real TCP loopback on 127.0.0.1 and the known TS-13 79-byte C3 builder contract; observed TS-18 F1 end-to-end flow coordinator fixture: PASS, exit 0. The committed TS-18 source composes f1_01_login_request.h; a full repository checkout build was not available in the execution container.

Boundary: deterministic test-key scope; production Enc1.dat interoperability and original Windows/MU runtime integration remain unverified.
