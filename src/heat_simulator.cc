#include "heat_simulator.hh"


HeatSimulator::HeatSimulator(std::string input_file) {
  std::cout << "Reading input file..." << std::endl;

  std::ifstream input(input_file);

  long x;
  long y;
  long z;

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
  auto new_mesh = new std::vector<double>(this->x * this->y * this->z, 0);
  for (unsigned n = 1; n < max_iter; n++) {
    for (long i = 0; i < this->x; i++) {
      for (long j = 0; j < this->y; j++) {
        for (long k = 0; k < this->z; k++) {
          new_mesh->at((i * this->x + j) * this->y + k) = compute(i, j, k, n);
        }
      }
    }
    this->mesh = new_mesh;
    new_mesh = new std::vector<double>(*this->mesh);
  }

  return this->mesh;
}


double HeatSimulator::compute(long i, long j, long k, unsigned n) {
  double alpha = 1;
  return this->mesh->at((i * this->x + j) * this->y + k) + alpha * compute_D(i, j, k);
}

double HeatSimulator::compute_D(long i, long j, long k) {
  return compute_Dx(i, j, k) + compute_Dy(i, j, k) + compute_Dz(i, j, k);
}

double HeatSimulator::compute_Dx(long i, long j, long k) {
  double a = i + 1 < this->x ? this->mesh->at(((i + 1) * this->x + j) * this->y + k) : 0;
  double b = 2 * this->mesh->at((i * this->x + j) * this->y + k);
  double c = i - 1 >= 0 ? this->mesh->at(((i - 1) * this->x + j) * this->y + k) : 0;
  return a - b + c;
}

double HeatSimulator::compute_Dy(long i, long j, long k) {
  double a = j + 1 < this->y ? this->mesh->at((i * this->x + (j + 1)) * this->y + k) : 0;
  double b = 2 * this->mesh->at((i * this->x + j) * this->y + k);
  double c = j - 1 >= 0 ? this->mesh->at((i * this->x + (j - 1)) * this->y + k) : 0;
  return a - b + c;
}

double HeatSimulator::compute_Dz(long i, long j, long k) {
  double a = k + 1 < this->z ? this->mesh->at((i * this->x + j) * this->y + k + 1) : 0;
  double b = 2 * this->mesh->at((i * this->x + j) * this->y + k);
  double c = k - 1 >= 0 ? this->mesh->at((i * this->x + j) * this->y + k - 1) : 0;
  return a - b + c;
}
