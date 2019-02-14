#pragma once

#include <vector>

namespace fbxsdk
{
	class FbxScene;
	class FbxNode;
	class FbxAnimStack;
	class FbxAnimLayer;
	class FbxAnimCurve;
}

class AnimationImporter
{
public:
	void load(fbxsdk::FbxScene * scene);

	struct Key
	{
		float mT;
		float mValue;
	};

private:
	void import_fbx_stack(fbxsdk::FbxAnimStack * stack, fbxsdk::FbxNode * node);
	void import_fbx_layer(fbxsdk::FbxAnimLayer * stack, fbxsdk::FbxNode * node);
	void import_fbx_channel(fbxsdk::FbxNode * node, fbxsdk::FbxAnimLayer * layer);
	std::vector<Key> import_fbx_curve(fbxsdk::FbxAnimCurve * curve);
};