#pragma once

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

class GLTexture
{
private:
	GLuint handle;

	GLTexture(GLuint handle);

public:
	typedef boost::shared_ptr<GLTexture> ptr;

	static ptr loadTexture(wchar_t* fileName);

	~GLTexture();

	GLuint getHandle() const;
	void use(GLint textureUnit) const;
};