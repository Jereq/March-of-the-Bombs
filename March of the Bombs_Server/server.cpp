#include <iostream>

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

#include <boost/lexical_cast.hpp>

#include "Acceptor.h"
#include "Lobby.h"

void registerPackets(boost::shared_ptr<PacketManager> const& packetManager)
{
	packetManager->addPacketPrototype(Packet::ptr(new Packet1SimpleMessage()));
	packetManager->addPacketPrototype(Packet::ptr(new Packet2Blob()));
	packetManager->addPacketPrototype(Packet::ptr(new Packet3Login()));
	packetManager->addPacketPrototype(Packet::ptr(new Packet4LoginAccepted()));
	packetManager->addPacketPrototype(Packet::ptr(new Packet5EntityMove()));
	packetManager->addPacketPrototype(Packet::ptr(new Packet6CreateGame()));
	packetManager->addPacketPrototype(Packet::ptr(new Packet7JoinGame()));
	packetManager->addPacketPrototype(Packet::ptr(new Packet8SetupGame()));
}

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
		registerPackets(packetManager);

		Acceptor::list acceptors;
		Lobby::ptr lobby(new Lobby(packetManager));
		for (int i = 1; i < argc; ++i)
		{
			unsigned short port = boost::lexical_cast<unsigned short>(argv[i]);

			tcp::endpoint endpoint(tcp::v4(), port);
			Acceptor::ptr acceptor(new Acceptor(io_service, endpoint, lobby));
			acceptors.push_back(acceptor);

			endpoint = tcp::endpoint(tcp::v6(), port);
			acceptor = Acceptor::ptr(new Acceptor(io_service, endpoint, lobby));
			acceptors.push_back(acceptor);
		}

		while (true)
		{
			try
			{
				io_service.run();
			}
			catch (PacketException& ex)
			{
				std::cerr << ex.what << std::endl;
			}
		}
	}
	catch (std::exception& ex)
	{
		std::cerr << "Exception: " << ex.what() << std::endl;
	}

	return EXIT_SUCCESS;
}