#include "GLTexture.h"

#ifndef IL_USE_PRAGMA_LIBS
	#define IL_USE_PRAGMA_LIBS
#endif
#include <IL/ilut.h>

#include <iostream>

std::map<std::wstring, GLTexture::w_ptr> GLTexture::textureMap;

GLTexture::GLTexture(GLuint handle)
	: handle(handle)
{
}

GLTexture::ptr GLTexture::loadTexture(std::wstring const& fileName, bool interpolate)
{
	ILuint ilName;
	ilGenImages(1, &ilName);
	ilBindImage(ilName);

	ILboolean result = ilLoadImage(fileName.c_str());
	if (result)
	{
		std::wcout << "Loaded texture: " << fileName << std::endl;
	}
	else
	{
		std::wcerr << "Could not load file: " << fileName << std::endl;
	}
	
	GLuint handle = ilutGLBindTexImage();
	result = ilutGLTexImage(0);

	if (!interpolate)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	ilDeleteImages(1, &ilName);

	return GLTexture::ptr(new GLTexture(handle));
}

GLTexture::~GLTexture()
{
	if (handle)
	{
		glDeleteTextures(1, &handle);
	}
}

GLTexture::ptr GLTexture::getTexture(std::wstring const& fileName, bool interpolate)
{
	GLTexture::ptr& tex = textureMap[fileName].lock();

	if (!tex)
	{
		tex = loadTexture(fileName, interpolate);
		textureMap[fileName] = tex;
	}

	return tex;
}

GLuint GLTexture::getHandle() const
{
	return handle;
}

void GLTexture::use(GLint textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, handle);
}