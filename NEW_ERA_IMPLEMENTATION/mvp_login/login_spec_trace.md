# MVP LOGIN — trace de conformidade com a spec

Fonte normativa: NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md.

## Pacote 1 — S→C F1:00
- [x] layout 12 B C1
- [x] parser
- [x] client Version[i]-(i+1) validation
- Handler: f1_login_rx.h

## Pacote 2 — C→S F1:01
- [x] logical builder
- [x] BuxConvert
- [x] XorData32 real
- [x] SimpleModulus C3 builder (test-key validation)
- [ ] production Enc1.dat interoperability

## Pacote 3 — S→C F1:01
- [x] C1 result parser, Size>=5
- [x] Value mapping {0,1,2,3,4,6}
- [x] compatibility success 0x20
- [x] generic default
- [x] TCP loopback

## RX boundary
- [x] truncation rejected
- [x] real TCP loopback
- [ ] original Windows/MU runtime
