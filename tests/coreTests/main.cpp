#include "CoreTests.hpp"

#include <iostream>

int main()
{
  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing ComponentList addition..." << std::endl;
  Kuma3D::TestComponentListAddition();
  std::cout << "ComponentList addition successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing ComponentList removal..." << std::endl;
  Kuma3D::TestComponentListRemoval();
  std::cout << "ComponentList removal successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing ComponentList merge..." << std::endl;
  Kuma3D::TestComponentListMerge();
  std::cout << "ComponentList merge successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Entity addition..." << std::endl;
  Kuma3D::TestEntityAddition();
  std::cout << "Entity addition successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Entity removal..." << std::endl;
  Kuma3D::TestEntityRemoval();
  std::cout << "Entity removal successful!" << std::endl;

  std::cout << "-------------------------------------" << std::endl;
  std::cout << "Testing Signature relevancy check..." << std::endl;
  Kuma3D::TestSignatureRelevancyCheck();
  std::cout << "Signature relevancy check successful!" << std::endl;

  return 0;
}
