/**
 *  ClientWindow.cpp
 *
 *  This file is part of 3DWebcam
 *
 *  This class is the client's graphic user interface.
 *	It displays a chat box, and buttons and labels
 *	that the user can use to connect to a server
 *	and send messages and files.
 *
 *  Author: Nicolas Kniebihler
 *
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "ClientWindow.h"
//-------------------------------------------------------------------


ClientWindow::ClientWindow()
{
	QVBoxLayout* verticalLayout_2 = new QVBoxLayout;
	QHBoxLayout* horizontalLayout = new QHBoxLayout;

	QLabel* label = new QLabel("Server's IP :");

	IPServer = new QLineEdit("127.0.0.1");
	IPServer->setMaximumSize(150, 16777215);

	QLabel* label_2 = new QLabel("Server's port :");

	serverPort = new QSpinBox;
	serverPort->setMinimumSize(60, 0);
	serverPort->setMinimum(1024);
	serverPort->setMaximum(65535);
	serverPort->setValue(50885);

	connectionButton = new QPushButton("Connect");
	connect(connectionButton, SIGNAL(clicked()), this, SLOT(on_connectionButton_clicked()));

	horizontalLayout->addWidget(label);
	horizontalLayout->addWidget(IPServer);
	horizontalLayout->addWidget(label_2);
	horizontalLayout->addWidget(serverPort);
	horizontalLayout->addWidget(connectionButton);

	messageList = new QTextEdit;
	messageList->setReadOnly(true);

	QHBoxLayout* horizontalLayout_2 = new QHBoxLayout;

	QLabel* label_3 = new QLabel("Username :");

	username = new QLineEdit;
	username->setMaximumSize(100, 16777215);

	QLabel* label_4 = new QLabel("Message :");

	message = new QLineEdit;

	sendButton = new QPushButton("Send");
	connect(sendButton, SIGNAL(clicked()), this, SLOT(on_sendButton_clicked()));
	
	sendFileButton = new QPushButton("Send a file");
	connect(sendFileButton, SIGNAL(clicked()), this, SLOT(on_sendFileButton_clicked()));

	horizontalLayout_2->addWidget(label_3);
	horizontalLayout_2->addWidget(username);
	horizontalLayout_2->addWidget(label_4);
	horizontalLayout_2->addWidget(message);
	horizontalLayout_2->addWidget(sendButton);
	horizontalLayout_2->addWidget(sendFileButton);

	verticalLayout_2->addLayout(horizontalLayout);
	verticalLayout_2->addWidget(messageList);
	verticalLayout_2->addLayout(horizontalLayout_2);

	setLayout(verticalLayout_2);
}

void ClientWindow::on_connectionButton_clicked() {
	messageList->append(tr("<em>Connection attempt in progress...</em>"));
	connectionButton->setEnabled(false);

	// Emit a signal with the server IP and the port we try to connect to
	emit connection(IPServer->text(), serverPort->value());
}

void ClientWindow::on_sendButton_clicked() {
	// Create the message to send
	QString messageToSend = tr("<strong>") + username->text() +tr("</strong> : ") + message->text();

	// Emit a signal with the message to send
	emit send(messageToSend);

	// Clear and focus the message editing box
	message->clear();
	message->setFocus();
}

void ClientWindow::on_sendFileButton_clicked() {
	// Request a file to the user and emit a signal with the path to this file
	emit sendFile(QFileDialog::getOpenFileName(0, "Send a file", QString(), "All (*)"));

	// Focus the message editing box
	message->setFocus();
}

void ClientWindow::on_message_returnPressed() {
	// Pressing Enter has the same effect as clicking the "Send" button
	on_sendButton_clicked();
}

void ClientWindow::connected() {
	messageList->append(tr("<em>Connection succeeded !</em>"));
	connectionButton->setEnabled(true);
}

void ClientWindow::disconnected() {
	messageList->append(tr("<em>Déconnecté du serveur</em>"));
}

void ClientWindow::errorSocket(QAbstractSocket::SocketError err) {
	// Get the client that has send the error
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket != 0) {
		messageList->append(tr("<em>ERROR : ") + socket->errorString() + tr("</em>"));
	}

	connectionButton->setEnabled(true);
}

void ClientWindow::display(const QString str) {
	messageList->append(str);
}

QString ClientWindow::getUsername() const {
	return username->text();
}
