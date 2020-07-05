#pragma once

#include <list>
#include <memory>

#include "code.h"
#include "code_util.h"

template <int NUM_DIGITS, int NUM_COLORS>
class Solver {
 public:
  Solver(bool allow_repetitions = true)
      : all_peg_outputs_(GenerateAllPegOutputs(NUM_DIGITS)),
        possible_guesses_(GenerateAllCodes<NUM_DIGITS, NUM_COLORS>(true)) {
    // Note possible guesses ALWAYS allow repition, but possible solutions may OR may not include
    // repetitions.
    const auto possible_solutions = allow_repetitions ? possible_guesses_ :
                                    GenerateAllCodes<NUM_DIGITS, NUM_COLORS>(false);
    // Convert vector to list.
    for (const auto &code : possible_solutions) possible_solutions_.emplace_back(code);
  }

  Code<NUM_DIGITS, NUM_COLORS> InitialGuess() {
    if (NUM_COLORS < 2) LOG(FATAL) << "Must be at least two colors";

    std::array<int, NUM_DIGITS> raw_code;
    for (int i = 0; i < NUM_DIGITS; i++) raw_code[i] = (i < (NUM_DIGITS / 2)) ? 0 : 1;

    last_guess_ = std::make_unique<Code<NUM_DIGITS, NUM_COLORS>>(raw_code);

    return {raw_code};
  }

  Code<NUM_DIGITS, NUM_COLORS> NextGuess(int red_pegs, int white_pegs) {
    if (red_pegs == NUM_DIGITS) {
      std::cout << "Y u still guessing? I won!" << std::endl;
      return *last_guess_;
    }

    RemoveInvalidPossibilities(red_pegs, white_pegs);

    if (possible_solutions_.size() ==1 ) {
      last_guess_ = std::make_unique<Code<NUM_DIGITS, NUM_COLORS>>(possible_solutions_.front());
      return possible_solutions_.front();
    }

    const auto next_guess = ApplyMinMax();

    last_guess_ = std::make_unique<Code<NUM_DIGITS, NUM_COLORS>>(next_guess);

    return next_guess;
  }

  // TODO(bpeele) add reset for multiple runs...
  int AutomatedGameplay(const Code<NUM_DIGITS, NUM_COLORS> &solution, bool verbose = false) {
    int num_guesses = 0;
    std::unique_ptr<Code<NUM_DIGITS, NUM_COLORS>> guess;
    std::pair<int, int> output = {0, 0};
    const int max_guesses = 100;

    if (verbose) PrintSolution(solution);

    while (output.first != NUM_DIGITS && num_guesses < max_guesses) {
      guess = (num_guesses == 0) ?
              std::make_unique<Code<NUM_DIGITS, NUM_COLORS>>(InitialGuess()) :
              std::make_unique<Code<NUM_DIGITS, NUM_COLORS>>(NextGuess(output.first, output.second));
      num_guesses++;

      output = solution.EvaluateGuess(*guess);

      if (verbose) {
        PrintGuess(num_guesses, *guess);
        PrintEval(output.first, output.second);
        PrintPossibleSolutions();
      }
    }

    if (output.first == NUM_DIGITS && verbose) {
      std::cout << "Found solution in " << num_guesses << " guesses!";
    }

    if (num_guesses == max_guesses) {
      LOG(WARNING) << "Did not find solution after " << num_guesses << " guesses :(";
    }

    return num_guesses;
  }

 private:
  bool IsPossibleSolution(const Code<NUM_DIGITS, NUM_COLORS> &possible_solution,
                          const Code<NUM_DIGITS, NUM_COLORS> &guess,
                          int red_pegs,
                          int white_pegs) {
    auto output = possible_solution.EvaluateGuess(guess);
    return (red_pegs == output.first && white_pegs == output.second);
  }

  int RemoveInvalidPossibilities(int red_pegs, int white_pegs) {
    auto iter = possible_solutions_.begin();
    while (iter != possible_solutions_.end()) {
      if (!IsPossibleSolution(*iter, *last_guess_, red_pegs, white_pegs)) {
        iter = possible_solutions_.erase(iter);
      } else {
        iter++;
      }
    }

    return possible_solutions_.size();
  }

  int PotentialElimination(const Code<NUM_DIGITS, NUM_COLORS> &guess, int red_pegs, int white_pegs) {
    int num_eliminations = 0;
    for (const auto solution : possible_solutions_) {
      if (!IsPossibleSolution(solution, guess, red_pegs, white_pegs)) num_eliminations++;
    }
    return num_eliminations;
  }

  Code<NUM_DIGITS, NUM_COLORS> ApplyMinMax() {
    int max_min_eliminated = std::numeric_limits<int>::min();

    // TODO(bpeele) handle ties better.....
    std::unique_ptr<Code<NUM_DIGITS, NUM_COLORS>> best_guess =
        std::make_unique<Code<NUM_DIGITS, NUM_COLORS>>(possible_guesses_.front());

    for (const auto &guess : possible_guesses_) {
      int min_eliminated = std::numeric_limits<int>::max();
      for (const auto &pegs : all_peg_outputs_) {
        int num_eliminated = PotentialElimination(guess, pegs.first, pegs.second);
        min_eliminated = std::min(min_eliminated, num_eliminated);
      }
      if (min_eliminated >= max_min_eliminated) {
        max_min_eliminated = min_eliminated;
        best_guess = std::make_unique<Code<NUM_DIGITS, NUM_COLORS>>(guess);
      }
    }

    // TODO(bpeele) hack, DRY, efficiency, etc....this is a shitty way to force tie to go to
    // possible solution
    for (const auto &guess : possible_solutions_) {
      int min_eliminated = std::numeric_limits<int>::max();
      for (const auto &pegs : all_peg_outputs_) {
        int num_eliminated = PotentialElimination(guess, pegs.first, pegs.second);
        min_eliminated = std::min(min_eliminated, num_eliminated);
      }
      if (min_eliminated >= max_min_eliminated) {
        max_min_eliminated = min_eliminated;
        best_guess = std::make_unique<Code<NUM_DIGITS, NUM_COLORS>>(guess);
      }
    }

    return *best_guess;
  }

  static void PrintSolution(const Code<NUM_DIGITS, NUM_COLORS> &solution) {
    std::cout << "Super secret solution is: " << solution << std::endl;
  }

  static void PrintGuess(int guess_number, const Code<NUM_DIGITS, NUM_COLORS> &guess) {
    std::cout << "[" << guess_number << "]: " << guess;
  }

  static void PrintEval(int red_pegs, int white_pegs) {
    std::cout << "  |  {" << red_pegs << ", " << white_pegs << "}";
  }

  void PrintPossibleSolutions() const {
    std::cout << "  |  " << possible_solutions_.size() << " possible solutions remaining" << std::endl;
  }

  const std::vector<std::pair<int, int>> all_peg_outputs_;

  const std::vector<Code<NUM_DIGITS, NUM_COLORS>> possible_guesses_;

  std::list<Code<NUM_DIGITS, NUM_COLORS>> possible_solutions_;

  std::unique_ptr<Code<NUM_DIGITS, NUM_COLORS>> last_guess_;
};