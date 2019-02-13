#version 450

// From input assembler
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv;
layout(location = 3) in vec4 v_indices;
layout(location = 4) in vec4 v_weights;

// SSBO
layout(std430, binding = 1) buffer bone_matrices
{
	mat4 matrices[];
};

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
	ivec4 indices = ivec4(v_indices);
	mat4 skinning_matrix = 
		matrices[indices.x] * v_weights.x +
		matrices[indices.y] * v_weights.y +
		matrices[indices.z] * v_weights.z +
		matrices[indices.w] * v_weights.w;
	
	gl_Position = projection * view * model * skinning_matrix * vec4(v_position, 1.0f);
	f_normal = (transpose(inverse(model)) * vec4(v_normal, 0.0f)).xyz;
	f_uv = v_uv;
}
