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

    // Update the Entity's Signature.
    mEntityToSignatureMap[entity][index] = false;
    EntitySignatureChanged.Notify(entity, mEntityToSignatureMap[entity]);
  }
  mComponentsToRemove.clear();

  // Remove all entities that have been scheduled for removal.
  for(const auto& entity : mEntitiesToRemove)
  {
    EntityPendingDeletion.Notify(entity, *this);
    mEntityToSignatureMap.erase(entity);
    mEntityGenerator.RemoveID(entity);
  }
  mEntitiesToRemove.clear();

  // Update the current EntityToSignatureMap.
  for(const auto& entitySignaturePair : mBufferEntityToSignatureMap)
  {
    auto entity = entitySignaturePair.first;
    auto signature = entitySignaturePair.second;
    mEntityToSignatureMap[entity] = signature;

    EntitySignatureChanged.Notify(entity, mEntityToSignatureMap[entity]);
  }
  mBufferEntityToSignatureMap.clear();
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
  mBufferEntityToSignatureMap[newEntity] = CreateSignature();

  return newEntity;
}

/******************************************************************************/
void Scene::RemoveEntity(Entity aEntity)
{
  if(mEntityToSignatureMap.find(aEntity) == mEntityToSignatureMap.end())
  {
    std::stringstream error;
    error << "Entity " << aEntity << " doesn't exist!";
    throw std::invalid_argument(error.str());
  }

  // Don't add the Entity to the removal list twice.
  if(!IsEntityScheduledForRemoval(aEntity))
  {
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
    EntityRemoved.Notify(aEntity, *this);
  }
}

/******************************************************************************/
bool Scene::IsEntityScheduledForRemoval(Entity aEntity) const
{
  auto foundEntity = std::find(mEntitiesToRemove.begin(), mEntitiesToRemove.end(), aEntity);
  return foundEntity != mEntitiesToRemove.end();
}

/******************************************************************************/
std::vector<Entity> Scene::GetEntitiesWithSignature(const Signature& aSignature) const
{
  std::vector<Entity> entities;

  for(const auto& entitySignaturePair : mEntityToSignatureMap)
  {
    if(IsSignatureRelevant(entitySignaturePair.second, aSignature))
    {
      entities.emplace_back(entitySignaturePair.first);
    }
  }

  return entities;
}

/******************************************************************************/
Signature Scene::GetSignatureForEntity(Entity aEntity) const
{
  auto foundEntity = mEntityToSignatureMap.find(aEntity);
  if(foundEntity == mEntityToSignatureMap.end())
  {
    std::stringstream error;
    error << "Entity " << aEntity << " doesn't exist!";
    throw std::invalid_argument(error.str());
  }

  return mEntityToSignatureMap.at(aEntity);
}

/******************************************************************************/
Signature Scene::CreateSignature() const
{
  return Signature(mComponentLists.size(), false);
}

/******************************************************************************/
void Scene::UpdateSignatures()
{
  for(auto& entitySignaturePair : mEntityToSignatureMap)
  {
    auto currentSignature = entitySignaturePair.second;
    auto newSignature = CreateSignature();
    for(int i = 0; i < currentSignature.size(); ++i)
    {
      newSignature[i] = currentSignature[i];
    }

    entitySignaturePair.second = newSignature;
  }

  for(auto& entitySignaturePair : mBufferEntityToSignatureMap)
  {
    auto currentSignature = entitySignaturePair.second;
    auto newSignature = CreateSignature();
    for(int i = 0; i < currentSignature.size(); ++i)
    {
      newSignature[i] = currentSignature[i];
    }

    entitySignaturePair.second = newSignature;
  }
}

} // namespace Kuma3D
