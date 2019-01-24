#version 450

// Inputs from previous stage
in vec2 f_uv;

// Uniforms
layout(binding = 0) uniform sampler2D image;

// Outputs to buffer
out vec4 out_color;

void main()
{
	out_color = texture(image, f_uv);
}
