add_library(core SHARED
        code.cpp
        code_util.cpp
        )

target_link_libraries(core glog::glog)

target_include_directories(core PUBLIC .)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  target_compile_options(core PRIVATE -O3 -Wall -Wextra -Werror -fPIC)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_options(core PRIVATE -Wall -Wextra -fPIC)
endif()
