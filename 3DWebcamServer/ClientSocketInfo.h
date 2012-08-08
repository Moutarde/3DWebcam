/**
 *  ClientSocketInfo.h
 *
 *  This file is part of 3DWebcamServer
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef CLIENTSOCKETINFO_H
#define CLIENTSOCKETINFO_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <QtGui>
#include <QtNetwork>
//-------------------------------------------------------------------


class ClientSocketInfo : public QObject {
	// Public functions
    public:
		// Constructors
        ClientSocketInfo(QObject* parent = 0);
		ClientSocketInfo(QTcpSocket* s, QObject* parent = 0);
		//Destructor
		~ClientSocketInfo();

		// Getters
		QTcpSocket*	getSocket() const;
		quint16		getMessageSize() const;
		QString		getUsername() const;
		bool		isConnected() const;

		// Setters
		void setUsername(const QString str);
		void setConnected(const bool b);
		
	// Private variables
    private:
		// The socket
		QTcpSocket* socket;

		// The size of the packet that is being recieved
        quint16 packetSize;

		QString username;

		bool connected;
};

#endif // CLIENTSOCKETINFO_H
