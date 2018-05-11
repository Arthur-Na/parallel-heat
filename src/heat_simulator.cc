#include "heat_simulator.hh"


HeatSimulator::HeatSimulator(std::string input_file) {
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
    mesh->at(x * this->x +  y * this->y + z) = next_value;
  }

  this->mesh = mesh;
}


std::vector<double>* HeatSimulator::simulate(unsigned max_iter) {
  auto new_mesh = new std::vector<double>(*this->mesh);
  for (unsigned n = 1; n < max_iter; n++) {
    for (std::size_t i = 0; i < this->x; i++) {
      for (std::size_t j = 0; j < this->y; j++) {
        for (std::size_t k = 0; k < this->z; k++) {
          new_mesh->at(i * this->x + j * this->y + k) = compute(i, j, k);
        }
      }
    }
  }

  return new_mesh;
}


double HeatSimulator::compute(std::size_t i, std::size_t j, std::size_t k) {
  return 0.;
}
