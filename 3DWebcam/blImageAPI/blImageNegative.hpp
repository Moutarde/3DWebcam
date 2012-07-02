#ifndef BL_IMAGENEGATIVE_HPP
#define BL_IMAGENEGATIVE_HPP


//-------------------------------------------------------------------
// FILE:            blImageNegative.hpp
// CLASS:           None
// BASE CLASS:      None
// PURPOSE:         A collection of templated function that
//                  take the negative of an image, no matter the
//                  depth of the image. (OpenCV does not take the
//                  negative of floating point images)
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    blImage -- The image structure
//                  cvNot -- To take the negative of images
//                           with a depth of unsigned char
// NOTES:
// DATE CREATED:    Oct/28/2010
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blImageNegative
// ARGUMENTS:           SrcImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Take the negative of a blImage no matter what
//                      the depth of the image is
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType>
inline void ImageNegative(const blImage<blType>& SrcImage,
                          blImage<blType>& DstImage)
{
    // Get the size of the image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the negative image is the
    // correct size, otherwise resize it accordingly
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage = blImage<blType>(Rows,Cols);

    // Get the min/max range for the depth of the image
    blType MinColor = SrcImage.GetRangeMin();
    blType MaxColor = SrcImage.GetRangeMax();

    // Step through the image pixels and calculate the
    // negative pixels (complement pixels)
    for(int i = 0; i < Rows; ++i)
        for(int j = 0; j < Cols; ++j)
            DstImage[i][j] = MaxColor - SrcImage[i][j] + MinColor;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blImageNegative
// ARGUMENTS:           SrcImage,DstImage
// TEMPLATE ARGUMENTS:  unsigned char (template specialization)
// PURPOSE:             Template specialization for unsigned char, in
//                      this case we can just use the cvNot function
// DEPENDENCIES:        blImage
//                      cvNot
//-------------------------------------------------------------------
template<>
inline void ImageNegative(const blImage<unsigned char>& SrcImage,
                          blImage<unsigned char>& DstImage)
{
    // Get the size of the image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the negative image is the
    // correct size, otherwise resize it accordingly
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage = blImage<unsigned char>(Rows,Cols);

    // Now that we made sure the destination
    // image is the correct size, we can just
    // call the cvNot function
    cvNot(SrcImage,DstImage);
}
//-------------------------------------------------------------------


#endif // BL_IMAGENEGATIVE_HPP
