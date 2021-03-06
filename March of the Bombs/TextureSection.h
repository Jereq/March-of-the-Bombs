#pragma once

#include "GLTexture.h"
#include "Rectanglef.h"

class TextureSection
{
private:
	GLTexture::ptr texture;
	Rectanglef section;

public:
	typedef boost::shared_ptr<TextureSection> ptr;

	TextureSection();
	TextureSection(std::wstring const& filename);
	TextureSection(std::wstring const& filename, bool interpolate);
	TextureSection(GLTexture::ptr const& texture);
	TextureSection(GLTexture::ptr const& texture, Rectanglef const& section);

	GLTexture::ptr getTexture() const;
	Rectanglef getSection() const;

	TextureSection cut(Rectanglef const& subSection) const;

	bool operator<(TextureSection const& rhs) const
	{
		if (texture < rhs.texture) return true;
		if (rhs.texture < texture) return false;
		if (section < rhs.section) return true;
		return false;
	}
};