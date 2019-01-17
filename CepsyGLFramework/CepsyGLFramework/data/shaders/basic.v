#version 450

// From input assembler
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv;

// Uniforms
layout(std140, binding = 0) uniform camera_matrices
{
	mat4 projection;
	mat4 view;
};
uniform mat4 model;

// Outputs to next stage
out vec3 f_normal;
out vec2 f_uv;

void main()
{
	gl_Position = projection * view * model * vec4(v_position, 1.0f);
	f_normal = (transpose(inverse(model)) * vec4(v_normal, 0.0f)).xyz;
	f_uv = v_uv;
}
