#pragma once

#include "Common/Base.h"
#include "myGlew.h"

#include <string>
#include <vector>

// For uniforms
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Shader : public Base
{
public:
	enum class Type{ 
		VERTEX = GL_VERTEX_SHADER, 
		TC = GL_TESS_CONTROL_SHADER, 
		TE = GL_TESS_EVALUATION_SHADER, 
		GEOMETRY = GL_GEOMETRY_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		COMPUTE = GL_COMPUTE_SHADER };

	Shader() = default;
	Shader(const char * path);
	~Shader();

	// No copy
	Shader(const Shader &) = delete;
	Shader & operator=(const Shader &) = delete;

	// Only move
	Shader(Shader && rhs);
	Shader & operator=(Shader && rhs);

	GLuint shader_id() const;

	static Type get_type_from_path(const std::string & path);
private:
	GLuint mShaderID = 0;
	Type mType;
};

class Program : public Base
{
public:
	// Shaders need to be in order, vertex->tc->te->geometry->fragment
	Program(const std::vector<Shader> & shaders);
	~Program();

	// No copy
	Program(const Program &) = delete;
	Program & operator=(const Program &) = delete;

	// Only move
	Program(Program && rhs);
	Program & operator=(Program && rhs);

	// Bind program for rendering
	void bind() const;

	// Set uniforms
	void set_uniform(const char * name, const glm::mat4 & value) const;
	void set_uniform(const char * name, const glm::vec3 & value) const;

private:
	GLuint mProgramID = 0;
};
