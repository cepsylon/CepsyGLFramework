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

	// Outputs to gui
	void to_gui() override;

	// Set camera for the rendering
	void set() const;

	glm::mat4 projection() const;
	glm::mat4 view() const;

private:
	enum ProjectionMode{ Perspective = 0, Orthographic };

	Viewport mViewport;	// Viewport
	union
	{
		struct {
			float mFieldOfViewY;
			float mRatio;
			float mNear;
			float mFar;
		};

		struct {
			float mLeft;
			float mRight;
			float mBottom;
			float mTop;
		};
	};
	int mProjectionMode = Perspective;	// Perspective or orthographic
};
