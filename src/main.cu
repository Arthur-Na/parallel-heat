#include <iostream>

__global__ void kernel(void)
{}

int main()
{
  kernel<<<1, 1>>>();
  std::cout << "hellooooo" << std::endl;
  return 0;
}
