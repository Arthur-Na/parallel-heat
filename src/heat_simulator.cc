#include "heat_simulator.hh"


HeatSimulator::HeatSimulator(std::string input_file)
{
  std::cout << "Reading input file..." << std::endl;

  std::ifstream input(input_file);

  long x, y, z;

  double default_initial_value = 0;

  input >> x_ >> y_ >> z_ >> default_initial_value;

  mesh_ = std::vector<double>(x_ * y_ * z_, default_initial_value);

  double next_value;
  while(input >> x >> y >> z >> next_value)
    mesh_.at((x * x_ +  y) * y_ + z) = next_value;
}


std::vector<double> HeatSimulator::simulate(unsigned max_iter)
{
  for (unsigned n = 1; n < max_iter; n++)
  {
    auto new_mesh = std::vector<double>(x_ * y_ * z_, 0);
    for (long i = 0; i < x_; i++)
      for (long j = 0; j < y_; j++)
        for (long k = 0; k < z_; k++)
          new_mesh.at((i * x_ + j) * y_ + k) = compute(i, j, k);
    mesh_ = new_mesh;
  }
  return mesh_;
}


double HeatSimulator::compute(long i, long j, long k)
{
  double alpha = 0.000019;
  return mesh_.at((i * x_ + j) * y_ + k) + alpha * compute_D(i, j, k);
}

double HeatSimulator::compute_D(long i, long j, long k)
{
  if (i != 0 && i != x_ - 1 && j != 0 && j != y_ - 1 && k != 0 && k != z_ - 1)
    return compute_Dx(i, j, k) + compute_Dy(i, j, k) + compute_Dz(i, j, k);
  else
    return 0;
}

double HeatSimulator::compute_Dx(long i, long j, long k)
{
  double a = i + 1 < x_ ? mesh_.at(((i + 1) * x_ + j) * y_ + k) : 0;
  double b = 2 * mesh_.at((i * x_ + j) * y_ + k);
  double c = i - 1 >= 0 ? mesh_.at(((i - 1) * x_ + j) * y_ + k) : 0;
  return a - b + c;
}

double HeatSimulator::compute_Dy(long i, long j, long k)
{
  double a = j + 1 < y_ ? mesh_.at((i * x_ + (j + 1)) * y_ + k) : 0;
  double b = 2 * mesh_.at((i * x_ + j) * y_ + k);
  double c = j - 1 >= 0 ? mesh_.at((i * x_ + (j - 1)) * y_ + k) : 0;
  return a - b + c;
}

double HeatSimulator::compute_Dz(long i, long j, long k)
{
  double a = k + 1 < z_ ? mesh_.at((i * x_ + j) * y_ + k + 1) : 0;
  double b = 2 * mesh_.at((i * x_ + j) * y_ + k);
  double c = k - 1 >= 0 ? mesh_.at((i * x_ + j) * y_ + k - 1) : 0;
  return a - b + c;
}
