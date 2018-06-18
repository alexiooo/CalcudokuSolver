cmake_minimum_required(VERSION 3.6)
project(CalcudokuSolver)

set(CMAKE_CXX_STANDARD 14)

add_executable(CalcudokuSolver main.cpp constraint.cc constraint.h variable.h cspsolver.cc cspsolver.h variable.cc bindings.cc)