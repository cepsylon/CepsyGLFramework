#pragma once

#include "Common/Base.h"

#include <glm/vec3.hpp>

class Program;

class Material : public Base
{
public:
	RTTI_H;

	Material() = default;
	Material(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, const glm::vec3 & osr);

	// Bind material
	void bind(const Program * program) const;

	// Gettors
	glm::vec3 ambient() const;
	glm::vec3 diffuse() const;
	glm::vec3 specular() const;
	glm::vec3 osr() const;

	// Settors
	void set_ambient(const glm::vec3 & ambient);
	void set_diffuse(const glm::vec3 & diffuse);
	void set_specular(const glm::vec3 & specular);
	void set_osr(const glm::vec3 & osr);

private:
	glm::vec3 mAmbient{ 0.0f };
	glm::vec3 mDiffuse{ 0.0f };
	glm::vec3 mSpecular{ 0.0f };
	glm::vec3 mOSR{ 0.0f };	// Opacity, shininess, reflectivity
};
