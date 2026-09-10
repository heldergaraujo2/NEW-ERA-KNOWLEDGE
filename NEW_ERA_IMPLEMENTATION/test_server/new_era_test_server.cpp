/*
  NEW-ERA Test Server (TS-0/TS-1/TS-2)
  - TCP listen (0.0.0.0:PORT)
  - accept 1 client
  - read modern header: id:u16 LE + size:u32 LE (6 bytes)
  - clamp size
  - read body
  - if id == 0x000C (BOTH_MESSAGE): log first bytes of inner classic frame (C1/C2/...)
  - TS-2: after first RX packet, TX scripted:
      - F3:03 CharacterInfo (from rx_character_info_f3_03_vectors.json, vector charinfo_basic_nontrivial)
      - F3:10 Inventory count1 (from rx_inventory_f3_10_vectors.json, vector inv_f3_10_count1_slot0_pattern)
      - F3:10 Inventory count0 (from rx_inventory_f3_10_vectors.json, vector inv_f3_10_count0_empty)

  NOTE: This is a TEST SERVER, not MU. Focus is fidelity of framing + vectors, not gameplay.
*/

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#ifdef _WIN32
  #ifndef NOMINMAX
  #define NOMINMAX
  #endif
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
  using socket_t = SOCKET;
  static constexpr socket_t kInvalidSocket = INVALID_SOCKET;
#else
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <unistd.h>
  using socket_t = int;
  static constexpr socket_t kInvalidSocket = -1;
#endif

static void CloseSocket(socket_t s) {
#ifdef _WIN32
  closesocket(s);
#else
  close(s);
#endif
}

static bool SocketInit(std::string* err) {
#ifdef _WIN32
  WSADATA wsa;
  const int rc = WSAStartup(MAKEWORD(2,2), &wsa);
  if (rc != 0) {
    if (err) *err = "WSAStartup failed: " + std::to_string(rc);
    return false;
  }
#else
  (void)err;
#endif
  return true;
}

static void SocketShutdown() {
#ifdef _WIN32
  WSACleanup();
#endif
}

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

static bool ReadExact(socket_t s, void* buf, size_t n, std::string* err) {
  uint8_t* p = reinterpret_cast<uint8_t*>(buf);
  size_t got = 0;
  while (got < n) {
#ifdef _WIN32
    int rc = recv(s, reinterpret_cast<char*>(p + got), (int)(n - got), 0);
    if (rc == 0) { if (err) *err = "peer closed"; return false; }
    if (rc == SOCKET_ERROR) { if (err) *err = "recv failed"; return false; }
#else
    ssize_t rc = recv(s, p + got, n - got, 0);
    if (rc == 0) { if (err) *err = "peer closed"; return false; }
    if (rc < 0) { if (err) *err = "recv failed"; return false; }
#endif
    got += (size_t)rc;
  }
  return true;
}

static bool WriteAll(socket_t s, const void* buf, size_t n, std::string* err) {
  const uint8_t* p = reinterpret_cast<const uint8_t*>(buf);
  size_t sent = 0;
  while (sent < n) {
#ifdef _WIN32
    int rc = send(s, reinterpret_cast<const char*>(p + sent), (int)(n - sent), 0);
    if (rc == SOCKET_ERROR || rc == 0) { if (err) *err = "send failed"; return false; }
#else
    ssize_t rc = send(s, p + sent, n - sent, 0);
    if (rc <= 0) { if (err) *err = "send failed"; return false; }
#endif
    sent += (size_t)rc;
  }
  return true;
}

static std::string HexDumpPrefix(const std::vector<uint8_t>& v, size_t maxBytes) {
  const size_t n = (v.size() < maxBytes) ? v.size() : maxBytes;
  std::string out;
  out.reserve(n * 3);
  char tmp[8];
  for (size_t i = 0; i < n; i++) {
    std::snprintf(tmp, sizeof(tmp), "%02x", (unsigned)v[i]);
    out += tmp;
    if (i + 1 != n) out += ' ';
  }
  return out;
}

