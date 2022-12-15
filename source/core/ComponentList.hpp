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
     * Clears out all components in this list.
     */
    virtual void Clear() = 0;

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
    void Clear() override
    {
      mComponents.clear();
      mEntityToIndexMap.clear();
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
        // Retrieve the component for this Entity.
        auto& component = list->GetComponentForEntity(entity);

        // Update the EntityToIndex map and move the component into this list.
        int index = mComponents.size();
        mEntityToIndexMap.emplace(entity, index);
        mComponents.emplace_back(std::move(component));
      }

      // Clear out the merged list.
      aList.Clear();
    }

    /**
     * Adds a component to the list. Note that Entities are not allowed
     * to have multiple components of the same type.
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
};

} // namespace Kuma3D

#endif
