#ifndef BL_IMAGEBLENDING_HPP
#define BL_IMAGEBLENDING_HPP


//-------------------------------------------------------------------
// FILE:            blImageBlending.hpp
// CLASS:           None
// BASE CLASS:      None
// PURPOSE:         A collection of algorithms I wrote to blend
//                  two images that emulate adobe photoshop's
//                  blending modes
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:
// NOTES:           - So far the following methods are available:
//                      - BlendMultiply
//                      - BlendScreen
//                      - BlendDarken
//                      - BlendLighten
//                      - BlendDifference
//                      - BlendLinearDodge
//                      - BlendOverlay
//                      - BlendHardLight
//                      - BlendLinearBurn
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
// FUNCTION:            BlendMultiplyPixel
// ARGUMENTS:           BaseImagePixel
//                      TopImagePixel
//                      DstImagePixel
//                      MinValue
//                      MaxValue
//                      Range
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Supporting function for BlendMultiply.  It
//                      blends a pixel from the top image with a
//                      pixel from the base image emulating
//                      photoshop's "Multiply" method
// DEPENDENCIES:        std::complex
//                      blColor3
//                      blColor4
//-------------------------------------------------------------------
template<typename blType>
inline void BlendMultiplyPixel(const blType& BaseImagePixel,
                               const blType& TopImagePixel,
                               blType& DstImagePixel,
                               const double& MinValue,
                               const double& MaxValue,
                               const double& Range)
{
    DstImagePixel = ((double(BaseImagePixel) - MinValue) * (double(TopImagePixel) - MinValue) / Range) + MinValue;
}
template<typename blType>
inline void BlendMultiplyPixel(const std::complex<blType>& BaseImagePixel,
                               const std::complex<blType>& TopImagePixel,
                               std::complex<blType>& DstImagePixel,
                               const double& MinValue,
                               const double& MaxValue,
                               const double& Range)
{
    DstImagePixel = ((std::complex<double>(BaseImagePixel) - std::complex<double>(MinValue)) * (std::complex<double>(TopImagePixel) - std::complex<double>(MinValue)) / Range) + std::complex<double>(MinValue);
}
template<typename blType>
inline void BlendMultiplyPixel(const blColor3<blType>& BaseImagePixel,
                               const blColor3<blType>& TopImagePixel,
                               blColor3<blType>& DstImagePixel,
                               const double& MinValue,
                               const double& MaxValue,
                               const double& Range)
{
    DstImagePixel = ((blColor3<double>(BaseImagePixel) - blColor3<double>(MinValue)) * (blColor3<double>(TopImagePixel) - blColor3<double>(MinValue)) / Range) + blColor3<double>(MinValue);
}
template<typename blType>
inline void BlendMultiplyPixel(const blColor4<blType>& BaseImagePixel,
                               const blColor4<blType>& TopImagePixel,
                               blColor4<blType>& DstImagePixel,
                               const double& MinValue,
                               const double& MaxValue,
                               const double& Range)
{
    DstImagePixel = ((blColor4<double>(BaseImagePixel) - blColor4<double>(MinValue)) * (blColor4<double>(TopImagePixel) - blColor4<double>(MinValue)) / Range) + blColor4<double>(MinValue);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendMultiply
// ARGUMENTS:           BaseImage,TopImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Blends the top image with the base image
//                      emulating photoshop's "Multiply" method
// DEPENDENCIES:        blImage
//                      std::min
//                      std::max
//                      BlendMultiplyPixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendMultiply(const blImage<blType>& BaseImage,
                          const blImage<blType>& TopImage,
                          blImage<blType>& DstImage)
{
    // Get the sizes of the
    // top and base images
    int Rows1 = BaseImage.size1();
    int Cols1 = BaseImage.size2();
    int Rows2 = TopImage.size1();
    int Cols2 = TopImage.size2();

    // In case the base and top images could
    // be of different size, then we pick the
    // base size as the resulting image size

    // Now let's make sure the destination
    // image is the same size as the base image
    if(DstImage.size1() != Rows1 || DstImage.size2() != Cols1)
        DstImage = blImage<blType>(Rows1,Cols1);

    // Since the base and top images could
    // have different sizes, then we pick
    // the smallest size to loop through and
    // apply the blending algorithm
    int RowsMin = std::min(Rows1,Rows2);
    int ColsMin = std::min(Cols1,Cols2);

    // Let's get the min and max values
    // that we can work with due to image
    // depth, and calculate the range
    // (We use double as that covers
    // all the possible cases)
    double MinValue = double(BaseImage.GetRangeMin());
    double MaxValue = double(BaseImage.GetRangeMax());
    double Range = MaxValue - MinValue;

    // Step through and apply the "Multiply" method
    for(int i = 0; i < RowsMin; ++i)
    {
        for(int j = 0; j < ColsMin; ++j)
        {
            // Calculate the value of the
            // destination image pixel
            BlendMultiplyPixel(BaseImage[i][j],
                               TopImage[i][j],
                               DstImage[i][j],
                               MinValue,
                               MaxValue,
                               Range);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendScreenPixel
// ARGUMENTS:           BaseImagePixel
//                      TopImagePixel
//                      DstImagePixel
//                      MinValue
//                      MaxValue
//                      Range
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Supporting function for BlendScreen.  It
//                      blends a pixel from the top image with a
//                      pixel from the base image emulating
//                      photoshop's "Screen" method
// DEPENDENCIES:        std::complex
//                      blColor3
//                      blColor4
//-------------------------------------------------------------------
template<typename blType>
inline void BlendScreenPixel(const blType& BaseImagePixel,
                             const blType& TopImagePixel,
                             blType& DstImagePixel,
                             const double& MinValue,
                             const double& MaxValue,
                             const double& Range)
{
    DstImagePixel = Range - (Range - (double(BaseImagePixel) - MinValue)) * (Range - (double(TopImagePixel) - MinValue)) / Range + MinValue;
}
template<typename blType>
inline void BlendScreenPixel(const std::complex<blType>& BaseImagePixel,
                             const std::complex<blType>& TopImagePixel,
                             std::complex<blType>& DstImagePixel,
                             const double& MinValue,
                             const double& MaxValue,
                             const double& Range)
{
    DstImagePixel = std::complex<double>(Range) - (std::complex<double>(Range) - (std::complex<double>(BaseImagePixel) - std::complex<double>(MinValue))) * (std::complex<double>(Range) - (std::complex<double>(TopImagePixel) - std::complex<double>(MinValue))) / Range + std::complex<double>(MinValue);
}
template<typename blType>
inline void BlendScreenPixel(const blColor3<blType>& BaseImagePixel,
                             const blColor3<blType>& TopImagePixel,
                             blColor3<blType>& DstImagePixel,
                             const double& MinValue,
                             const double& MaxValue,
                             const double& Range)
{
    DstImagePixel = blColor3<double>(Range) - (blColor3<double>(Range) - (blColor3<double>(BaseImagePixel) - blColor3<double>(MinValue))) * (blColor3<double>(Range) - (blColor3<double>(TopImagePixel) - blColor3<double>(MinValue))) / Range + blColor3<double>(MinValue);
}
template<typename blType>
inline void BlendScreenPixel(const blColor4<blType>& BaseImagePixel,
                             const blColor4<blType>& TopImagePixel,
                             blColor4<blType>& DstImagePixel,
                             const double& MinValue,
                             const double& MaxValue,
                             const double& Range)
{
    DstImagePixel = blColor4<double>(Range) - (blColor4<double>(Range) - (blColor4<double>(BaseImagePixel) - blColor4<double>(MinValue))) * (blColor4<double>(Range) - (blColor4<double>(TopImagePixel) - blColor4<double>(MinValue))) / Range + blColor4<double>(MinValue);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendScreen
// ARGUMENTS:           BaseImage,TopImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Blends the top image with the base image
//                      emulating photoshop's "Screen" method
// DEPENDENCIES:        blImage
//                      std::min
//                      std::max
//                      BlendScreenPixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendScreen(const blImage<blType>& BaseImage,
                        const blImage<blType>& TopImage,
                        blImage<blType>& DstImage)
{
    // Get the sizes of the
    // top and base images
    int Rows1 = BaseImage.size1();
    int Cols1 = BaseImage.size2();
    int Rows2 = TopImage.size1();
    int Cols2 = TopImage.size2();

    // In case the base and top images could
    // be of different size, then we pick the
    // base size as the resulting image size

    // Now let's make sure the destination
    // image is the same size as the base image
    if(DstImage.size1() != Rows1 || DstImage.size2() != Cols1)
        DstImage = blImage<blType>(Rows1,Cols1);

    // Since the base and top images could
    // have different sizes, then we pick
    // the smallest size to loop through and
    // apply the blending algorithm
    int RowsMin = std::min(Rows1,Rows2);
    int ColsMin = std::min(Cols1,Cols2);

    // Let's get the min and max values
    // that we can work with due to image
    // depth, and calculate the range
    // (We use double as that covers
    // all the possible cases)
    double MinValue = double(BaseImage.GetRangeMin());
    double MaxValue = double(BaseImage.GetRangeMax());
    double Range = MaxValue - MinValue;

    // Step through and apply the "Screen" method
    for(int i = 0; i < RowsMin; ++i)
    {
        for(int j = 0; j < ColsMin; ++j)
        {
            // Calculate the value of the
            // destination image pixel
            BlendScreenPixel(BaseImage[i][j],
                             TopImage[i][j],
                             DstImage[i][j],
                             MinValue,
                             MaxValue,
                             Range);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendDarkenPixel
// ARGUMENTS:           BaseImagePixel
//                      TopImagePixel
//                      DstImagePixel
//                      MinValue
//                      MaxValue
//                      Range
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Supporting function for BlendDarken.  It
//                      blends a pixel from the top image with a
//                      pixel from the base image emulating
//                      photoshop's "Darken" method
// DEPENDENCIES:        std::complex
//                      blColor3
//                      blColor4
//                      std::min
//-------------------------------------------------------------------
template<typename blType>
inline void BlendDarkenPixel(const blType& BaseImagePixel,
                             const blType& TopImagePixel,
                             blType& DstImagePixel)
{
    DstImagePixel = blType(std::min(BaseImagePixel,TopImagePixel));
}
template<typename blType>
inline void BlendDarkenPixel(const std::complex<blType>& BaseImagePixel,
                             const std::complex<blType>& TopImagePixel,
                             std::complex<blType>& DstImagePixel)
{
    DstImagePixel = std::complex<blType>(std::min(BaseImagePixel.real(),TopImagePixel.real()),
                                         std::min(BaseImagePixel.imag(),TopImagePixel.imag()));
}
template<typename blType>
inline void BlendDarkenPixel(const blColor3<blType>& BaseImagePixel,
                             const blColor3<blType>& TopImagePixel,
                             blColor3<blType>& DstImagePixel)
{
    DstImagePixel = blColor3<blType>(std::min(BaseImagePixel.m_Blue,TopImagePixel.m_Blue),
                                     std::min(BaseImagePixel.m_Green,TopImagePixel.m_Green),
                                     std::min(BaseImagePixel.m_Red,TopImagePixel.m_Red));
}
template<typename blType>
inline void BlendDarkenPixel(const blColor4<blType>& BaseImagePixel,
                             const blColor4<blType>& TopImagePixel,
                             blColor4<blType>& DstImagePixel)
{
    DstImagePixel = blColor4<blType>(std::min(BaseImagePixel.m_Blue,TopImagePixel.m_Blue),
                                     std::min(BaseImagePixel.m_Green,TopImagePixel.m_Green),
                                     std::min(BaseImagePixel.m_Red,TopImagePixel.m_Red),
                                     std::min(BaseImagePixel.m_Alpha,TopImagePixel.m_Alpha));
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendDarken
// ARGUMENTS:           BaseImage,TopImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Blends the top image with the base image
//                      emulating photoshop's "Darken" method
// DEPENDENCIES:        blImage
//                      std::min
//                      std::max
//                      BlendDarkenPixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendDarken(const blImage<blType>& BaseImage,
                        const blImage<blType>& TopImage,
                        blImage<blType>& DstImage)
{
    // Get the sizes of the
    // top and base images
    int Rows1 = BaseImage.size1();
    int Cols1 = BaseImage.size2();
    int Rows2 = TopImage.size1();
    int Cols2 = TopImage.size2();

    // In case the base and top images could
    // be of different size, then we pick the
    // base size as the resulting image size

    // Now let's make sure the destination
    // image is the same size as the base image
    if(DstImage.size1() != Rows1 || DstImage.size2() != Cols1)
        DstImage = blImage<blType>(Rows1,Cols1);

    // Since the base and top images could
    // have different sizes, then we pick
    // the smallest size to loop through and
    // apply the blending algorithm
    int RowsMin = std::min(Rows1,Rows2);
    int ColsMin = std::min(Cols1,Cols2);

    // Step through and apply the "Darken" method
    for(int i = 0; i < RowsMin; ++i)
    {
        for(int j = 0; j < ColsMin; ++j)
        {
            // Calculate the value of the
            // destination image pixel
            BlendDarkenPixel(BaseImage[i][j],
                             TopImage[i][j],
                             DstImage[i][j]);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendLightenPixel
// ARGUMENTS:           BaseImagePixel
//                      TopImagePixel
//                      DstImagePixel
//                      MinValue
//                      MaxValue
//                      Range
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Supporting function for BlendDarken.  It
//                      blends a pixel from the top image with a
//                      pixel from the base image emulating
//                      photoshop's "Lighten" method
// DEPENDENCIES:        std::complex
//                      blColor3
//                      blColor4
//                      std::min
//-------------------------------------------------------------------
template<typename blType>
inline void BlendLightenPixel(const blType& BaseImagePixel,
                              const blType& TopImagePixel,
                              blType& DstImagePixel)
{
    DstImagePixel = blType(std::max(BaseImagePixel,TopImagePixel));
}
template<typename blType>
inline void BlendLightenPixel(const std::complex<blType>& BaseImagePixel,
                              const std::complex<blType>& TopImagePixel,
                              std::complex<blType>& DstImagePixel)
{
    DstImagePixel = std::complex<blType>(std::max(BaseImagePixel.real(),TopImagePixel.real()),
                                         std::max(BaseImagePixel.imag(),TopImagePixel.imag()));
}
template<typename blType>
inline void BlendLightenPixel(const blColor3<blType>& BaseImagePixel,
                              const blColor3<blType>& TopImagePixel,
                              blColor3<blType>& DstImagePixel)
{
    DstImagePixel = blColor3<blType>(std::max(BaseImagePixel.m_Blue,TopImagePixel.m_Blue),
                                     std::max(BaseImagePixel.m_Green,TopImagePixel.m_Green),
                                     std::max(BaseImagePixel.m_Red,TopImagePixel.m_Red));
}
template<typename blType>
inline void BlendLightenPixel(const blColor4<blType>& BaseImagePixel,
                              const blColor4<blType>& TopImagePixel,
                              blColor4<blType>& DstImagePixel)
{
    DstImagePixel = blColor4<blType>(std::max(BaseImagePixel.m_Blue,TopImagePixel.m_Blue),
                                     std::max(BaseImagePixel.m_Green,TopImagePixel.m_Green),
                                     std::max(BaseImagePixel.m_Red,TopImagePixel.m_Red),
                                     std::max(BaseImagePixel.m_Alpha,TopImagePixel.m_Alpha));
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendLighten
// ARGUMENTS:           BaseImage,TopImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Blends the top image with the base image
//                      emulating photoshop's "Lighten" method
// DEPENDENCIES:        blImage
//                      std::min
//                      std::max
//                      BlendLightenPixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendLighten(const blImage<blType>& BaseImage,
                         const blImage<blType>& TopImage,
                         blImage<blType>& DstImage)
{
    // Get the sizes of the
    // top and base images
    int Rows1 = BaseImage.size1();
    int Cols1 = BaseImage.size2();
    int Rows2 = TopImage.size1();
    int Cols2 = TopImage.size2();

    // In case the base and top images could
    // be of different size, then we pick the
    // base size as the resulting image size

    // Now let's make sure the destination
    // image is the same size as the base image
    if(DstImage.size1() != Rows1 || DstImage.size2() != Cols1)
        DstImage = blImage<blType>(Rows1,Cols1);

    // Since the base and top images could
    // have different sizes, then we pick
    // the smallest size to loop through and
    // apply the blending algorithm
    int RowsMin = std::min(Rows1,Rows2);
    int ColsMin = std::min(Cols1,Cols2);

    // Step through and apply the "Lighten" method
    for(int i = 0; i < RowsMin; ++i)
    {
        for(int j = 0; j < ColsMin; ++j)
        {
            // Calculate the value of the
            // destination image pixel
            BlendLightenPixel(BaseImage[i][j],
                              TopImage[i][j],
                              DstImage[i][j]);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendDifferencePixel
// ARGUMENTS:           BaseImagePixel
//                      TopImagePixel
//                      DstImagePixel
//                      MinValue
//                      MaxValue
//                      Range
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Supporting function for BlendDifference.  It
//                      blends a pixel from the top image with a
//                      pixel from the base image emulating
//                      photoshop's "Difference" method
// DEPENDENCIES:        std::complex
//                      blColor3
//                      blColor4
//                      std::abs
//-------------------------------------------------------------------
template<typename blType>
inline void BlendDifferencePixel(const blType& BaseImagePixel,
                                 const blType& TopImagePixel,
                                 blType& DstImagePixel,
                                 const double& MinValue,
                                 const double& MaxValue,
                                 const double& Range)
{
    DstImagePixel = std::abs(BaseImagePixel - TopImagePixel) + MinValue;
}
template<typename blType>
inline void BlendDifferencePixel(const std::complex<blType>& BaseImagePixel,
                                 const std::complex<blType>& TopImagePixel,
                                 std::complex<blType>& DstImagePixel,
                                 const double& MinValue,
                                 const double& MaxValue,
                                 const double& Range)
{
    DstImagePixel = std::complex<blType>(std::abs(BaseImagePixel.real() - TopImagePixel.real()),
                                         std::abs(BaseImagePixel.imag() - TopImagePixel.imag())) +
                    std::complex<blType>(MinValue);
}
template<typename blType>
inline void BlendDifferencePixel(const blColor3<blType>& BaseImagePixel,
                                 const blColor3<blType>& TopImagePixel,
                                 blColor3<blType>& DstImagePixel,
                                 const double& MinValue,
                                 const double& MaxValue,
                                 const double& Range)
{
    DstImagePixel = blColor3<blType>(std::abs(BaseImagePixel.m_Blue - TopImagePixel.m_Blue),
                                     std::abs(BaseImagePixel.m_Green - TopImagePixel.m_Green),
                                     std::abs(BaseImagePixel.m_Red - TopImagePixel.m_Red)) +
                    blColor3<blType>(MinValue);
}
template<typename blType>
inline void BlendDifferencePixel(const blColor4<blType>& BaseImagePixel,
                                 const blColor4<blType>& TopImagePixel,
                                 blColor4<blType>& DstImagePixel,
                                 const double& MinValue,
                                 const double& MaxValue,
                                 const double& Range)
{
    DstImagePixel = blColor4<blType>(std::abs(BaseImagePixel.m_Blue - TopImagePixel.m_Blue),
                                     std::abs(BaseImagePixel.m_Green - TopImagePixel.m_Green),
                                     std::abs(BaseImagePixel.m_Red - TopImagePixel.m_Red),
                                     std::abs(BaseImagePixel.m_Alpha - TopImagePixel.m_Alpha)) +
                    blColor4<blType>(MinValue);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendDifference
// ARGUMENTS:           BaseImage,TopImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Blends the top image with the base image
//                      emulating photoshop's "Difference" method
// DEPENDENCIES:        blImage
//                      std::min
//                      std::max
//                      BlendDifferencePixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendDifference(const blImage<blType>& BaseImage,
                            const blImage<blType>& TopImage,
                            blImage<blType>& DstImage)
{
    // Get the sizes of the
    // top and base images
    int Rows1 = BaseImage.size1();
    int Cols1 = BaseImage.size2();
    int Rows2 = TopImage.size1();
    int Cols2 = TopImage.size2();

    // In case the base and top images could
    // be of different size, then we pick the
    // base size as the resulting image size

    // Now let's make sure the destination
    // image is the same size as the base image
    if(DstImage.size1() != Rows1 || DstImage.size2() != Cols1)
        DstImage = blImage<blType>(Rows1,Cols1);

    // Since the base and top images could
    // have different sizes, then we pick
    // the smallest size to loop through and
    // apply the blending algorithm
    int RowsMin = std::min(Rows1,Rows2);
    int ColsMin = std::min(Cols1,Cols2);

    // Let's get the min and max values
    // that we can work with due to image
    // depth, and calculate the range
    // (We use double as that covers
    // all the possible cases)
    double MinValue = double(BaseImage.GetRangeMin());
    double MaxValue = double(BaseImage.GetRangeMax());
    double Range = MaxValue - MinValue;

    // Step through and apply the "Difference" method
    for(int i = 0; i < RowsMin; ++i)
    {
        for(int j = 0; j < ColsMin; ++j)
        {
            // Calculate the value of the
            // destination image pixel
            BlendDifferencePixel(BaseImage[i][j],
                                 TopImage[i][j],
                                 DstImage[i][j],
                                 MinValue,
                                 MaxValue,
                                 Range);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendLinearDodgePixel
// ARGUMENTS:           BaseImagePixel
//                      TopImagePixel
//                      DstImagePixel
//                      MinValue
//                      MaxValue
//                      Range
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Supporting function for BlendLinearDodge.  It
//                      blends a pixel from the top image with a
//                      pixel from the base image emulating
//                      photoshop's "Linear Dodge" method
// DEPENDENCIES:        std::complex
//                      blColor3
//                      blColor4
//                      std::min
//-------------------------------------------------------------------
template<typename blType>
inline void BlendLinearDodgePixel(const blType& BaseImagePixel,
                                  const blType& TopImagePixel,
                                  blType& DstImagePixel,
                                  const double& MinValue,
                                  const double& MaxValue,
                                  const double& Range)
{
    DstImagePixel = std::min(MaxValue,double(BaseImagePixel) + double(TopImagePixel));
}
template<typename blType>
inline void BlendLinearDodgePixel(const std::complex<blType>& BaseImagePixel,
                                  const std::complex<blType>& TopImagePixel,
                                  std::complex<blType>& DstImagePixel,
                                  const double& MinValue,
                                  const double& MaxValue,
                                  const double& Range)
{
    DstImagePixel = std::complex<blType>(std::min(MaxValue,double(BaseImagePixel.real()) + double(TopImagePixel.real())),
                                         std::min(MaxValue,double(BaseImagePixel.imag()) + double(TopImagePixel.imag())));
}
template<typename blType>
inline void BlendLinearDodgePixel(const blColor3<blType>& BaseImagePixel,
                                  const blColor3<blType>& TopImagePixel,
                                  blColor3<blType>& DstImagePixel,
                                  const double& MinValue,
                                  const double& MaxValue,
                                  const double& Range)
{
    DstImagePixel = blColor3<blType>(std::min(MaxValue,double(BaseImagePixel.m_Blue) + double(TopImagePixel.m_Blue)),
                                     std::min(MaxValue,double(BaseImagePixel.m_Green) + double(TopImagePixel.m_Green)),
                                     std::min(MaxValue,double(BaseImagePixel.m_Red) + double(TopImagePixel.m_Red)));
}
template<typename blType>
inline void BlendLinearDodgePixel(const blColor4<blType>& BaseImagePixel,
                                  const blColor4<blType>& TopImagePixel,
                                  blColor4<blType>& DstImagePixel,
                                  const double& MinValue,
                                  const double& MaxValue,
                                  const double& Range)
{
    DstImagePixel = blColor4<blType>(std::min(MaxValue,double(BaseImagePixel.m_Blue) + double(TopImagePixel.m_Blue)),
                                     std::min(MaxValue,double(BaseImagePixel.m_Green) + double(TopImagePixel.m_Green)),
                                     std::min(MaxValue,double(BaseImagePixel.m_Red) + double(TopImagePixel.m_Red)),
                                     std::min(MaxValue,double(BaseImagePixel.m_Alpha) + double(TopImagePixel.m_Alpha)));
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendLinearDodge
// ARGUMENTS:           BaseImage,TopImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Blends the top image with the base image
//                      emulating photoshop's "Linear Dodge" method
// DEPENDENCIES:        blImage
//                      std::min
//                      std::max
//                      BlendLinearDodgePixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendLinearDodge(const blImage<blType>& BaseImage,
                             const blImage<blType>& TopImage,
                             blImage<blType>& DstImage)
{
    // Get the sizes of the
    // top and base images
    int Rows1 = BaseImage.size1();
    int Cols1 = BaseImage.size2();
    int Rows2 = TopImage.size1();
    int Cols2 = TopImage.size2();

    // In case the base and top images could
    // be of different size, then we pick the
    // base size as the resulting image size

    // Now let's make sure the destination
    // image is the same size as the base image
    if(DstImage.size1() != Rows1 || DstImage.size2() != Cols1)
        DstImage = blImage<blType>(Rows1,Cols1);

    // Since the base and top images could
    // have different sizes, then we pick
    // the smallest size to loop through and
    // apply the blending algorithm
    int RowsMin = std::min(Rows1,Rows2);
    int ColsMin = std::min(Cols1,Cols2);

    // Let's get the min and max values
    // that we can work with due to image
    // depth, and calculate the range
    // (We use double as that covers
    // all the possible cases)
    double MinValue = double(BaseImage.GetRangeMin());
    double MaxValue = double(BaseImage.GetRangeMax());
    double Range = MaxValue - MinValue;

    // Step through and apply the "Linear Dodge" method
    for(int i = 0; i < RowsMin; ++i)
    {
        for(int j = 0; j < ColsMin; ++j)
        {
            // Calculate the value of the
            // destination image pixel
            BlendLinearDodgePixel(BaseImage[i][j],
                                  TopImage[i][j],
                                  DstImage[i][j],
                                  MinValue,
                                  MaxValue,
                                  Range);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendOverlayPixel
// ARGUMENTS:           BaseImagePixel
//                      TopImagePixel
//                      DstImagePixel
//                      MinValue
//                      MaxValue
//                      Range
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Supporting function for BlendLinearDodge.  It
//                      blends a pixel from the top image with a
//                      pixel from the base image emulating
//                      photoshop's "Overlay" method
// DEPENDENCIES:        BlendMultiplyPixel
//                      BlendScreenPixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendOverlayPixel(const blType& BaseImagePixel,
                              const blType& TopImagePixel,
                              blType& DstImagePixel,
                              const double& MinValue,
                              const double& MaxValue,
                              const double& Range)
{
    if(double(BaseImagePixel) < (MinValue + MaxValue)/2.0)
        DstImagePixel = 2.0*((double(BaseImagePixel) - MinValue) * (double(TopImagePixel) - MinValue) / Range) + MinValue;
    else
        DstImagePixel = Range - 2.0*(Range - (double(BaseImagePixel) - MinValue)) * (Range - (double(TopImagePixel) - MinValue)) / Range + MinValue;
}
template<typename blType>
inline void BlendOverlayPixel(const std::complex<blType>& BaseImagePixel,
                              const std::complex<blType>& TopImagePixel,
                              std::complex<blType>& DstImagePixel,
                              const double& MinValue,
                              const double& MaxValue,
                              const double& Range)
{
    // Calculate the real part
    if(double(BaseImagePixel.real()) < (MinValue + MaxValue)/2.0)
        DstImagePixel.real() = 2.0*((double(BaseImagePixel.real()) - MinValue) * (double(TopImagePixel.real()) - MinValue) / Range) + MinValue;
    else
        DstImagePixel.real() = Range - 2.0*(Range - (double(BaseImagePixel.real()) - MinValue)) * (Range - (double(TopImagePixel) - MinValue)) / Range + MinValue;

    // Calculate the imaginary part
    if(double(BaseImagePixel.imag()) < (MinValue + MaxValue)/2.0)
        DstImagePixel.imag() = 2.0*((double(BaseImagePixel.imag()) - MinValue) * (double(TopImagePixel.imag()) - MinValue) / Range) + MinValue;
    else
        DstImagePixel.imag() = Range - 2.0*(Range - (double(BaseImagePixel.imag()) - MinValue)) * (Range - (double(TopImagePixel.imag()) - MinValue)) / Range + MinValue;
}
template<typename blType>
inline void BlendOverlayPixel(const blColor3<blType>& BaseImagePixel,
                              const blColor3<blType>& TopImagePixel,
                              blColor3<blType>& DstImagePixel,
                              const double& MinValue,
                              const double& MaxValue,
                              const double& Range)
{
    // Calculate the blue component
    if(double(BaseImagePixel.m_Blue) < (MinValue + MaxValue)/2.0)
        DstImagePixel.m_Blue = 2.0*((double(BaseImagePixel.m_Blue) - MinValue) * (double(TopImagePixel.m_Blue) - MinValue) / Range) + MinValue;
    else
        DstImagePixel.m_Blue = Range - 2.0*(Range - (double(BaseImagePixel.m_Blue) - MinValue)) * (Range - (double(TopImagePixel.m_Blue) - MinValue)) / Range + MinValue;

    // Calculate the green component
    if(double(BaseImagePixel.m_Green) < (MinValue + MaxValue)/2.0)
        DstImagePixel.m_Green = 2.0*((double(BaseImagePixel.m_Green) - MinValue) * (double(TopImagePixel.m_Green) - MinValue) / Range) + MinValue;
    else
        DstImagePixel.m_Green = Range - 2.0*(Range - (double(BaseImagePixel.m_Green) - MinValue)) * (Range - (double(TopImagePixel.m_Green) - MinValue)) / Range + MinValue;

    // Calculate the blue component
    if(double(BaseImagePixel.m_Red) < (MinValue + MaxValue)/2.0)
        DstImagePixel.m_Red = 2.0*((double(BaseImagePixel.m_Red) - MinValue) * (double(TopImagePixel.m_Red) - MinValue) / Range) + MinValue;
    else
        DstImagePixel.m_Red = Range - 2.0*(Range - (double(BaseImagePixel.m_Red) - MinValue)) * (Range - (double(TopImagePixel.m_Red) - MinValue)) / Range + MinValue;
}
template<typename blType>
inline void BlendOverlayPixel(const blColor4<blType>& BaseImagePixel,
                              const blColor4<blType>& TopImagePixel,
                              blColor4<blType>& DstImagePixel,
                              const double& MinValue,
                              const double& MaxValue,
                              const double& Range)
{
    // Calculate the blue component
    if(double(BaseImagePixel.m_Blue) < (MinValue + MaxValue)/2.0)
        DstImagePixel.m_Blue = 2.0*((double(BaseImagePixel.m_Blue) - MinValue) * (double(TopImagePixel.m_Blue) - MinValue) / Range) + MinValue;
    else
        DstImagePixel.m_Blue = Range - 2.0*(Range - (double(BaseImagePixel.m_Blue) - MinValue)) * (Range - (double(TopImagePixel.m_Blue) - MinValue)) / Range + MinValue;

    // Calculate the green component
    if(double(BaseImagePixel.m_Green) < (MinValue + MaxValue)/2.0)
        DstImagePixel.m_Green = 2.0*((double(BaseImagePixel.m_Green) - MinValue) * (double(TopImagePixel.m_Green) - MinValue) / Range) + MinValue;
    else
        DstImagePixel.m_Green = Range - 2.0*(Range - (double(BaseImagePixel.m_Green) - MinValue)) * (Range - (double(TopImagePixel.m_Green) - MinValue)) / Range + MinValue;

    // Calculate the blue component
    if(double(BaseImagePixel.m_Red) < (MinValue + MaxValue)/2.0)
        DstImagePixel.m_Red = 2.0*((double(BaseImagePixel.m_Red) - MinValue) * (double(TopImagePixel.m_Red) - MinValue) / Range) + MinValue;
    else
        DstImagePixel.m_Red = Range - 2.0*(Range - (double(BaseImagePixel.m_Red) - MinValue)) * (Range - (double(TopImagePixel.m_Red) - MinValue)) / Range + MinValue;

    // Calculate the blue component
    if(double(BaseImagePixel.m_Alpha) < (MinValue + MaxValue)/2.0)
        DstImagePixel.m_Alpha = 2.0*((double(BaseImagePixel.m_Alpha) - MinValue) * (double(TopImagePixel.m_Alpha) - MinValue) / Range) + MinValue;
    else
        DstImagePixel.m_Alpha = Range - 2.0*(Range - (double(BaseImagePixel.m_Alpha) - MinValue)) * (Range - (double(TopImagePixel.m_Alpha) - MinValue)) / Range + MinValue;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendOverlay
// ARGUMENTS:           BaseImage,TopImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Blends the top image with the base image
//                      emulating photoshop's "Overlay" method
// DEPENDENCIES:        blImage
//                      std::min
//                      std::max
//                      BlendLinearDodgePixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendOverlay(const blImage<blType>& BaseImage,
                         const blImage<blType>& TopImage,
                         blImage<blType>& DstImage)
{
    // Get the sizes of the
    // top and base images
    int Rows1 = BaseImage.size1();
    int Cols1 = BaseImage.size2();
    int Rows2 = TopImage.size1();
    int Cols2 = TopImage.size2();

    // In case the base and top images could
    // be of different size, then we pick the
    // base size as the resulting image size

    // Now let's make sure the destination
    // image is the same size as the base image
    if(DstImage.size1() != Rows1 || DstImage.size2() != Cols1)
        DstImage = blImage<blType>(Rows1,Cols1);

    // Since the base and top images could
    // have different sizes, then we pick
    // the smallest size to loop through and
    // apply the blending algorithm
    int RowsMin = std::min(Rows1,Rows2);
    int ColsMin = std::min(Cols1,Cols2);

    // Let's get the min and max values
    // that we can work with due to image
    // depth, and calculate the range
    // (We use double as that covers
    // all the possible cases)
    double MinValue = double(BaseImage.GetRangeMin());
    double MaxValue = double(BaseImage.GetRangeMax());
    double Range = MaxValue - MinValue;

    // Step through and apply the "Overlay" method
    for(int i = 0; i < RowsMin; ++i)
    {
        for(int j = 0; j < ColsMin; ++j)
        {
            // Calculate the value of the
            // destination image pixel
            BlendOverlayPixel(BaseImage[i][j],
                              TopImage[i][j],
                              DstImage[i][j],
                              MinValue,
                              MaxValue,
                              Range);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendHardLight
// ARGUMENTS:           BaseImage,TopImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Blends the top image with the base image
//                      emulating photoshop's "Hard Light" method
// DEPENDENCIES:        blImage
//                      std::min
//                      std::max
//                      BlendOverlayPixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendHardLight(const blImage<blType>& BaseImage,
                           const blImage<blType>& TopImage,
                           blImage<blType>& DstImage)
{
    // Get the sizes of the
    // top and base images
    int Rows1 = BaseImage.size1();
    int Cols1 = BaseImage.size2();
    int Rows2 = TopImage.size1();
    int Cols2 = TopImage.size2();

    // In case the base and top images could
    // be of different size, then we pick the
    // base size as the resulting image size

    // Now let's make sure the destination
    // image is the same size as the base image
    if(DstImage.size1() != Rows1 || DstImage.size2() != Cols1)
        DstImage = blImage<blType>(Rows1,Cols1);

    // Since the base and top images could
    // have different sizes, then we pick
    // the smallest size to loop through and
    // apply the blending algorithm
    int RowsMin = std::min(Rows1,Rows2);
    int ColsMin = std::min(Cols1,Cols2);

    // Let's get the min and max values
    // that we can work with due to image
    // depth, and calculate the range
    // (We use double as that covers
    // all the possible cases)
    double MinValue = double(BaseImage.GetRangeMin());
    double MaxValue = double(BaseImage.GetRangeMax());
    double Range = MaxValue - MinValue;

    // Step through and apply the "Overlay" method
    for(int i = 0; i < RowsMin; ++i)
    {
        for(int j = 0; j < ColsMin; ++j)
        {
            // Calculate the value of the
            // destination image pixel
            BlendOverlayPixel(TopImage[i][j],
                              BaseImage[i][j],
                              DstImage[i][j],
                              MinValue,
                              MaxValue,
                              Range);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendLinearBurnPixel
// ARGUMENTS:           BaseImagePixel
//                      TopImagePixel
//                      DstImagePixel
//                      MinValue
//                      MaxValue
//                      Range
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Supporting function for BlendLinearBurn.  It
//                      blends a pixel from the top image with a
//                      pixel from the base image emulating
//                      photoshop's "Linear Burn" method
// DEPENDENCIES:        std::complex
//                      blColor3
//                      blColor4
//                      std::max
//-------------------------------------------------------------------
template<typename blType>
inline void BlendLinearBurnPixel(const blType& BaseImagePixel,
                                 const blType& TopImagePixel,
                                 blType& DstImagePixel,
                                 const double& MinValue,
                                 const double& MaxValue,
                                 const double& Range)
{
    DstImagePixel = std::max(MinValue,double(BaseImagePixel) + double(TopImagePixel) - MaxValue);
}
template<typename blType>
inline void BlendLinearBurnPixel(const std::complex<blType>& BaseImagePixel,
                                  const std::complex<blType>& TopImagePixel,
                                  std::complex<blType>& DstImagePixel,
                                  const double& MinValue,
                                  const double& MaxValue,
                                  const double& Range)
{
    DstImagePixel = std::complex<blType>(std::max(MinValue,double(BaseImagePixel.real()) + double(TopImagePixel.real()) - MaxValue),
                                         std::max(MinValue,double(BaseImagePixel.imag()) + double(TopImagePixel.imag()) - MaxValue));
}
template<typename blType>
inline void BlendLinearBurnPixel(const blColor3<blType>& BaseImagePixel,
                                  const blColor3<blType>& TopImagePixel,
                                  blColor3<blType>& DstImagePixel,
                                  const double& MinValue,
                                  const double& MaxValue,
                                  const double& Range)
{
    DstImagePixel = blColor3<blType>(std::max(MinValue,double(BaseImagePixel.m_Blue) + double(TopImagePixel.m_Blue) - MaxValue),
                                     std::max(MinValue,double(BaseImagePixel.m_Green) + double(TopImagePixel.m_Green) - MaxValue),
                                     std::max(MinValue,double(BaseImagePixel.m_Red) + double(TopImagePixel.m_Red) - MaxValue));
}
template<typename blType>
inline void BlendLinearBurnPixel(const blColor4<blType>& BaseImagePixel,
                                  const blColor4<blType>& TopImagePixel,
                                  blColor4<blType>& DstImagePixel,
                                  const double& MinValue,
                                  const double& MaxValue,
                                  const double& Range)
{
    DstImagePixel = blColor4<blType>(std::max(MinValue,double(BaseImagePixel.m_Blue) + double(TopImagePixel.m_Blue) - MaxValue),
                                     std::max(MinValue,double(BaseImagePixel.m_Green) + double(TopImagePixel.m_Green) - MaxValue),
                                     std::max(MinValue,double(BaseImagePixel.m_Red) + double(TopImagePixel.m_Red) - MaxValue),
                                     std::max(MinValue,double(BaseImagePixel.m_Alpha) + double(TopImagePixel.m_Alpha) - MaxValue));
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            BlendLinearBurn
// ARGUMENTS:           BaseImage,TopImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Blends the top image with the base image
//                      emulating photoshop's "Linear Burn" method
// DEPENDENCIES:        blImage
//                      std::min
//                      std::max
//                      BlendLinearDodgePixel
//-------------------------------------------------------------------
template<typename blType>
inline void BlendLinearBurn(const blImage<blType>& BaseImage,
                            const blImage<blType>& TopImage,
                            blImage<blType>& DstImage)
{
    // Get the sizes of the
    // top and base images
    int Rows1 = BaseImage.size1();
    int Cols1 = BaseImage.size2();
    int Rows2 = TopImage.size1();
    int Cols2 = TopImage.size2();

    // In case the base and top images could
    // be of different size, then we pick the
    // base size as the resulting image size

    // Now let's make sure the destination
    // image is the same size as the base image
    if(DstImage.size1() != Rows1 || DstImage.size2() != Cols1)
        DstImage = blImage<blType>(Rows1,Cols1);

    // Since the base and top images could
    // have different sizes, then we pick
    // the smallest size to loop through and
    // apply the blending algorithm
    int RowsMin = std::min(Rows1,Rows2);
    int ColsMin = std::min(Cols1,Cols2);

    // Let's get the min and max values
    // that we can work with due to image
    // depth, and calculate the range
    // (We use double as that covers
    // all the possible cases)
    double MinValue = double(BaseImage.GetRangeMin());
    double MaxValue = double(BaseImage.GetRangeMax());
    double Range = MaxValue - MinValue;

    // Step through and apply the "Linear Burn" method
    for(int i = 0; i < RowsMin; ++i)
    {
        for(int j = 0; j < ColsMin; ++j)
        {
            // Calculate the value of the
            // destination image pixel
            BlendLinearBurnPixel(BaseImage[i][j],
                                 TopImage[i][j],
                                 DstImage[i][j],
                                 MinValue,
                                 MaxValue,
                                 Range);
        }
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGEBLENDING_HPP
