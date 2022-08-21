#include "Scene.hpp"

#include "EntitySignals.hpp"

namespace Kuma3D {

/******************************************************************************/
void Scene::OperateSystems(double aTime)
{
  // Perform logic for each System.
  for(auto& system : mSystems)
  {
    system->Operate(*this, aTime);
  }

  // Remove all components that have been scheduled for removal.
  for(const auto& entityComponentPair : mComponentsToRemove)
  {
    auto entity = entityComponentPair.first;
    auto index = entityComponentPair.second;
    mComponentLists[index]->RemoveComponentFromEntity(entity);

    mEntityToSignatureMap[entity][index] = false;
    EntitySignatureChanged.Notify(entity, mEntityToSignatureMap[entity]);
  }
  mComponentsToRemove.clear();

  // Remove all entities that have been scheduled for removal.
  for(const auto& entity : mEntitiesToRemove)
  {
    EntityPendingDeletion.Notify(entity);
    mEntityToSignatureMap.erase(entity);
    mEntityGenerator.RemoveID(entity);
  }
  mEntitiesToRemove.clear();
}

/******************************************************************************/
void Scene::AddSystem(std::unique_ptr<System> aSystem)
{
  mSystems.emplace_back(std::move(aSystem));
  mSystems.back()->Initialize(*this);
}

/******************************************************************************/
Entity Scene::CreateEntity()
{
  auto newEntity =  mEntityGenerator.GenerateID();
  mEntityToSignatureMap[newEntity] = CreateSignature();

  return newEntity;
}

/******************************************************************************/
void Scene::RemoveEntity(const Entity& aEntity)
{
  if(mEntityToSignatureMap.find(aEntity) == mEntityToSignatureMap.end())
  {
    std::stringstream error;
    error << "Entity " << aEntity << " doesn't exist!";
    throw std::invalid_argument(error.str());
  }

  // Schedule each component on this entity for removal.
  auto signature = mEntityToSignatureMap[aEntity];
  for(int i = 0; i < signature.size(); ++i)
  {
    if(signature[i])
    {
      std::pair<Entity, unsigned int> entityComponentPair(aEntity, i);
      mComponentsToRemove.emplace_back(entityComponentPair);
    }
  }

  // Schedule the Entity itself for removal.
  mEntitiesToRemove.emplace_back(aEntity);
}

/******************************************************************************/
Signature Scene::CreateSignature() const
{
  return Signature(mComponentLists.size(), false);
}

} // namespace Kuma3D
