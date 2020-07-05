add_library(solver SHARED
        solver.cpp
        )

target_link_libraries(solver core)

target_include_directories(solver PUBLIC .)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  target_compile_options(solver PRIVATE -O3 -Wall -Wextra -Werror -fPIC)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_options(solver PRIVATE -Wall -Wextra -fPIC)
endif()
