#pragma once

#include <format>
#include <random>

namespace Core {
class Random {
 public:
  static void Init() { _random.seed(std::random_device()()); }

  static float Float() {
    return static_cast<float>(_distribution(_random)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
  }

  static std::string generate() {
    static constexpr char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    constexpr size_t len = 16;

    static thread_local std::mt19937_64 gen{std::random_device{}()};
    static thread_local std::uniform_int_distribution<size_t> dist(0, std::size(charset) - 2);

    std::string id(len, '\0');
    for (auto& c : id) {
      c = charset[dist(gen)];
    }

    return id;
  }

 private:
  static std::mt19937 _random;
  static std::uniform_int_distribution<std::mt19937::result_type> _distribution;
};
}  // namespace Core