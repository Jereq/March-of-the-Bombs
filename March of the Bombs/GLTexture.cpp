#include "GLTexture.h"

#include "TGALoader.h"

GLint GLTexture::handleCount = 0;

GLTexture::GLTexture(GLsizei width, GLsizei height, GLint dataFormat, int numberOfChannels, GLint desiredFormat, GLfloat* data)
	: width(width), height(height), dataFormat(dataFormat),
	numberOfChannels(numberOfChannels), desiredFormat(desiredFormat),
	data(data), handle(handleCount++)
{}

GLTexture::ptr GLTexture::loadFromFileTGA(const char* fileName)
{
	std::unique_ptr<tga_data_t> rawData(tga_data_load(fileName));

	if (!rawData)
		return GLTexture::ptr();

	GLsizei width = rawData->w;
	GLsizei height = rawData->h;
	unsigned char channels = rawData->channels;

	static const GLint formats[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

	GLint dataFormat = formats[channels - 1];
	int size = width * height * channels;

	GLfloat* data = new GLfloat[size];
	for (int i = 0; i < size; i++)
	{
		data[i] = static_cast<GLfloat>(rawData->data[i]) / 255.f;
	}

	return GLTexture::ptr(new GLTexture(width, height, dataFormat, channels, dataFormat, data));
}

GLTexture::~GLTexture()
{
	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}
}

GLfloat* GLTexture::getData()
{
	return data;
}

GLsizei GLTexture::getWidth()
{
	return width;
}

GLsizei GLTexture::getHeight()
{
	return height;
}

GLint GLTexture::getDataFormat()
{
	return dataFormat;
}

GLint GLTexture::getDesiredFormat()
{
	return desiredFormat;
}

GLenum GLTexture::getType()
{
	return GL_FLOAT;
}

int GLTexture::getNumberOfChannels()
{
	return numberOfChannels;
}

GLint GLTexture::getHandle() const
{
	return handle;
}

void GLTexture::setDesiredFormat(GLint format)
{
	desiredFormat = format;
}

void GLTexture::applyBoxFilter()
{
	GLfloat kernel[3][3] =
	{
		{ 1, 2, 1 },
		{ 2, 3, 2 },
		{ 1, 2, 1 }
	};

	GLfloat* newData = new GLfloat[width * height * numberOfChannels];

	for (int v = 0; v < height; v++)
	{
		for (int u = 0; u < width; u++)
		{
			for (int ch = 0; ch < numberOfChannels; ch++)
			{
				int index = (v * width + u) * numberOfChannels + ch;

				GLfloat weight = 0;
				GLfloat value = 0;

				for (int y = -1; y <= 1; y++)
				{
					int indV = v + y;
					if (indV >= 0 && indV < height)
					{
						for (int x = -1; x <= 1; x++)
						{
							int indU = u + x;
							if (indU >= 0 && indU < height)
							{
								GLfloat kernelWeight = kernel[x + 1][y + 1];
								weight += kernelWeight;
								
								int kernelIndex = (indV * width + indU) * numberOfChannels + ch;
								value += data[kernelIndex] * kernelWeight;
							}
						}
					}
				}

				newData[index] = value / weight;
			}
		}
	}

	delete[] data;
	data = newData;

	return;
}