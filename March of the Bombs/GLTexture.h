#pragma once

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>
#include <string>

class GLTexture
{
public:
	typedef boost::shared_ptr<GLTexture> ptr;
	typedef boost::weak_ptr<GLTexture> w_ptr;
	
private:
	static std::map<std::wstring, GLTexture::w_ptr> textureMap;

	GLuint handle;

	GLTexture(GLuint handle);

	static ptr loadTexture(std::wstring const& fileName, bool interpolate);

public:
	~GLTexture();

	static ptr getTexture(std::wstring const& fileName, bool interpolate);

	GLuint getHandle() const;
	void use(GLint textureUnit) const;
};