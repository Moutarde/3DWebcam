#ifndef BL_SIGNALPROCESSING_HPP
#define BL_SIGNALPROCESSING_HPP


//-------------------------------------------------------------------
// FILE:            blSignalProcessing.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of algorithms I wrote to use
//                  the blImageAPI for signal processing
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:
//
// NOTES:
//
// DATE CREATED:    Dec/30/2010
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
// FUNCTION:            fft2
// ARGUMENTS:           SrcImage
//                      DstImage
//                      ShouldfftBeDoneOnIndividualRows
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Takes the forward fast fourier transform
//                      of an image and stores it into a destination
//                      image of different type
//                      The third parameter is to choose whether
//                      to perform a 2d fft or an fft on individual
//                      rows
// DEPENDENCIES:        blImage
//                      std::complex
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void fft2(const blImage<blType>& SrcImage,
                 blImage< std::complex<blType2> >& DstImage,
                 const bool& ShouldfftBeDoneOnIndividualRows = false)
{
    // Turn the source image into
    // a complex image of the same
    // type as the destination image
    blImage< std::complex<blType2> > SourceImage = SrcImage;

    // Let's make sure that the
    // destination image is of
    // the correct size
    if(DstImage.size1() != SrcImage.size1() ||
       DstImage.size2() != SrcImage.size2())
    {
        DstImage.CreateImage(SrcImage.size1(),SrcImage.size2());
    }

    // Compute the fft
    if(ShouldfftBeDoneOnIndividualRows)
        cvDFT(SourceImage,DstImage,CV_DXT_FORWARD|CV_DXT_ROWS);
    else
        cvDFT(SourceImage,DstImage,CV_DXT_FORWARD);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            fft2
// ARGUMENTS:           SrcImage
//                      DstImage
//                      ShouldfftBeDoneOnIndividualRows
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Special case of when the source is
//                      the same type as the destination
//                      image, in which case we do not need
//                      to convert the source image (we save a step)
//                      The third parameter is to choose whether
//                      to perform a 2d fft or an fft on individual
//                      rows
// DEPENDENCIES:        blImage
//                      std::complex
//-------------------------------------------------------------------
template<typename blType>
inline void fft2(const blImage<blType>& SrcImage,
                 blImage< std::complex<blType> >& DstImage,
                 const bool& ShouldfftBeDoneOnIndividualRows = false)
{
    // Let's make sure that the
    // destination image is of
    // the correct size
    if(DstImage.size1() != SrcImage.size1() ||
       DstImage.size2() != SrcImage.size2())
    {
        DstImage.CreateImage(SrcImage.size1(),SrcImage.size2());
    }

    // Compute the fft
    if(ShouldfftBeDoneOnIndividualRows)
        cvDFT(SrcImage,DstImage,CV_DXT_FORWARD|CV_DXT_ROWS);
    else
        cvDFT(SrcImage,DstImage,CV_DXT_FORWARD);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            ifft2
// ARGUMENTS:           SrcImage
//                      DstImage
//                      ShouldfftBeDoneOnIndividualRows
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Takes the inverse fast fourier transform
//                      of an image and stores it into a destination
//                      image of different type.
//                      The third parameter is to choose whether
//                      to perform a 2d ifft or an ifft on individual
//                      rows
// DEPENDENCIES:        blImage
//                      std::complex
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void ifft2(const blImage< std::complex<blType> >& SrcImage,
                  blImage<blType2>& DstImage,
                  const bool& ShouldifftBeDoneOnIndividualRows = false)
{
    // Turn the source image into
    // a complex image of the same
    // type as the destination image
    blImage< std::complex<blType2> > SourceImage = SrcImage;

    // Let's make sure that the
    // destination image is of
    // the correct size
    if(DstImage.size1() != SrcImage.size1() ||
       DstImage.size2() != SrcImage.size2())
    {
        DstImage.CreateImage(SrcImage.size1(),SrcImage.size2());
    }

    // Compute the inverse fft
    if(ShouldifftBeDoneOnIndividualRows)
        cvDFT(SourceImage,DstImage,CV_DXT_INV_SCALE|CV_DXT_ROWS);
    else
        cvDFT(SourceImage,DstImage,CV_DXT_INV_SCALE);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            ifft2
// ARGUMENTS:           SrcImage
//                      DstImage
//                      ShouldfftBeDoneOnIndividualRows
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Special case of when the source is
//                      the same type as the destination
//                      image, in which case we do not need
//                      to convert the source image (we save a step)
//                      The third parameter is to choose whether
//                      to perform a 2d ifft or an ifft on individual
//                      rows
// DEPENDENCIES:        blImage
//                      std::complex
//-------------------------------------------------------------------
template<typename blType>
inline void ifft2(const blImage< std::complex<blType> >& SrcImage,
                  blImage<blType>& DstImage,
                  const bool& ShouldifftBeDoneOnIndividualRows = false)
{
    // Let's make sure that the
    // destination image is of
    // the correct size
    if(DstImage.size1() != SrcImage.size1() ||
       DstImage.size2() != SrcImage.size2())
    {
        DstImage.CreateImage(SrcImage.size1(),SrcImage.size2());
    }

    // Compute the inverse fft
    if(ShouldifftBeDoneOnIndividualRows)
        cvDFT(SrcImage,DstImage,CV_DXT_INV_SCALE|CV_DXT_ROWS);
    else
        cvDFT(SrcImage,DstImage,CV_DXT_INV_SCALE);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            fftPowerSpectrum
// ARGUMENTS:           SrcImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Calculates the power spectrum of
//                      a fast fourier transform image of one
//                      type and saves it as a destination
//                      image of another type
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void fftPowerSpectrum(const blImage< std::complex<blType> >& SrcImage,
                             blImage<blType2>& DstImage)
{
    // Separate the complex source
    // image into its individual
    // channels (Real and Imaginary)
    blImage<blType> RealChannel;
    blImage<blType> ImgChannel;
    QueryChannels(SrcImage,RealChannel,ImgChannel);

    // We convert the real and
    // imaginary channels to
    // double first
    // (This is so that we can
    // square the channels without
    // saturating the numbers due
    // to their depth being too small)
    blImage<double> RealChannelDouble = RealChannel;
    blImage<double> ImgChannelDouble = ImgChannel;

    // Compute the magnitude of the spectrum
    cvPow(RealChannelDouble,RealChannelDouble,2.0);
    cvPow(ImgChannelDouble,ImgChannelDouble,2.0);
    cvAdd(RealChannelDouble,ImgChannelDouble,RealChannelDouble);
    cvPow(RealChannelDouble,RealChannelDouble,0.5);

    // Compute the log(1 + Mag)
    cvAddS(RealChannelDouble,cvScalar(1.0),RealChannelDouble);
    cvLog(RealChannelDouble,RealChannelDouble);

    // Get minimum and maximum values
    double MinValue,MaxValue;
    cvMinMaxLoc(RealChannelDouble,&MinValue,&MaxValue);

    // Normalize the image
    cvConvertScale(RealChannelDouble,
                   RealChannelDouble,
                   1.0/(MaxValue - MinValue),0);

    // Store the image into the Destination Image
    DstImage = RealChannelDouble;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            fftPowerSpectrumAndShift
// ARGUMENTS:           SrcImage,DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Calculates the power spectrum of
//                      a fast fourier transform image of one
//                      type and saves it as a destination
//                      image of another type.
//                      It then shifts the image as to
//                      center the frequencies with the zero
//                      frequencies being placed at the center
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void fftPowerSpectrumAndShift(const blImage< std::complex<blType> >& SrcImage,
                                     blImage<blType2>& DstImage)
{
    fftPowerSpectrum(SrcImage,DstImage);

    // Shift the image to center the
    // zero frequencies to the center
    // of the image
    ShiftImageByMRowsAndNCols(DstImage,
                              DstImage,
                              SrcImage.size1()/2,
                              SrcImage.size2()/2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            SimpleRowLowPassFilter
// ARGUMENTS:           SrcImage
//                      DstImage
//                      SamplingPeriod
//                      CutoffFrequency
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Takes an image of any type and size, and
//                      runs each row through a simple low
//                      pass filter.
//                      It saves the filtered image into DstImage
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType>
inline void SimpleRowLowPassFilter(const blImage<blType>& SrcImage,
                                   blImage<blType>& DstImage,
                                   const double& SamplingPeriod,
                                   const double& CutoffFrequency)
{
    // Get the sizes of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the source
    // image is not zero sized
    if(Rows <= 0 || Cols <= 0)
    {
        // Error -- Tried to row filter
        //          an image of size zero
        return;
    }

    // Let's make sure that the destination
    // image is the same size
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Calculate the RC constant
    double RCconstant = blType(1)/(blType(2.0*PI)*CutoffFrequency);

    // Calculate alpha
    double Alpha = SamplingPeriod/(RCconstant + SamplingPeriod);

    // Set the filtered initial
    // values to zero
    for(int i = 0; i < Rows; ++i)
        DstImage[i][0] = SrcImage[i][0];

    // Now let's filter
    // the source image
    for(int i = 0; i < Rows; ++i)
        for(int j = 1; j < Cols; ++j)
            DstImage[i][j] = DstImage[i][j-1] + Alpha*(SrcImage[i][j] - DstImage[i][j-1]);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            SimpleRowHighPassFilter
// ARGUMENTS:           SrcImage
//                      DstImage
//                      SamplingPeriod
//                      CutoffFrequency
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Takes an image of any type and size, and
//                      runs each row through a simple high
//                      pass filter.
//                      It saves the filtered image into DstImage
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType>
inline void SimpleRowHighPassFilter(const blImage<blType>& SrcImage,
                                    blImage<blType>& DstImage,
                                    const double& SamplingPeriod,
                                    const double& CutoffFrequency)
{
    // Get the sizes of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the source
    // image is not zero sized
    if(Rows <= 0 || Cols <= 0)
    {
        // Error -- Tried to row filter
        //          an image of size zero
        return;
    }

    // Let's make sure that the destination
    // image is the same size
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Calculate the RC constant
    blType RCconstant = blType(1)/(blType(2.0*PI)*CutoffFrequency);

    // Calculate alpha
    blType Alpha = SamplingPeriod/(RCconstant + SamplingPeriod);

    // Set the initial values
    for(int i = 0; i < Rows; ++i)
        DstImage[i][0] = SrcImage[i][0];

    // Now let's filter
    // the source image
    for(int i = 0; i < Rows; ++i)
        for(int j = 1; j < Cols; ++j)
            DstImage[i][j] = Alpha*(DstImage[i][j-1] + SrcImage[i][j] - SrcImage[i][j-1]);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            SimpleColLowPassFilter
// ARGUMENTS:           SrcImage
//                      DstImage
//                      SamplingPeriod
//                      CutoffFrequency
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Takes an image of any type and size, and
//                      runs each column through a simple low
//                      pass filter.
//                      It saves the filtered image into DstImage
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType>
inline void SimpleColLowPassFilter(const blImage<blType>& SrcImage,
                                   blImage<blType>& DstImage,
                                   const double& SamplingPeriod,
                                   const double& CutoffFrequency)
{
    // Get the sizes of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the source
    // image is not zero sized
    if(Rows <= 0 || Cols <= 0)
    {
        // Error -- Tried to row filter
        //          an image of size zero
        return;
    }

    // Let's make sure that the destination
    // image is the same size
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Calculate the RC constant
    double RCconstant = blType(1)/(blType(2.0*PI)*CutoffFrequency);

    // Calculate alpha
    double Alpha = SamplingPeriod/(RCconstant + SamplingPeriod);

    // Set the filtered initial
    // values to zero
    for(int i = 0; i < Cols; ++i)
        DstImage[0][i] = SrcImage[0][i];

    // Now let's filter
    // the source image
    for(int i = 0; i < Cols; ++i)
        for(int j = 1; j < Rows; ++j)
            DstImage[j][i] = DstImage[j-1][i] + Alpha*(SrcImage[j][i] - DstImage[j-1][i]);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            SimpleColHighPassFilter
// ARGUMENTS:           SrcImage
//                      DstImage
//                      SamplingPeriod
//                      CutoffFrequency
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Takes an image of any type and size, and
//                      runs each column through a simple
//                      high pass filter.
//                      It saves the filtered image into DstImage
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType>
inline void SimpleColHighPassFilter(const blImage<blType>& SrcImage,
                                    blImage<blType>& DstImage,
                                    const double& SamplingPeriod,
                                    const double& CutoffFrequency)
{
    // Get the sizes of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the source
    // image is not zero sized
    if(Rows <= 0 || Cols <= 0)
    {
        // Error -- Tried to row filter
        //          an image of size zero
        return;
    }

    // Let's make sure that the destination
    // image is the same size
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Calculate the RC constant
    blType RCconstant = blType(1)/(blType(2.0*PI)*CutoffFrequency);

    // Calculate alpha
    blType Alpha = SamplingPeriod/(RCconstant + SamplingPeriod);

    // Set the initial values
    for(int i = 0; i < Cols; ++i)
        DstImage[0][i] = SrcImage[0][i];

    // Now let's filter
    // the source image
    for(int i = 0; i < Cols; ++i)
        for(int j = 1; j < Rows; ++j)
            DstImage[j][i] = Alpha*(DstImage[j-1][i] + SrcImage[j][i] - SrcImage[j-1][i]);
}
//-------------------------------------------------------------------


#endif // BL_SIGNALPROCESSING_HPP
