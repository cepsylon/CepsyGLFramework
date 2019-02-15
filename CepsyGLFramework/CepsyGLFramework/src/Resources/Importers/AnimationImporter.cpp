#include "AnimationImporter.h"

#include "Graphics/Skeleton.h"
#include "Application/Application.h"

#include <fbxsdk/fbxsdk.h>

#include <string>

AnimationImporter::AnimationImporter(const Skeleton & skeleton)
	: mSkeleton(skeleton)
	, mAnimation(skeleton.bone_count())
{ }

void AnimationImporter::load(FbxScene * scene)
{
	int stack_count = scene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < stack_count; ++i)
	{
		FbxAnimStack * stack = scene->GetSrcObject<FbxAnimStack>(i);
		import_fbx_stack(stack, scene->GetRootNode());
	}

	application.resources().create<Animation>("idle", std::move(mAnimation));
}

void AnimationImporter::import_fbx_stack(FbxAnimStack * stack, fbxsdk::FbxNode * node)
{
	int layer_count = stack->GetMemberCount<FbxAnimLayer>();
	for (int i = 0; i < layer_count; ++i)
	{
		FbxAnimLayer * layer = stack->GetMember<FbxAnimLayer>(i);
		import_fbx_layer(layer, node);
	}
}

void AnimationImporter::import_fbx_layer(FbxAnimLayer * layer, fbxsdk::FbxNode * node)
{
	// Load channels
	import_fbx_channel(node, layer);

	// Load all child animations
	for (int child_index = 0; child_index < node->GetChildCount(); ++child_index)
		import_fbx_layer(layer, node->GetChild(child_index));
}

void AnimationImporter::import_fbx_channel(FbxNode * node, FbxAnimLayer * layer)
{
	// Rotation
	FbxAnimCurve * curve_x = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	FbxAnimCurve * curve_y = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	FbxAnimCurve * curve_z = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve_x && curve_y && curve_z)
	{
		std::vector<Animation::KeyframeRotation> rotation = merge_rotation(import_fbx_curve(curve_x), import_fbx_curve(curve_y), import_fbx_curve(curve_z));

		if (rotation.size() > 1)
		{
			std::string name = node->GetName();
			int bone_index = mSkeleton.find(name);
			if (bone_index != -1)
				mAnimation[bone_index].mRotation = std::move(rotation);
		}
	}

	// Translation
	curve_x = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
	curve_y = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
	curve_z = node->LclTranslation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);
	if (curve_x && curve_y && curve_z)
	{
		std::vector<Animation::KeyframeTranslation> translation = merge_translation(import_fbx_curve(curve_x), import_fbx_curve(curve_y), import_fbx_curve(curve_z));

		if (translation.size() > 1)
		{
			std::string name = node->GetName();
			int bone_index = mSkeleton.find(name);
			if (bone_index != -1)
				mAnimation[bone_index].mTranslation = std::move(translation);
		}
	}
}

std::vector<AnimationImporter::KeyFloat> AnimationImporter::import_fbx_curve(fbxsdk::FbxAnimCurve * curve)
{
	if (curve == nullptr)
		return std::vector<KeyFloat>{};

	std::vector<KeyFloat> channel(curve->KeyGetCount());
	for (unsigned i = 0; i < channel.size(); ++i)
		channel[i] = KeyFloat{ static_cast<float>(curve->KeyGetTime(i).GetSecondDouble()), static_cast<float>(curve->KeyGetValue(i)) };

	return channel;
}

// For now, we assume that the sizes and times always match
std::vector<Animation::KeyframeRotation> AnimationImporter::merge_rotation(const std::vector<KeyFloat> & x_rotation, const std::vector<KeyFloat> & y_rotation, const std::vector<KeyFloat> & z_rotation)
{
	std::vector<Animation::KeyframeRotation> rotation(x_rotation.size());
	for (unsigned i = 0; i < x_rotation.size(); ++i)
		rotation[i] = Animation::KeyframeRotation{ glm::vec3{ glm::radians(x_rotation[i].mValue), glm::radians(y_rotation[i].mValue), glm::radians(z_rotation[i].mValue) }, x_rotation[i].mT };
	return rotation;
}

std::vector<Animation::KeyframeTranslation> AnimationImporter::merge_translation(const std::vector<KeyFloat> & x, const std::vector<KeyFloat> & y, const std::vector<KeyFloat> & z)
{
	std::vector<Animation::KeyframeTranslation> translation(x.size());
	for (unsigned i = 0; i < x.size(); ++i)
		translation[i] = Animation::KeyframeTranslation{ glm::vec3{ x[i].mValue, y[i].mValue, z[i].mValue }, x[i].mT };
	return translation;
}
