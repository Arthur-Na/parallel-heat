#pragma once

#include <iostream>
#include <fstream>

static void HandleError(cudaError_t err, const char *file, int line)
{
  if (err != cudaSuccess)
  {
    printf("%s in %s at line %d\n", cudaGetErrorString(err), file, line);
    exit( EXIT_FAILURE );
  }
}

#define HANDLE_ERROR( err ) (HandleError(err, __FILE__, __LINE__))

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

    int get_size() { return x_ * y_ * z_;}

    float* simulate_cuda(unsigned max_iter);
};
