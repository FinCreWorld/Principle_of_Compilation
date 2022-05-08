cmake_minimum_required(VERSION 3.20)
project(jpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(jpp main.cpp LA/hash.h LA/hash.cpp LA/sign.h LA/sign.cpp LA/LA.h LA/LA.cpp LA/tool.h LA/tool.cpp LA/LA_test.h LA/LA_test.cpp LR/LR_table.cpp LR/LR_table.h LR/build_expr.h LR/LR_test.h LR/build_expr.cpp LR/LR.cpp LR/LR.h LA/buffer.cpp LA/buffer.h LR/action_fun.h LR/action_fun.cpp CG/CG.h CG/CG.cpp CG/CG_test.h CG/CG_test.cpp)
