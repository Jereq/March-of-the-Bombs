#include "Graphics.h"

void Graphics::loadShaders()
{
	using std::cout;
	using std::cerr;
	using std::endl;

	if (!prog.compileShaderFromFile("Shaders/standard2d.vert", GLSLShader::VERTEX))
	{
		cerr << "standard2d vertex shader failed to compile!" << endl << prog.log() << endl;
	}
	if (!prog.compileShaderFromFile("Shaders/standard2d.frag", GLSLShader::FRAGMENT))
	{
		cerr << "standard2d fragment shader failed to compile!" << endl << prog.log() << endl;
	}

	prog.bindAttribLocation(0, "vertexPosition");
	prog.bindAttribLocation(1, "textureCoordinates");

	if (!prog.link())
	{
		cerr << "standard2d shader program failed to link!" << endl << prog.log() << endl;
	}

	cout << "Printing standard2d shader program information..." << endl;
	prog.printActiveUniforms();
	prog.printActiveAttribs();
	printf("\n");
}

Graphics::Graphics()
{
	loadShaders();
}

void Graphics::drawTexture(GLTexture::ptr texture, Rectanglef const& target)
{
	glm::vec2 pos = target.getPosition();
	glm::vec2 size = target.getSize();

	glm::vec3 positionData[] =
	{
		glm::vec3(pos.x         , pos.y         , 0),
		glm::vec3(pos.x + size.x, pos.y         , 0),
		glm::vec3(pos.x         , pos.y + size.y, 0),
		glm::vec3(pos.x + size.x, pos.y + size.y, 0)
	};

	glm::vec2 textureData[] =
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1)
	};

	GLuint bufferHandles[2];
	glGenBuffers(2, bufferHandles);

	GLuint positionBufferHandle = bufferHandles[0];
	GLuint textureBufferHandle = bufferHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, textureBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), textureData, GL_STATIC_DRAW);

	GLuint vaoHandle;
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, textureBufferHandle);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);


	glActiveTexture(GL_TEXTURE0 + texture->getHandle());
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, texture->getDesiredFormat(), texture->getWidth(), texture->getHeight(),
		0, texture->getDataFormat(), texture->getType(), texture->getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	prog.setUniform("texture", texture->getHandle());

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glDeleteTextures(1, &textureId);
	glDeleteVertexArrays(1, &vaoHandle);
	glDeleteBuffers(1, bufferHandles);
}