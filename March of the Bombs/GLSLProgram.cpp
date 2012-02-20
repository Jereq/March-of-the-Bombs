#include "GLSLProgram.h"

#include <fstream>
#include <sstream>

int GLSLProgram::getUniformLocation(const char* name) const
{
	return glGetUniformLocation(handle, name);
}

bool GLSLProgram::fileExists(const string& fileName) const
{
	std::ifstream ifile(fileName);
	return ifile.is_open();
}

GLSLProgram::GLSLProgram()
{
	handle = 0;
	linked = false;
}

bool GLSLProgram::compileShaderFromFile(const char* fileName, GLSLShader::GLSLShaderType type)
{
	if (!fileExists(fileName))
	{
		logString = "File not found.";
		return false;
	}

	if (handle <= 0)
	{
		handle = glCreateProgram();
		if (handle == 0)
		{
			logString = "Unable to create shader program.";
			return false;
		}
	}

	std::ifstream inFile(fileName);
	if (!inFile)
		return false;

	std::ostringstream code;
	while (inFile.good())
	{
		int c = inFile.get();
		if (!inFile.eof())
			code << static_cast<char>(c);
	}
	inFile.close();

	return compileShaderFromString(code.str(), type);
}

bool GLSLProgram::compileShaderFromString(const string& source,	GLSLShader::GLSLShaderType type)
{
	if (handle <= 0)
	{
		handle = glCreateProgram();
		if (handle == 0)
		{
			logString = "Unable to create shader program.";
			return false;
		}
	}

	GLuint shaderHandle = 0;

	switch (type)
	{
	case GLSLShader::VERTEX:
		shaderHandle = glCreateShader(GL_VERTEX_SHADER);
		break;

	case GLSLShader::FRAGMENT:
		shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		break;

	case GLSLShader::GEOMETRY:
		shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
		break;

	case GLSLShader::TESS_CONTROL:
		shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
		break;

	case GLSLShader::TESS_EVALUATION:
		shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
		break;

	default:
		return false;
	}

	const char* c_code = source.c_str();
	glShaderSource(shaderHandle, 1, &c_code, NULL);

	glCompileShader(shaderHandle);

	int result;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		int length = 0;
		logString = "";
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
		if (length > 0)
		{
			char* c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}

		return false;
	}
	else
	{
		glAttachShader(handle, shaderHandle);
		return true;
	}
}

bool GLSLProgram::link()
{
	if (linked)
		return true;
	if (handle <= 0)
		return false;

	glLinkProgram(handle);

	int status = 0;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		int length = 0;
		logString = "";

		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);

		if (length > 0)
		{
			char* c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(handle, length, &written, c_log);
			logString = c_log;
			delete[] c_log;
		}

		return false;
	}
	else
	{
		linked = true;
		return linked;
	}
}

void GLSLProgram::use() const
{
	if (handle <= 0 || (!linked))
		return;

	glUseProgram(handle);
}

string GLSLProgram::log()
{
	return logString;
}

int GLSLProgram::getHandle()
{
	return handle;
}

bool GLSLProgram::isLinked()
{
	return linked;
}

void GLSLProgram::bindAttribLocation(GLuint location, const char* name)
{
	glBindAttribLocation(handle, location, name);
}

void GLSLProgram::bindFragDataLocation(GLuint location, const char* name)
{
	glBindFragDataLocation(handle, location, name);
}

void GLSLProgram::setUniform(const char* name, float x, float y, float z) const
{
	use();
	int loc = getUniformLocation(name);
	if (loc >= 0)
	{
		glUniform3f(loc, x, y, z);
	}
}

void GLSLProgram::setUniform(const char* name, const glm::vec3& v) const
{
	this->setUniform(name, v.x, v.y, v.z);
}

void GLSLProgram::setUniform(const char* name, const glm::vec4& v) const
{
	use();
	int loc = getUniformLocation(name);
	if (loc >= 0)
	{
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	}
}

void GLSLProgram::setUniform(const char* name, const glm::mat3& m) const
{
	use();
	int loc = getUniformLocation(name);
	if (loc >= 0)
	{
		glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
	}
}

void GLSLProgram::setUniform(const char* name, const glm::mat4& m) const
{
	use();
	int loc = getUniformLocation(name);
	if (loc >= 0)
	{
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
	}
}

void GLSLProgram::setUniform(const char* name, float val) const
{
	use();
	int loc = getUniformLocation(name);
	if (loc >= 0)
	{
		glUniform1f(loc, val);
	}
}

void GLSLProgram::setUniform(const char* name, GLint val) const
{
	use();
	int loc = getUniformLocation(name);
	if (loc >= 0)
	{
		glUniform1i(loc, val);
	}
}

void GLSLProgram::setUniform(const char* name, GLuint val) const
{
	use();
	int loc = getUniformLocation(name);
	if (loc >= 0)
	{
		glUniform1ui(loc, val);
	}
}

void GLSLProgram::setUniform(const char* name, bool val) const
{
	use();
	int loc = getUniformLocation(name);
	if (loc >= 0)
	{
		glUniform1i(loc, val);
	}
}

void GLSLProgram::printActiveUniforms() const
{
	GLint nUniforms, written, size, location, maxLen;
	GLchar* name;
	GLenum type;

	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &nUniforms);

	name = new GLchar[maxLen];

	printf(" Location | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nUniforms; i++)
	{
		glGetActiveUniform(handle, i, maxLen, &written, &size, &type, name);
		location = glGetUniformLocation(handle, name);
		printf(" %-8d | %s\n", location, name);
	}

	delete[] name;
}

void GLSLProgram::printActiveAttribs() const
{
	GLint nAttribs, written, size, location, maxLen;
	GLchar* name;
	GLenum type;

	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLen);
	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

	name = new GLchar[maxLen];

	printf(" Index | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nAttribs; i++)
	{
		glGetActiveAttrib(handle, i, maxLen, &written, &size, &type, name);
		location = glGetAttribLocation(handle, name);
		printf(" %-5d | %s\n", location, name);
	}

	delete[] name;
}