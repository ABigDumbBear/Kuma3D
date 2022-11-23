#include "System.hpp"

#include <algorithm>
#include <functional>

#include "EntitySignals.hpp"

namespace Kuma3D {

/******************************************************************************/
System::System()
{
  EntitySignatureChanged.Connect(mObserver, [this](const Entity& aEntity,
                                                   const Signature& aSignature)
  {
    this->HandleEntitySignatureChanged(aEntity, aSignature);
  });

  EntityPendingDeletion.Connect(mObserver, [this](const Entity& aEntity)
  {
    this->HandleEntityPendingDeletion(aEntity);
  });
}

/******************************************************************************/
void System::SetSignature(const Signature& aSignature)
{
  mSignature = aSignature;
  mEntities.clear();
}

/******************************************************************************/
void System::HandleEntitySignatureChanged(const Entity& aEntity,
                                          const Signature& aSignature)
{
  auto relevant = IsSignatureRelevant(aSignature, mSignature);
  auto foundEntity = std::find(mEntities.begin(), mEntities.end(), aEntity);
  if(foundEntity != mEntities.end())
  {
    // This Entity is already being kept track of, so check if the Signature
    // is still relevant for this system. If it isn't, remove the Entity.
    if(!relevant)
    {
      HandleEntityBecameIneligible(*foundEntity);
      mEntities.erase(foundEntity);
    }
  }
  else if(relevant)
  {
    // Start keeping track of this Entity.
    HandleEntityBecameEligible(aEntity);
    mEntities.emplace_back(aEntity);
  }
}

/******************************************************************************/
void System::HandleEntityPendingDeletion(const Entity& aEntity)
{
  auto foundEntity = std::find(mEntities.begin(), mEntities.end(), aEntity);
  if(foundEntity != mEntities.end())
  {
    HandleEntityBecameIneligible(aEntity);
    mEntities.erase(foundEntity);
  }
}

} // namespace Kuma3D
