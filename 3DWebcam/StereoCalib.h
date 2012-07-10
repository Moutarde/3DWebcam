#ifndef STEREOCALIB_H
#define STEREOCALIB_H

#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <cv.h>
#include <cxmisc.h>
#include <highgui.h>
#include <cvaux.h>
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <QWidget>
#include <QDir>

#include "QOpenCVWidget.h"

using namespace std;

void StereoCalib(const char* imageList, int nx, int ny, int useUncalibrated, float _squareSize);

#endif // STEREOCALIB_H