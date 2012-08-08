/**
 *  Client.h
 *
 *  This file is part of 3DWebcam
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef CLIENT_H
#define CLIENT_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <QtGui>
#include <QtNetwork>

#include "ClientWindow.h"
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Global constants
//-------------------------------------------------------------------
#define MESSAGE		0
#define USERNAME	1
#define FILE264		2
//-------------------------------------------------------------------


class Client : public QObject {
	Q_OBJECT

	// Public functions
    public:
		// Constructors
        Client(QObject* parent = 0);
		Client(ClientWindow* w, QObject* parent = 0);
		// Destructor
		~Client();

		// Getter
		QTcpSocket* getSocket() const;

	private slots:
		// Slot called when a packet (or sub-packet) has been recieved
        void dataRecieved();

		// Slot called when the user wants to connect to the server
		void connection(QString serverId, int port);

		// Slot called when the client recieves the confirmation
		// that he's connected to the server
		void connected();

		// Slot called when the suer wants to send a message
		void send(QString str);

		// Slot called when the user wants to send a file
		void sendFile(QString str);

	// Private variables
    private:
		// The socket
		QTcpSocket* socket;

		// The size of the packet that is being recieved
        quint16 packetSize;

		// The graphic user interface associated with the client
		ClientWindow* window;
};

#endif // CLIENT_H
