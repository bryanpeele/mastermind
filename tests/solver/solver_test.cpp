#include "solver.h"

#include "gtest/gtest.h"

TEST(Solver, InitialGuess) {
  constexpr int num_digits = 4;
  constexpr int num_colors = 3;
  auto solver = Solver<num_digits, num_colors>();
  auto initial_guess = solver.InitialGuess();
  EXPECT_EQ(0, initial_guess[0]);
  EXPECT_EQ(0, initial_guess[1]);
  EXPECT_EQ(1, initial_guess[2]);
  EXPECT_EQ(1, initial_guess[3]);
}

TEST(Solver, Solve) {
  constexpr int num_digits = 4;
  constexpr int num_colors = 6;

  const auto solution = Code<num_digits, num_colors>({5, 2, 3, 1});

  auto solver = Solver<num_digits, num_colors>();

  solver.AutomatedGameplay(solution, true);
}

TEST(Solver, Solve2) {
  constexpr int num_digits = 4;
  constexpr int num_colors = 8;

  const auto solution = Code<num_digits, num_colors>({0, 4, 4, 3});

  auto solver = Solver<num_digits, num_colors>();

  solver.AutomatedGameplay(solution, true);
}

TEST(Solver, Solve3) {
  constexpr int num_digits = 5;
  constexpr int num_colors = 6;

  const auto solution = Code<num_digits, num_colors>({0, 4, 4, 3, 2});

  auto solver = Solver<num_digits, num_colors>();

  solver.AutomatedGameplay(solution, true);
}