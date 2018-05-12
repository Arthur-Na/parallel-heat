#include <iostream>

#include "heat_simulator.hh"


int main(int argc, char* argv[]) {

  if (argc != 4) {
    std::cerr << "Usage : ./heat-diffusion [input_file] [output_file] [nb_iter]" << std::endl;
    return 1;
  }

  long nb_iter = std::stoi(argv[3]);
  if (nb_iter < 0) {
    std::cerr << "Usage : ./heat-diffusion [input_file] [output_file] [nb_iter]" << std::endl
              << "nb_iter must be positive." << std::endl;
    return 1;
  }

  auto simulator = HeatSimulator(argv[1]);

  std::cout << "Computing..." << std::endl;

  auto result = simulator.simulate(nb_iter);

  for (double a : result)
    std::cout << a << " ";

  std::cout << "Done." << std::endl;
}
