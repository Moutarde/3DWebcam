/**
 *  ClientWindow.h
 *
 *  This file is part of 3DWebcam
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <QtGui>
#include <QtNetwork>
//-------------------------------------------------------------------


class ClientWindow : public QWidget
{
    Q_OBJECT

	// Public functions
    public:
		//Constructor
        ClientWindow();

		// Display a message in the chat box
		void display(const QString str);

		// Display that the client is connected to the server
        void connected();

		// Getters
		QString getUsername() const;
		QStringList getClientsList() const;

		void appendToClientsList(const QString str);

    public slots:
		// Slots called when buttons are clicked
        void on_connectionButton_clicked();
        void on_sendButton_clicked();
		void on_sendFileButton_clicked();
		// Slot called when the Enter key is pressed
        void on_message_returnPressed();

		// Slot called when the client is disconnected
        void disconnected();

		// Slot called when an error occurs
        void errorSocket(QAbstractSocket::SocketError err);

	signals:
		void connection(QString serverId, int port);
		void send(QString str);
		void sendFile(QString str);

	// Private variables
    private:
		QTextEdit* messageList;
		QPushButton* connectionButton;
		QPushButton* sendButton;
		QPushButton* sendFileButton;
		QSpinBox* serverPort;
		QLineEdit* IPServer;
		QLineEdit* username;
		QLineEdit* message;
		QStringList clientsList;
		QStringListModel* model;
};

#endif // CLIENTWINDOW_H
