#pragma once

#include "vtk_viewer.hh"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <tuple>
#include <vector>

class HeatSimulator {
  private:
    long x_;
    long y_;
    long z_;
    long max_value_;

    std::vector<double> mesh_;

    double compute(long i, long j, long k);
    double compute_v2(long index);
    double compute_D(long i, long j, long k);
    double compute_Dx(long i, long j, long k);
    double compute_Dy(long i, long j, long k);
    double compute_Dz(long i, long j, long k);

    std::tuple<long, long, long> get_coordinates(long index);
    long get_index(long x, long y, long z);

  public:
    HeatSimulator(std::string input_file);

    int get_x() { return x_; }
    int get_y() { return y_; }
    int get_z() { return z_; }

    std::vector<double> simulate(unsigned max_iter);
    std::vector<double> simulate_v2(unsigned max_iter);
    std::vector<double> simulate_parallel(unsigned max_iter);
    std::vector<double> simulate_draw(unsigned max_iter, vtk::VtkViewer& vtk);
};
