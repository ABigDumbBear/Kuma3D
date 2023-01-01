#ifndef COMPONENTLIST_HPP
#define COMPONENTLIST_HPP

#include <map>
#include <vector>

#include <sstream>
#include <stdexcept>

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
     * Takes all the components from a given list and adds them into this one.
     *
     * @param aList The list to merge.
     */
    virtual void Merge(ComponentList& aList) = 0;

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
    ComponentListT<T>(size_t aMax)
    {
      // Initialize the vector with empty components.
      for(size_t i = 0; i < aMax; ++i)
      {
        mComponents.emplace_back(T());
      }
    }

    /**
     * Retrieves a list of Entities that have a component in this list.
     *
     * @return A list of Entities that have a component in this list.
     */
    std::vector<Entity> GetEntities() const
    {
      std::vector<Entity> entities;

      for(const auto& entityIndexPair : mEntityToIndexMap)
      {
        entities.emplace_back(entityIndexPair.first);
      }

      return entities;
    }

    /**
     * Clears out all components in this list.
     */
    void Clear()
    {
      mEntityToIndexMap.clear();
      mNumValidComponents = 0;
    }

    /**
     * Takes all the components from a given list and adds them into this one.
     *
     * @param aList The list to merge.
     */
    void Merge(ComponentList& aList) override
    {
      auto list = dynamic_cast<ComponentListT<T>*>(&aList);
      if(list == nullptr)
      {
        std::stringstream error;
        error << "Two ComponentLists of different types can't be merged!";
        throw std::invalid_argument(error.str());
      }

      for(const auto& entity : list->GetEntities())
      {
        // Retrieve the component for this Entity and add it to this list.
        auto& component = list->GetComponentForEntity(entity);
        AddComponentToEntity(entity, component);
      }

      // Clear out the merged list.
      list->Clear();
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
      if(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end())
      {
        std::stringstream error;
        error << "Entity " << aEntity << " already has a component of type ";
        error << typeid(T).name();
        throw std::invalid_argument(error.str());
      }

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
      if(mEntityToIndexMap.find(aEntity) == mEntityToIndexMap.end())
      {
        std::stringstream error;
        error << "Entity " << aEntity << " doesn't have a component of type ";
        error << typeid(T).name();
        throw std::invalid_argument(error.str());
      }

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
      if(mEntityToIndexMap.find(aEntity) == mEntityToIndexMap.end())
      {
        std::stringstream error;
        error << "Entity " << aEntity << " doesn't have a component of type ";
        error << typeid(T).name();
        throw std::invalid_argument(error.str());
      }

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
    std::map<Entity, unsigned int> mEntityToIndexMap;
    std::vector<T> mComponents;

    size_t mNumValidComponents { 0 };
};

} // namespace Kuma3D

#endif
