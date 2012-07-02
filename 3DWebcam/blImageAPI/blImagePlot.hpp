#ifndef BL_IMAGEPLOT_HPP
#define BL_IMAGEPLOT_HPP


//-------------------------------------------------------------------
// FILE:            blImagePlot.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of simple functions I created
//                  to plot data on an image
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    - blImage
//                  - blColor3
//
// NOTES:
//
// DATE CREATED:    Nov/30/2010
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums used for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            PlotGrid
//
// ARGUMENTS:           - ImagePlot
//                      - NumOfXAxisLines
//                      - NumOfYAxisLines
//                      - GridPercentageOfImageHeight
//                      - GridPercentageOfImageWidth
//                      - GridPercentageOfImageYStart
//                      - GridPercentageOfImageXStart
//                      - LineColor
//                      - LineThickness
//                      - LineType
//
// TEMPLATE ARGUMENTS:  blType
//
// PURPOSE:             This function draws a grid on the plot
//
// DEPENDENCIES:        - blImage
//-------------------------------------------------------------------
template<typename blType>
inline void PlotGrid(blImage<blType>& ImagePlot,
                     const int& NumOfXAxisLines,
                     const int& NumOfYAxisLines,
                     const double& GridPercentageOfImageHeight = 0.60,
                     const double& GridPercentageOfImageWidth = 0.60,
                     const double& GridPercentageOfImageYStart = 0.20,
                     const double& GridPercentageOfImageXStart = 0.20,
                     const CvScalar& LineColor = CV_RGB(100,100,100),
                     const int& LineThickness = 1,
                     const int& LineType = CV_AA)
{
    // Let's calculate the starting
    // point of where the plot grid
    // will be drawn on the image
    int xStart = GridPercentageOfImageXStart*double(ImagePlot.size2());
    int yStart = GridPercentageOfImageYStart*double(ImagePlot.size1());

    // Let's calculate the ending
    // point of where the plot grid
    // will be drawn on the image
    int xEnd = xStart + double(ImagePlot.size2())*GridPercentageOfImageWidth;
    int yEnd = yStart + double(ImagePlot.size2())*GridPercentageOfImageHeight;

    // Let's calculate the x and y steps
    double xStep = double(xEnd - xStart)/double(NumOfXAxisLines - 1);
    double yStep = double(yEnd - yStart)/double(NumOfYAxisLines - 1);

    // Draw the horizontal lines
    for(int i = 0; i < NumOfYAxisLines; ++i)
    {
        cvLine(ImagePlot,
               cvPoint(xStart,double(ImagePlot.size1()) - yStart - double(i)*yStep),
               cvPoint(xEnd,double(ImagePlot.size1()) - yStart - double(i)*yStep),
               LineColor,
               LineThickness,
               LineType);
    }

    // Draw the vertical lines
    for(int i = 0; i < NumOfXAxisLines; ++i)
    {
        cvLine(ImagePlot,
               cvPoint(xStart + double(i)*xStep,yStart),
               cvPoint(xStart + double(i)*xStep,yEnd),
               LineColor,
               LineThickness,
               LineType);
    }
}
//-------------------------------------------------------------------


/*//-------------------------------------------------------------------
// FUNCTION:            PlotAxes
//
// ARGUMENTS:           - ImagePlot
//                      - NumOfXAxisLines
//                      - NumOfYAxisLines
//                      - AxesPercentageOfImageHeight
//                      - AxesPercentageOfImageWidth
//                      - AxesPercentageOfImageYStart
//                      - AxesPercentageOfImageXStart
//                      - Font
//                      - TextColor
//                      - LineColor
//                      - LineThickness
//                      - LineType
//
// TEMPLATE ARGUMENTS:  blType
//
// PURPOSE:             This function draws the plot's axes
//
// DEPENDENCIES:        - blImage
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void PlotAxes(blImage<blType>& ImagePlot,
                     const int& NumOfXAxisLines,
                     const int& NumOfYAxisLines,
                     const blType2& xMinValue,
                     const blType2& xMaxValue,
                     const blType2& yMinValue,
                     const blType2& yMaxValue,
                     const double& AxesPercentageOfImageHeight = 0.60,
                     const double& AxesPercentageOfImageWidth = 0.60,
                     const double& AxesPercentageOfImageYStart = 0.20,
                     const double& AxesPercentageOfImageXStart = 0.20,
                     const CvFont& Font,
                     const CvScalar& TextColor = CV_RGB(100,100,100),
                     const CvScalar& LineColor = CV_RGB(100,100,100),
                     const int& LineThickness = 1,
                     const int& LineType = CV_AA)
{
    // Let's calculate the position
    // ff where the plot axes
    // will be drawn on the image
    int xStart = AxesPercentageOfImageXStart*double(ImagePlot.size2());
    int yStart = AxesPercentageOfImageYStart*double(ImagePlot.size1());

    // Let's calculate the ending
    // point of where the plot grid
    // will be drawn on the image
    int xEnd = xStart + double(ImagePlot.size2())*AxesPercentageOfImageWidth;
    int yEnd = yStart + double(ImagePlot.size2())*AxesPercentageOfImageHeight;

    // Let's calculate the x and y steps
    double xStep = double(xEnd - xStart)/double(NumOfXAxisLines - 1);
    double yStep = double(yEnd - yStart)/double(NumOfYAxisLines - 1);

    // Since we're only printing numbers
    // on axes, we're going to calculate
    // the maximum character width and height
    // so that the text will be nicely formatted
    // when we print axes values on the image
    CvSize CharSize;
    int CharBaseline;
    cvGetTextSize('0',&Font,&CharSize,&CharBaseline);

    // Draw the y-axis numbers
    for(int i = 0; i < NumOfYAxisLines; ++i)
    {
    }

    // Draw the x-axis numbers
    for(int i = 0; i < NumOfXAxisLines; ++i)
    {
    }
}
//-------------------------------------------------------------------*/


