#include <iostream>

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include "GameServer.h"
#include "Lobby.h"

int main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cerr << "Usage: " << argv[0] << " <port> [<port> ...]" << std::endl;
			return EXIT_FAILURE;
		}

		boost::asio::io_service io_service;

		boost::shared_ptr<PacketManager> packetManager(new PacketManager());
		GameServer::list servers;
		Lobby lobby(packetManager);
		for (int i = 1; i < argc; ++i)
		{
			tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
			GameServer::ptr server(new GameServer(io_service, endpoint, lobby));
			servers.push_back(server);
		}

		io_service.run();
	}
	catch (std::exception& ex)
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
	}

	return EXIT_SUCCESS;
}