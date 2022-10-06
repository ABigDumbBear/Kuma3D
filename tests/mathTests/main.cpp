#include "MathTests.hpp"

#include <iostream>

int main()
{
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Vec3 addition..." << std::endl;
  Kuma3D::TestVec3Addition();
  std::cout << "Vec3 addition successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Vec3 subtraction..." << std::endl;
  Kuma3D::TestVec3Subtraction();
  std::cout << "Vec3 subtraction successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 multiplication..." << std::endl;
  Kuma3D::TestMat4Multiplication();
  std::cout << "Mat4 multiplication successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 multiplication with Vec3..." << std::endl;
  Kuma3D::TestMat4MultiplicationWithVec3();
  std::cout << "Mat4 multiplication with Vec3 successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 translation..." << std::endl;
  Kuma3D::TestMat4Translation();
  std::cout << "Mat4 translation successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 rotation..." << std::endl;
  Kuma3D::TestMat4Rotation();
  std::cout << "Mat4 rotation successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Mat4 scaling..." << std::endl;
  Kuma3D::TestMat4Scaling();
  std::cout << "Mat4 scaling successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing linear interpolation..." << std::endl;
  Kuma3D::TestLerp();
  std::cout << "Linear interpolation successful!" << std::endl;

  return 0;
}
