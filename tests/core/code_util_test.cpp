#include "code_util.h"

#include "gtest/gtest.h"

TEST(CodeUtil, CodeZeroTest) {
  const auto zero = Code<2, 2>();
  EXPECT_EQ(0, zero[0]);
  EXPECT_EQ(0, zero[1]);
  // std::cout << zero << std::endl;
}

TEST(CodeUtil, CodeTest) {
  const auto code = Code<2, 2>({0, 1});
  EXPECT_EQ(0, code[0]);
  EXPECT_EQ(1, code[1]);
  // std::cout << code << std::endl;
}

TEST(CodeUtil, GenerateAllCodes) {
  constexpr int num_digits = 3;
  constexpr int num_colors = 3;
  constexpr bool allow_repeats = true;
  const auto codes = GenerateAllCodes<num_digits, num_colors>(allow_repeats);
  EXPECT_EQ(std::pow(num_colors, num_digits), codes.size());

  // TODO(bpeele) add real test..
  // std::cout << "Num codes: " << codes.size() << std::endl;
  // for (const auto &code : codes) {
  //   std::cout << "  " << code << std::endl;
  // }
}

TEST(CodeUtil, GenerateAllPegs) {
  constexpr int num_digits = 3;
  const auto pegs = GenerateAllPegOutputs(num_digits);
  // EXPECT_EQ(std::pow(num_colors, num_digits), codes.size());

  // TODO(bpeele) add real test..
  std::cout << "Num pegs: " << pegs.size() << std::endl;
  for (const auto &peg : pegs) {
    std::cout << "  " << peg.first << ", " << peg.second << std::endl;
  }
}

TEST(CodeUtil, CodeEval) {
  const auto code = Code<4, 4>({0, 1, 2, 3});

  {
    const auto guess = Code<4, 4>({0, 1, 2, 3});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(4, eval.first);
    EXPECT_EQ(0, eval.second);
  }

  {
    const auto guess = Code<4, 4>({1, 2, 3, 0});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(0, eval.first);
    EXPECT_EQ(4, eval.second);
  }

  {
    const auto guess = Code<4, 4>({3, 1, 2, 0});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(2, eval.first);
    EXPECT_EQ(2, eval.second);
  }

  {
    const auto guess = Code<4, 4>({0, 0, 0, 0});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(1, eval.first);
    EXPECT_EQ(0, eval.second);
  }

  {
    const auto guess = Code<4, 4>({3, 1, 1, 3});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(2, eval.first);
    EXPECT_EQ(0, eval.second);
  }
}

TEST(CodeUtil, CodeEvalRepeats) {
  const auto code = Code<4, 4>({0, 0, 1, 1});

  {
    const auto guess = Code<4, 4>({0, 0, 1, 1});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(4, eval.first);
    EXPECT_EQ(0, eval.second);
  }


  {
    const auto guess = Code<4, 4>({1, 2, 3, 0});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(0, eval.first);
    EXPECT_EQ(2, eval.second);
  }

  {
    const auto guess = Code<4, 4>({3, 1, 2, 0});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(0, eval.first);
    EXPECT_EQ(2, eval.second);
  }

  {
    const auto guess = Code<4, 4>({0, 0, 0, 0});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(2, eval.first);
    EXPECT_EQ(0, eval.second);
  }

  {
    const auto guess = Code<4, 4>({3, 1, 1, 3});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(1, eval.first);
    EXPECT_EQ(1, eval.second);
  }

  {
    const auto guess = Code<4, 4>({0, 2, 1, 1});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(3, eval.first);
    EXPECT_EQ(0, eval.second);
  }

  {
    const auto guess = Code<4, 4>({0, 1, 2, 1});
    const auto eval = code.EvaluateGuess(guess);
    EXPECT_EQ(2, eval.first);
    EXPECT_EQ(1, eval.second);
  }
}