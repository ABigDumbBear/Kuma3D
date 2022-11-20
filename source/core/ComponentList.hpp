#ifndef COMPONENTLIST_HPP
#define COMPONENTLIST_HPP

#include <map>
#include <vector>

#include <sstream>
#include <stdexcept>

#include "Types.hpp"

namespace Kuma3D {

/**
 * A base class for a component container. For more info, see the template
 * class ComponentListT.
 */
class ComponentList
{
  public:
    virtual ~ComponentList() = default;

    /**
     * Moves all components in the buffer into the main component vector,
     * then clears out the buffer.
     */
    virtual void ApplyChanges() = 0;

    /**
     * Removes a component from a given Entity. This is made virtual so that
     * outside code (specifically a Scene) can remove components without
     * needing to know the type of said component.
     *
     * @param aEntity The Entity to remove a component from.
     */
    virtual void RemoveComponentFromEntity(Entity aEntity) = 0;
};

/**
 * A container object for components of type T. When adding components,
 * they are first added into a separate buffer and can't be retrieved until
 * ApplyChanges() is called.
 *
 * This is done to ensure that newly added components don't get updated
 * by a System before the end of the frame in which they were added.
 */
template<typename T>
class ComponentListT : public ComponentList
{
  public:

    /**
     * Moves all components in the buffer into the main component vector,
     * then clears out the buffer.
     */
    void ApplyChanges() override
    {
      for(const auto& entityIndexPair : mEntityToBufferMap)
      {
        // Move the component from the buffer to the data.
        mData.emplace_back(std::move(mBuffer[entityIndexPair.second]));

        // Map the Entity to the data index.
        mEntityToDataMap.emplace(entityIndexPair.first, entityIndexPair.second);
      }

      // Clear out the buffer.
      mEntityToBufferMap.clear();
      mBuffer.clear();
    }

    /**
     * Adds a component to the list. Note that Entities are not allowed
     * to have multiple components of the same type.
     *
     * @param aEntity The Entity to associate the component with.
     * @param aComponent The component to add.
     */
    void AddComponentToEntity(Entity aEntity,
                              T& aComponent)
    {
      if(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end())
      {
        std::stringstream error;
        error << "Entity " << aEntity << " already has a component of type ";
        error << typeid(T).name();
        throw std::invalid_argument(error.str());
      }

      auto newIndex = mComponents.size();
      mEntityToIndexMap.emplace(aEntity, newIndex);
      mComponents.emplace_back(std::move(aComponent));
    }

    /**
     * Removes a component associated with the given Entity from the list.
     *
     * @param aEntity The Entity to remove a component from.
     */
    void RemoveComponentFromEntity(Entity aEntity) override
    {
      if(mEntityToIndexMap.find(aEntity) == mEntityToIndexMap.end())
      {
        std::stringstream error;
        error << "Entity " << aEntity << " doesn't have a component of type ";
        error << typeid(T).name();
        throw std::invalid_argument(error.str());
      }

      // Erase the component at the correct index for the Entity.
      auto removedComponentIndex = mEntityToIndexMap[aEntity];
      mComponents.erase(mComponents.begin() + removedComponentIndex);

      // Remove the entry from the EntityToIndex map.
      mEntityToIndexMap.erase(aEntity);

      // Erasing an item from a vector shifts each item afterwards back
      // by one. So, in order to keep the EntityToIndex map valid, each
      // index greater than or equal to the index of the erased item needs
      // to be decremented by one.
      for(auto& entityIndexPair : mEntityToIndexMap)
      {
        if(entityIndexPair.second >= removedComponentIndex)
        {
          --entityIndexPair.second;
        }
      }
    }

    /**
     * Returns a component of type T associated with the given Entity.
     *
     * @return A component of type T.
     */
    T& GetComponentForEntity(Entity aEntity)
    {
      if(mEntityToIndexMap.find(aEntity) == mEntityToIndexMap.end())
      {
        std::stringstream error;
        error << "Entity " << aEntity << " doesn't have a component of type ";
        error << typeid(T).name();
        throw std::invalid_argument(error.str());
      }

      return mComponents[mEntityToIndexMap[aEntity]];
    }

  private:
    std::map<Entity, unsigned int> mEntityToDataMap;
    std::vector<T> mData;

    std::map<Entity, unsigned int> mEntityToBufferMap;
    std::vector<T> mBuffer;
};

} // namespace Kuma3D

#endif
