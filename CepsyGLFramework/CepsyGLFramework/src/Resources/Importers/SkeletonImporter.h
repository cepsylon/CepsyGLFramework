#pragma once

#include <string>
#include <vector>

class Skeleton;
template <typename T>
class ResourceHandle;

namespace fbxsdk
{
	class FbxSkeleton;
}

class SkeletonImporter
{
public:
	static ResourceHandle<Skeleton> load(const std::string & key, fbxsdk::FbxSkeleton * skeleton_node);

private:
	static void import_bones_rec(fbxsdk::FbxSkeleton * skeleton_node, int parent_index, Skeleton & skeleton);
};
