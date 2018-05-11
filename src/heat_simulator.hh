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

    HeatSimulator(std::string input_file);
};
