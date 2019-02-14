#include "FBXImporter.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "Application/Application.h"
#include "Graphics/Model.h"
#include "Graphics/SkeletalModel.h"

#include <fbxsdk/core/fbxpropertydef.h>
#include <fbxsdk/fbxsdk.h>
#include <glm/vec3.hpp>

#include <vector>


void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher = false);
void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void(*DisplayCurve) (FbxAnimCurve* pCurve), void(*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher);
void DisplayCurveKeys(FbxAnimCurve* pCurve);
void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);

void DisplayAnimation(FbxScene* pScene)
{
	int i;
	for (i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

		FbxString lOutputString = "Animation Stack Name: ";
		lOutputString += lAnimStack->GetName();
		lOutputString += "\n\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimation(lAnimStack, pScene->GetRootNode(), true);
		DisplayAnimation(lAnimStack, pScene->GetRootNode());
	}
}

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
{
	int l;
	int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
	FbxString lOutputString;

	lOutputString = "Animation stack contains ";
	lOutputString += nbAnimLayers;
	lOutputString += " Animation Layer(s)\n";
	FBXSDK_printf(lOutputString);

	for (l = 0; l < nbAnimLayers; l++)
	{
		FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

		lOutputString = "AnimLayer ";
		lOutputString += l;
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimation(lAnimLayer, pNode, isSwitcher);
	}
}

void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
{
	int lModelCount;
	FbxString lOutputString;

	lOutputString = "     Node Name: ";
	lOutputString += pNode->GetName();
	lOutputString += "\n\n";
	FBXSDK_printf(lOutputString);

	DisplayChannels(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher);
	FBXSDK_printf("\n");

	for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{
		DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
	}
}


