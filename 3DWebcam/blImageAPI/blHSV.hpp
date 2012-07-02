#ifndef BL_HSV_HPP
#define BL_HSV_HPP


//-------------------------------------------------------------------
// FILE:            blHSV.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of simple functions I created to
//                  convert images from and to the HSV color space
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blColor3 -- To represent color
//                  blImage -- The image structure
//                  cvCvtColor -- Used to convert unsigned char
//                                images from BGR to HSV and viceversa
//                                The cvCvtColor function is a little
//                                faster for unsigned char images
//
// NOTES:
// DATE CREATED:    Nov/02/2010
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
// FUNCTION:            FromBGRtoHSVColor
// ARGUMENTS:           SrcColor
//                      DstColor
//                      SrcRangeMin
//                      SrcRange
//                      SrcImageDepth
//                      DstRangeMin
//                      DstRange
//                      DstImageDept
// TEMPLATE ARGUMENTS:  None
// PURPOSE:             Convert a color from the BGR space to
//                      the HSV space
// DEPENDENCIES:        blColor3
//-------------------------------------------------------------------
inline void FromBGRtoHSVColor(blColor3<double> SrcColor,
                              blColor3<double>& DstColor,
                              const double& SrcRangeMin,
                              const double& SrcRange,
                              const int& SrcImageDepth,
                              const double& DstRangeMin,
                              const double& DstRange,
                              const int& DstImageDepth)
{
    // Map the color to [0,1]
    SrcColor.m_Blue += SrcRangeMin;
    SrcColor.m_Green += SrcRangeMin;
    SrcColor.m_Red += SrcRangeMin;
    SrcColor /= SrcRange;

    // Calculate the V value
    double V = std::max(std::max(SrcColor.m_Blue,SrcColor.m_Green),SrcColor.m_Red);

    // Calculate the min value
    double Min = std::min(std::min(SrcColor.m_Blue,SrcColor.m_Green),SrcColor.m_Red);

    // Calculate the Chroma Value
    double Chroma = V - Min;

    // Calculate the Saturation value
    double Sat = 0;
    if(Chroma != 0)
        Sat = Chroma/V;

    // Calculate the Hue Value
    double Hue = 0;
    if(Chroma != 0)
    {
        if(V == SrcColor.m_Red)
        {
            Hue = 60.0*(SrcColor.m_Green - SrcColor.m_Blue)/Chroma;

            // In this case Hue could be negative, so we
            // add 6 to it
            if(Hue < 0)
                Hue += 360.0;
        }

        else if(V == SrcColor.m_Green)
            Hue = 120.0 + 60.0*(SrcColor.m_Blue - SrcColor.m_Red)/Chroma;

        else
            Hue = 240 + 60.0*(SrcColor.m_Red - SrcColor.m_Green)/Chroma;
    }

    // Now that we know the HSV values we
    // save them into the destination image
    // being careful about the depth of the
    // destination image

    // H is stored as Blue
    // S is stored as Green
    // V is stored as Red

    // The H value will be stored differently
    // depending on the depth of the image

    // We do not account for unsigned char nor
    // for float as those will be taken care
    // of in template specializations

    switch(DstImageDepth)
    {
    case IPL_DEPTH_8U:
        Hue /= 2.0;
        break;

    case IPL_DEPTH_8S:
        Hue -= Hue/2.0;
        Hue /= 2.0;
        break;

    case IPL_DEPTH_16S:
        Hue -= Hue/2.0;
        break;

    case IPL_DEPTH_32S:
        Hue -= Hue/2.0;
        break;
    }

    // We have to modify S and V due to depth
    Sat = Sat*DstRange + DstRangeMin;
    V = V*DstRange + DstRangeMin;

    // Now we create the HSV color
    DstColor.m_Blue = Hue;
    DstColor.m_Green = Sat;
    DstColor.m_Red = V;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            FromHSVtoBGRColor
// ARGUMENTS:           SrcColor
//                      DstColor
//                      SrcRangeMin
//                      SrcRange
//                      SrcImageDepth
//                      DstRangeMin
//                      DstRange
//                      DstImageDept
// TEMPLATE ARGUMENTS:  None
// PURPOSE:             Convert a color from the HSV space to
//                      the BGR space
// DEPENDENCIES:        blColor3
//-------------------------------------------------------------------
inline void FromHSVtoBGRColor(blColor3<double> SrcColor,
                              blColor3<double>& DstColor,
                              const double& SrcRangeMin,
                              const double& SrcRange,
                              const int& SrcImageDepth,
                              const double& DstRangeMin,
                              const double& DstRange,
                              const int& DstImageDepth)
{
    // The H S V values are stored in
    // SrcColor as Blue,Green and Red
    // respectively
    double Hue = SrcColor.m_Blue;
    double Sat = SrcColor.m_Green;
    double V = SrcColor.m_Red;

    // Remap S and V back to [0,1]
    Sat -= SrcRangeMin;
    Sat /= SrcRange;
    V -= SrcRangeMin;
    V /= SrcRange;

    // Remap Hue back to [0,6]
    switch(SrcImageDepth)
    {
    case IPL_DEPTH_8U:
        Hue *= 2.0;
        break;

    case IPL_DEPTH_8S:
        Hue *= 2.0;
        Hue += Hue/2.0;
        break;

    case IPL_DEPTH_16S:
        Hue += Hue/2.0;
        break;

    case IPL_DEPTH_32S:
        Hue += Hue/2.0;
        break;
    }

    Hue /= 60.0;

    // Calculate the integral
    // and fractional part of Hue
    int Integral = int(Hue);

    double Fractional = Hue - Integral;

    // Calculate the Colors
    double p = V*(1.0 - Sat);
    double q = V*(1.0 - Sat*Fractional);
    double t = V*(1.0 - (Sat*(1.0 - Fractional)));

    // Depending on the integral
    // part of the Hue, we assign
    // the colors differently
    switch(Integral)
    {
    case 0:
        DstColor.m_Blue = p;
        DstColor.m_Green = t;
        DstColor.m_Red = V;
        break;

    case 1:
        DstColor.m_Blue = p;
        DstColor.m_Green = V;
        DstColor.m_Red = q;
        break;

    case 2:
        DstColor.m_Blue = t;
        DstColor.m_Green = V;
        DstColor.m_Red = p;
        break;

    case 3:
        DstColor.m_Blue = V;
        DstColor.m_Green = q;
        DstColor.m_Red = p;
        break;

    case 4:
        DstColor.m_Blue = V;
        DstColor.m_Green = p;
        DstColor.m_Red = t;
        break;

    default:
        DstColor.m_Blue = q;
        DstColor.m_Green = p;
        DstColor.m_Red = V;
        break;
    }

    // We have to remap the color
    // to its correct depth
    DstColor.m_Blue = DstColor.m_Blue*DstRange + DstRangeMin;
    DstColor.m_Green = DstColor.m_Green*DstRange + DstRangeMin;
    DstColor.m_Red = DstColor.m_Red*DstRange + DstRangeMin;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            FromBGRtoHSV
// ARGUMENTS:           SrcImage,DstImage
// TEMPLATE ARGUMENTS:  blType,blType2
// PURPOSE:             Convert a generic three-channel BGR color
//                      image into a three-channel HSV color image
// DEPENDENCIES:        blColor3
//                      blImage
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void FromBGRtoHSV(const blImage< blColor3<blType> >& SrcImage,
                         blImage< blColor3<blType2> >& DstImage)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the destination image
    // is the correct size
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Get the min and max possible
    // values for the source image
    double SrcRangeMin = SrcImage.GetRangeMin();
    double SrcRangeMax = SrcImage.GetRangeMax();
    double SrcRange = SrcRangeMax - SrcRangeMin;
    int SrcImageDepth = SrcImage.GetDepth();

    // Get the min and max possible
    // values for the destination image
    double DstRangeMin = DstImage.GetRangeMin();
    double DstRangeMax = DstImage.GetRangeMax();
    double DstRange = DstRangeMax - SrcRangeMin;
    int DstImageDepth = DstImage.GetDepth();

    // Temporary color variable
    blColor3<double> SrcColor;
    blColor3<double> DstColor;
    for(int i = 0; i < Rows; ++i)
    {
        for(int j = 0; j < Cols; ++j)
        {
            SrcColor = SrcImage[i][j];
            FromBGRtoHSVColor(SrcColor,
                              DstColor,
                              SrcRangeMin,
                              SrcRange,
                              SrcImageDepth,
                              DstRangeMin,
                              DstRange,
                              DstImageDepth);
            DstImage[i][j] = DstColor;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Template specialization for unsigned char images
// -- Since cvCvtColor is faster for unsigned char images
//-------------------------------------------------------------------
template<>
inline void FromBGRtoHSV(const blImage< blColor3<unsigned char> >& SrcImage,
                         blImage< blColor3<unsigned char> >& DstImage)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the destination image
    // is the correct size
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Now that we made sure that the images
    // are of the same size, we just use
    // the cvCvtColor function
    cvCvtColor(SrcImage,DstImage,CV_BGR2HSV);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            FromHSVtoBGR
// ARGUMENTS:           SrcImage,DstImage
// TEMPLATE ARGUMENTS:  blType,blType2
// PURPOSE:             Convert a generic three-channel HSV color
//                      image back to a three-channel BGR color image
// DEPENDENCIES:        blColor3
//                      blImage
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void FromHSVtoBGR(const blImage< blColor3<blType> >& SrcImage,
                         blImage< blColor3<blType2> >& DstImage)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the destination image
    // is the correct size
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Get the min and max possible
    // values for the source image
    double SrcRangeMin = SrcImage.GetRangeMin();
    double SrcRangeMax = SrcImage.GetRangeMax();
    double SrcRange = SrcRangeMax - SrcRangeMin;
    int SrcImageDepth = SrcImage.GetDepth();

    // Get the min and max possible
    // values for the destination image
    double DstRangeMin = DstImage.GetRangeMin();
    double DstRangeMax = DstImage.GetRangeMax();
    double DstRange = DstRangeMax - SrcRangeMin;
    int DstImageDepth = DstImage.GetDepth();

    // Temporary color variable
    blColor3<double> SrcColor;
    blColor3<double> DstColor;

    for(int i = 0; i < Rows; ++i)
    {
        for(int j = 0; j < Cols; ++j)
        {
            SrcColor = SrcImage[i][j];
            FromHSVtoBGRColor(SrcColor,
                              DstColor,
                              SrcRangeMin,
                              SrcRange,
                              SrcImageDepth,
                              DstRangeMin,
                              DstRange,
                              DstImageDepth);
            DstImage[i][j] = DstColor;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Template specialization for unsigned char images
// -- Since cvCvtColor is faster for unsigned char images
//-------------------------------------------------------------------
template<>
inline void FromHSVtoBGR(const blImage< blColor3<unsigned char> >& SrcImage,
                         blImage< blColor3<unsigned char> >& DstImage)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the destination image
    // is the correct size
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Now that we made sure that the images
    // are of the same size, we just use
    // the cvCvtColor function
    cvCvtColor(SrcImage,DstImage,CV_HSV2BGR);
}
//-------------------------------------------------------------------


#endif // BL_HSV_HPP
