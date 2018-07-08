#include <iostream>

#include <iomanip>
#include <chrono>
#include <ctime>
#include <thread>

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

  std::streamsize ss = std::cout.precision();
  auto simulator = HeatSimulator(argv[1]);

  std::cout << "Computing..." << std::endl;


  std::clock_t c_start, c_end;
  std::chrono::high_resolution_clock::time_point t_start, t_end;

  c_start = std::clock();
  t_start = std::chrono::high_resolution_clock::now();
  auto result_seq = simulator.simulate_v2(nb_iter);
  c_end = std::clock(); 
  t_end = std::chrono::high_resolution_clock::now();
  std::cout << std::fixed << std::setprecision(2) << "CPU time used: "
            << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms\n"
            << "Wall clock time passed: "
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()
            << " ms\n";

  
  c_start = std::clock();
  t_start = std::chrono::high_resolution_clock::now();
  auto result_para = simulator.simulate_parallel(nb_iter);
  c_end = std::clock(); 
  t_end = std::chrono::high_resolution_clock::now();
  std::cout << "PARALLEL TIME\n"
            << std::fixed << std::setprecision(2) << "CPU time used: "
            << 1000.0 * (c_end-c_start) / CLOCKS_PER_SEC << " ms\n"
            << "Wall clock time passed: "
            << std::chrono::duration<double, std::milli>(t_end-t_start).count()
            << " ms\n";

  std::cout << "Done." << std::endl;
  std::cout << std::setprecision(2) << std::endl;
}
