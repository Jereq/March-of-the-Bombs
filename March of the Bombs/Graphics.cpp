#include "Graphics.h"

#include <iostream>
#include <boost/foreach.hpp>

#include "Game.h"
#include "BlockModelData.h"
#include "PlaneModelData.h"
#include "StandardBombModelData.h"

void Graphics::drawTextureInstance(TextureInstance const& texInst) const
{
	glm::vec2 pos = texInst.target.getPosition();
	glm::vec2 size = texInst.target.getSize();
	float depth = texInst.depth;

	Rectanglef source = texInst.texture.getSection();
	glm::vec2 sourcePos = source.getPosition();
	glm::vec2 sourceSize = source.getSize();

	glm::vec3 positionData[] =
	{
		glm::vec3(pos.x         , pos.y         , depth),
		glm::vec3(pos.x + size.x, pos.y         , depth),
		glm::vec3(pos.x         , pos.y + size.y, depth),
		glm::vec3(pos.x + size.x, pos.y + size.y, depth)
	};

	glm::vec2 textureData[] =
	{
		glm::vec2(sourcePos.x               , sourcePos.y               ),
		glm::vec2(sourcePos.x + sourceSize.x, sourcePos.y               ),
		glm::vec2(sourcePos.x               , sourcePos.y + sourceSize.y),
		glm::vec2(sourcePos.x + sourceSize.x, sourcePos.y + sourceSize.y)
	};

	GLuint positionBufferHandle = textureBuffers2D[0];
	GLuint textureBufferHandle = textureBuffers2D[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positionData), positionData);

	glBindBuffer(GL_ARRAY_BUFFER, textureBufferHandle);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textureData), textureData);

	texInst.texture.getTexture()->use(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Graphics::loadShaders()
{
	load2DShaders();
	loadModelShadeShaders();
	loadModelShadowShaders();
}

void Graphics::load2DShaders()
{
	using std::cout;
	using std::cerr;
	using std::endl;

	if (!prog2D.compileShaderFromFile("Shaders/standard2d.vert", GLSLShader::VERTEX))
	{
		cerr << "standard2d vertex shader failed to compile!" << endl << prog2D.log() << endl;
	}
	if (!prog2D.compileShaderFromFile("Shaders/standard2d.frag", GLSLShader::FRAGMENT))
	{
		cerr << "standard2d fragment shader failed to compile!" << endl << prog2D.log() << endl;
	}

	prog2D.bindAttribLocation(0, "vertexPosition");
	prog2D.bindAttribLocation(1, "textureCoordinates");

	if (!prog2D.link())
	{
		cerr << "standard2d shader program failed to link!" << endl << prog2D.log() << endl;
	}

	cout << "Printing standard2d shader program information..." << endl;
	prog2D.printActiveUniforms();
	prog2D.printActiveAttribs();
	printf("\n");

	prog2D.use();
	prog2D.setUniform("image", (GLint)0);
}

void Graphics::loadModelShadeShaders()
{
	if (!progModelShade.compileShaderFromFile("Shaders/model.vert", GLSLShader::VERTEX))
	{
		printf("Model vertex shader failed to compile!\n%s\n", progModelShade.log().c_str());
		exit(EXIT_FAILURE);
	}
	if (!progModelShade.compileShaderFromFile("Shaders/model.frag", GLSLShader::FRAGMENT))
	{
		printf("Model fragment shader failed to compile!\n%s\n", progModelShade.log().c_str());
		exit(EXIT_FAILURE);
	}

	progModelShade.bindAttribLocation(0, "vertexPosition");
	progModelShade.bindAttribLocation(1, "vertexTextureCoordinate");
	progModelShade.bindAttribLocation(2, "vertexNormal");

	if (!progModelShade.link())
	{
		printf("Model shader program failed to link!\n%s\n", progModelShade.log().c_str());
		exit(EXIT_FAILURE);
	}

	printf("Printing model shader program information...\n");
	progModelShade.printActiveUniforms();
	progModelShade.printActiveAttribs();
	printf("\n");

	progModelShade.use();
	progModelShade.setUniform("shadowMap", (GLint)0);
	progModelShade.setUniform("diffuseMap", (GLint)5);
}

void Graphics::loadModelShadowShaders()
{
	if (progModelShadow.isLinked())
	{
		return;
	}

	if (!progModelShadow.compileShaderFromFile("Shaders/modelShadow.vert", GLSLShader::VERTEX))
	{
		printf("Model shadow vertex shader failed to compile!\n%s\n", progModelShadow.log().c_str());
		exit(EXIT_FAILURE);
	}
	if (!progModelShadow.compileShaderFromFile("Shaders/modelShadow.frag", GLSLShader::FRAGMENT))
	{
		printf("Model shadow fragment shader failed to compile!\n%s\n", progModelShadow.log().c_str());
		exit(EXIT_FAILURE);
	}

	progModelShadow.bindAttribLocation(0, "vertexPosition");

	if (!progModelShadow.link())
	{
		printf("Model shadow shader program failed to link!\n%s\n", progModelShadow.log().c_str());
		exit(EXIT_FAILURE);
	}

	printf("Printing model shadow shader program information...\n");
	progModelShadow.printActiveUniforms();
	progModelShadow.printActiveAttribs();
	printf("\n");
}

void Graphics::prepareTextureBuffers()
{
	glGenBuffers(2, textureBuffers2D);

	GLuint positionBufferHandle = textureBuffers2D[0];
	GLuint textureBufferHandle = textureBuffers2D[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, textureBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), NULL, GL_STATIC_DRAW);

	glGenVertexArrays(1, &texture2DVAO);
	glBindVertexArray(texture2DVAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, textureBufferHandle);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Graphics::setupModelShader()
{
	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 projectionMatrix = camera->getProjectionMatrix();

	progModelShade.use();
	progModelShade.setUniform("viewMatrix", viewMatrix);
	progModelShade.setUniform("projectionMatrix", projectionMatrix);

	if(light)
	{
		progModelShade.setUniform("light.position", viewMatrix * light->getPosition());
		progModelShade.setUniform("light.intensity", light->getIntensity());
		progModelShade.setUniform("shadowMatrix", light->getViewProjectionMatrix());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, light->getShadowTexture());
	}
}

