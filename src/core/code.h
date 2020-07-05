#pragma once

#include <array>

#include "glog/logging.h"

template <int NUM_DIGITS, int NUM_COLORS>
class Code {
 public:
  Code() : code_(CreateCodeZero()) {}

  Code(std::array<int, NUM_DIGITS> code) : code_(code) {
    if (!ValidateCodeColors()) LOG(FATAL) << "Invalid color!";  // TODO(bpeele) verbose output...
  }

  int operator[](int i) const {
    if (i < 0 || i >= NUM_DIGITS) LOG(FATAL) << "Inavlid digit!";  // TODO(bpeele) verbose output...
    return code_[i];
  }

  Code Next() const {
    auto next_code = code_;

    // TODO(bpeele) check for max!!!!

    for (int i = NUM_DIGITS - 1; i >= 0; i--) {
      if (next_code[i] < (NUM_COLORS - 1)) {
        next_code[i]++;
        return {next_code};
      } else {
        next_code[i] = 0;
      }
    }

    LOG(FATAL) << "Undefined behavior!";
    return {};
  }

  std::pair<int, int> EvaluateGuess(const Code &guess) const {
    // TODO(bpeele) more elegant eval?

    int num_right_color_right_place = 0;
    int num_right_color_wrong_place = 0;

    std::array<bool, NUM_DIGITS> guess_used;
    std::fill(std::begin(guess_used), std::end(guess_used), false);
    auto code_used = guess_used;

    for (int i = 0; i < NUM_DIGITS; i++) {
      if (code_[i] == guess[i]) {
        num_right_color_right_place++;
        code_used[i] = true;
        guess_used[i] = true;
      }
    }

    for (int i = 0; i < NUM_DIGITS; i++) {
      if (!code_used[i]) {
        for (int j = 0; j < NUM_DIGITS; j++) {
          if (!guess_used[j]) {
            if (!code_used[i] && code_[i] == guess[j]) {
              num_right_color_wrong_place++;
              code_used[i] = true;
              guess_used[j] = true;
            }
          }
        }
      }
    }

    return {num_right_color_right_place, num_right_color_wrong_place};
  }

  friend std::ostream &operator<<(std::ostream &os, const Code &code) {
    os << "[";
    std::string delimiter = "";
    for (int i = 0; i < NUM_DIGITS; i++) {
      os << delimiter << code[i];
      delimiter = ", ";
    }
    os << "]";
    return os;
  }

 private:
  std::array<int, NUM_DIGITS> CreateCodeZero() const {
    std::array<int, NUM_DIGITS> zero;
    std::fill(std::begin(zero), std::end(zero), 0);
    return zero;
  }

  bool ValidateCodeColors() {
    for (int i = 0; i < NUM_DIGITS; i++) if (code_[i] >= NUM_COLORS || code_[i] < 0) return false;
    return true;
  }

  const std::array<int, NUM_DIGITS> code_;
};


using Code4d6c = Code<4, 6>;
using Code4d8c = Code<4, 8>;
