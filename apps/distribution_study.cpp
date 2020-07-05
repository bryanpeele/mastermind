#include "solver.h"

template <int NUM_DIGITS, int NUM_COLORS>
void RunDistributionStudy(bool allow_repeats) {
  if (!allow_repeats) LOG(FATAL) << "Not yet supported!";  // TODO(bpeele);

  const auto all_solutions = GenerateAllCodes<NUM_DIGITS, NUM_COLORS>(allow_repeats);

  int total_guesses = 0;
  std::vector<int> distribution;

  for (const auto &solution : all_solutions) {
    auto solver = Solver<NUM_DIGITS, NUM_COLORS>();  // TODO(bpeele) add reset
    int num_guesses = solver.AutomatedGameplay(solution, false);

    total_guesses += num_guesses;

    while(distribution.size() < (num_guesses + 1)) distribution.push_back(0);
    distribution[num_guesses]++;
  }

  std::cout << "*****************************************************************" << std::endl;
  std::cout << "* Num. digits: " << NUM_DIGITS
            << " | Num. colors: " << NUM_COLORS
            << " | Allow repeats: " << (allow_repeats ? "TRUE" : "FALSE") << std::endl;
  std::cout << "* ................................................................." << std::endl;
  std:: cout << "*" << std::endl;
  std::cout << "* Possible solutions: " << all_solutions.size() << std::endl;
  std::cout << "* Total guesses:      " << total_guesses << std::endl;
  std::cout << "* Mean guesses:       " << static_cast<double>(total_guesses) /
                                         static_cast<double>(all_solutions.size()) << std::endl;
  std::cout << "* Max guesses:        " << distribution.size() - 1 << std::endl;

  int max_guesses_per_num = 0;
  for (const int i : distribution) max_guesses_per_num = std::max(max_guesses_per_num, i);

  const int char_per_max_distribution = 50;
  const double viz_factor = static_cast<double>(char_per_max_distribution) /
                            static_cast<double>(max_guesses_per_num);

  std:: cout << "*" << std::endl;
  std:: cout << "* Distribution" << std::endl;
  for (int i = 0; i < distribution.size(); i++) {
    double length = static_cast<double>(distribution[i]) * viz_factor;
    std::cout << "*   [" << i << "] ";
    for (int i = 0; i < static_cast<int>(length); i++) std::cout << "*";
    std::cout << std::endl;
  }

  std::cout << "*" << std::endl;
  std::cout << "*****************************************************************" << std::endl;
}

int main() {
  {
    constexpr int num_digits = 4;
    constexpr int num_colors = 6;
    constexpr bool allow_repeats = true;
    RunDistributionStudy<num_digits, num_colors>(allow_repeats);
  }

  {
    constexpr int num_digits = 4;
    constexpr int num_colors = 8;
    constexpr bool allow_repeats = true;
    RunDistributionStudy<num_digits, num_colors>(allow_repeats);
  }
}