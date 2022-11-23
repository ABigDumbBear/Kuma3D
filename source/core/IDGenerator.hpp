#ifndef IDGENERATOR_HPP
#define IDGENERATOR_HPP

#include <limits.h>
#include <queue>

namespace Kuma3D {

using ID = unsigned int;

/**
 * A simple unique ID generator. IDs are unsigned integers, and
 * this generator can only define IDs up to UINT_MAX.
 */
class IDGenerator
{
  public:
    IDGenerator();

    /**
     * Creates and returns a unique ID.
     *
     * @return A unique ID.
     */
    ID GenerateID();

    /**
     * Removes an ID and keeps track of it. Removed IDs are reused
     * in GenerateID().
     *
     * @param aID The ID to remove.
     */
    void RemoveID(const ID& aID);

  private:
    ID mIDCounter;
    std::queue<ID> mAvailableIDs;

    static const unsigned int MAX_IDS = UINT_MAX;
};

} // namespace Kuma3D

#endif
