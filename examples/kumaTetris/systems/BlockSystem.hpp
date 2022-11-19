#ifndef BLOCKSYSTEM_HPP
#define BLOCKSYSTEM_HPP

#include <System.hpp>

#include <map>
#include <vector>

#include <InputSignals.hpp>
#include <Observer.hpp>

#include "Block.hpp"

namespace KumaTetris {

class BlockSystem : public Kuma3D::System
{
  public:

    /**
     * Initializes the System by registering the relevant components.
     *
     * @param aScene The Scene this System was added to.
     */
    void Initialize(Kuma3D::Scene& aScene) override;

    /**
     * Moves each Entity with a Block component.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    void Operate(Kuma3D::Scene& aScene, double aTime) override;

  protected:

    /**
     * A handler function that gets called whenever an Entity is added
     * to this System.
     *
     * @param aEntity The Entity that was added.
     */
    void HandleEntityBecameEligible(Kuma3D::Entity aEntity) override;

  private:

    /**
     * A handler function that gets called whenever a key is pressed.
     *
     * @param aKey The key that was pressed.
     * @param aMods Any mods active on the key when it was pressed.
     */
    void HandleKeyPressed(const Kuma3D::KeyCode& aKey, int aMods);

    /**
     * A handler function that gets called whenever a key is released.
     *
     * @param aKey The key that was released.
     * @param aMods Any mods active on the key when it was pressed.
     */
    void HandleKeyReleased(const Kuma3D::KeyCode& aKey, int aMods);

    /**
     * Checks a given Block against the list of fallen tiles to check
     * if there's a collision.
     *
     * @param aBlock The Block to check for collisions.
     * @return Whether there is a collision or not.
     */
    bool IsBlockColliding(const Block& aBlock);

    /**
     * Checks if it is possible to move a given Block to a given GridPosition.
     *
     * @param aBlock The Block to move.
     * @param aPosition The position to move the Block to.
     * @return Whether it's possible to move the Block.
     */
    bool IsMoveValid(const Block& aBlock, const GridPosition& aPosition);

    /**
     * Checks if it is possible to rotate a given Block at is current position.
     *
     * @param aBlock The Block to move.
     * @param aRotation The rotation to check.
     * @return Whether it's possible to rotate the Block.
     */
    bool IsRotationValid(const Block& aBlock, int aRotation);

    /**
     * Checks for filled rows and removes them.
     */
    void RemoveFilledRows();

    /**
     * Updates the Transform components of each tile Entity based on their
     * position in the grid.
     *
     * @param aScene The Scene containing the Entity data.
     */
    void UpdateFallenTileEntities(Kuma3D::Scene& aScene);

    std::map<Kuma3D::Entity, Kuma3D::Entity[4]> mEntityTileMap;
    std::vector<Kuma3D::Entity> mNewEntities;
    std::vector<Kuma3D::Entity> mEntitiesToRemove;

    bool mFallenTiles[25][10];
    Kuma3D::Entity mFallenTileEntities[25][10];

    std::vector<Kuma3D::KeyCode> mLastFrameKeyPresses;
    std::vector<Kuma3D::KeyCode> mPressedKeys;

    float mTimeSinceLastFall { 0.0 };

    Kuma3D::Observer mObserver;

    static const int mTileSizeInPixels { 32 };
    static const float mDefaultFallSpeed;
};

} // namespace KumaTetris

#endif