static bool ReadFileToString(const std::string& path, std::string* out, std::string* err) {
  std::ifstream f(path, std::ios::binary);
  if (!f) {
    if (err) *err = "cannot open file: " + path;
    return false;
  }
  std::string s;
  f.seekg(0, std::ios::end);
  std::streamoff n = f.tellg();
  if (n < 0) n = 0;
  f.seekg(0, std::ios::beg);
  s.resize((size_t)n);
  if (!s.empty()) {
    f.read(&s[0], (std::streamsize)s.size());
    if (!f) {
      if (err) *err = "read failed: " + path;
      return false;
    }
  }
  *out = std::move(s);
  return true;
}

static int HexNibble(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
  if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
  return -1;
}

static bool HexToBytes(const std::string& hex, std::vector<uint8_t>* out, std::string* err) {
  if ((hex.size() % 2) != 0) {
    if (err) *err = "hex length must be even";
    return false;
  }
  std::vector<uint8_t> v;
  v.reserve(hex.size() / 2);
  for (size_t i = 0; i < hex.size(); i += 2) {
    int a = HexNibble(hex[i]);
    int b = HexNibble(hex[i + 1]);
    if (a < 0 || b < 0) {
      if (err) *err = "invalid hex at offset " + std::to_string(i);
      return false;
    }
    v.push_back((uint8_t)((a << 4) | b));
  }
  *out = std::move(v);
  return true;
}

// Minimal JSON extractor for our known vectors files:
// find {"name":"X", ... "frame_hex":"..."} and return frame_hex.
// This is intentionally tiny to keep build trivial on Windows (no external json lib).
static bool ExtractFrameHexByVectorName(
    const std::string& jsonText,
    const std::string& vectorName,
    std::string* frameHex,
    std::string* err) {

  const std::string needleName = "\"name\": \"" + vectorName + "\"";
  size_t pos = jsonText.find(needleName);
  if (pos == std::string::npos) {
    if (err) *err = "vector name not found: " + vectorName;
    return false;
  }

  // Search forward (bounded) for "frame_hex".
  size_t posFrame = jsonText.find("\"frame_hex\"", pos);
  if (posFrame == std::string::npos) {
    if (err) *err = "frame_hex not found near vector: " + vectorName;
    return false;
  }

  size_t posColon = jsonText.find(':', posFrame);
  if (posColon == std::string::npos) {
    if (err) *err = "malformed JSON near frame_hex";
    return false;
  }

  size_t q1 = jsonText.find('"', posColon);
  if (q1 == std::string::npos) {
    if (err) *err = "malformed JSON (missing opening quote for frame_hex)";
    return false;
  }
  size_t q2 = jsonText.find('"', q1 + 1);
  if (q2 == std::string::npos) {
    if (err) *err = "malformed JSON (missing closing quote for frame_hex)";
    return false;
  }

  *frameHex = jsonText.substr(q1 + 1, q2 - (q1 + 1));
  return true;
}

static bool LoadClassicFrameFromVectorsFile(
    const std::string& vectorsPath,
    const std::string& vectorName,
    std::vector<uint8_t>* classicFrame,
    std::string* err) {

  std::string js;
  if (!ReadFileToString(vectorsPath, &js, err)) return false;

  std::string frameHex;
  if (!ExtractFrameHexByVectorName(js, vectorName, &frameHex, err)) return false;

  std::vector<uint8_t> bytes;
  if (!HexToBytes(frameHex, &bytes, err)) return false;

  if (bytes.empty()) {
    if (err) *err = "classic frame is empty for vector: " + vectorName;
    return false;
  }
  const uint8_t t = bytes[0];
  if (t != 0xC1 && t != 0xC2 && t != 0xC3 && t != 0xC4) {
    if (err) *err = "classic frame does not start with C1/C2/C3/C4 for vector: " + vectorName;
    return false;
  }

  *classicFrame = std::move(bytes);
  return true;
}

