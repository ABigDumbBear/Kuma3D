#include "ComponentTests.hpp"

#include <iostream>

int main()
{
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Transform positioning..." << std::endl;
  Kuma3D::TestTransformPosition();
  std::cout << "Transform positioning successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Transform rotating..." << std::endl;
  Kuma3D::TestTransformRotation();
  std::cout << "Transform rotation successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Transform scaling..." << std::endl;
  Kuma3D::TestTransformScaling();
  std::cout << "Transform scaling successful!" << std::endl;

  return 0;
}
