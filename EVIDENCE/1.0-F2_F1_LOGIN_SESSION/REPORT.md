# 1.0-F2 — F1 login session state machine

STATUS: EXECUTED / DELIVERED / PASS

Composes the proven F1:00 RX handshake and F1:01 RX result rules into an explicit client login-session state machine. No new wire semantics are invented.

States: Disconnected, AwaitingHandshake, AwaitingResult, LoggedIn, Rejected.

F1:00 requires exact 12-byte C1 framing, Result=1, and Version[i] = expected[i]-(i+1); HeroKey is captured from bytes 5-6 as big-endian. F1:01 requires C1 Size>=5; Values 0x01/0x20 map to success, core failure values map to rejection, and unknown values use the documented generic fallback.

Golden F1:00: c10cf100011234342e2f3d3d
Golden F1:01 success: c105f10101
Negative: truncated 11-byte F1:00 rejected; F1:01 Value 0x02 rejected.

Validation: C++17, -Wall -Wextra -Wpedantic. Observed TS-17 F1 login session state-machine loopback: PASS. Exit 0.

Boundary: deterministic state composition only; original Windows/MU runtime and production-key interoperability remain unverified.