static bool SendModernFrame(socket_t s, uint16_t id, const std::vector<uint8_t>& body, std::string* err) {
  uint8_t hdr[6];
  WriteLE16(hdr + 0, id);
  WriteLE32(hdr + 2, (uint32_t)body.size());

  if (!WriteAll(s, hdr, sizeof(hdr), err)) return false;
  if (!body.empty()) {
    if (!WriteAll(s, body.data(), body.size(), err)) return false;
  }
  return true;
}

static bool SendBothMessage(socket_t s, const std::vector<uint8_t>& classicFrame, std::string* err) {
  static constexpr uint16_t kProto_BOTH_MESSAGE = 0x000C;
  return SendModernFrame(s, kProto_BOTH_MESSAGE, classicFrame, err);
}

static socket_t CreateListenSocket(uint16_t port, std::string* err) {
  socket_t s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s == kInvalidSocket) {
    if (err) *err = "socket() failed";
    return kInvalidSocket;
  }

  int opt = 1;
#ifdef _WIN32
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

#ifdef _WIN32
  if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
#else
  if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
#endif
    if (err) *err = "bind() failed (port in use?)";
    CloseSocket(s);
    return kInvalidSocket;
  }

#ifdef _WIN32
  if (listen(s, 1) == SOCKET_ERROR) {
#else
  if (listen(s, 1) != 0) {
#endif
    if (err) *err = "listen() failed";
    CloseSocket(s);
    return kInvalidSocket;
  }

  return s;
}

