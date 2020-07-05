#include <iostream>  // TODO(bpeele) remove me
#include <math.h>
#include <memory>

#include "code.h"

template <int NUM_DIGITS>
bool ColorAlreadyUsed(const std::array<int, NUM_DIGITS> &raw_code, int digit, int color) {
  for (int i = 0; i < digit; i++) if (raw_code[i] == color) return true;
  return false;
}

template <int NUM_DIGITS, int NUM_COLORS>
void RecursiveSetDigit(std::vector<Code<NUM_DIGITS, NUM_COLORS>> &all_codes,
                       std::array<int, NUM_DIGITS> &raw_code,
                       int digit) {
  if (digit == NUM_DIGITS) {
    all_codes.emplace_back(raw_code);
  } else {
    for (int color = 0; color < NUM_COLORS; color++) {
      if (!ColorAlreadyUsed<NUM_DIGITS>(raw_code, digit, color)) {
        raw_code[digit] = color;
        RecursiveSetDigit<NUM_DIGITS, NUM_COLORS>(all_codes, raw_code, digit + 1);
      }
    }
  }
}

template <int NUM_DIGITS, int NUM_COLORS>
std::vector<Code<NUM_DIGITS, NUM_COLORS>> GenerateAllCodes(bool allow_repeats = false) {
  std::vector<Code<NUM_DIGITS, NUM_COLORS>> all_codes;

  if (allow_repeats) {
    auto code = Code<NUM_DIGITS, NUM_COLORS>();
    all_codes.push_back(code);
    for (int i = 0; i < std::pow(NUM_COLORS, NUM_DIGITS) - 1; i++) {
      all_codes.push_back(all_codes.back().Next());
    }
  } else {
    if (NUM_COLORS < NUM_DIGITS) LOG(FATAL) << "Bad!";
    std::array<int, NUM_DIGITS> raw_code;
    RecursiveSetDigit<NUM_DIGITS, NUM_COLORS>(all_codes, raw_code, 0);
  }

  return all_codes;
}

std::vector<std::pair<int, int>> GenerateAllPegOutputs(int num_digits) {
  std::vector<std::pair<int, int>> pegs;

  for (int num_red = 0; num_red <= num_digits; num_red++) {
    for (int num_white = 0; num_white <= (num_digits - num_red); num_white++) {
      pegs.emplace_back(num_red, num_white);
    }
  }

  return pegs;
}