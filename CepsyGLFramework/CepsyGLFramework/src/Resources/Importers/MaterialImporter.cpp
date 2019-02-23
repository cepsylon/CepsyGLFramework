#include "MaterialImporter.h"

#include "Graphics/Material.h"

Material MaterialImporter::load(FbxSurfaceMaterial * material)
{
	// Create and name material
	Material new_material;
	new_material.set_name(material->GetName());

	// Get all attributes depending on the type of the material
	if (material->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		FbxSurfacePhong * phong = static_cast<FbxSurfacePhong *>(material);
		new_material.set_ambient(glm::vec3{ phong->Ambient.Get()[0], phong->Ambient.Get()[1] , phong->Ambient.Get()[2] });
		new_material.set_diffuse(glm::vec3{ phong->Diffuse.Get()[0], phong->Diffuse.Get()[1] , phong->Diffuse.Get()[2] });
		new_material.set_specular(glm::vec3{phong->Specular.Get()[0], phong->Specular.Get()[1] , phong->Specular.Get()[2] });
		new_material.set_osr(glm::vec3{ phong->TransparencyFactor, phong->Shininess, phong->ReflectionFactor });

		// Need to figure out what this is for
		double factor = phong->AmbientFactor;
		factor = phong->DiffuseFactor;
		factor = phong->SpecularFactor;

		// Textures are stored here
		int texture_count = phong->Ambient.GetSrcObjectCount();
		texture_count = phong->Diffuse.GetSrcObjectCount();
		texture_count = phong->Diffuse.GetSrcPropertyCount();
	}
	else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// TODO
	}

	return new_material;
}
