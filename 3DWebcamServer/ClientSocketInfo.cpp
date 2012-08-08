/**
 *  ClientSocketInfo.cpp
 *
 *  This file is part of 3DWebcamServer
 *
 *  This class contains all the informations
 *	the server needs to know about the clients
 *	that are connected to it.
 *
 *  Author: Nicolas Kniebihler
 *
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "ClientSocketInfo.h"
//-------------------------------------------------------------------


ClientSocketInfo::ClientSocketInfo(QObject* parent) :
	QObject(parent),
	socket(new QTcpSocket(parent)),
    packetSize(0),
	connected(false)
{
}

ClientSocketInfo::ClientSocketInfo(QTcpSocket* s, QObject* parent) :
	QObject(parent),
	socket(s),
	packetSize(0),
	connected(false)
{
}

ClientSocketInfo::~ClientSocketInfo() {
	socket->deleteLater();
}

QTcpSocket* ClientSocketInfo::getSocket() const {
	return socket;
}

quint16 ClientSocketInfo::getMessageSize() const {
	return packetSize;
}

void ClientSocketInfo::setUsername(const QString str) {
	username = str;
}

QString ClientSocketInfo::getUsername() const {
	return username;
}

void ClientSocketInfo::setConnected(const bool b) {
	connected = b;
}

bool ClientSocketInfo::isConnected() const {
	return connected;
}
