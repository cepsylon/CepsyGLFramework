#version 450

// From input assembler
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec4 v_color;

// Uniforms
layout(std140, binding = 0) uniform camera_matrices
{
	mat4 projection;
	mat4 view;
};

// Outputs to next stage
out vec4 f_color;

void main()
{
	gl_Position = projection * view * vec4(v_position, 1.0f);
	f_color = v_color;
}
