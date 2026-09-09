/*
  NEW-ERA MVP Windows Client Harness (WIN-1)
  - Connect to NEW-ERA Test Server (TCP)
  - Send 1 modern frame to trigger TS-2 scripted replies
  - Receive replies and (when id==0x000C) parse inner classic frames using MVP core parsers:
      - F3:03 CharacterInfo (C1 fixed 0x42)
      - F3:10 Inventory (C2 variable)

  NOTE: this harness uses Winsock directly; the protocol logic comes from mvp_login_client.cpp.
*/
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

// Pull in the NEW-ERA MVP core (parsers/builders/worldstate) as a single TU include,
// matching the existing harness pattern on Linux.
#define NEWERA_MVP_NO_MAIN
#include "../mvp_login_client.cpp"

static uint16_t ReadLE16(const uint8_t* p) {
  return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}
static uint32_t ReadLE32(const uint8_t* p) {
  return (uint32_t)p[0]
      | ((uint32_t)p[1] << 8)
      | ((uint32_t)p[2] << 16)
      | ((uint32_t)p[3] << 24);
}
static void WriteLE16(uint8_t* p, uint16_t v) {
  p[0] = (uint8_t)(v & 0xFF);
  p[1] = (uint8_t)((v >> 8) & 0xFF);
}
static void WriteLE32(uint8_t* p, uint32_t v) {
  p[0] = (uint8_t)(v & 0xFF);
  p[1] = (uint8_t)((v >> 8) & 0xFF);
  p[2] = (uint8_t)((v >> 16) & 0xFF);
  p[3] = (uint8_t)((v >> 24) & 0xFF);
}

static bool ReadExact(SOCKET s, void* buf, size_t n) {
  uint8_t* p = (uint8_t*)buf;
  size_t got = 0;
  while (got < n) {
    int rc = recv(s, (char*)(p + got), (int)(n - got), 0);
    if (rc <= 0) return false;
    got += (size_t)rc;
  }
  return true;
}

static bool WriteAll(SOCKET s, const void* buf, size_t n) {
  const uint8_t* p = (const uint8_t*)buf;
  size_t sent = 0;
  while (sent < n) {
    int rc = send(s, (const char*)(p + sent), (int)(n - sent), 0);
    if (rc <= 0) return false;
    sent += (size_t)rc;
  }
  return true;
}

static std::string HexPrefix(const std::vector<uint8_t>& v, size_t maxBytes) {
  size_t n = v.size() < maxBytes ? v.size() : maxBytes;
  std::string out;
  char tmp[8];
  for (size_t i = 0; i < n; i++) {
    std::snprintf(tmp, sizeof(tmp), "%02x", (unsigned)v[i]);
    out += tmp;
    if (i + 1 != n) out += ' ';
  }
  return out;
}

static void TryParseClassic(const std::vector<uint8_t>& frame) {
  std::string err;

  // F3:03 (CharacterInfo) - C1 fixed size, layout: [C1][0x42][F3][03]...
  if (frame.size() >= 4 && frame[0] == 0xC1 && frame[2] == 0xF3 && frame[3] == 0x03) {
    newera::mvp::CharacterInfoF3_03_Event ev{};
    bool ok = newera::mvp::ParseFrame_CharacterInfo_F3_03_C1(frame, &ev, &err);
    if (!ok) {
      std::cout << "  parse F3:03 FAIL: " << err << "\n";
      return;
    }
    std::cout << "  parse F3:03 OK: x=" << (int)ev.x << " y=" << (int)ev.y
              << " map=" << (int)ev.map << " dir=" << (int)ev.dir
              << " money=" << ev.money << "\n";
    return;
  }

  // F3:10 (Inventory) - C2 variable size BE, layout: [C2][szH][szL][F3][10][count]...
  if (frame.size() >= 6 && frame[0] == 0xC2 && frame[3] == 0xF3 && frame[4] == 0x10) {
    newera::mvp::InventoryF3_10_Event inv{};
    bool ok = newera::mvp::ParseFrame_InventoryF3_10_C2(frame, &inv, &err);
    if (!ok) {
      std::cout << "  parse F3:10 FAIL: " << err << "\n";
      return;
    }
    std::cout << "  parse F3:10 OK: count=" << (int)inv.count << "\n";
    return;
  }

  std::cout << "  (no parser hooked for this classic frame)\n";
}

int main(int argc, char** argv) {
  const char* host = (argc >= 2) ? argv[1] : "127.0.0.1";
  const uint16_t port = (argc >= 3) ? (uint16_t)std::atoi(argv[2]) : (uint16_t)55901;

  WSADATA wsa{};
  if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
    std::cerr << "WSAStartup failed\n";
    return 2;
  }

  SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s == INVALID_SOCKET) {
    std::cerr << "socket() failed\n";
    WSACleanup();
    return 2;
  }

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
    std::cerr << "inet_pton failed for host=" << host << "\n";
    closesocket(s);
    WSACleanup();
    return 2;
  }

  std::cout << "[WIN-1] connect " << host << ":" << port << " ...\n";
  if (connect(s, (sockaddr*)&addr, sizeof(addr)) != 0) {
    std::cerr << "connect() failed\n";
    closesocket(s);
    WSACleanup();
    return 2;
  }
  std::cout << "[WIN-1] connected\n";

  // Send one modern frame (id=0x0007, size=5, body="abcde") to trigger TS-2.
  uint8_t hdr[6];
  WriteLE16(hdr + 0, 0x0007);
  WriteLE32(hdr + 2, 5);
  uint8_t body[5] = { 'a','b','c','d','e' };

  if (!WriteAll(s, hdr, sizeof(hdr)) || !WriteAll(s, body, sizeof(body))) {
    std::cerr << "[WIN-1] send failed\n";
    closesocket(s);
    WSACleanup();
    return 2;
  }
  std::cout << "[WIN-1] sent trigger\n";

  for (int i = 0; i < 3; i++) {
    uint8_t rh[6];
    if (!ReadExact(s, rh, sizeof(rh))) {
      std::cerr << "[WIN-1] read header failed at i=" << i << "\n";
      break;
    }
    uint16_t id = ReadLE16(rh + 0);
    uint32_t sz = ReadLE32(rh + 2);
    std::vector<uint8_t> rb(sz);
    if (sz && !ReadExact(s, rb.data(), rb.size())) {
      std::cerr << "[WIN-1] read body failed at i=" << i << "\n";
      break;
    }

    std::cout << "[WIN-1] RX#" << i << " id=0x" << std::hex << (unsigned)id << std::dec
              << " size=" << sz << " body_prefix=[" << HexPrefix(rb, 16) << "]\n";

    if (id == 0x000C) {
      TryParseClassic(rb);
    }
  }

  closesocket(s);
  WSACleanup();
  return 0;
}
