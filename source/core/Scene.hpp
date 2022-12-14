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

#include "Entity.hpp"
#include "Signature.hpp"

namespace Kuma3D {

/**
 * A Scene contains all the information necessary for a single level,
 * area, screen, etc. in a game. An example of this could be the
 * inside of a house, or a dungeon, or a wide-open field.
 *
 * More specifically, a Scene contains a separate list of components for
 * each type of component needed, as well as a map of Entity IDs to indexes,
 * which can be used to look up a component for a specific Entity.
 *
 * When adding a component, the component is kept in a buffer until the
 * end of the frame in which it was added. While it is in the buffer, it
 * can't be accessed, and the Entity it's associated with is treated as
 * not having that component. This ensures that no System operates on the
 * component until the next frame.
 *
 * Removing Entities and components works the same way; it doesn't get
 * removed until the end of the frame in which RemoveEntity() or
 * RemoveComponentFromEntity() was called. This ensures that no Entities
 * or components are skipped by any Systems.
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
    void RemoveEntity(Entity aEntity);

    /**
     * Returns whether an Entity is scheduled for removal at the end of the
     * current frame.
     *
     * @param aEntity The Entity to check.
     * @return Whether the Entity is scheduled for removal.
     */
    bool IsEntityScheduledForRemoval(Entity aEntity);

    /**
     * Returns a list of Entities that fit a given Signature.
     *
     * @param aSignature The Signature to retrieve Entities for.
     * @return A list of Entities that fit the Signature.
     */
    std::vector<Entity> GetEntitiesWithSignature(const Signature& aSignature) const;

    /**
     * Returns the Signature for a given Entity.
     *
     * @param aEntity The Entity to retrieve the Signature for.
     * @return The Signature for the Entity.
     */
    Signature GetSignatureForEntity(Entity aEntity);

    /**
     * Takes ownership of a System and asks it to operate on each of its
     * Entities during OperateSystems().
     *
     * @param aSystem The system to add.
     */
    void AddSystem(std::unique_ptr<System> aSystem);

    /**
     * Returns whether the Scene already contains a System of the given type.
     *
     * @return Whether the Scene already contains a System of the given type.
     */
    template<typename T>
    bool ContainsSystemOfType()
    {
      bool containsSystem = false;
      for(const auto& system : mSystems)
      {
        if(dynamic_cast<T*>(system.get()))
        {
          containsSystem = true;
          break;
        }
      }

      return containsSystem;
    }

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

      // Create two new ComponentLists.
      mComponentLists.emplace_back(std::make_unique<ComponentListT<T>>());
      mBufferLists.emplace_back(std::make_unique<ComponentListT<T>>());

      // Update the EntityToSignature maps.
      UpdateSignatures();
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
    void AddComponentToEntity(Entity aEntity, T& aComponent)
    {
      auto bufferList = GetBufferComponentListForType<T>();
      if(bufferList == nullptr)
      {
        std::stringstream error;
        error << "Component type " << typeid(T).name() << " hasn't been registered!";
        throw std::logic_error(error.str());
      }

      bufferList->AddComponentToEntity(aEntity, aComponent);

      // Update the EntityToSignature map.
      if(mBufferEntityToSignatureMap[aEntity].empty())
      {
        mBufferEntityToSignatureMap[aEntity] = mEntityToSignatureMap[aEntity];
      }
      mBufferEntityToSignatureMap[aEntity][GetComponentIndex<T>()] = true;
    }

    /**
     * Adds a component of type T to the appropriate list and associates
     * it with the given Entity. This version of the function creates a
     * component of type T, rather than taking one as a parameter.
     *
     * @param aEntity The Entity to add a component to.
     */
    template<typename T>
    void AddComponentToEntity(Entity aEntity)
    {
      auto bufferList = GetBufferComponentListForType<T>();
      if(bufferList == nullptr)
      {
        std::stringstream error;
        error << "Component type " << typeid(T).name() << " hasn't been registered!";
        throw std::logic_error(error.str());
      }

      T component;
      bufferList->AddComponentToEntity(aEntity, component);

      // Update the EntityToSignature map.
      if(mBufferEntityToSignatureMap[aEntity].empty())
      {
        mBufferEntityToSignatureMap[aEntity] = mEntityToSignatureMap[aEntity];
      }
      mBufferEntityToSignatureMap[aEntity][GetComponentIndex<T>()] = true;
    }

