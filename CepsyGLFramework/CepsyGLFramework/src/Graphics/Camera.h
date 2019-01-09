#pragma once

#include "Common/Component.h"
#include "Viewport.h"

#include <glm/mat4x4.hpp>

class Camera : public Component
{
public:
	Camera();
	~Camera();

	// Initialize
	void initialize() override;

	// Update
	void update() override;

	// Shutdown
	void shutdown() override;

	// Set camera for the rendering
	void set() const;

	glm::mat4 projection() const;
	glm::mat4 view() const;

private:
	Viewport mViewport;	// Viewport
	union
	{
		struct {
			float mFieldOfViewY;
			float mRatio;
			float mFar;
			float mNear;
		};

		struct {
			float mLeft;
			float mRight;
			float mBottom;
			float mTop;
		};
	};
	bool mIsPerspective = true;	// Perspective or orthographic
};
