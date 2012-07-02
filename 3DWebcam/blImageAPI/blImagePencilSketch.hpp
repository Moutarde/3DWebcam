#ifndef BL_IMAGEPENCILSKETCH_HPP
#define BL_IMAGEPENCILSKETCH_HPP


//-------------------------------------------------------------------
// FILE:            blImagePencilSketch.hpp
// CLASS:           None
// BASE CLASS:      None
// PURPOSE:         A simple function used to take an image and
//                  make it look like it was sketched using a pencil
//                  which can be in B/W or in Color
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:
// NOTES:
// DATE CREATED:    Dec/22/2010
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
// FUNCTION:            ImageToPencilSketch
// ARGUMENTS:           SrcImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Transform an image into a B/W pencil sketch
// DEPENDENCIES:        ImageNegative
//                      cvSmooth
//                      BlendLinearDodge
//-------------------------------------------------------------------
template<typename blType>
inline void ImageToPencilSketch(const blImage<blType>& SrcImage,
                                blImage<blType>& DstImage,
                                const int& SmoothType = CV_BLUR,
                                const int& param1 = 19,
                                const int& param2 = 19,
                                const int& param3 = 0,
                                const double& param4 = 0)
{
    // Invert the source image
    ImageNegative(SrcImage,DstImage);

    // Blur the copied image
    // using a Gaussian Blur
    cvSmooth(DstImage,DstImage,SmoothType,param1,param2,param3,param4);

    // Blend the destination image
    // with the original one
    BlendLinearDodge(SrcImage,DstImage,DstImage);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            ImageToPencilSketch
// ARGUMENTS:           SrcImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Transform an image into a B/W pencil sketch
// DEPENDENCIES:        ImageNegative
//                      cvSmooth
//                      BlendLinearDodge
// NOTES:               This is a template specialization of
//                      the function to handle colored images
//-------------------------------------------------------------------
template<typename blType>
inline void ImageToPencilSketch(const blImage< blColor3<blType> >& SrcImage,
                                blImage<blType>& DstImage,
                                const int& SmoothType = CV_BLUR,
                                const int& param1 = 19,
                                const int& param2 = 19,
                                const int& param3 = 0,
                                const double& param4 = 0)
{
    // Copy the image
    blImage<blType> CopiedImage = SrcImage;

    // Invert the copied image
    ImageNegative(CopiedImage,DstImage);

    // Blur the copied image
    // using a Gaussian Blur
    cvSmooth(DstImage,DstImage,SmoothType,param1,param2,param3,param4);

    // Blend the destination image
    // with the original one
    BlendLinearDodge(CopiedImage,DstImage,DstImage);
}
//-------------------------------------------------------------------


#endif // BL_IMAGEPENCILSKETCH_HPP
