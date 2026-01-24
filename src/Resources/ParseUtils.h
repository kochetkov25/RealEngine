#pragma once

#include <memory>

#include "MeshAsset.h"
#include "SkeletonAsset.h"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"

namespace Resources {

/**
 * @brief Parse an Assimp mesh and material into a MeshAsset
 *
 * Extracts geometry data (vertices, normals, texture coordinates), texture
 * information, and optional bone data from an Assimp mesh and its associated
 * material.
 *
 * @param mesh The Assimp mesh to parse (must not be null)
 * @param material The material associated with the mesh (must not be null)
 * @return A shared pointer to the parsed MeshAsset, or nullptr on failure
 *
 * @note The function validates input parameters and mesh data. Returns nullptr
 * if:
 *   - mesh or material is null
 *   - mesh has no vertices
 *   - mesh has no positions
 *
 * @note Texture coordinates are only parsed if the mesh has texture coordinates
 * available. Missing texture coordinates will result in an empty texCoords
 * vector.
 *
 * @note Bone data is only parsed if the mesh has bones. The bone data is
 * stored in an optional that will be empty if no bones are present.
 */
[[nodiscard]] std::shared_ptr<MeshAsset> parseMesh(const aiMesh *mesh, const aiMaterial *material);

[[nodiscard]] std::shared_ptr<SkeletonAsset> parseSkeleton(const aiScene *scene, const aiMesh *mesh);

}  // namespace Resources