void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void(*DisplayCurve) (FbxAnimCurve* pCurve), void(*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher)
{
	FbxAnimCurve* lAnimCurve = NULL;

	// Display general curves.
	if (!isSwitcher)
	{
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			FBXSDK_printf("        TX\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			FBXSDK_printf("        TY\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			FBXSDK_printf("        TZ\n");
			DisplayCurve(lAnimCurve);
		}

		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			FBXSDK_printf("        RX\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			FBXSDK_printf("        RY\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			FBXSDK_printf("        RZ\n");
			DisplayCurve(lAnimCurve);
		}

		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			FBXSDK_printf("        SX\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			FBXSDK_printf("        SY\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			FBXSDK_printf("        SZ\n");
			DisplayCurve(lAnimCurve);
		}
	}

	// Display curves specific to a light or marker.
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute)
	{
		lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_RED);
		if (lAnimCurve)
		{
			FBXSDK_printf("        Red\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_GREEN);
		if (lAnimCurve)
		{
			FBXSDK_printf("        Green\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_BLUE);
		if (lAnimCurve)
		{
			FBXSDK_printf("        Blue\n");
			DisplayCurve(lAnimCurve);
		}

		// Display curves specific to a light.
		FbxLight* light = pNode->GetLight();
		if (light)
		{
			lAnimCurve = light->Intensity.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Intensity\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = light->OuterAngle.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Outer Angle\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = light->Fog.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Fog\n");
				DisplayCurve(lAnimCurve);
			}
		}

		// Display curves specific to a camera.
		FbxCamera* camera = pNode->GetCamera();
		if (camera)
		{
			lAnimCurve = camera->FieldOfView.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Field of View\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->FieldOfViewX.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Field of View X\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->FieldOfViewY.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Field of View Y\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->OpticalCenterX.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Optical Center X\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->OpticalCenterY.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Optical Center Y\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->Roll.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Roll\n");
				DisplayCurve(lAnimCurve);
			}
		}

		// Display curves specific to a geometry.
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
			lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
			lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
		{
			FbxGeometry* lGeometry = (FbxGeometry*)lNodeAttribute;

			int lBlendShapeDeformerCount = lGeometry->GetDeformerCount(FbxDeformer::eBlendShape);
			for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeDeformerCount; ++lBlendShapeIndex)
			{
				FbxBlendShape* lBlendShape = (FbxBlendShape*)lGeometry->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

				int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
				for (int lChannelIndex = 0; lChannelIndex < lBlendShapeChannelCount; ++lChannelIndex)
				{
					FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
					const char* lChannelName = lChannel->GetName();

					lAnimCurve = lGeometry->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer, true);
					if (lAnimCurve)
					{
						FBXSDK_printf("        Shape %s\n", lChannelName);
						DisplayCurve(lAnimCurve);
					}
				}
			}
		}
	}

	// Display curves specific to properties
	FbxProperty lProperty = pNode->GetFirstProperty();
	while (lProperty.IsValid())
	{
		
		if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
		{
			FbxString lFbxFCurveNodeName = lProperty.GetName();
			FbxAnimCurveNode* lCurveNode = lProperty.GetCurveNode(pAnimLayer);

			if (!lCurveNode) {
				lProperty = pNode->GetNextProperty(lProperty);
				continue;
			}

			FbxDataType lDataType = lProperty.GetPropertyDataType();
			if (lDataType.GetType() == eFbxBool || lDataType.GetType() == eFbxDouble || lDataType.GetType() == eFbxFloat || lDataType.GetType() == eFbxInt)
			{
				FbxString lMessage;

				lMessage = "        Property ";
				lMessage += lProperty.GetName();
				if (lProperty.GetLabel().GetLen() > 0)
				{
					lMessage += " (Label: ";
					lMessage += lProperty.GetLabel();
					lMessage += ")";
				};

				printf("%s\n", lMessage.Buffer());

				for (int c = 0; c < lCurveNode->GetCurveCount(0U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(0U, c);
					if (lAnimCurve)
						DisplayCurve(lAnimCurve);
				}
			}
			else if (lDataType.GetType() == eFbxDouble3 || lDataType.GetType() == eFbxDouble4 || lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT))
			{
				char* lComponentName1 = (lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_RED : (char*)"X";
				char* lComponentName2 = (lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_GREEN : (char*)"Y";
				char* lComponentName3 = (lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_BLUE : (char*)"Z";
				FbxString      lMessage;

				lMessage = "        Property ";
				lMessage += lProperty.GetName();
				if (lProperty.GetLabel().GetLen() > 0)
				{
					lMessage += " (Label: ";
					lMessage += lProperty.GetLabel();
					lMessage += ")";
				}
				printf("%s\n",lMessage.Buffer());

				for (int c = 0; c < lCurveNode->GetCurveCount(0U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(0U, c);
					if (lAnimCurve)
					{
						printf("        Component %s\n", lComponentName1);
						DisplayCurve(lAnimCurve);
					}
				}

				for (int c = 0; c < lCurveNode->GetCurveCount(1U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(1U, c);
					if (lAnimCurve)
					{
						printf("        Component %s\n", lComponentName2);
						DisplayCurve(lAnimCurve);
					}
				}

				for (int c = 0; c < lCurveNode->GetCurveCount(2U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(2U, c);
					if (lAnimCurve)
					{
						printf("        Component %s\n", lComponentName3);
						DisplayCurve(lAnimCurve);
					}
				}
			}
			else if (lDataType.GetType() == eFbxEnum)
			{
				FbxString lMessage;

				lMessage = "        Property ";
				lMessage += lProperty.GetName();
				if (lProperty.GetLabel().GetLen() > 0)
				{
					lMessage += " (Label: ";
					lMessage += lProperty.GetLabel();
					lMessage += ")";
				};
				printf("%s\n", lMessage.Buffer());

				for (int c = 0; c < lCurveNode->GetCurveCount(0U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(0U, c);
					if (lAnimCurve)
						DisplayListCurve(lAnimCurve, &lProperty);
				}
			}
		}

		lProperty = pNode->GetNextProperty(lProperty);
	} // while

}


static int InterpolationFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant) return 1;
	if ((flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear) return 2;
	if ((flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic) return 3;
	return 0;
}

static int ConstantmodeFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard) return 1;
	if ((flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext) return 2;
	return 0;
}

static int TangentmodeFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto) return 1;
	if ((flags & FbxAnimCurveDef::eTangentAutoBreak) == FbxAnimCurveDef::eTangentAutoBreak) return 2;
	if ((flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB) return 3;
	if ((flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser) return 4;
	if ((flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak) return 5;
	if ((flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak) return 6;
	return 0;
}

static int TangentweightFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone) return 1;
	if ((flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight) return 2;
	if ((flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft) return 3;
	return 0;
}

static int TangentVelocityFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone) return 1;
	if ((flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight) return 2;
	if ((flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft) return 3;
	return 0;
}

void DisplayCurveKeys(FbxAnimCurve* pCurve)
{
	static const char* interpolation[] = { "?", "constant", "linear", "cubic" };
	static const char* constantMode[] = { "?", "Standard", "Next" };
	static const char* cubicMode[] = { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
	static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

	FbxTime   lKeyTime;
	float   lKeyValue;
	char    lTimeString[256];
	FbxString lOutputString;
	int     lCount;

	int lKeyCount = pCurve->KeyGetCount();

	for (lCount = 0; lCount < lKeyCount; lCount++)
	{
		lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
		lKeyTime = pCurve->KeyGetTime(lCount);

		lOutputString = "            Key Time: ";
		lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
		lOutputString += ".... Key Value: ";
		lOutputString += lKeyValue;
		lOutputString += " [ ";
		lOutputString += interpolation[InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount))];
		if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
		{
			lOutputString += " | ";
			lOutputString += constantMode[ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount))];
		}
		else if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
		{
			lOutputString += " | ";
			lOutputString += cubicMode[TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount))];
			lOutputString += " | ";
			lOutputString += tangentWVMode[TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode())];
			lOutputString += " | ";
			lOutputString += tangentWVMode[TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode())];
		}
		lOutputString += " ]";
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}
}

