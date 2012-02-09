#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>

#include <iostream>

#include "Game.h"

void Packet1SimpleMessage::dispatch(void* context) const
{
	Game* game = static_cast<Game*>(context);
	game->deliver(shared_from_this());

	std::cout << "<Client> " << getMessage() << std::endl;
}

void Packet2Blob::dispatch(void* context) const
{
	Game* game = static_cast<Game*>(context);
	game->deliver(shared_from_this());

	std::cout << "Received blob from client: " << getBlobLength() << " bytes" << std::endl;
}

void Packet3Login::dispatch(void* context) const
{
	Game* game = static_cast<Game*>(context);
	game->deliver(shared_from_this());

	std::cout << getName() << " logged in" << std::endl;
}