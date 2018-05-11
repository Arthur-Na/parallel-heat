#pragma once

#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>


class HeatSimulator {
  protected:
    std::size_t x;
    std::size_t y;
    std::size_t z;

    std::vector<double> *mesh;

    double compute(std::size_t i, std::size_t j, std::size_t k, unsigned n);
    double compute_D(std::size_t i, std::size_t j, std::size_t k);
    double compute_Dx(std::size_t i, std::size_t j, std::size_t k);
    double compute_Dy(std::size_t i, std::size_t j, std::size_t k);
    double compute_Dz(std::size_t i, std::size_t j, std::size_t k);

  public:
    HeatSimulator(std::string input_file);

    std::vector<double>* simulate(unsigned max_iter);
};
