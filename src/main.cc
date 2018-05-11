#include <iostream>

#include "heat_simulator.hh"


int main(int argc, char* argv[]) {

  if (argc != 3) {
    std::cerr << "Usage : ./heat-diffusion [input_file] [output_file]" << std::endl;
    return 1;
  }

  auto simulator = HeatSimulator(argv[1]);

  std::cout << "Computing..." << std::endl;


  std::cout << "Done." << std::endl;
}
