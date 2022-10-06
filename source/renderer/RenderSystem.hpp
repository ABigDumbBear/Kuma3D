#ifndef RENDERSYSTEM_HPP
#define RENDERSYSTEM_HPP

#include "System.hpp"

#include <map>

#include "Camera.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"

#include "Mat4.hpp"

#include "Observer.hpp"

namespace Kuma3D {

class RenderSystem : public System
{
  public:

    /**
     * Initializes the System by registering the Mesh and Transform component
     * types, if they aren't registered already. This function also sets the
     * Signature of the System to keep track of Entities with Mesh and
     * Transform components.
     *
     * @param aScene The Scene this System was added to.
     */
    void Initialize(Scene& aScene) override;

    /**
     * Renders each Entity with a Mesh and Transform component.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    void Operate(Scene& aScene, double aTime) override;

    /**
     * Creates a new Entity and attaches a Camera component to it.
     * This function also registers the Camera component in the given Scene
     * if it isn't already registered.
     *
     * @param aScene The Scene to add a camera to.
     * @return The Entity ID of the camera.
     */
    Entity CreateCamera(Scene& aScene);

  protected:

    /**
     * A handler function that gets called whenever an Entity becomes
     * eligible for this System.
     *
     * @param eEntity The Entity that became eligible.
     */
    void HandleEntityBecameEligible(const Entity& aEntity) override;

    /**
     * A handler function that gets called whenever an Entity becomes
     * ineligible for this System.
     *
     * @param eEntity The Entity that became ineligible.
     */
    void HandleEntityBecameIneligible(const Entity& aEntity) override;

  private:

    /**
     * A handler function that gets called just before the game exits.
     *
     * @param aTime The time of exiting.
     */
    void HandleGamePendingExit(double aTime);

    /**
     * Calculates and returns a model matrix that converts a point
     * into the coordinate system defined by the given Transform.
     *
     * @param aTransform The Transform to calculate a matrix for.
     * @return A transformation matrix for the Transform.
     */
    Mat4 CalculateModelMatrix(const Transform& aTransform);

    /**
     * Calculates and returns a view matrix for the given Camera with the
     * given Transform.
     *
     * @param aCamera The Camera to create a matrix for.
     * @param aTransform The Transform assigned to the Camera.
     * @return A view matrix for the Camera.
     */
    Mat4 CalculateViewMatrix(const Camera& aCamera,
                             const Transform& aTransform);

    /**
     * Calculates a projection matrix (either orthographic or perspective) for
     * the given Camera.
     *
     * @param aSystem The coordinate system to use for the projection matrix.
     * @param aCamera The Camera to create a matrix for.
     * @return A projection matrix for the Camera.
     */
    Mat4 CalculateProjectionMatrix(const CoordinateSystem& aSystem,
                                   const Camera& aCamera);

    /**
     * Sorts the given list of Entities by their distance along the given
     * Camera's forward axis.
     *
     * @param aScene The Scene containing the Entities.
     * @param aCamera The Camera to sort by.
     * @param aEntities The Entities to sort.
     */
    void SortEntitiesByCameraDistance(Scene& aScene,
                                      const Entity& aCamera,
                                      std::vector<Entity>& aEntities);

    /**
     * Draws a collection of entities from the perspective of a camera.
     *
     * @param aScene The Scene containing the entities.
     * @param aCamera The Camera from which to draw the entities.
     * @param aEntities The entities to draw.
     */
    void DrawEntities(Scene& aScene,
                      const Entity& aCamera,
                      const std::vector<Entity>& aEntities);

    /**
     * Updates the OpenGL buffer data (vertices and indices) for the given
     * Entity.
     *
     * @param aEntity The Entity to update buffers for.
     * @param aVertices The vertices to copy into the vertex buffer.
     * @param aIndices The indices to copy into the element buffer.
     */
    void UpdateBuffersForEntity(const Entity& aEntity,
                                const std::vector<MeshVertex>& aVertices,
                                const std::vector<unsigned int>& aIndices);

    std::vector<Entity> mCameraEntities;

    std::map<Entity, unsigned int> mVertexArrayMap;
    std::map<Entity, unsigned int> mVertexBufferMap;
    std::map<Entity, unsigned int> mElementBufferMap;

    Observer mObserver;
};

} // namespace Kuma3D

#endif
