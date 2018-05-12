#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>


class HeatSimulator {
  private:
    long x_;
    long y_;
    long z_;

    std::vector<double> mesh_;

    double compute(long i, long j, long k);
    double compute_D(long i, long j, long k);
    double compute_Dx(long i, long j, long k);
    double compute_Dy(long i, long j, long k);
    double compute_Dz(long i, long j, long k);

  public:
    HeatSimulator(std::string input_file);

    std::vector<double> simulate(unsigned max_iter);
};
