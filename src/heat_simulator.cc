#include "heat_simulator.hh"
#include <omp.h>

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

  max_value_ = *std::max_element(mesh_.begin(), mesh_.end());
}

std::tuple<long, long, long> HeatSimulator::get_coordinates(long index)
{
   long z = index / (x_ * y_);
   index -= (z * x_ * y_);
   long x = index % x_;
   long y = index / y_;
   return std::make_tuple(z, y, x);
}

long HeatSimulator::get_index(long x, long y, long z)
{
  return (z * x_ * y_) + (y * x_) + x;
}

std::vector<double> HeatSimulator::simulate(unsigned max_iter)
{
  for (unsigned n = 1; n < max_iter; n++)
  {
    auto new_mesh = std::vector<double>(x_ * y_ * z_, 0);
    for (long i = 0; i < x_; i++)
      for (long j = 0; j < y_; j++)
        for (long k = 0; k < z_; k++)
          new_mesh.at(get_index(i, j, k)) = compute(i, j, k);
          
          
    //new_mesh.at((i * x_ + j) * y_ + k) = compute(i, j, k);
    mesh_ = new_mesh;
  }
  return mesh_;
}

std::vector<double> HeatSimulator::simulate_parallel(unsigned max_iter)
{
  for (unsigned n = 1; n < max_iter; n++)
  {
    auto new_mesh = std::vector<double>(x_ * y_ * z_, 0);
    #pragma omp parallel for
    for (long i = 0; i < x_; i++)
      for (long j = 0; j < y_; j++)
        for (long k = 0; k < z_; k++)
          new_mesh.at((i * x_ + j) * y_ + k) = compute(i, j, k);
    mesh_ = new_mesh;
  }
  return mesh_;
}


unsigned HeatSimulator::validate_loop(unsigned max_iter)
{
  for (unsigned n = 1; n < max_iter; ++n)
  {
    auto v_three_loop = std::vector<std::tuple<long, long, long>>(x_ * y_ * z_);
    auto new_mesh_tloop = std::vector<double>(x_ * y_ * z_, 0);
    for (long i = 0; i < x_; i++)
    {
      for (long j = 0; j < y_; j++)
      {
        for (long k = 0; k < z_; k++)
        {
          v_three_loop.push_back(test(i, j, k));
          new_mesh_tloop.at((i * x_ + j) * y_ + k) = compute(i, j, k);
        }
      }
    }

    auto v_one_loop = std::vector<std::tuple<long, long, long>>(x_ * y_ * z_);
    auto new_mesh_oloop = std::vector<double>(x_ * y_ * z_, 0);
    for (unsigned i = 0; i < x_ * y_ * z_; ++i)
    {
      const auto coords = get_coordinates(i);
      v_one_loop.push_back(test(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords)));
      new_mesh_oloop.at(i) = compute_v2(i);
    }

    for (unsigned i = 0; i < x_ * y_ * z_; ++i)
    {
      if (v_three_loop[i] != v_one_loop[i])
      {
        std::cout << "Not matching at iteration number: " << n << std::endl;
        std::cout << "Not matching on index number: " << i << std::endl;
        std::cout << "Coordinates for three_loop: "
                  << std::get<0>(v_three_loop[i]) << " - "
                  << std::get<1>(v_three_loop[i]) << " - "
                  << std::get<2>(v_three_loop[i]) << std::endl;
        std::cout << "Coordinates for one_loop: "
                  << std::get<0>(v_one_loop[i]) << " - "
                  << std::get<1>(v_one_loop[i]) << " - "
                  << std::get<2>(v_one_loop[i]) << std::endl;
        return n;
      }

      //if (new_mesh_oloop[i] != new_mesh_tloop[i])
      if (std::fabs(new_mesh_oloop[i] - new_mesh_tloop[i]) >= std::numeric_limits<double>::epsilon())
      {
        std::cout << "Not matching at iteration number: " << n << std::endl;
        std::cout << "Not matching on index number: " << i << std::endl;
        std::cout << "Value found for three_loop: " << new_mesh_tloop[i] << std::endl;
        std::cout << "Value found for one_loop: " << new_mesh_oloop[i] << std::endl;
        return n;
      }
    }

    mesh_ = new_mesh_tloop;
  }
  return max_iter;
}


std::vector<double> HeatSimulator::simulate_v2(unsigned max_iter)
{
  for (unsigned n = 0; n < max_iter; ++n)
  {
    auto new_mesh = std::vector<double>(x_ * y_ * z_, 0);
    for (long index = 0; index < x_ * y_ * z_; ++index)
      new_mesh.at(index) = compute_v2(index);
    mesh_ = new_mesh;
  }
  return mesh_;
}

std::vector<double> HeatSimulator::simulate_draw(unsigned max_iter, vtk::VtkViewer& vtk)
{
  for (unsigned n = 0; n < max_iter; ++n)
  {
    auto new_mesh = std::vector<double>(x_ * y_ * z_, 0);
    for (long index = 0; index < x_ * y_ * z_; ++index)
      new_mesh.at(index) = compute_v2(index);
    mesh_ = new_mesh;
    //viewer(mesh_, x_, y_, z_, max_value_, renderer);
    vtk.update(mesh_, x_, y_, z_, max_value_);
  }
  return mesh_;
}

double HeatSimulator::compute(long i, long j, long k)
{
  double alpha = 0.000019;
  return mesh_.at((i * x_ + j) * y_ + k) + alpha * compute_D(i, j, k);
}

double HeatSimulator::compute_v2(long index)
{
  //double alpha = 0.000019;
  double alpha = 0.0019;
  std::tuple<long, long, long> coords = get_coordinates(index);
  long i = std::get<0>(coords);
  long j = std::get<1>(coords);
  long k = std::get<2>(coords);
  //return mesh_.at((i * x_ + j) * y_ + k) + alpha * compute_D(i, j, k);
  return mesh_.at(index) + alpha * compute_D(i, j, k);
  //return mesh_.at(index) + alpha * compute_D(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
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

std::tuple<long, long, long> HeatSimulator::test(long i, long j, long k)
{
  return std::make_tuple(i, j, k);
}
