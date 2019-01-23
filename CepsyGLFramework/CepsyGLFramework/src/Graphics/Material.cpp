#include "Material.h"

#include "Program.h"

RTTI_I(Material, Base);

Material::Material(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, const glm::vec3 & osr)
	: mAmbient(ambient)
	, mDiffuse(diffuse)
	, mSpecular(specular)
	, mOSR(osr)
{ }

void Material::bind(const Program * program) const
{
	program->set_uniform("ambient", mAmbient);
	program->set_uniform("diffuse", mDiffuse);
	program->set_uniform("specular", mSpecular);
}

glm::vec3 Material::ambient() const { return mAmbient; }
glm::vec3 Material::diffuse() const { return mDiffuse; }
glm::vec3 Material::specular() const { return mSpecular; }
glm::vec3 Material::osr() const { return mOSR; }

void Material::set_ambient(const glm::vec3 & ambient) { mAmbient = ambient; }
void Material::set_diffuse(const glm::vec3 & diffuse) { mDiffuse = diffuse; }
void Material::set_specular(const glm::vec3 & specular) { mSpecular = specular; }
void Material::set_osr(const glm::vec3 & osr) { mOSR = osr; }
