#include "MainMeny.h"

MainMeny::MainMeny()
{
	MainMeny::createBackground();				//creates the vector holding all the backgrounds for this screen
	MainMeny::createButtons();					//creates the vector holding all the buttons for this screen
}

MainMeny::~MainMeny()
{
}

//Mainmenys updateinfo
void MainMeny::update()
{
	/*
	for(int i = 0; i < buttons.size(); i++)
	{
		if(mouserect.intersects(buttons[i].rectanglef)
		{
			buttons[i].changeState();
		}
	}
	*/
}

//MainMenys drawinfo
void MainMeny::draw(Graphics::ptr graphics)
{
	
	//starts to render all the buttons
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i].render(graphics);
	}	

	//starts to render all the backgrounds
	for(unsigned int i = 0; i < Backgrounds.size(); i++)
	{
		Backgrounds[i].render(graphics);
	}
	
}

void MainMeny::createButtons()
{
	GLTexture::ptr TestButton1 = GLTexture::loadTexture(L"images/skull.tga");
	GLTexture::ptr LobbyButton = GLTexture::loadTexture(L"images/ButtonLobby.png");
	GLTexture::ptr HtPButton = GLTexture::loadTexture(L"images/ButtonHtP.png");
	GLTexture::ptr OptionButton = GLTexture::loadTexture(L"images/ButtonOption.png");
	GLTexture::ptr CreditsButton = GLTexture::loadTexture(L"images/ButtonCredits.png");
	GLTexture::ptr ExitButton = GLTexture::loadTexture(L"images/ButtonExit.png");

	//class	 name	unpressed	 pressed	 rectangle	lower left corner		upper right corner
	Button button0(LobbyButton, LobbyButton, Rectanglef(glm::vec2(0.25f,0.42f),glm::vec2(0.50f,0.15f)));
	Button button1(HtPButton, HtPButton, Rectanglef(glm::vec2(0.30f,0.32f),glm::vec2(0.40f,0.08f)));
	Button button2(OptionButton, OptionButton, Rectanglef(glm::vec2(0.33f,0.22f),glm::vec2(0.35f,0.08f)));
	Button button3(CreditsButton, CreditsButton, Rectanglef(glm::vec2(0.36f,0.12f),glm::vec2(0.30f,0.08f)));
	Button button4(ExitButton, ExitButton, Rectanglef(glm::vec2(0.39f,0.02f),glm::vec2(0.25f,0.06f)));


	buttons.push_back(button0);
	buttons.push_back(button1);
	buttons.push_back(button2);
	buttons.push_back(button3);
	buttons.push_back(button4);
}

void MainMeny::createBackground()
{
	GLTexture::ptr Background = GLTexture::loadTexture(L"images/MotBbackground.png");

	SimpleImage Background1(Background, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(1.00f,1.00f)));

	Backgrounds.push_back(Background1);
}