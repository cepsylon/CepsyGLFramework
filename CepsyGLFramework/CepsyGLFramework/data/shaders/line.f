#version 450

// Inputs from previous stage
in vec4 f_color;

// Outputs to buffer
out vec4 out_color;

void main()
{
	out_color = f_color;
}
