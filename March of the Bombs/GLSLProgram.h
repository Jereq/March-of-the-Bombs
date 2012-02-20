#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
using std::string;

namespace GLSLShader
{
	enum GLSLShaderType
	{
		VERTEX, FRAGMENT, GEOMETRY, TESS_CONTROL,
		TESS_EVALUATION
	};
};

class GLSLProgram
{
private:
	int handle;
	bool linked;
	string logString;
	
	int getUniformLocation(const char* name) const;
	bool fileExists(const string& fileName) const;

public:
	GLSLProgram();

	bool compileShaderFromFile(const char* fileName,
		GLSLShader::GLSLShaderType type);
	bool compileShaderFromString( const string& source,
		GLSLShader::GLSLShaderType type);
	bool link();
	void use() const;

	string log();

	int getHandle();
	bool isLinked();

	void bindAttribLocation(GLuint location,
		const char* name);
	void bindFragDataLocation(GLuint location,
		const char* name);

	void setUniform(const char* name, float x, float y, float z) const;
	void setUniform(const char* name, const glm::vec3& v) const;
	void setUniform(const char* name, const glm::vec4& v) const;
	void setUniform(const char* name, const glm::mat3& m) const;
	void setUniform(const char* name, const glm::mat4& m) const;
	void setUniform(const char* name, float val) const;
	void setUniform(const char* name, GLint val) const;
	void setUniform(const char* name, GLuint val) const;
	void setUniform(const char* name, bool val) const;

	void printActiveUniforms() const;
	void printActiveAttribs() const;
};