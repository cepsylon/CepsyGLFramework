#include "AnimationImporter.h"

#include <fbxsdk/fbxsdk.h>

#include <string>

void AnimationImporter::load(FbxScene * scene)
{
	int stack_count = scene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < stack_count; ++i)
	{
		FbxAnimStack * stack = scene->GetSrcObject<FbxAnimStack>(i);
		import_fbx_stack(stack, scene->GetRootNode());
	}
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
	// Loop all properties
	for (FbxProperty property = node->GetFirstProperty(); property.IsValid(); property = node->GetNextProperty(property))
	{
		FbxAnimCurve * curve_x = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X);
		FbxAnimCurve * curve_y = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y);
		FbxAnimCurve * curve_z = node->LclRotation.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z);

		std::vector<Key> rotation_x = import_fbx_curve(curve_x);
		std::vector<Key> rotation_y = import_fbx_curve(curve_y);
		std::vector<Key> rotation_z = import_fbx_curve(curve_z);
	}
}

std::vector<AnimationImporter::Key> AnimationImporter::import_fbx_curve(fbxsdk::FbxAnimCurve * curve)
{
	if (curve)
		return std::vector<Key>{};

	std::vector<Key> channel(curve->KeyGetCount());
	FbxTime time = curve->KeyGetTime(0);
	for (unsigned i = 0; i < channel.size(); ++i)
		channel[i] = Key{ curve->KeyGetTime(i).GetSecondDouble(), static_cast<float>(curve->KeyGetValue(i)) };
}