void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
{
	FbxTime   lKeyTime;
	int     lKeyValue;
	char    lTimeString[256];
	FbxString lListValue;
	FbxString lOutputString;
	int     lCount;

	int lKeyCount = pCurve->KeyGetCount();

	for (lCount = 0; lCount < lKeyCount; lCount++)
	{
		lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
		lKeyTime = pCurve->KeyGetTime(lCount);

		lOutputString = "            Key Time: ";
		lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
		lOutputString += ".... Key Value: ";
		lOutputString += lKeyValue;
		lOutputString += " (";
		lOutputString += pProperty->GetEnumValue(lKeyValue);
		lOutputString += ")";

		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}
}
















void FBXImporter::load(const std::string & path)
{
	// Create new SDK manager
	FbxManager * fbx_manager = FbxManager::Create();

	// Create input settings
	FbxIOSettings * io_settings = FbxIOSettings::Create(fbx_manager, IOSROOT);
	fbx_manager->SetIOSettings(io_settings);

	// Create importer with the path as a name, this does not load the file
	FbxImporter * importer = FbxImporter::Create(fbx_manager, path.c_str());

	// Initialize importer
	if (!importer->Initialize(path.c_str(), -1, fbx_manager->GetIOSettings()))
	{
		printf("Error loading file %s\n", path.c_str());
		return;
	}

	// Create scene and import it
	FbxScene * scene = FbxScene::Create(fbx_manager, "Scene");
	importer->Import(scene);

	// Import all nodes
	FbxNode * root = scene->GetRootNode();
	if (root)
	{
		for (int i = 0; i < root->GetChildCount(); ++i)
			import(root->GetChild(i));
	}

	// Animation importing
	//for (int i = 0; i < scene->GetSrcObjectCount<FbxAnimStack>(); ++i)
	//{
	//	FbxAnimStack * stack = scene->GetSrcObject<FbxAnimStack>(i);
	//	printf("Animation stack name: %s\n", stack->GetName());
	//
	//	// Animation layer
	//	for (int j = 0; j < stack->GetMemberCount<FbxAnimLayer>(); ++j)
	//	{
	//		FbxAnimLayer * layer = stack->GetMember<FbxAnimLayer>(j);
	//		printf("\tAnimation layer name: %s\n", layer->GetName());
	//	}
	//}
	DisplayAnimation(scene);

	// Free everything
	importer->Destroy();
	fbx_manager->Destroy();

	// Create the model
	unsigned start = path.find_last_of("/") + 1;
	unsigned end = path.find_last_of(".");
	std::string name = path.substr(start, end - start);
	if (mSkeleton.empty())
		application.resources().create<Model>(name, std::move(mMeshes), std::move(mMaterials));
	else
		application.resources().create<SkeletalModel>(name, std::move(mMeshes), std::move(mMaterials), std::move(mSkeleton));
}

void FBXImporter::import(FbxNode * node)
{
	// Import all attributes to the node
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		import(node->GetNodeAttributeByIndex(i));

	// Import all children
	for (int i = 0; i < node->GetChildCount(); ++i)
		import(node->GetChild(i));
}

