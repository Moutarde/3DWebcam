#ifndef BL_IMAGESHIFTING_HPP
#define BL_IMAGESHIFTING_HPP


//-------------------------------------------------------------------
// FILE:            blImageShifting.hpp
// CLASS:           None
// BASE CLASS:      None
// PURPOSE:         A collection of simple functions I wrote
//                  to shift functions up, down, left, right.
//                  I will be adding more functions as I build them
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:
// NOTES:
// DATE CREATED:    Jan/01/2011
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
// FUNCTION:            ShiftImageByMRowsAndNCols
// ARGUMENTS:           SrcImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Takes an image and shifts it by N columns
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType>
inline void ShiftImageByMRowsAndNCols(const blImage<blType>& SrcImage,
                                      blImage<blType>& DstImage,
                                      const int& HowManyRowsToShiftBy,
                                      const int& HowManyColsToShiftBy)
{
    // Let's get the size
    // of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the
    // destination image is the
    // same size as the source
    // image
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Because the indeces can be
    // negative or bigger than the
    // size of the image, then we
    // do some simple math to "wrap"
    // the indeces around
    int M = HowManyRowsToShiftBy % Rows;
    int N = HowManyColsToShiftBy % Cols;

    // Let's make a copy of the source
    // image because the destination
    // image could be the same image
    // as the source image, and that
    // would cause problems
    blImage<blType> ImageCopy = SrcImage;

    // Create the indeces used to
    // refer to the source image
    int RowIndex;
    int ColIndex;

    // Now it's just a matter of going
    // through the image and offsetting
    // the indeces by the right amount
    for(int i = 0; i < Rows; ++i)
    {
        RowIndex = (i + M) % Rows;
        if(RowIndex < 0)
            RowIndex = Rows + RowIndex;

        for(int j = 0; j < Cols; ++j)
        {
            ColIndex = (j + N) % Cols;
            if(ColIndex < 0)
                ColIndex = Cols + ColIndex;

            DstImage[i][j] = ImageCopy[RowIndex][ColIndex];
        }
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGESHIFTING_HPP
