/**
 *  Client.cpp
 *
 *  This file is part of 3DWebcam
 *
 *  This class is the client.
 *	A client can communicate with other clients via a server.
 *	It can recieve and send messages and files.
 *	It has a graphic user interface from which the user can use it.
 *
 *  Author: Nicolas Kniebihler
 *
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "Client.h"
//-------------------------------------------------------------------


Client::Client(QObject* parent) : QObject(parent) {
	socket = new QTcpSocket(parent);
	packetSize = 0;
	window = new ClientWindow();
	window->show();

	// Connect the slots
	connect(socket, SIGNAL(readyRead()), this, SLOT(dataRecieved()));
	connect(socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(socket, SIGNAL(disconnected()), window, SLOT(disconnected()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), window, SLOT(errorSocket(QAbstractSocket::SocketError)));
	connect(window, SIGNAL(connection(QString, int)), this, SLOT(connection(QString, int)));
	connect(window, SIGNAL(send(QString)), this, SLOT(send(QString)));
	connect(window, SIGNAL(sendFile(QString)), this, SLOT(sendFile(QString)));
}

Client::Client(ClientWindow* w, QObject* parent) : QObject(parent) {
	socket = new QTcpSocket(parent);
	packetSize = 0;
	window = w;
	window->show();
	
	// Connect the slots
	connect(socket, SIGNAL(readyRead()), this, SLOT(dataRecieved()));
	connect(socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(socket, SIGNAL(disconnected()), window, SLOT(disconnected()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), window, SLOT(errorSocket(QAbstractSocket::SocketError)));
	connect(window, SIGNAL(connection(QString, int)), this, SLOT(connection(QString, int)));
	connect(window, SIGNAL(send(QString)), this, SLOT(send(QString)));
	connect(window, SIGNAL(sendFile(QString)), this, SLOT(sendFile(QString)));
}

Client::~Client() {
	socket->deleteLater();
}

QTcpSocket* Client::getSocket() const {
	return socket;
}

void Client::dataRecieved() {
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
	quint16 packetType;
	in >> packetType;

	if(packetType == MESSAGE) {	// If the packet is a text message
		// Now we can get the entire message
		QString messageRecieved;
		in >> messageRecieved;

		// Display the message on the chat
		window->display(messageRecieved);
	}
	else if(packetType == FILE264) {	// If the packet is a .264 file
		// Get the file's size
		quint16 fileSize;
		in >> fileSize;

		window->display("File recieved");

		// Save the file in "recieved_file"
		QFile file("recieved_file");
		file.open(QIODevice::WriteOnly);

		file.write(socket->readAll().right(fileSize));
		
		file.close();
	}
	else {
		window->display("Message recieved with wrong format...");
	}

	// Set packetSize to 0 so that we can recieve a new packet
	packetSize = 0;
}

void Client::connection(QString serverId, int port) {
	// Disable the previous connections if there are some
	socket->abort();
	// Connect to the required server
	socket->connectToHost(serverId, port);
}

void Client::connected() {
	// Display that the client is connected in the chat window
	window->connected();

	// Send the username to the server
	QByteArray paquet;
	QDataStream out(&paquet, QIODevice::WriteOnly);

	// Create the packet to send
	QString messageToSend = window->getUsername();

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
	socket->write(paquet);
}

void Client::send(QString str) {
	// Create the packet to send
	QByteArray paquet;
	QDataStream out(&paquet, QIODevice::WriteOnly);
	
	// Save space for the packet's size
	out << (quint16) 0;
	// Packet's type
	out << (quint16) MESSAGE;
	// Message
	out << str;
	// Replace the buffer a the beginning of the packet
	out.device()->seek(0);
	// Write the packet's size on the space we saved before
	out << (quint16) (paquet.size() - sizeof(quint16));
	
	// Send the packet
	socket->write(paquet);
}

void Client::sendFile(QString str) {
	// Create the file with the path given
	QFile file(str);
	// Open the selected file
	file.open(QIODevice::ReadOnly);

	// Create the packet to send
	QByteArray paquet;
	QDataStream out(&paquet, QIODevice::WriteOnly);
	
	// Save space for the packet's size
	out << (quint16) 0;
	// Packet's type
	out << (quint16) FILE264;
	// File's size
	out << (quint16) file.size();
	// File's content
	out << file.readAll();
	// Replace the buffer a the beginning of the packet
	out.device()->seek(0);
	// Write the packet's size on the space we saved before
	out << (quint16) (paquet.size() - sizeof(quint16));
	
	// Close the file
	file.close();
	
	// Send the packet
	socket->write(paquet);
}
