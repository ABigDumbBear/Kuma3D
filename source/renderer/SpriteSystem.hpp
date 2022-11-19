#ifndef SPRITESYSTEM_HPP
#define SPRITESYSTEM_HPP

#include <map>

#include "System.hpp"

#include "Mesh.hpp"
#include "Sprite.hpp"

namespace Kuma3D {

class SpriteSystem : public System
{
  public:

    /**
     * Initializes the System by registering the Sprite and Mesh components.
     *
     * @param aScene The Scene this System was added to.
     */
    void Initialize(Scene& aScene) override;

    /**
     * Updates the Mesh of each Sprite.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    void Operate(Scene& aScene, double aTime) override;

  protected:

    /**
     * A handler function that gets called whenever an Entity becomes
     * eligible for this System.
     *
     * @param eEntity The Entity that became eligible.
     */
    void HandleEntityBecameEligible(Entity aEntity) override;

    /**
     * A handler function that gets called whenever an Entity becomes
     * ineligible for this System.
     *
     * @param eEntity The Entity that became ineligible.
     */
    void HandleEntityBecameIneligible(Entity aEntity) override;

  private:

    /**
     * Updates a Mesh to display the given Sprite.
     *
     * @param aMesh The Mesh to update.
     * @param aSprite The Sprite to display.
     */
    void UpdateMeshToDisplaySprite(Mesh& aMesh, const Sprite& aSprite);

    std::vector<Entity> mNewEntities;
    std::map<Entity, double> mEntityTimeMap;
};

} // namespace Kuma3D

#endif
