#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

class HeatSimulator {
  private:
    long x_;
    long y_;
    long z_;

    std::vector<double> mesh_;

    double compute(long i, long j, long k);
    double compute_v2(long index);
    double compute_D(long i, long j, long k);
    double compute_Dx(long i, long j, long k);
    double compute_Dy(long i, long j, long k);
    double compute_Dz(long i, long j, long k);

    std::tuple<int, int, int> get_coordinates(int index);
    int get_index(int x, int y, int z);

  public:
    HeatSimulator(std::string input_file);

    std::vector<double> simulate(unsigned max_iter);
    std::vector<double> simulate_v2(unsigned max_iter);
    std::vector<double> simulate_parallel(unsigned max_iter);
};
