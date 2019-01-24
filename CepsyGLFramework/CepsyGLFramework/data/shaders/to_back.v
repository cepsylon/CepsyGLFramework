#version 450

// From input assembler
layout(location = 0) in vec2 v_position;
layout(location = 1) in vec2 v_uv;

// Outputs to next stage
out vec2 f_uv;

void main()
{
	gl_Position = vec4(v_position, 0.0f, 1.0f);
	f_uv = v_uv;
}
