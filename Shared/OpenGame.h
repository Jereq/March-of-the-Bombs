#pragma once

#include <string>
#include <stdint.h>

class OpenGame
{
private:
	uint16_t gameID;
	std::string mapName;
	std::string playerName;

public:
	OpenGame();
	OpenGame(unsigned short gameID, std::string const& mapName, std::string const& playerName);

	unsigned short getGameID() const;
	std::string const& getMapName() const;
	std::string const& getPlayerName() const;

	size_t packedSize() const;

	size_t pack(char destination[]) const;
	size_t unpack(char const source[]);
};

size_t pack(OpenGame const values[], size_t count, char destination[]);
size_t unpack(OpenGame values[], size_t count, char const source[]);