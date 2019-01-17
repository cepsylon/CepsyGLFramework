#version 450

// Inputs from previous stage
in vec3 f_normal;
in vec2 f_uv;

// Uniforms
uniform vec3 light_direction;

// Outputs to buffer
out vec4 out_color;

void main()
{
	float diffuse = dot(normalize(f_normal), light_direction);
	out_color = vec4(diffuse, diffuse, diffuse, 1.0f);
}
