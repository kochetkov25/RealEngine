#pragma once

#include <memory>

struct aiAnimation;
struct aiMaterial;
struct aiMesh;
struct aiScene;

namespace Resources {
class AnimationAsset;
class MeshAsset;
class SkeletonAsset;

[[nodiscard]] std::shared_ptr<MeshAsset> parseMesh(const aiMesh *mesh, const aiMaterial *material);

[[nodiscard]] std::shared_ptr<SkeletonAsset> parseSkeleton(const aiScene *scene);

[[nodiscard]] std::shared_ptr<AnimationAsset> parseAnimation(const aiAnimation *animation,
                                                             const SkeletonAsset &skeleton);
}  // namespace Resources
