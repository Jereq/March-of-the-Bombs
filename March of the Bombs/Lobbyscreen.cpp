#include "LobbyScreen.h"

#include "Game.h"
#include "MainMeny.h"
#include "GameScreen.h"
#include "CreateGameScreen.h"
#include "LoadingScreen.h"

LobbyScreen::LobbyScreen()
	: game(Game::getInstance())
{
	createBackground();
	createButtons();
	createTextFields();
	createListofGame();
}

LobbyScreen::~LobbyScreen()
{
}

void LobbyScreen::update(float deltaTime)
{
	if (client && client->isRunning())
	{
		while (client->hasReceivedPackets() && !nextScreen)
		{
			Packet::ptr packet = client->popReceivedPacket();
			packet->dispatch(&shared_from_this());
		}
	}

	std::deque<Event::ptr>& events = game->getEvents();
	std::deque<Event::ptr>::size_type numEvents = events.size();
	std::deque<Event::ptr>::size_type eventsHandled = 0;

	while (!events.empty() && eventsHandled++ < numEvents)
	{
		Event::ptr ev = events.front();
		events.pop_front();

		switch (ev->eventType)
		{
		case EventType::Keyboard:
			{
				KeyboardEvent* keyEvent = static_cast<KeyboardEvent*>(ev.get());

				KeyboardEventMethod(keyEvent);
			}
			break;

		case EventType::MouseButton:
			{
				MouseButtonEvent* mbEvent = static_cast<MouseButtonEvent*>(ev.get());				

				MousePressEventMethod(mbEvent);
				
			}
			break;
		case EventType::MouseMove:
			{
				MouseMoveEvent* mmEvent = static_cast<MouseMoveEvent*>(ev.get());

				MouseTouchEventMethod(mmEvent);
			}
			break;
		}
	}
}

void LobbyScreen::draw(Graphics::ptr graphics)
{
	//starts to render all the backgrounds
	for(unsigned int i = 0; i < Backgrounds.size(); i++)
	{
		Backgrounds[i].render(graphics);
	}

	//starts to render all the buttons
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->render(graphics);
	}

	//starts to render all the textFields
	for(unsigned int i = 0; i < textfields.size(); i++)
	{
		textfields[i]->render(graphics);
	}
		
	//starts to render all the selectionLists
	for(unsigned int i = 0; i < SelectionLists.size(); i++)
	{
		SelectionLists[i]->render(graphics);
	}
}

Screen::ptr LobbyScreen::getNextScreen()
{
	Screen::ptr screen;
	screen.swap(nextScreen);
	return screen;
}

void LobbyScreen::createButtons()
{
	TextureSection BackButton	(L"images/NewBI/BackBtn1.png");
	TextureSection JGButton		(L"images/NewBI/JGBtn1.png");
	TextureSection CGButton		(L"images/NewBI/CGBtn1.png");
	TextureSection SInButton	(L"images/NewBI/SignInBtn1.png");
	TextureSection RButton		(L"images/NewBI/RefreshBtn1.png");

	TextureSection BackButtonT	(L"images/NewBI/BackBtn2.png");
	TextureSection JGButtonT	(L"images/NewBI/JGBtn2.png");
	TextureSection CGButtonT	(L"images/NewBI/CGBtn2.png");
	TextureSection SInButtonT	(L"images/NewBI/SignInBtn2.png");
	TextureSection RButtonT		(L"images/NewBI/RefreshBtn2.png");

	backButton			= Button::ptr(new Button(BackButton,	BackButtonT,	Rectanglef(glm::vec2(0.40f,0.02f),glm::vec2(0.20f,0.10f)), 0.0f));
	joinGameButton		= Button::ptr(new Button(JGButton,		JGButtonT,		Rectanglef(glm::vec2(0.70f,0.75f),glm::vec2(0.20f,0.10f)), 0.0f));
	createGameButton	= Button::ptr(new Button(CGButton,		CGButtonT,		Rectanglef(glm::vec2(0.40f,0.75f),glm::vec2(0.20f,0.10f)), 0.0f));
	signInButton		= Button::ptr(new Button(SInButton,		SInButtonT,		Rectanglef(glm::vec2(0.10f,0.75f),glm::vec2(0.20f,0.10f)), 0.0f));
	refreshButton		= Button::ptr(new Button(RButton,		RButtonT,		Rectanglef(glm::vec2(0.70f,0.02f),glm::vec2(0.20f,0.10f)), 0.0f));
	
	buttons.push_back(backButton);
	buttons.push_back(refreshButton);
	buttons.push_back(joinGameButton);
	buttons.push_back(createGameButton);
	buttons.push_back(signInButton);
}

void LobbyScreen::createBackground()
{
	TextureSection Background(L"images/NewBI/Start.png");

	SimpleImage Background1(Background, Rectanglef(glm::vec2(0.00f,0.00f),glm::vec2(1.00f,1.00f)), 0.99f);

	Backgrounds.push_back(Background1);
}

