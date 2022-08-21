#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>

#include "Observer.hpp"
#include "Types.hpp"

namespace Kuma3D {

class Scene;

/**
 * A System performs some logic on each eligible Entity. An Entity is
 * considered eligible if its Signature contains the System's Signature.
 *
 * For example, if an Entity's Signature is 0111, and the System's
 * Signature is 0101, that Entity is eligible.
 */
class System
{
  public:
    System();
    virtual ~System() = default;

    /**
     * A virtual function that gets called whenever this System is added
     * to a Scene.
     *
     * @param aScene The Scene this System was added to.
     */
    virtual void Initialize(Scene& aScene) {}

    /**
     * A virtual function that performs some logic on each Entity in
     * a Scene.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    virtual void Operate(Scene& aScene, double aTime) = 0;

  protected:

    /**
     * Sets the Signature for this System.
     *
     * @param aSignature The new Signature.
     */
    void SetSignature(const Signature& aSignature);

    /**
     * Returns each eligible Entity.
     *
     * @return Each eligible Entity.
     */
    const std::vector<Entity>& GetEntities() { return mEntities; }

    /**
     * A virtual function that gets called whenever an Entity becomes
     * eligible for this System.
     *
     * @param eEntity The Entity that became eligible.
     */
    virtual void HandleEntityBecameEligible(const Entity& aEntity) {}

    /**
     * A virtual function that gets called whenever an Entity becomes
     * ineligible for this System.
     *
     * @param eEntity The Entity that became ineligible.
     */
    virtual void HandleEntityBecameIneligible(const Entity& aEntity) {}

  private:

    /**
     * A handler function that gets called whenever an Entity's Signature
     * changes. If the new Signature makes the Entity eligible for this
     * System, it gets added to mEntities. If the new Signature makes the
     * Entity ineligible, it gets removed instead.
     *
     * @param aEntity The Entity whose Signature was changed.
     * @param aSignature The new Signature of the Entity.
     */
    void HandleEntitySignatureChanged(const Entity& aEntity,
                                      const Signature& aSignature);

    /**
     * A handler function that gets called just before an Entity is removed
     * from a Scene.
     *
     * @param aEntity The Entity that's about to be removed.
     */
    void HandleEntityPendingDeletion(const Entity& aEntity);

    /**
     * Returns true if the given Signature contains the System's Signature.
     *
     * @return Whether the given Signature contains the System's Signature.
     */
    bool IsSignatureRelevant(const Signature& aSignature);

    std::vector<Entity> mEntities;
    Signature mSignature;

    Observer mObserver;
};

} // namespace Kuma3D

#endif
