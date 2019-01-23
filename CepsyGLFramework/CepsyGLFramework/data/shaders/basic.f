#version 450

// Inputs from previous stage
in vec3 f_normal;
in vec2 f_uv;

// Uniforms
uniform vec3 light_direction;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;

// Outputs to buffer
out vec4 out_color;

void main()
{
	float d = dot(normalize(f_normal), normalize(light_direction));
	out_color = vec4(diffuse * d + specular * d, 1.0f);
}
