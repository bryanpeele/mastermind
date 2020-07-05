add_executable(solver_test solver_test.cpp)
target_link_libraries(solver_test core gtest gtest_main solver)
gtest_add_tests(TARGET solver_test)