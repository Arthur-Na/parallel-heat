#include <iostream>

#include "heat_simulator.hh"
#include "vtk_viewer.hh"

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

  //auto renderer = build_renderer();
  auto vtk = vtk::VtkViewer();
  auto simulator = HeatSimulator(argv[1]);
  vtk.init(simulator.get_x(), simulator.get_y(), simulator.get_z());

  std::cout << "Computing..." << std::endl;

  auto res = simulator.simulate_draw(nb_iter, vtk);

  vtk.show();
}
