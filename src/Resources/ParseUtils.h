#pragma once

#include <memory>

#include "AnimationAsset.h"
#include "MeshAsset.h"
#include "SkeletonAsset.h"
#include "assimp/material.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"

namespace Resources {
[[nodiscard]] std::shared_ptr<MeshAsset> parseMesh(const aiMesh *mesh, const aiMaterial *material);

[[nodiscard]] std::shared_ptr<SkeletonAsset> parseSkeleton(const aiScene *scene);

[[nodiscard]] std::shared_ptr<AnimationAsset> parseAnimation(const aiAnimation *animation,
                                                             const SkeletonAsset &skeleton);
}  // namespace Resources
