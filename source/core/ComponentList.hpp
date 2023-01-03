#ifndef COMPONENTLIST_HPP
#define COMPONENTLIST_HPP

#include <unordered_map>
#include <vector>

#include "Entity.hpp"

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
     * Constructor.
     *
     * @param aMax The maximum number of components.
     */
    ComponentListT<T>(std::size_t aMax)
    {
      // Initialize the vector with default components.
      for(std::size_t i = 0; i < aMax; ++i)
      {
        mComponents.emplace_back(T());
      }
    }

    /**
     * Adds a component to the list. Note that Entities are not allowed
     * to have multiple components of the same type. If the ComponentList
     * is full, this function does nothing.
     *
     * The component is moved (not copied) into the underlying container.
     *
     * @param aEntity The Entity to associate the component with.
     * @param aComponent The component to add.
     */
    void AddComponentToEntity(Entity aEntity,
                              T& aComponent)
    {
      if(mNumValidComponents < mComponents.size())
      {
        auto index = mNumValidComponents;
        mComponents[index] = std::move(aComponent);
        ++mNumValidComponents;

        mEntityToIndexMap.emplace(aEntity, index);
      }
    }

    /**
     * Removes a component associated with the given Entity from the list.
     *
     * @param aEntity The Entity to remove a component from.
     */
    void RemoveComponentFromEntity(Entity aEntity) override
    {
      // Move the last valid component into the removed component's spot.
      auto removedIndex = mEntityToIndexMap[aEntity];
      auto lastValidIndex = mNumValidComponents - 1;
      mComponents[removedIndex] = std::move(mComponents[lastValidIndex]);

      // Find the Entity that corresponds to the last valid component
      // and update its index.
      for(auto& entityIndexPair : mEntityToIndexMap)
      {
        if(entityIndexPair.second == lastValidIndex)
        {
          entityIndexPair.second = removedIndex;
          break;
        }
      }
      --mNumValidComponents;

      // Remove the Entity from the index map.
      mEntityToIndexMap.erase(aEntity);
    }

    /**
     * Returns a component of type T associated with the given Entity.
     *
     * @return A component of type T.
     */
    const T& GetComponentForEntity(Entity aEntity) const
    {
      return mComponents.at(mEntityToIndexMap.at(aEntity));
    }

    /**
     * Returns a component of type T associated with the given Entity. This
     * version of the function provides write access to the component.
     *
     * @return A component of type T.
     */
    T& GetComponentForEntity(Entity aEntity)
    {
      auto constList = const_cast<const ComponentListT<T>*>(this);
      return const_cast<T&>(constList->GetComponentForEntity(aEntity));
    }

  private:
    std::unordered_map<Entity, unsigned int> mEntityToIndexMap;
    std::vector<T> mComponents;

    std::size_t mNumValidComponents { 0 };
};

} // namespace Kuma3D

#endif