void FBXImporter::import(FbxNodeAttribute * attribute)
{
	if (!attribute)
		return;

	// Check the type and import depending on that
	// For now, we will only import meshes
	switch (attribute->GetAttributeType())
	{
	case FbxNodeAttribute::eMesh:
		import_mesh(attribute);
		break;
	case FbxNodeAttribute::eSkeleton:
	{
		FbxSkeleton * skeleton = static_cast<FbxSkeleton *>(attribute);
		if(skeleton->IsSkeletonRoot())
			import_skeleton(skeleton);
	}
		break;
	default:
		break;
	}
}

void FBXImporter::import_mesh(FbxNodeAttribute * attribute)
{
	// Load mesh
	FbxMesh * mesh = static_cast<FbxMesh *>(attribute);
	mMeshes.emplace_back(MeshImporter::load(mesh, mSkeleton));

	// Check for materials and load
	FbxNode * material_node = mesh->GetNode();
	if (material_node)
	{
		for (int i = 0; i < material_node->GetMaterialCount(); ++i)
			mMaterials.emplace_back(MaterialImporter::load(material_node->GetMaterial(i)));
	}
}

void FBXImporter::import_skeleton(FbxSkeleton * skeleton)
{
	std::vector<Skeleton::Bone> bones;
	import_bones_rec(skeleton, -1, bones);
	for (auto & bone : bones)
		bone.mBindMatrix = glm::inverse(bone.mBindMatrix);
	mSkeleton = Skeleton{ std::move(bones) };
}

void FBXImporter::import_bones_rec(FbxSkeleton * skeleton_node, int parent_index, std::vector<Skeleton::Bone> & bones)
{
	FbxNode * node = skeleton_node->GetNode();
	FbxDouble3 position = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	glm::quat quaternion = glm::vec3{ rotation[0], rotation[1], rotation[2] };
	glm::mat4 bind_matrix = glm::mat4_cast(quaternion);
	bind_matrix[3][0] = static_cast<float>(position[0]);
	bind_matrix[3][1] = static_cast<float>(position[1]);
	bind_matrix[3][2] = static_cast<float>(position[2]);

	// Add child to parent
	int next_parent_index = bones.size();
	if (parent_index != -1)
	{
		bones[parent_index].mChildrenIndices.emplace_back(next_parent_index);
		bind_matrix = bones[parent_index].mBindMatrix * bind_matrix;
	}

	// Add child
	bones.emplace_back(Skeleton::Bone{ bind_matrix, {}, node->GetName(),
		quaternion,
		glm::vec3{ position[0], position[1], position[2] }
	});

	// Import children
	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		for (int j = 0; j < node->GetNodeAttributeCount(); ++j)
		{
			FbxNodeAttribute * attribute = node->GetChild(i)->GetNodeAttributeByIndex(j);
			if (attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				import_bones_rec(static_cast<FbxSkeleton *>(attribute), next_parent_index, bones);
				break;
			}
		}
	}
}

#ifdef _DEBUG
void FBXImporter::print_scene(FbxScene * scene)
{
	FbxNode * root = scene->GetRootNode();
	if (root)
	{
		// Print everything
		for (int i = 0; i < root->GetChildCount(); ++i)
			print_node(root->GetChild(i), 0);
	}
}

void FBXImporter::print_node(FbxNode * node, int tab_count)
{
	// Print node name
	print_tabs(tab_count);
	const char * name = node->GetName();
	printf("<node name='%s'>\n", name);

	int tabs = tab_count + 1;
	// Print node attributes
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		print_attribute(node->GetNodeAttributeByIndex(i), tabs);

	// Print node children
	for (int i = 0; i < node->GetChildCount(); ++i)
		print_node(node->GetChild(i), tabs);

	print_tabs(tab_count);
	printf("</node>\n");
}

void FBXImporter::print_attribute(FbxNodeAttribute * attribute, int tab_count)
{
	if (!attribute)
		return;

	std::string type = attribute_type(attribute->GetAttributeType());
	print_tabs(tab_count);
	printf("<attribute type='%s' name='%s'>\n", type.c_str(), attribute->GetName());
}


std::string FBXImporter::attribute_type(FbxNodeAttribute::EType type)
{
	switch (type)
	{
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

void FBXImporter::print_tabs(int count)
{
	for (int i = 0; i < count; ++i)
		printf("\t");
}
#endif
