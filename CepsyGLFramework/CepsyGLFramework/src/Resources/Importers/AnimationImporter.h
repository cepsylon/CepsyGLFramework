#pragma once

#include "Graphics/Animation.h"

#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <string>

namespace fbxsdk
{
	class FbxScene;
	class FbxNode;
	class FbxAnimStack;
	class FbxAnimLayer;
	class FbxAnimCurve;
}

class Skeleton;

class AnimationImporter
{
public:
	AnimationImporter(const Skeleton & skeleton);
	void load(const std::string & key, fbxsdk::FbxScene * scene);

	struct KeyFloat
	{
		float mT;
		float mValue;
	};

private:
	void import_fbx_stack(fbxsdk::FbxAnimStack * stack, fbxsdk::FbxNode * node);
	void import_fbx_layer(fbxsdk::FbxAnimLayer * stack, fbxsdk::FbxNode * node);
	void import_fbx_channel(fbxsdk::FbxNode * node, fbxsdk::FbxAnimLayer * layer);
	static std::vector<KeyFloat> import_fbx_curve(fbxsdk::FbxAnimCurve * curve);
	static std::vector<Animation::KeyframeRotation> merge_rotation(const std::vector<KeyFloat> & x_rotation, const std::vector<KeyFloat> & y_rotation, const std::vector<KeyFloat> & z_rotation);
	static std::vector<Animation::KeyframeTranslation> merge_translation(const std::vector<KeyFloat> & x, const std::vector<KeyFloat> & y, const std::vector<KeyFloat> & z);

	const Skeleton & mSkeleton;
	Animation mAnimation;
};