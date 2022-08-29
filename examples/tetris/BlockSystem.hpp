#ifndef BLOCKSYSTEM_HPP
#define BLOCKSYSTEM_HPP

#include <System.hpp>

#include <map>
#include <vector>

#include <InputSignals.hpp>
#include <Observer.hpp>

#include "Block.hpp"

namespace Tetris {

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
    void HandleEntityBecameEligible(const Kuma3D::Entity& aEntity) override;

  private:

    /**
     * A handler function that gets called whenever a key is pressed.
     *
     * @param aKey The key that was pressed.
     * @param aMods Any mods active on the key when it was pressed.
     */
    void HandleKeyPressed(const Kuma3D::KeyCode& aKey, int aMods);

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
     * Checks for rows that have been completely filled and removes them.
     */
    void RemoveFilledRows();

    // Maps entities with a block component (positions in 2D) to entities with
    // meshes and transform components (positions in 3D). Each block is
    // composed of four 3D cubes.
    std::map<Kuma3D::Entity, Kuma3D::Entity[4]> mEntityTileMap;

    // Keeps track of new blocks that need to have meshes and transforms
    // created.
    std::vector<Kuma3D::Entity> mNewEntities;

    // Keeps track of all the tiles that have fallen and are no longer moving.
    bool mFallenTiles[25][10];

    Kuma3D::KeyCode mKeyPress { Kuma3D::KeyCode::eKEY_UNKNOWN };

    float mTimeSinceLastFall { 0.0 };

    Kuma3D::Observer mObserver;

    static const int mTileSizeInPixels { 32 };
    static const float mFallSpeed;
};

} // namespace Tetris

#endif
