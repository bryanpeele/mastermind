#include <iostream>  // TODO(bpeele) remove me
#include <math.h>
#include <memory>

#include "code.h"

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
    LOG(FATAL) << "Not yet imeplemented!";  // TODO(bpeele) implement
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