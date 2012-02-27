#include "Graphics.h"

#include <boost/foreach.hpp>

#include "Game.h"

void Graphics::drawTextureInstance(TextureInstance const& texInst) const
{
	glm::vec2 pos = texInst.target.getPosition();
	glm::vec2 size = texInst.target.getSize();
	float depth = texInst.depth;

	glm::vec3 positionData[] =
	{
		glm::vec3(pos.x         , pos.y         , depth),
		glm::vec3(pos.x + size.x, pos.y         , depth),
		glm::vec3(pos.x         , pos.y + size.y, depth),
		glm::vec3(pos.x + size.x, pos.y + size.y, depth)
	};

	GLuint positionBufferHandle = textureBuffers2D[0];
	GLuint textureBufferHandle = textureBuffers2D[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(glm::vec3), positionData);

	texInst.texture->use(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Graphics::loadShaders()
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

	prog2D.setUniform("image", (GLint)0);
}

void Graphics::prepareTextureBuffers()
{
	const static glm::vec2 textureData[] =
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1)
	};

	glGenBuffers(2, textureBuffers2D);

	GLuint positionBufferHandle = textureBuffers2D[0];
	GLuint textureBufferHandle = textureBuffers2D[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, textureBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), textureData, GL_STATIC_DRAW);

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

Graphics::Graphics()
{
	AttachmentPoint::ptr attPoint = AttachmentPoint::ptr(new AttachmentPoint(glm::vec3(-20, 15, 20), glm::vec3(-30, -45, 0)));
	camera = Camera::ptr(new Camera(attPoint));

	loadShaders();
	prepareTextureBuffers();
}

Graphics::~Graphics()
{
	glDeleteBuffers(2, textureBuffers2D);
	glDeleteVertexArrays(1, &texture2DVAO);
}

void Graphics::drawTexture(GLTexture::ptr const& texture, Rectanglef const& target, float depth)
{
	textureInstances.insert(TextureInstance(texture, target, depth));
}

void Graphics::drawModel(Model::ptr const& model)
{
	models.push_back(model);
}

void Graphics::render()
{
	glCullFace(GL_FRONT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(-1.1f, -4.f);
	glPolygonOffset(-1.f, -3.f);

	BOOST_FOREACH(PointLight::ptr const& light, primaryLights)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, light->getShadowBuffer());
		glViewport(0, 0, light->getShadowResolution(), light->getShadowResolution());
		glClear(GL_DEPTH_BUFFER_BIT);

		BOOST_FOREACH(Model::ptr const& model, models)
		{
			model->drawShadow(light);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Game::ptr const& game = Game::getInstance();
	glViewport(0, 0, game->getWindowWidth(), game->getWindowHeight());

	glCullFace(GL_BACK);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->updateViewMatrix();

	BOOST_FOREACH(Model::ptr const& model, models)
	{
		model->draw(*this);
	}

	models.clear();


	glBindVertexArray(texture2DVAO);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	BOOST_FOREACH(TextureInstance const& texInst, textureInstances)
	{
		drawTextureInstance(texInst);
	}

	glDisable(GL_BLEND);

	glBindVertexArray(0);

	textureInstances.clear();


	/**
	 * Debug code that displays the first lights depth texture
	**/

	//if (primaryLights.size() > 0)
	//{
	//	glBindVertexArray(texture2DVAO);

	//	glm::vec2 pos(0, 0);
	//	glm::vec2 size(0.5f, 0.5f);
	//	float depth = 0.99f;

	//	glm::vec3 positionData[] =
	//	{
	//		glm::vec3(pos.x         , pos.y         , depth),
	//		glm::vec3(pos.x + size.x, pos.y         , depth),
	//		glm::vec3(pos.x         , pos.y + size.y, depth),
	//		glm::vec3(pos.x + size.x, pos.y + size.y, depth)
	//	};

	//	GLuint positionBufferHandle = textureBuffers2D[0];
	//	GLuint textureBufferHandle = textureBuffers2D[1];

	//	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	//	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(glm::vec3), positionData);

	//	prog2D.use();
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, primaryLights[0]->getShadowTexture());
	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//}


	return;
}

Camera::ptr Graphics::getCamera() const
{
	return camera;
}

void Graphics::setPrimaryLights(std::vector<PointLight::ptr> const& lights)
{
	primaryLights = lights;
}

std::vector<PointLight::ptr> const& Graphics::getPrimaryLights() const
{
	return primaryLights;
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