    /**
     * Schedules a removal of a component of type T from the given Entity.
     * The removal won't actually occur until the end of the next call to
     * OperateSystems().
     *
     * @param aEntity The Entity to remove a component from.
     */
    template<typename T>
    void RemoveComponentFromEntity(Entity aEntity)
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
    bool DoesEntityHaveComponent(Entity aEntity)
    {
      bool success = false;

      // Check the current map.
      auto foundEntity = mEntityToSignatureMap.find(aEntity);
      if(foundEntity != mEntityToSignatureMap.end())
      {
        success = (*foundEntity).second[GetComponentIndex<T>()];
      }

      if(!success)
      {
        // Check the buffer map.
        auto foundBufferEntity = mBufferEntityToSignatureMap.find(aEntity);
        if(foundBufferEntity != mBufferEntityToSignatureMap.end())
        {
          success = (*foundBufferEntity).second[GetComponentIndex<T>()];
        }
      }

      return success;
    };

    /**
     * Returns a component of type T associated with the given Entity.
     *
     * @param aEntity The Entity to retrieve a component for.
     * @return A component of type T associated with the given Entity.
     */
    template<typename T>
    T& GetComponentForEntity(Entity aEntity)
    {
      if(!IsComponentTypeRegistered<T>())
      {
        std::stringstream error;
        error << "Component type " << typeid(T).name() << " hasn't been registered!";
        throw std::logic_error(error.str());
      }

      ComponentListT<T>* list = nullptr;
      auto componentIndex = GetComponentIndex<T>();

      // Determine whether to query the current ComponentList or the
      // buffer ComponentList.
      if(mEntityToSignatureMap[aEntity][componentIndex])
      {
        list = GetComponentListForType<T>();
      }
      else if(mBufferEntityToSignatureMap[aEntity][componentIndex])
      {
        list = GetBufferComponentListForType<T>();
      }

      if(list == nullptr)
      {
        std::stringstream error;
        error << "Entity " << aEntity << " doesn't have a component of type ";
        error << typeid(T).name();
        throw std::invalid_argument(error.str());
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

    /**
     * Returns a list of buffer components for a given type.
     *
     * @return A list of buffer components for a given type, or nullptr if
     *         no such list exists.
     */
    template<typename T>
    ComponentListT<T>* GetBufferComponentListForType()
    {
      ComponentListT<T>* componentList = nullptr;
      for(auto& list : mBufferLists)
      {
        componentList = dynamic_cast<ComponentListT<T>*>(list.get());
        if(componentList != nullptr)
        {
          break;
        }
      }

      return componentList;
    }

    /**
     * Updates the Signature of each Entity.
     */
    void UpdateSignatures();

    // Contains each System currently in the Scene.
    std::vector<std::unique_ptr<System>> mSystems;

    // Represents the current state of each Entity in the Scene.
    std::map<Entity, Signature> mEntityToSignatureMap;
    std::vector<std::unique_ptr<ComponentList>> mComponentLists;

    // Represents the buffered state of each Entity in the Scene,
    // which will become current at the end of each frame.
    std::map<Entity, Signature> mBufferEntityToSignatureMap;
    std::vector<std::unique_ptr<ComponentList>> mBufferLists;

    // Contains all data to remove at the end of each frame.
    std::vector<Entity> mEntitiesToRemove;
    std::vector<std::pair<Entity, unsigned int>> mComponentsToRemove;

    IDGenerator mEntityGenerator;
};

} // namespace Kuma3D

#endif
