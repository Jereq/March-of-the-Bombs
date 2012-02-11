#pragma once

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

class GLTexture
{
private:
	static GLint handleCount;

	GLfloat* data;
	GLsizei width;
	GLsizei height;
	GLint dataFormat;
	GLint desiredFormat;
	int numberOfChannels;
	GLint handle;

	GLTexture(GLsizei width, GLsizei height, GLint dataFormat, int numberOfChannels, GLint desiredFormat, GLfloat* data);

public:
	typedef boost::shared_ptr<GLTexture> ptr;
	static ptr loadFromFileTGA(const char* fileName);

	~GLTexture();

	GLfloat* getData();
	GLsizei getWidth();
	GLsizei getHeight();
	GLint getDataFormat();
	GLint getDesiredFormat();
	GLenum getType();
	int getNumberOfChannels();
	GLint getHandle() const;

	void setDesiredFormat(GLint format);

	void applyBoxFilter();
};