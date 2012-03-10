#include "OpenGame.h"

#include "Pack.h"

OpenGame::OpenGame()
{
}

OpenGame::OpenGame(unsigned short gameID, std::string const& mapName, std::string const& playerName)
	: gameID(gameID), mapName(mapName), playerName(playerName)
{
}

unsigned short OpenGame::getGameID() const
{
	return gameID;
}

std::string const& OpenGame::getMapName() const
{
	return mapName;
}

std::string const& OpenGame::getPlayerName() const
{
	return playerName;
}

size_t OpenGame::packedSize() const
{
	return sizeof(uint16_t) +
		util::packedSize(mapName) +
		util::packedSize(playerName);
}

size_t OpenGame::pack(char destination[]) const
{
	char* currDest = destination;

	currDest += util::pack(&gameID, 1, currDest);
	currDest += util::pack(&mapName, 1, currDest);
	currDest += util::pack(&playerName, 1, currDest);

	return currDest - destination;
}

size_t OpenGame::unpack(char const source[])
{
	char const* currSource = source;

	currSource += util::unpack(&gameID, 1, currSource);
	currSource += util::unpack(&mapName, 1, currSource);
	currSource += util::unpack(&playerName, 1, currSource);

	return currSource - source;
}

size_t pack(OpenGame const values[], size_t count, char destination[])
{
	char* currDest = destination;

	for (size_t i = 0; i < count; i++)
	{
		currDest += values[i].pack(currDest);
	}

	return currDest - destination;
}

size_t unpack(OpenGame values[], size_t count, char const source[])
{
	char const* currSource = source;

	for (size_t i = 0; i < count; i++)
	{
		currSource += values[i].unpack(currSource);
	}

	return currSource - source;
}