/**
 *  Server.cpp
 *
 *  This file is part of 3DWebcamServer
 *
 *  This class is the server.
 *	A server can accept connections from clients.
 *	Then it sends the packets recieved from those clients
 *	to the other connected clients.
 *
 *  Author: Nicolas Kniebihler
 *
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "Server.h"
//-------------------------------------------------------------------


Server::Server(QObject* parent) : QObject(parent) {
	packetSize = 0;
	server = new QTcpServer(parent);
	window = new ServerWindow();
	window->show();

	// Start the server on all IPs available on the port 50585
	if (!server->listen(QHostAddress::Any, 50885)) {	// If the server didn't start correctly
		window->getServerStatus()->setText(tr("The server could not start :<br />") + server->errorString());
	}
	else {
		window->getServerStatus()->setText(tr("The server has started on port <strong>") + QString::number(server->serverPort()) + tr("</strong>.<br />Clients can now connect."));
		connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
	}
}

void Server::newConnection() {
	window->displayInfo("New connection");

	ClientSocketInfo *newClient = new ClientSocketInfo(server->nextPendingConnection());
	// Add the new client to the list of connected clients
	clients << newClient;

	// Connect the signals of the client's socket to the server's slots
	connect(newClient->getSocket(), SIGNAL(readyRead()), this, SLOT(dataRecieved()));
	connect(newClient->getSocket(), SIGNAL(disconnected()), this, SLOT(clientDisconnection()));
}

void Server::dataRecieved() {
	// Get the QTcpSocket of the client that has send the packet
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	// If we didn't find the client, return
	if (socket == 0) {
		return;
	}

	// Search the client corresponding to the QTcpSocket found
	ClientSocketInfo* client = NULL;
	for (int i = 0; i < clients.size(); i++) {
		if(clients[i]->getSocket() == socket)
			client = clients[i];
	}

	// If we didn't find it, return
	if(client == NULL) {
		return;
	}
	
	// Store the packet in the data stream "in"
	QDataStream in(socket);
	
	// If it is a new packet, try to get it's size
	if (packetSize == 0) {
		// If we haven't recieved a quantity of data
		// sufficient to read the packet's size, return
		if (socket->bytesAvailable() < (int)sizeof(quint16)) {
			return;
		}
		
		// Get the packet's size
		in >> packetSize;
	}
	
	// If we haven't recieved the entire packet, return
	if (socket->bytesAvailable() < packetSize) {
		return;
	}
	
	// Get the packet's type
	quint16 messageType;
	in >> messageType;

	if(messageType == MESSAGE) {	// If the packet is a text message
		// Now we can get the entire message
		QString message;
		in >> message;

		window->displayInfo("Data recieved : " + message);

		// Send back the message to all clients
		sendToAllClients(message);
	}
	else if(messageType == USERNAME) {	// If the packet is a username
		// Now we can get the username
		QString username;
		in >> username;

		window->displayInfo("Data recieved : " + username);

		if(client->isConnected()) {	// If this client is already connected
			// Send to all clients that he changed his username
			sendToAllClients(tr("<strong>") + client->getUsername() + tr("</strong><em> has changed his username to : </em><strong>") + username + "</strong>");
		}
		else {
			// Send the username to the clients
			QByteArray paquet;
			QDataStream out(&paquet, QIODevice::WriteOnly);

			QString messageToSend = username;

			out << (quint16) 0;
			out << (quint16) USERNAME;
			out << messageToSend;
			out.device()->seek(0);
			out << (quint16) (paquet.size() - sizeof(quint16));

			sendToAllOtherClients(paquet, client);

			for (int i = 0; i < clients.size(); i++) {
				if(clients[i] != client) {
					// Send the username to the server
					QByteArray paquet;
					QDataStream out(&paquet, QIODevice::WriteOnly);

					// Create the packet to send
					QString messageToSend = clients[i]->getUsername();

					// Save space for the packet's size
					out << (quint16) 0;
					// Packet's type
					out << (quint16) USERNAME;
					// Message
					out << messageToSend;
					// Replace the buffer a the beginning of the packet
					out.device()->seek(0);
					// Write the packet's size on the space we saved before
					out << (quint16) (paquet.size() - sizeof(quint16));

					// Send the packet
					//while(!client->getSocket()->isReadable()){}
					client->getSocket()->write(paquet);
				}
			}
			client->setConnected(true);
		}
		// Change the username
		client->setUsername(username);
	}
	else if(messageType == FILE264) {	// If the packet is a .264 file
		// Get the file's size
		quint16 fileSize;
		in >> fileSize;

		// Redo the packet
		QByteArray paquet;
		QDataStream f(&paquet, QIODevice::WriteOnly);
		f << (quint16) 0;
		f << (quint16) FILE264;
		f << (quint16) fileSize;
		f << socket->readAll().right(fileSize);
		f.device()->seek(0);
		f << (quint16) (paquet.size() - sizeof(quint16));

		window->displayInfo("File recieved");

		// Send the packet to all other clients
		sendToAllOtherClients(paquet, client);
	}
	else {
		window->displayInfo("ERROR");
	}
	
	// Set packetSize to 0 so that we can recieve a new packet
	packetSize = 0;
}

void Server::clientDisconnection() {
	// Get the QTcpSocket of the client that has disconnected
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	// If we didn't find the client, return
	if (socket == 0) {
		return;
	}
	
	// Search the client corresponding to the QTcpSocket found
	ClientSocketInfo* client = NULL;
	for (int i = 0; i < clients.size(); i++) {
		if(clients[i]->getSocket() == socket)
			client = clients[i];
	}
	
	if(client != NULL) {	// If we found it
		sendToAllClients(tr("<strong>") + client->getUsername() + tr("</strong><em> has disconnected</em>"));
	
		// Remove the client from the list
		clients.removeOne(client);

		delete client;
	}
}

void Server::sendToAllClients(const QString &message) const {
	// Create the packet to send
	QByteArray paquet;
	QDataStream out(&paquet, QIODevice::WriteOnly);
	
	// Save space for the packet's size
	out << (quint16) 0;
	// Packet's type
	out << (quint16) MESSAGE;
	// Message
	out << message;
	// Replace the buffer a the beginning of the packet
	out.device()->seek(0);
	// Write the packet's size on the space we saved before
	out << (quint16) (paquet.size() - sizeof(quint16));

	// Send the packet to all the connected clients
	for (int i = 0; i < clients.size(); i++) {
		clients[i]->getSocket()->write(paquet);
	}
}

void Server::sendToAllOtherClients(const QByteArray &stream, const ClientSocketInfo* client) const {
	// Send the packet to all the connected clients, except the client given in parameter
	for (int i = 0; i < clients.size(); i++) {
		if(clients[i] != client) {
			clients[i]->getSocket()->write(stream);
		}
	}
}

void Server::sendTo(const QString &message, const ClientSocketInfo* client) const {
	// Create the packet to send
	QByteArray paquet;
	QDataStream out(&paquet, QIODevice::WriteOnly);
	
	// Save space for the packet's size
	out << (quint16) 0;
	// Packet's type
	out << (quint16) MESSAGE;
	// Message
	out << message;
	// Replace the buffer a the beginning of the packet
	out.device()->seek(0);
	// Write the packet's size on the space we saved before
	out << (quint16) (paquet.size() - sizeof(quint16));
	
	// Send the packet to the client given in parameter
	client->getSocket()->write(paquet);
}
