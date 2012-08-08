/**
 *  ServerWindow.h
 *
 *  This file is part of 3DWebcamServer
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <QtGui>
//-------------------------------------------------------------------


class ServerWindow : public QWidget {
	// Public functions
    public:
		// Constructor
        ServerWindow();

		// Getter
		QLabel* getServerStatus() const;

		// Display some informations
		void displayInfo(const QString str);

	// Private variables
    private:
        QLabel* serverStatus;
		QLabel* infoBox;
        QPushButton* quitButton;
};

#endif // SERVERWINDOW_H

