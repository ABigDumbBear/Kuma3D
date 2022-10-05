#include "ComponentTests.hpp"

#include <iostream>

int main()
{
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Transform positioning..." << std::endl;
  Kuma3D::TestTransformPosition();
  std::cout << "Transform positioning successful!" << std::endl;

  return 0;
}
