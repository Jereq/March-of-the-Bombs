#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>

#include <iostream>

void Packet1SimpleMessage::dispatch(void* context) const
{
	std::cout << "<Server> " << getMessage() << std::endl;
}

void Packet2Blob::dispatch(void* constex) const
{
	std::cout << "Received blob from server: " << getBlobLength() << " bytes" << std::endl;
}

void Packet3Login::dispatch(void* context) const
{
	std::cout << getName() << " logged in to the server" << std::endl;
}