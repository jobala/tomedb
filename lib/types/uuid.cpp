module;
#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
export module uuidv7;

export namespace tome
{
using uuid_v7 = std::array<uint8_t, 16>;

uuid_v7 generate_uuidv7()
{
  uuid_v7 uuid{};

  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  uint64_t millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

  uuid[0] = static_cast<uint8_t>((millis >> 40) & 0xFF);
  uuid[1] = static_cast<uint8_t>((millis >> 32) & 0xFF);
  uuid[2] = static_cast<uint8_t>((millis >> 24) & 0xFF);
  uuid[3] = static_cast<uint8_t>((millis >> 16) & 0xFF);
  uuid[4] = static_cast<uint8_t>((millis >> 8) & 0xFF);
  uuid[5] = static_cast<uint8_t>(millis & 0xFF);

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint16_t> dis(0, 255);
  for (size_t i = 6; i < 16; ++i)
  {
    uuid[i] = static_cast<uint8_t>(dis(gen));
  }

  uuid[6] = (uuid[6] & 0x0F) | 0x70;
  uuid[8] = (uuid[8] & 0x3F) | 0x80;

  return uuid;
}

std::string to_string(const uuid_v7 &uuid)
{
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (size_t i = 0; i < 16; ++i)
  {
    ss << std::setw(2) << static_cast<int>(uuid[i]);
    if (i == 3 || i == 5 || i == 7 || i == 9)
    {
      ss << "-";
    }
  }
  return ss.str();
}
} // namespace tome