int main(int argc, char** argv) {
  const uint16_t port = (argc >= 2) ? (uint16_t)std::atoi(argv[1]) : (uint16_t)55901;
  const std::string repoRoot = (argc >= 3) ? std::string(argv[2]) : std::string(".");
  static constexpr uint32_t kMaxBody = 256 * 1024; // clamp against absurd sizes
  static constexpr uint16_t kProto_BOTH_MESSAGE = 0x000C;

  const std::string pathCharInfo = repoRoot + "/rx_character_info_f3_03_vectors.json";
  const std::string pathInv = repoRoot + "/rx_inventory_f3_10_vectors.json";

  std::string err;
  if (!SocketInit(&err)) {
    std::cerr << "SocketInit error: " << err << "\n";
    return 2;
  }

  socket_t ls = CreateListenSocket(port, &err);
  if (ls == kInvalidSocket) {
    std::cerr << "Listen error: " << err << "\n";
    SocketShutdown();
    return 2;
  }

  std::cout << "[NEW-ERA TS] Listening on 0.0.0.0:" << port << " ...\n";
  std::cout << "[NEW-ERA TS] repoRoot=" << repoRoot << "\n";
  for (;;) {

  sockaddr_in peer{};
#ifdef _WIN32
  int peerLen = sizeof(peer);
#else
  socklen_t peerLen = sizeof(peer);
#endif
  socket_t cs = accept(ls, reinterpret_cast<sockaddr*>(&peer), &peerLen);
  if (cs == kInvalidSocket) {
    std::cerr << "accept() failed\n";
    CloseSocket(ls);
    SocketShutdown();
    return 2;
  }

  char peerIp[64]{};
  inet_ntop(AF_INET, &peer.sin_addr, peerIp, sizeof(peerIp));
  std::cout << "[NEW-ERA TS] Client connected from " << peerIp << ":" << ntohs(peer.sin_port) << "\n";

  // Load scripted frames now (fail fast if vectors missing).
  std::vector<uint8_t> classic_f3_03;
  std::vector<uint8_t> classic_f3_10_count1;
  std::vector<uint8_t> classic_f3_10_count0;

  if (!LoadClassicFrameFromVectorsFile(pathCharInfo, "charinfo_basic_nontrivial", &classic_f3_03, &err)) {
    std::cerr << "[NEW-ERA TS] Failed loading charinfo vector: " << err << "\n";
    CloseSocket(cs); CloseSocket(ls); SocketShutdown();
    return 2;
  }
  if (!LoadClassicFrameFromVectorsFile(pathInv, "inv_f3_10_count1_slot0_pattern", &classic_f3_10_count1, &err)) {
    std::cerr << "[NEW-ERA TS] Failed loading inventory count1 vector: " << err << "\n";
    CloseSocket(cs); CloseSocket(ls); SocketShutdown();
    return 2;
  }
  if (!LoadClassicFrameFromVectorsFile(pathInv, "inv_f3_10_count0_empty", &classic_f3_10_count0, &err)) {
    std::cerr << "[NEW-ERA TS] Failed loading inventory count0 vector: " << err << "\n";
    CloseSocket(cs); CloseSocket(ls); SocketShutdown();
    return 2;
  }

  bool scriptedSent = false;

  for (;;) {
    uint8_t hdr[6];
    if (!ReadExact(cs, hdr, sizeof(hdr), &err)) {
      std::cout << "[NEW-ERA TS] Read header ended: " << err << "\n";
      break;
    }

    const uint16_t id = ReadLE16(hdr + 0);
    const uint32_t size = ReadLE32(hdr + 2);

    if (size > kMaxBody) {
      std::cout << "[NEW-ERA TS] Disconnect: absurd size=" << size << " (max " << kMaxBody << ")\n";
      break;
    }

    std::vector<uint8_t> body(size);
    if (size > 0) {
      if (!ReadExact(cs, body.data(), body.size(), &err)) {
        std::cout << "[NEW-ERA TS] Read body ended: " << err << "\n";
        break;
      }
    }

    std::cout << "[NEW-ERA TS] RX id=0x" << std::hex << (unsigned)id << std::dec
              << " size=" << size;

    if (id == kProto_BOTH_MESSAGE) {
      std::cout << " BOTH_MESSAGE";
      if (!body.empty()) {
        std::cout << " classic_prefix=[" << HexDumpPrefix(body, 16) << "]";
        const uint8_t classicType = body[0];
        if (classicType == 0xC1 || classicType == 0xC2 || classicType == 0xC3 || classicType == 0xC4) {
          std::cout << " classic_type=0x" << std::hex << (unsigned)classicType << std::dec;
          if (body.size() >= 4) {
            std::cout << " classic_head=0x" << std::hex << (unsigned)body[2] << std::dec;
          }
        }
      }
    }
    std::cout << "\n";

    // TS-2: after first RX, send scripted classic frames inside BOTH_MESSAGE.
    if (!scriptedSent) {
      scriptedSent = true;
      std::cout << "[NEW-ERA TS] TX scripted BOTH_MESSAGE: F3:03 (" << classic_f3_03.size()
                << " B), F3:10 count1 (" << classic_f3_10_count1.size()
                << " B), F3:10 count0 (" << classic_f3_10_count0.size() << " B)\n";

      if (!SendBothMessage(cs, classic_f3_03, &err)) {
        std::cout << "[NEW-ERA TS] TX failed (F3:03): " << err << "\n";
        break;
      }
      if (!SendBothMessage(cs, classic_f3_10_count1, &err)) {
        std::cout << "[NEW-ERA TS] TX failed (F3:10 count1): " << err << "\n";
        break;
      }
      if (!SendBothMessage(cs, classic_f3_10_count0, &err)) {
        std::cout << "[NEW-ERA TS] TX failed (F3:10 count0): " << err << "\n";
        break;
      }
    }
  }

  CloseSocket(cs);
  std::cout << "[NEW-ERA TS] Client disconnected\n";
  }

  CloseSocket(ls);
  SocketShutdown();
  return 0;
}
