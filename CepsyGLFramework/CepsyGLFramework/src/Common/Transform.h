#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Transform
{
public:
	Transform() = default;
	Transform(const Transform & rhs);

	// Output to gui
	void to_gui();

	// Gettors
	glm::quat rotation() const;
	glm::vec3 position() const;
	glm::vec3 scale() const;
	glm::vec3 euler_angles() const;


	// Settors
	void set_rotation(const glm::vec3 & rotation);
	void set_rotation(const glm::quat & quaternion);
	void set_position(const glm::vec3 & position);
	void set_scale(const glm::vec3 & scale);

	// Computes model matrix
	glm::mat4 model() const;

private:
	glm::quat mRotation;
	glm::vec3 mTranslation;
	glm::vec3 mScale;
};
