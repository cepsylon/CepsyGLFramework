#pragma once

#include <string>
#include <vector>

class Skeleton;
struct Bone;

namespace fbxsdk
{
	class FbxSkeleton;
}

class SkeletonImporter
{
public:
	static Skeleton * load(const std::string & key, fbxsdk::FbxSkeleton * skeleton);

private:
	static void import_bones_rec(fbxsdk::FbxSkeleton * skeleton, int parent_index, std::vector<Bone> & bones);
};
