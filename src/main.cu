#include "heat_simulator_cuda.hh"

#include <iomanip>

int main(int argc, char** argv)
{
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


  cudaEvent_t start;
  cudaEvent_t stop;

  HANDLE_ERROR(cudaEventCreate(&start));
  HANDLE_ERROR(cudaEventCreate(&stop));
  HANDLE_ERROR(cudaEventRecord(start, 0));

  float *res = simulator.simulate_cuda(nb_iter);

  HANDLE_ERROR(cudaEventRecord(stop, 0));
  HANDLE_ERROR(cudaEventSynchronize(stop));

  float elapsedTime;
  HANDLE_ERROR(cudaEventElapsedTime(&elapsedTime, start, stop));
  printf( "Time to generate: %3.1f ms\n", elapsedTime );
  HANDLE_ERROR(cudaEventDestroy(start));
  HANDLE_ERROR(cudaEventDestroy(stop));

  //std::cout << std::setprecision(2);
//  for (unsigned i = 0; i < simulator.get_size(); ++i)
//    std::cout << std::fixed << std::setprecision(2) << res[i] << " ";
//  std::cout << std::endl;

  return 0;
}