void Graphics::setupModelShadowShader(PointLight::ptr const& light)
{
	glm::mat4 viewProjectionMatrix = light->getProjectionMatrix() * light->getViewMatrix();

	progModelShadow.use();
	progModelShadow.setUniform("viewProjectionMatrix", viewProjectionMatrix);
}

Graphics::Graphics()
{
	AttachmentPoint::ptr attPoint = AttachmentPoint::ptr(new AttachmentPoint(glm::vec3(-20, 15, 20), glm::vec3(-30, -45, 0)));
	camera = Camera::ptr(new Camera(attPoint));

	loadShaders();
	prepareTextureBuffers();

	modelDatas.insert(BlockModelData::getHardInstance());
	modelDatas.insert(BlockModelData::getSoftInstance());
	modelDatas.insert(PlaneModelData::getInstance());
	modelDatas.insert(StandardBombModelData::getInstance());
}

Graphics::~Graphics()
{
	glDeleteBuffers(2, textureBuffers2D);
	glDeleteVertexArrays(1, &texture2DVAO);
}

void Graphics::drawTexture(TextureSection const& texture, Rectanglef const& target, float depth)
{
	textureInstances.insert(TextureInstance(texture, target, depth));
}

void Graphics::drawModel(Model::ptr const& model)
{
	model->addInstance();
}

void Graphics::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (backgroundTexture)
	{
		glBindVertexArray(texture2DVAO);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		prog2D.use();

		drawTextureInstance(TextureInstance(*backgroundTexture, Rectanglef(glm::vec2(0.f), glm::vec2(1.f)), 1));

		glDisable(GL_BLEND);
		glBindVertexArray(0);
	}

	if(light)
	{
		glCullFace(GL_FRONT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		//glPolygonOffset(-1.1f, -4.f);

		glBindFramebuffer(GL_FRAMEBUFFER, light->getShadowBuffer());
		glViewport(0, 0, light->getShadowResolution(), light->getShadowResolution());
		glClear(GL_DEPTH_BUFFER_BIT);

		setupModelShadowShader(light);
		BOOST_FOREACH(ModelData::ptr const& data, modelDatas)
		{
			data->drawInstancesShadow(progModelShadow);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Game::ptr const& game = Game::getInstance();
		glViewport(0, 0, game->getWindowWidth(), game->getWindowHeight());

		glCullFace(GL_BACK);
		glDisable(GL_POLYGON_OFFSET_FILL);

		camera->updateViewMatrix();

		setupModelShader();
		BOOST_FOREACH(ModelData::ptr const& data, modelDatas)
		{
			data->drawInstances(progModelShade);
			data->clearInstancesToDraw();
		}
	}


	glClear(GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(texture2DVAO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	prog2D.use();
	BOOST_FOREACH(TextureInstance const& texInst, textureInstances)
	{
		drawTextureInstance(texInst);
	}

	glDisable(GL_BLEND);

	glBindVertexArray(0);

	textureInstances.clear();

	return;
}

Camera::ptr Graphics::getCamera() const
{
	return camera;
}

PointLight::ptr const& Graphics::getLight() const
{
	return light;
}

void Graphics::setLight(PointLight::ptr const& light)
{
	this->light = light;
}

TextureSection::ptr const& Graphics::getBackground() const
{
	return backgroundTexture;
}

void Graphics::setBackground(TextureSection::ptr const& background)
{
	backgroundTexture = background;
}

void Graphics::updateViewport()
{
	Game::ptr const& game = Game::getInstance();

	int width = game->getWindowWidth();
	int height = game->getWindowHeight();

	glViewport(0, 0, width, height);

	float aspect = static_cast<float>(width) / static_cast<float>(height);
	camera->updateProjectionMatrix(aspect);
}