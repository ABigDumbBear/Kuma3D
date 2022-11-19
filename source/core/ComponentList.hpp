#ifndef COMPONENTLIST_HPP
#define COMPONENTLIST_HPP

#include <map>
#include <vector>

#include <sstream>
#include <stdexcept>

#include "Types.hpp"

namespace Kuma3D {

/**
 * The base class for the templated ComponentListT. This is necessary because
 * it's not possible to maintain a collection of templated classes in the
 * same data structure; instead, the data structure must maintain a collection
 * of pointers to the base class.
 *
 * This limitation is most relevant in the Scene class, which maintains a
 * vector of ComponentLists.
 */
class ComponentList
{
  public:
    virtual ~ComponentList() = default;

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
 * A data structure that contains a vector of components of type T.
 */
template<typename T>
class ComponentListT : public ComponentList
{
  public:

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
    std::map<Entity, unsigned int> mEntityToIndexMap;
    std::vector<T> mComponents;
};

} // namespace Kuma3D

#endif
