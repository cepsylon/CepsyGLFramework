#include "Program.h"

#include <fstream>
#include <iostream>
#include <map>

// This will be done by the importer once we have one
std::string get_name_from_path(const std::string & path)
{
	int start = path.find_last_of("/");
	start = start == std::string::npos ? 0 : start + 1;
	return path.substr(start, path.size() - start);
}

Shader::Shader(const char * path)
	: Base(get_name_from_path(path))
	, mType(get_type_from_path(path))
{
	// Open file
	std::ifstream file{ path };
	if (file.is_open())
	{
		// Read code
		std::string code{ "" };
		std::string line{ "" };
		while (std::getline(file, line))
			code += line + "\n";

		// Create, upload code and compile
		mShaderID = glCreateShader(static_cast<GLenum>(mType));
		const GLchar * p_code = (const GLchar *)code.c_str();
		glShaderSource(mShaderID, 1, &p_code, 0);
		glCompileShader(mShaderID);

		// Error checking
		GLint compiled = 0;
		glGetShaderiv(mShaderID, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE)
		{
			// Get error string and delete shader
			GLint error_length = 0;
			glGetShaderiv(mShaderID, GL_INFO_LOG_LENGTH, &error_length);
			line.resize(error_length);
			glGetShaderInfoLog(mShaderID, error_length, &error_length, &line[0]);
			glDeleteShader(mShaderID); mShaderID = 0;

			// Output error info
			printf("Error compiling shader %s:\n%s\n", path, line.c_str());
		}
	}
}

Shader::~Shader()
{
	if (mShaderID)
		glDeleteShader(mShaderID);
}

Shader::Shader(Shader && rhs)
	: Base(std::move(rhs))
	, mShaderID(rhs.mShaderID)
	, mType(rhs.mType)
{
	rhs.mShaderID = 0;
}

Shader & Shader::operator=(Shader && rhs)
{
	if (this != &rhs)
	{
		Base::operator=(std::move(rhs));

		if (mShaderID)
			glDeleteShader(mShaderID);

		mShaderID = rhs.mShaderID; rhs.mShaderID = 0;
		mType = rhs.mType;
	}

	return *this;
}

GLuint Shader::shader_id() const { return mShaderID; }

Shader::Type Shader::get_type_from_path(const std::string & path)
{
	static std::map<char, Type> keys{ 
		{ 'v', Shader::Type::VERTEX },
		{ 't', Shader::Type::TC },
		{ 'e', Shader::Type::TE },
		{ 'g', Shader::Type::GEOMETRY },
	  { 'f', Shader::Type::FRAGMENT },
		{ 'c', Shader::Type::COMPUTE }
		};

	auto it = keys.find(path.back());
	if (it == keys.end())
		throw "Trying to load non supported shader type\n";

	return it->second;
}

Program::Program(const std::vector<Shader> & shaders)
	: mProgramID(glCreateProgram())
{
	// Attach shaders
	for (const auto & shader : shaders)
		glAttachShader(mProgramID, shader.shader_id());

	// Link shaders
	glLinkProgram(mProgramID);

	// Error checking
	GLint error_code = 0;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &error_code);
	if (error_code == GL_FALSE)
	{
		// Get error string and delete the program
		GLint error_length = 0;
		glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &error_length);
		std::string error;
		error.resize(error_length);
		glGetProgramInfoLog(mProgramID, error_length, &error_length, &error[0]);
		glDeleteProgram(mProgramID); mProgramID = 0;

		// Output error
		printf("Error linking program with shaders:\n");
		for (const auto & shader : shaders)
			printf("\t%s\n", shader.name().c_str());
		printf("With error: %s\n", error.c_str());
		return;
	}

	// Detach shaders
	for (const auto & shader : shaders)
		glDetachShader(mProgramID, shader.shader_id());
}

Program::~Program()
{
	if (mProgramID)
		glDeleteProgram(mProgramID);
}

Program::Program(Program && rhs)
	: Base(std::move(rhs))
	, mProgramID(rhs.mProgramID)
{
	rhs.mProgramID = 0;
}

Program & Program::operator=(Program && rhs)
{
	if (this != &rhs)
	{
		Base::operator=(std::move(rhs));

		if (mProgramID)
			glDeleteProgram(mProgramID);

		mProgramID = rhs.mProgramID; rhs.mProgramID = 0;
	}

	return *this;
}

void Program::bind() const
{
	glUseProgram(mProgramID);
}

void Program::set_uniform(const char * name, const glm::mat4 & value) const
{
	GLint location = glGetUniformLocation(mProgramID, name);
	if (0 <= location)
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void Program::set_uniform(const char * name, const glm::vec3 & value) const
{
	GLint location = glGetUniformLocation(mProgramID, name);
	if (0 <= location)
		glUniform3f(location, value.x, value.y, value.z);
}

