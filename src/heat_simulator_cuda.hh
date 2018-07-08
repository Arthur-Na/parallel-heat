#pragma once

#include <iostream>
#include <fstream>

class HeatSimulator {
  private:
    long x_;
    long y_;
    long z_;
    float max_value_;

    //std::vector<float> mesh_;
    float* mesh_;

  public:
    HeatSimulator(std::string input_file);
    ~HeatSimulator();

    int get_x() { return x_; }
    int get_y() { return y_; }
    int get_z() { return z_; }

    float* simulate_cuda(unsigned max_iter);
};