void LobbyScreen::createTextFields()
{
	TextureSection TFBackground(L"Images/TFBackground.png");
	TextureSection DTFBackground(L"Images/DTFBackground.png");

	SignInTF	= TextField::ptr(new TextField(TFBackground,DTFBackground,Rectanglef(glm::vec2(0.05f,0.90f),glm::vec2(0.40f,0.05f)),0.05f));
	IPTF		= TextField::ptr(new TextField(TFBackground,DTFBackground,Rectanglef(glm::vec2(0.55f,0.90f),glm::vec2(0.40f,0.05f)),0.05f));

	textfields.push_back(SignInTF);
	textfields.push_back(IPTF);
}

void LobbyScreen::createListofGame()
{
	TextureSection LoGBackground(L"Images/TFBackground.png");

	selectionList = SelectionList::ptr(new SelectionList(LoGBackground,Rectanglef(glm::vec2(0.05f,0.17f),glm::vec2(0.90f,0.50f)),0.05f));

	SelectionLists.push_back(selectionList);
}

void LobbyScreen::KeyboardEventMethod(KeyboardEvent* keyEvent)
{
	const static char ESC = 0x1B;

	if (keyEvent->keyEventType == KeyboardEventType::Pressed)
	{
		switch (keyEvent->key)
		{
		case ESC:
			game->close();
			break;

		default:
			{
				SignInTF->updateString(keyEvent);
				IPTF->updateString(keyEvent);
			}
			break;
		}
	}
}

void LobbyScreen::MousePressEventMethod(MouseButtonEvent* mbEvent)
{
	if (mbEvent->button == MouseButton::Left && mbEvent->state == MouseButtonState::Pressed)
	{
		if (backButton->getState() == Hovered)
		{
			nextScreen = Screen::ptr(new MainMeny());
			game->getEvents().clear();
		}
		else if (signInButton->getState() == Hovered && signInButton->disabled == false)
		{
			client.reset();
			client.reset(new GameClient(IPTF->getString(), "1694"));


			playerName = SignInTF->getString();
			Packet::ptr packet = Packet::ptr(new Packet3Login(playerName));
			client->write(packet);
			signInButton->disabled = true;
			signInButton->setState(Unused);
			
		}
		else if (createGameButton->getState() == Hovered)
		{
			if (client)
			{
				nextScreen.reset(new CreateGameScreen(shared_from_this(), client, playerName, playerID));
				game->getEvents().clear();
			}
		}
		else if (joinGameButton->getState() == Hovered)
		{
			if (client && !openGames.empty())
			{
				int tempindex = selectionList->getindex();
				if(tempindex >= 0 && (unsigned int)tempindex < openGames.size())
				{
					Packet::ptr packet = Packet::ptr(new Packet7JoinGame(openGames[tempindex].getGameID()));
					client->write(packet);
				}


			}
		}
		else if (refreshButton->getState() == Hovered)
		{
			Packet::ptr packet11(new Packet11RequestOpenGames());
			client->write(packet11);
			openGames.clear();
		}

		for(unsigned int i = 0; i < textfields.size(); i++)
		{
			textfields[i]->active = (textfields[i]->getState() == Targeted);
		}

		selectionList->updateselection(mbEvent);
	}
}

void LobbyScreen::MouseTouchEventMethod(MouseMoveEvent* mmEvent)
{
	BOOST_FOREACH(Button::ptr& button, buttons)
	{
		if(button->disabled == false)
		{
			if(button->intersects(mmEvent->position))
			{
				button->setState(Hovered);
			}
			else
			{
				button->setState(Unused);
			}
		}
	}

	BOOST_FOREACH(TextField::ptr& textfield, textfields)
	{
		if(textfield->intersects(mmEvent->position))
		{
			textfield->setState(Targeted);
		}
		else
		{
			textfield->setState(NotTargeted);
		}
	}
}

void LobbyScreen::converttoStringVector()
{
	//plats för kod till att fylla strängvectorn
	selectionList->GameList.clear();
	for (unsigned int i = 0; i < openGames.size(); i ++)
	{
		selectionList->GameList.push_back(openGames[i].getMapName() + " " + openGames[i].getPlayerName());
	}
}

void LobbyScreen::handlePacket4LoginAccepted(Packet4LoginAccepted::const_ptr const& packet)
{
	Packet4LoginAccepted const* packet4 = static_cast<Packet4LoginAccepted const*>(packet.get());

	playerID = packet4->getPlayerID();

	Packet::ptr packet11(new Packet11RequestOpenGames());
	client->write(packet11);
}

void LobbyScreen::handlePacket8SetupGame(Packet8SetupGame::const_ptr const& packet)
{
	Packet8SetupGame const* packet8 = static_cast<Packet8SetupGame const*>(packet.get());

	glm::vec3 myColor = glm::vec3(1.f, 1.f, 0.f) - packet8->getOpponentColor();
	
	nextScreen.reset(new LoadingScreen(client, playerName, playerID, myColor,
		"Hasse", packet8->getOpponentID(), packet8->getOpponentColor(),
		packet8->getMapName(), packet8->getBaseID()));
	game->getEvents().clear();
}

void LobbyScreen::handlePacket10PlayerReady(Packet10PlayerReady::const_ptr const& packet)
{
	nextScreen = newGame;
	game->getEvents().clear();
}

void LobbyScreen::handlePacket12OpenGames(Packet12OpenGames::const_ptr const& packet)
{
	Packet12OpenGames const* packet12 = static_cast<Packet12OpenGames const*>(packet.get());

	openGames = packet12->getOpenGames();
	converttoStringVector();
}