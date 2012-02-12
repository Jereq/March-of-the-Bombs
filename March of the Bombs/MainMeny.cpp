#include "MainMeny.h"

MainMeny::MainMeny()
{
	GLTexture::ptr TestTexture1 = GLTexture::loadFromFileTGA("images/skull.tga");
	Button tempbutton(TestTexture1, TestTexture1,Rectangle());
	buttons.push_back(tempbutton);
}

MainMeny::~MainMeny()
{
}

//Mainmenys updateinfo
void MainMeny::update()
{

}

//MainMenys drawinfo
void MainMeny::draw(Graphics::ptr graphics)
{
	for(int i = 0; i < buttons.size(); i++)
	{
		buttons[i].render(graphics);
	}
}