#include "heat_simulator_cuda.hh"

HeatSimulator::HeatSimulator(std::string input_file)
{
  std::ifstream input(input_file);

  long x, y, z;

  float default_initial_value = 0;

  input >> x_ >> y_ >> z_ >> default_initial_value;

  mesh_ = std::vector<float>(x_ * y_ * z_, default_initial_value);

  float next_value;
  while(input >> x >> y >> z >> next_value)
  {
    mesh_.at((x * x_ +  y) * y_ + z) = next_value;
  }
}

__device__ float kernel_computeDX(float *mesh, int idx, int idy, int idz, int sx, int sy)
{
  float a = idx + 1 < sx ? mesh[((idx + 1) * sx + idy) * sy + idz] : 0;
  float b = 2 * mesh[(idx * sx + idy) * sy + idz];
  float c = idx - 1 >= 0 ? mesh[((idx - 1) * sx + idy) * sy + idz] : 0;
  return a - b + c;
}

__device__ float kernel_computeDY(float *mesh, int idx, int idy, int idz, int sx, int sy)
{
  float a = idy + 1 < sy ? mesh[(idx * sx + (idy + 1)) * sy + idz] : 0;
  float b = 2 * mesh[(idx * sx + idy) * sy + idz];
  float c = idy - 1 >= 0 ? mesh[(idx * sx + (idy - 1)) * sy + idz] : 0;
  return a - b + c;
}

__device__ float kernel_computeDZ(float *mesh, int idx, int idy, int idz, int sx, int sy, int sz)
{
  float a = idz + 1 < sz ? mesh[(idx * sx + idy) * sy + idz + 1] : 0;
  float b = 2 * mesh[(idx * sx + idy) * sy + idz];
  float c = idz - 1 >= 0 ? mesh[(idx * sx + idy) * sy + idz - 1] : 0;
  return a - b + c;
}

__device__ float kernel_computeD(float *mesh, int idx, int idy, int idz, int sx, int sy, int sz)
{
  if (idx != 0 && idx != sx - 1 && idy != 0 && idy != sy - 1 && idz != 0 && idz != sz - 1)
    return kernel_computeDX(mesh, idx, idy, idz, sx, sy)
           + kernel_computeDY(mesh, idx, idy, idz, sx, sy)
           + kernel_computeDZ(mesh, idx, idy, idz, sx, sy, sz);
  else
    return 0;
}

__global__ void kernel_compute(float* mesh_in, int sx, int sy, int sz, float* mesh_out)
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int idy = blockIdx.y * blockDim.y + threadIdx.y;
  int idz = blockIdx.z * blockDim.z + threadIdx.z;

  if (idx < sx && idy < sy && idz < sz)
  {
    const float alpha = 0.000019;
    int index = (idx * sx + idy) * sy + idz; 
    
    mesh_out[index] = mesh_in[index] + alpha * kernel_computeD(mesh_in, idx, idy, idz, sx, sy, sz);
  }
}

__global__ void kernel_copy(float* in, float* out, int sx, int sy, int sz)
{
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int idy = blockIdx.y * blockDim.y + threadIdx.y;
  int idz = blockIdx.z * blockDim.z + threadIdx.z;

  if (idx < sx && idy < sy && idz < sz)
  {
    int index = (idx * sx + idy) * sy + idz; 
    out[index] = in[index];
  }
}

#define BLKXSIZE 8
#define BLKYSIZE 8
#define BLKZSIZE 16

float* HeatSimulator::simulate_cuda(unsigned max_iter)
{
  int SIZE = x_ * y_ * z_;
  const dim3 block_size(BLKXSIZE, BLKYSIZE, BLKZSIZE);
  const dim3 grid_size(((x_ + BLKXSIZE - 1) / BLKXSIZE),
                       ((y_ + BLKYSIZE - 1) / BLKYSIZE),
                       ((z_ + BLKZSIZE - 1) / BLKZSIZE));


  float* new_mesh = mesh_.data();
  float* mesh_out;
  HANDLE_ERROR(cudaMalloc((void**) &mesh_out, SIZE * sizeof(float)));
  float* iter_n;
  HANDLE_ERROR(cudaMalloc((void**) &iter_n, SIZE * sizeof(float)));
  HANDLE_ERROR(cudaMemcpy(iter_n, new_mesh, SIZE * sizeof(float), cudaMemcpyHostToDevice));

  for (unsigned n = 1; n < max_iter; ++n)
  {
    kernel_compute<<<grid_size, block_size>>>(iter_n, x_, y_, z_, mesh_out);
    kernel_copy<<<grid_size, block_size>>>(mesh_out, iter_n, x_, y_, z_);
  }

  HANDLE_ERROR(cudaMemcpy(new_mesh, mesh_out, SIZE * sizeof(float), cudaMemcpyDeviceToHost));

  HANDLE_ERROR(cudaFree(mesh_out));
  HANDLE_ERROR(cudaFree(iter_n));
  return new_mesh;
}
