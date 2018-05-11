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

  public:
    HeatSimulator(std::size_t x, std::size_t y, std::size_t z) {
      this->x = x;
      this->y = y;
      this->z = z;
    }

    HeatSimulator(std::string input_file) {
      std::cout << "Reading input file..." << std::endl;

      std::ifstream input(input_file);

      std::size_t x;
      std::size_t y;
      std::size_t z;

      double default_initial_value = 0;

      input >> x >> y >> z >> default_initial_value;

      this->x = x;
      this->y = y;
      this->z = z;

      auto mesh = new std::vector<double>(x * y * z, default_initial_value);

      double next_value;
      while(input >> x >> y >> z >> next_value) {
        mesh->at(x * y * z) = next_value;
      }
    }
};
