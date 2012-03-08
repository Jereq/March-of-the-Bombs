#include "TextureSection.h"

TextureSection::TextureSection(std::wstring const& fileName)
	: texture(GLTexture::getTexture(fileName)), section(Rectanglef(glm::vec2(0), glm::vec2(1)))
{
}

TextureSection::TextureSection(GLTexture::ptr const& texture)
	: texture(texture), section(Rectanglef(glm::vec2(0), glm::vec2(1)))
{
}

TextureSection::TextureSection(GLTexture::ptr const& texture, Rectanglef const& section)
	: texture(texture), section(section)
{
}

GLTexture::ptr TextureSection::getTexture() const
{
	return texture;
}

Rectanglef TextureSection::getSection() const
{
	return section;
}

TextureSection TextureSection::cut(Rectanglef const& subSection) const
{
	Rectanglef newSection(section.getPosition() + subSection.getPosition() * section.getSize(), subSection.getSize() * section.getSize());

	return TextureSection(texture, newSection);
}