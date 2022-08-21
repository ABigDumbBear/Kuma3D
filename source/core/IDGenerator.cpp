#include "IDGenerator.hpp"

#include <stdexcept>

namespace Kuma3D {

/******************************************************************************/
IDGenerator::IDGenerator()
  : mIDCounter(0)
{
}

/******************************************************************************/
ID IDGenerator::GenerateID()
{
  ID newID = 0;

  // If there are any available IDs, return the first available one.
  if(!mAvailableIDs.empty())
  {
    newID = mAvailableIDs.front();
    mAvailableIDs.pop();
  }
  else
  {
    // There are no available IDs, so try to create a new one.
    if(mIDCounter == MAX_IDS)
    {
      throw std::range_error("ID limit reached!");
    }

    newID = mIDCounter;
    ++mIDCounter;
  }

  return newID;
}

/******************************************************************************/
void IDGenerator::RemoveID(const ID& aID)
{
  // An ID can only be removed if it was created first. Since the generator
  // creates IDs in sequential fashion, we can tell if an ID was created
  // by checking if it is lower than the ID counter.
  if(aID >= mIDCounter)
  {
    throw std::invalid_argument("Can't remove non-existing ID!");
  }

  // This ID should now be "removed" and made available for reuse.
  mAvailableIDs.emplace(aID);
}

} // namespace Kuma3D
