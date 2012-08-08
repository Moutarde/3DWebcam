/**
 *  main.c
 *
 *  This file is part of 3DWebcamServer
 *
 *  This is the main file.
 *  It is the base of the program, and contains the main function.
 *
 *  Author: Nicolas Kniebihler
 *	
 *  Copyright © 2012. All rights reserved.
 *
 */

//-------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------
#include <QApplication>
#include "Server.h"
//-------------------------------------------------------------------


/**
 *	This is the main function.
 *	This function is called when the program is launched.
 */
int main(int argc, char **argv) {
	// Qt application
	QApplication app(argc, argv);

	// Create a new server
	new Server;

	return app.exec();
}