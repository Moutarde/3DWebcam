/**
 *  Server.h
 *
 *  This file is part of 3DWebcamServer
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef SERVER_H
#define SERVER_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <QtGui>
#include <QtNetwork>

#include "ClientSocketInfo.h"
#include "ServerWindow.h"
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Global constants
//-------------------------------------------------------------------
#define MESSAGE 0
#define USERNAME 1
#define FILE264 2
//-------------------------------------------------------------------


class Server : public QObject {
	Q_OBJECT

	// Public functions
	public:
		// Constructor
		Server(QObject* parent = 0);

		// Sending functions
        void sendToAllClients(const QString &message) const;
		void sendToAllOtherClients(const QByteArray &stream, const ClientSocketInfo* client) const;
        void sendTo(const QString &message, const ClientSocketInfo* client) const;

    private slots:
		// Slot called when a new client wants to connect to the server
        void newConnection();
		
		// Slot called when a packet (or sub-packet) has been recieved from a client
        void dataRecieved();

		// Slot called when a client disconnects from the server
        void clientDisconnection();

	// Private variables
	private:
		// The socket
		QTcpServer* server;

		// The list of the connected clients
        QList<ClientSocketInfo *> clients;

		// The size of the packet that is being recieved
        quint16 packetSize;
		
		// The graphic user interface associated with the server
		ServerWindow* window;
};

#endif // SERVER_H