//-------------------------------------------------------------------
// FUNCTION:            LinePlot
//
// ARGUMENTS:           - ImagePlot
//                      - DataArray
//                      - Rows
//                      - Cols
//                      - MinValue
//                      - MaxValue
//                      - WhichRowToStartFrom
//                      - WhichColToStartFrom
//                      - HowManyRowsToPlot
//                      - HowManyColsToPlot
//                      - PlotPercentageOfImageHeight
//                      - PlotPercentageOfImageWidth
//                      - PlotPercentageOfImageYStart
//                      - PlotPercentageOfImageXStart
//                      - LineThickness
//                      - LineType
//
// TEMPLATE ARGUMENTS:  blType
//
// PURPOSE:             Simple line plot connecting data points
//                      with lines
//
// DEPENDENCIES:        - blImage
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void LinePlot(blImage<blType>& ImagePlot,
                     const blType2* DataArray,
                     const int& Rows,
                     const int& Cols,
                     const blType2& MinValue,
                     const blType2& MaxValue,
                     int WhichRowToStartFrom,
                     int WhichColToStartFrom,
                     int HowManyRowsToPlot,
                     int HowManyColsToPlot,
                     const CvScalar* ColorArray,
                     const int& NumOfColors,
                     const double& PlotPercentageOfImageHeight = 0.60,
                     const double& PlotPercentageOfImageWidth = 0.60,
                     const double& PlotPercentageOfImageYStart = 0.20,
                     const double& PlotPercentageOfImageXStart = 0.20,
                     const int& LineThickness = 0,
                     const int& LineType = CV_AA)
{
    // Check if the pointer is NULL
    if(DataArray == NULL)
    {
        // Error -- Tried to plot data by
        //          passing a NULL pointer
        return;
    }

    // If the size of the data array is
    // zero, then we don't have to
    // plot anything, so we just quit
    if(Rows <= 0 || Cols <= 0)
    {
        // Error -- Tried to plot data but
        //          the size of the data
        //          array is zero
        return;
    }

    // Let's calculate what the final
    // size of the plot will be once
    // it's plotted onto the image
    int PlotRows = double(ImagePlot.size1())*PlotPercentageOfImageHeight;
    int PlotCols = double(ImagePlot.size2())*PlotPercentageOfImageWidth;

    // Let's calculate the starting
    // point of where the plot will
    // be drawn on the image
    int xStart = PlotPercentageOfImageXStart*double(ImagePlot.size2());
    int yStart = PlotPercentageOfImageYStart*double(ImagePlot.size1());

    // Let's calculate the scale that
    // we'll multiply each data point
    // by in order to fit the data
    // on the image
    double yScale = double(PlotRows)/double(MaxValue - MinValue);
    double xScale = double(PlotCols)/double(HowManyColsToPlot - 1);

    // If the row or col to start
    // from are bigger than the
    // available number of rows or
    // cols, then we wrap them around.
    // Same if they are negative, we
    // wrap them around
    if(WhichRowToStartFrom >= 0)
        WhichRowToStartFrom = WhichRowToStartFrom % Rows;
    else
        WhichRowToStartFrom = Rows + (WhichRowToStartFrom % Rows);

    if(WhichColToStartFrom >= 0)
        WhichColToStartFrom = WhichColToStartFrom % Cols;
    else
        WhichColToStartFrom = Cols + (WhichColToStartFrom % Cols);

    // Variable used to step through
    // the data and get the value
    blType2 Value = DataArray[0];

    // Temporary points used to
    // plot the data points
    CvPoint MyPoint1 = cvPoint(xStart,double(ImagePlot.size1()) - yStart - double(Value - MinValue)*yScale);
    CvPoint MyPoint2 = MyPoint1;

    // We're going to use the color array
    // to choose how to color each data
    // set.  If the color array is null,
    // then we just pick a default color
    int ColorIndex = 0;

    // We're going to plot different rows, where
    // each row represents a set of data points.
    //
    // If the number of rows to plot is bigger than
    // the number of available rows, then the plotter
    // replots the rows until the desired number of
    // rows is plotted (the samething for
    // the number of columns to plot)
    for(int i = 0; i < HowManyRowsToPlot; ++i)
    {
        Value = DataArray[(i%Rows)*Cols];

        MyPoint1 = cvPoint(xStart,double(ImagePlot.size1()) - yStart - double(Value - MinValue)*yScale);
        MyPoint2 = MyPoint1;

        for(int j = 0; j < HowManyColsToPlot; ++j)
        {
            // Get the value of the current data point
            Value = DataArray[(i%Rows)*Cols + (j%Cols)];

            // Calculate the plot point coordinate
            // (We invert the y coordinate so that
            // the 0,0 point will be the bottom
            // left point, instead of the
            // top left point)
            MyPoint2.x = xStart + double(j)*xScale;
            MyPoint2.y = double(ImagePlot.size1()) - yStart - double(Value - MinValue)*yScale;

            // Draw the line connecting
            // point1 and point2
            if(ColorArray != NULL)
            {
                cvLine(ImagePlot,
                       MyPoint1,
                       MyPoint2,
                       ColorArray[ColorIndex % NumOfColors],
                       LineThickness,
                       LineType);
            }
            else
            {
                cvLine(ImagePlot,
                       MyPoint1,
                       MyPoint2,
                       CV_RGB(0,0,255),
                       LineThickness,
                       LineType);
            }

            // Store the current point
            MyPoint1 = MyPoint2;
        }

        // Go to the next color
        ++ColorIndex;
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGEPLOT_HPP
