#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>
#include <Packet4LoginAccepted.h>

#include <iostream>

#include "Context.h"

void Packet1SimpleMessage::dispatch(void* context) const
{
	Player::ptr cont = *static_cast<Player::ptr*>(context);
	cont->handlePacket1SimpleMessage(shared_from_this());
}

void Packet2Blob::dispatch(void* context) const
{
	Player::ptr cont = *static_cast<Player::ptr*>(context);
	cont->handlePacket2Blob(shared_from_this());
}

void Packet3Login::dispatch(void* context) const
{
	Player::ptr cont = *static_cast<Player::ptr*>(context);
	cont->handlePacket3Login(shared_from_this());
}

void Packet4LoginAccepted::dispatch(void* context) const
{
	Player::ptr cont = *static_cast<Player::ptr*>(context);
	cont->handlePacket4LoginAccepted(shared_from_this());
}