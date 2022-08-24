#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <map>
#include <memory>
#include <vector>

#include <sstream>
#include <stdexcept>

#include "ComponentList.hpp"
#include "IDGenerator.hpp"
#include "System.hpp"
#include "Types.hpp"

#include "EntitySignals.hpp"

namespace Kuma3D {

/**
 * A Scene contains all the information necessary for a single level,
 * area, screen, etc. in a game. An example of this could be the
 * inside of a house, or a dungeon, or a wide-open field.
 *
 * More specifically, a Scene contains a separate list of components for
 * each type of component needed, as well as a map of Entity IDs to indexes,
 * which can be used to look up a component for a specific Entity.
 */
class Scene
{
  public:

    /**
     * Asks each system to perform its logic. Note that systems will
     * perform logic in the order in which they were added.
     *
     * @param aTime The time at the start of the current frame.
     */
    void OperateSystems(double aTime);

    /**
     * Creates and returns a unique Entity ID.
     *
     * @return A unique Entity ID.
     */
    Entity CreateEntity();

    /**
     * Schedules an Entity for removal, along with all of its components.
     * The removal won't actually occur until the end of the next call to
     * OperateSystems().
     *
     * @param aEntity The Entity ID to remove.
     */
    void RemoveEntity(const Entity& aEntity);

    /**
     * Takes ownership of a System and asks it to operate on each of its
     * Entities during OperateSystems().
     *
     * @param aSystem The system to add.
     */
    void AddSystem(std::unique_ptr<System> aSystem);

    /**
     * Creates and returns an empty Signature for use with a System.
     *
     * @return An empty Signature.
     */
    Signature CreateSignature() const;

    /**
     * Returns the index of a component type.
     *
     * @return The index of a component type.
     */
    template<typename T>
    unsigned int GetComponentIndex()
    {
      auto list = GetComponentListForType<T>();
      if(list == nullptr)
      {
        std::stringstream error;
        error << "Component type " << typeid(T).name() << " hasn't been registered!";
        throw std::logic_error(error.str());
      }

      unsigned int index = 0;
      for(auto& componentList : mComponentLists)
      {
        if(componentList.get() == list)
        {
          break;
        }
        ++index;
      }

      return index;
    }

    /**
     * "Registers" a component type by creating a vector to store components
     * of that type.
     */
    template<typename T>
    void RegisterComponentType()
    {
      if(IsComponentTypeRegistered<T>())
      {
        std::stringstream error;
        error << "Component type " << typeid(T).name() << " has already been registered!";
        throw std::logic_error(error.str());
      }

      // Create a new ComponentList.
      mComponentLists.emplace_back(std::make_unique<ComponentListT<T>>());

      // Update the EntityToSignature map.
      for(auto& entitySignaturePair : mEntityToSignatureMap)
      {
        entitySignaturePair.second.emplace_back(false);
      }
    }

    /**
     * Returns whether a component type has already been registered.
     *
     * @return Whether a component of type T is already registered.
     */
    template<typename T>
    bool IsComponentTypeRegistered()
    {
      return (GetComponentListForType<T>() != nullptr);
    }

    /**
     * Adds a component of type T to the appropriate list and associates
     * it with the given Entity.
     *
     * @param aEntity The Entity to add a component to.
     * @param aComponent The component to add.
     */
    template<typename T>
    void AddComponentToEntity(const Entity& aEntity, T& aComponent)
    {
      auto list = GetComponentListForType<T>();
      if(list == nullptr)
      {
        std::stringstream error;
        error << "Component type " << typeid(T).name() << " hasn't been registered!";
        throw std::logic_error(error.str());
      }

      list->AddComponentToEntity(aEntity, aComponent);

      // Update the EntityToSignature map.
      mEntityToSignatureMap[aEntity][GetComponentIndex<T>()] = true;
      EntitySignatureChanged.Notify(aEntity, mEntityToSignatureMap[aEntity]);
    }

    /**
     * Adds a component of type T to the appropriate list and associates
     * it with the given Entity. This version of the function creates a
     * component of type T, rather than taking one as a parameter.
     *
     * @param aEntity The Entity to add a component to.
     */
    template<typename T>
    void AddComponentToEntity(const Entity& aEntity)
    {
      auto list = GetComponentListForType<T>();
      if(list == nullptr)
      {
        std::stringstream error;
        error << "Component type " << typeid(T).name() << " hasn't been registered!";
        throw std::logic_error(error.str());
      }

      T component;
      list->AddComponentToEntity(aEntity, component);

      // Update the EntityToSignature map.
      mEntityToSignatureMap[aEntity][GetComponentIndex<T>()] = true;
      EntitySignatureChanged.Notify(aEntity, mEntityToSignatureMap[aEntity]);
    }

    /**
     * Schedules a removal of a component of type T from the given Entity.
     * The removal won't actually occur until the end of the next call to
     * OperateSystems().
     *
     * @param aEntity The Entity to remove a component from.
     */
    template<typename T>
    void RemoveComponentFromEntity(const Entity& aEntity)
    {
      if(GetComponentListForType<T>() == nullptr)
      {
        std::stringstream error;
        error << "Component type " << typeid(T).name() << " hasn't been registered!";
        throw std::logic_error(error.str());
      }

      // Schedule this component on the given Entity for removal.
      std::pair<Entity, unsigned int> entityComponentPair(aEntity, GetComponentIndex<T>());
      mComponentsToRemove.emplace_back(entityComponentPair);
    };

    /**
     * Returns whether the given Entity has a component of type T.
     *
     * @param aEntity The Entity to check.
     * @return Whether the Entity has a component of type T.
     */
    template<typename T>
    bool DoesEntityHaveComponent(const Entity& aEntity)
    {
      return mEntityToSignatureMap[aEntity][GetComponentIndex<T>()];
    };

    /**
     * Returns a component of type T associated with the given Entity.
     *
     * @param aEntity The Entity to retrieve a component for.
     * @return A component of type T associated with the given Entity.
     */
    template<typename T>
    T& GetComponentForEntity(const Entity& aEntity)
    {
      auto list = GetComponentListForType<T>();
      if(list == nullptr)
      {
        std::stringstream error;
        error << "Component type " << typeid(T).name() << " hasn't been registered!";
        throw std::logic_error(error.str());
      }

      return list->GetComponentForEntity(aEntity);
    }

  private:

    /**
     * Returns a list of components for a given type.
     *
     * @return A list of components for a given type, or nullptr if
     *         no such list exists.
     */
    template<typename T>
    ComponentListT<T>* GetComponentListForType()
    {
      ComponentListT<T>* componentList = nullptr;
      for(auto& list : mComponentLists)
      {
        componentList = dynamic_cast<ComponentListT<T>*>(list.get());
        if(componentList != nullptr)
        {
          break;
        }
      }

      return componentList;
    }

    // Keeps track of the components associated with an Entity.
    std::map<Entity, Signature> mEntityToSignatureMap;

    std::vector<Entity> mEntitiesToRemove;
    std::vector<std::pair<Entity, unsigned int>> mComponentsToRemove;

    std::vector<std::unique_ptr<ComponentList>> mComponentLists;
    std::vector<std::unique_ptr<System>> mSystems;

    IDGenerator mEntityGenerator;
};

} // namespace Kuma3D

#endif
