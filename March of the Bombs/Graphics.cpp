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

void Graphics::drawTexture(GLTexture::ptr texture)
{
	prog.use();
}