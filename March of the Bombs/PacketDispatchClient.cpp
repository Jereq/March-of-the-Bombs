#include <Packet1SimpleMessage.h>
#include <Packet2Blob.h>
#include <Packet3Login.h>
#include <Packet4LoginAccepted.h>
#include <Packet5EntityMove.h>
#include <Packet6CreateGame.h>
#include <Packet7JoinGame.h>

#include <iostream>

#include "Game.h"

void Packet1SimpleMessage::dispatch(void* context) const
{
	GUIMain::ptr cont = *reinterpret_cast<GUIMain::ptr*>(context);
	cont->handlePacket1SimpleMessage(shared_from_this());
}

void Packet2Blob::dispatch(void* context) const
{
	GUIMain::ptr cont = *reinterpret_cast<GUIMain::ptr*>(context);
	cont->handlePacket2Blob(shared_from_this());
}

void Packet3Login::dispatch(void* context) const
{
	GUIMain::ptr cont = *reinterpret_cast<GUIMain::ptr*>(context);
	cont->handlePacket3Login(shared_from_this());
}

void Packet4LoginAccepted::dispatch(void* context) const
{
	GUIMain::ptr cont = *reinterpret_cast<GUIMain::ptr*>(context);
	cont->handlePacket4LoginAccepted(shared_from_this());
}

void Packet5EntityMove::dispatch(void* context) const
{
	GUIMain::ptr cont = *reinterpret_cast<GUIMain::ptr*>(context);
	cont->handlePacket5EntityMove(shared_from_this());
}

void Packet6CreateGame::dispatch(void* context) const
{
	GUIMain::ptr cont = *reinterpret_cast<GUIMain::ptr*>(context);
	cont->handlePacket6CreateGame(shared_from_this());
}

void Packet7JoinGame::dispatch(void* context) const
{
	GUIMain::ptr cont = *reinterpret_cast<GUIMain::ptr*>(context);
	cont->handlePacket7JoinGame(shared_from_this());
}