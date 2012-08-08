/**
 *  ServerWindow.cpp
 *
 *  This file is part of 3DWebcamServer
 *
 *  This class is the server's graphic user interface.
 *	It displays the status and some informations about the server.
 *
 *  Author: Nicolas Kniebihler
 *
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include "ServerWindow.h"
//-------------------------------------------------------------------


ServerWindow::ServerWindow() {
    serverStatus = new QLabel;
    infoBox = new QLabel;
	quitButton = new QPushButton(tr("Quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(serverStatus);
    layout->addWidget(infoBox);
    layout->addWidget(quitButton);
    setLayout(layout);

    setWindowTitle(tr("3DWebcam - Server"));
}

QLabel* ServerWindow::getServerStatus() const {
	return serverStatus;
}

void ServerWindow::displayInfo(const QString str) {
	infoBox->setText(str);
}
