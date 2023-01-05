#ifndef SCENE_HPP
#define SCENE_HPP

#include <algorithm>
#include <memory>
#include <typeinfo>
#include <unordered_map>
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
    bool IsEntityScheduledForRemoval(Entity aEntity) const;

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
    Signature GetSignatureForEntity(Entity aEntity) const;

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
    unsigned int GetComponentIndex() const
    {
      return mComponentToIndexMap.at(typeid(T).name());
    }

    /**
     * "Registers" a component type by creating a ComponentList for that type.
     *
     * @param aMax The maximum amount of components of this type. The default
     *             is 5000.
     */
    template<typename T>
    void RegisterComponentType(size_t aMax = 5000)
    {
      // Create a new ComponentList.
      mComponentLists.emplace_back(std::make_unique<ComponentListT<T>>(aMax));

      // Update the ComponentToIndex map.
      mComponentToIndexMap.emplace(typeid(T).name(), mComponentLists.size() - 1);

      // Update the EntityToSignature maps.
      UpdateSignatures();
    }

    /**
     * Returns whether a component type has already been registered.
     *
     * @return Whether a component of type T is already registered.
     */
    template<typename T>
    bool IsComponentTypeRegistered() const
    {
      return mComponentToIndexMap.find(typeid(T).name()) != mComponentToIndexMap.end();
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
      auto index = mComponentToIndexMap[typeid(T).name()];
      auto list = dynamic_cast<ComponentListT<T>*>(mComponentLists[index].get());
      list->AddComponentToEntity(aEntity, aComponent);

      // Update the buffered EntityToSignature map.
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
      auto index = mComponentToIndexMap[typeid(T).name()];
      auto list = dynamic_cast<ComponentListT<T>*>(mComponentLists[index].get());

      T component;
      list->AddComponentToEntity(aEntity, component);

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
      // Schedule this component on the given Entity for removal.
      std::pair<Entity, unsigned int> entityComponentPair(aEntity, GetComponentIndex<T>());
      mComponentsToRemove.emplace_back(entityComponentPair);
    };

    /**
     * Returns a component of type T associated with the given Entity.
     *
     * @param aEntity The Entity to retrieve a component for.
     * @return A component of type T associated with the given Entity.
     */
    template<typename T>
    const T& GetComponentForEntity(Entity aEntity) const
    {
      auto componentIndex = GetComponentIndex<T>();
      auto list = dynamic_cast<ComponentListT<T>*>(mComponentLists.at(componentIndex).get());
      return list->GetComponentForEntity(aEntity);
    }

    /**
     * Returns a component of type T associated with the given Entity. This
     * version of the function provides write access.
     *
     * @param aEntity The Entity to retrieve a component for.
     * @return A component of type T associated with the given Entity.
     */
    template<typename T>
    T& GetComponentForEntity(Entity aEntity)
    {
      auto constScene = const_cast<const Scene*>(this);
      return const_cast<T&>(constScene->GetComponentForEntity<T>(aEntity));
    }

  private:

    /**
     * Updates the Signature of each Entity.
     */
    void UpdateSignatures();

    // Contains each System currently in the Scene.
    std::vector<std::unique_ptr<System>> mSystems;

    // Contains a list for each component type in the Scene.
    std::vector<std::unique_ptr<ComponentList>> mComponentLists;

    // Maps component type names to the index of their corresponding ComponentList.
    std::unordered_map<const char*, unsigned int> mComponentToIndexMap;

    // Represents the current state of each Entity in the Scene.
    std::unordered_map<Entity, Signature> mEntityToSignatureMap;

    // Represents the buffered state of each Entity in the Scene,
    // which will become current at the end of each frame.
    std::unordered_map<Entity, Signature> mBufferEntityToSignatureMap;

    // Contains all data to remove at the end of each frame.
    std::vector<Entity> mEntitiesToRemove;
    std::vector<std::pair<Entity, unsigned int>> mComponentsToRemove;

    IDGenerator mEntityGenerator;
};

} // namespace Kuma3D

#endif
