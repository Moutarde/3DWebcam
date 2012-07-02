#ifndef BL_IMAGECHANNELS_HPP
#define BL_IMAGECHANNELS_HPP


//-------------------------------------------------------------------
// FILE:            blImageChannels.hpp
// CLASS:           None
// BASE CLASS:      None
// PURPOSE:         A collection of simple functions I created
//                  to play with image channels
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    blImage -- The image structure
//                  cvSplit -- To split an image into its channels
//                  cvMerge -- To combine channels into an image
// NOTES:
// DATE CREATED:    Jun/03/2010
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
// FUNCTION:            QueryChannel
// ARGUMENTS:           SrcImage,DstImageChannel,WhichChannel
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Grab a single channel from a complex image
// DEPENDENCIES:        blImage
//                      cvSplit
//                      std::complex
//-------------------------------------------------------------------
template<typename blType>
inline void QueryChannel(const blImage< std::complex<blType> >& SrcImage,
                         blImage<blType>& DstImageChannel,
                         const int& WhichChannel)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // If the channel image is not the correct size then resize it
    if(Rows != DstImageChannel.size1() || Cols != DstImageChannel.size2())
        DstImageChannel.CreateImage(Rows,Cols);

    // Call the opencv function to get the individual channel image
    switch(WhichChannel)
    {
    case 0:
        // Blue Channel
        cvSplit(SrcImage,DstImageChannel,NULL,NULL,NULL);
        break;

    case 1:
        // Green Channel
        cvSplit(SrcImage,NULL,DstImageChannel,NULL,NULL);
        break;

    default:
        // Error
        // In this case the wrong channel index
        // was specified and thus we pass the
        // first channel as default
        cvSplit(SrcImage,DstImageChannel,NULL,NULL,NULL);
        break;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            QueryChannel
// ARGUMENTS:           SrcImage,DstImageChannel,WhichChannel
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Grab a single channel from a colored image
//                      of three channels
// DEPENDENCIES:        blImage
//                      cvSplit
//-------------------------------------------------------------------
template<typename blType>
inline void QueryChannel(const blImage< blColor3<blType> >& SrcImage,
                         blImage<blType>& DstImageChannel,
                         const int& WhichChannel)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // If the channel image is not the correct size then resize it
    if(Rows != DstImageChannel.size1() || Cols != DstImageChannel.size2())
        DstImageChannel.CreateImage(Rows,Cols);

    // Call the opencv function to get the individual channel image
    switch(WhichChannel)
    {
    case 0:
        // Blue Channel
        cvSplit(SrcImage,DstImageChannel,NULL,NULL,NULL);
        break;

    case 1:
        // Green Channel
        cvSplit(SrcImage,NULL,DstImageChannel,NULL,NULL);
        break;

    case 2:
        // Red Channel
        cvSplit(SrcImage,NULL,NULL,DstImageChannel,NULL);
        break;

    default:
        // Error
        // In this case the wrong channel index
        // was specified and thus we pass the
        // first channel as default
        cvSplit(SrcImage,DstImageChannel,NULL,NULL,NULL);
        break;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            QueryChannel
// ARGUMENTS:           SrcImage,DstImageChannel,WhichChannel
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Grab a single channel from a colored image
//                      of four channels
// DEPENDENCIES:        blImage
//                      cvSplit
//-------------------------------------------------------------------
template<typename blType>
inline void QueryChannel(const blImage< blColor4<blType> >& SrcImage,
                         blImage<blType>& DstImageChannel,
                         const int& WhichChannel)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // If the channel image is not the correct size then resize it
    if(Rows != DstImageChannel.size1() || Cols != DstImageChannel.size2())
        DstImageChannel.CreateImage(Rows,Cols);

    // Call the opencv function to get the individual channel image
    switch(WhichChannel)
    {
    case 0:
        // Blue Channel
        cvSplit(SrcImage,DstImageChannel,NULL,NULL,NULL);
        break;

    case 1:
        // Green Channel
        cvSplit(SrcImage,NULL,DstImageChannel,NULL,NULL);
        break;

    case 2:
        // Red Channel
        cvSplit(SrcImage,NULL,NULL,DstImageChannel,NULL);
        break;

    case 3:
        // Alpha Channel
        cvSplit(SrcImage,NULL,NULL,NULL,DstImageChannel);
        break;

    default:
        // Error
        // In this case the wrong channel index
        // was specified and thus we pass the
        // blue channel as default
        cvSplit(SrcImage,DstImageChannel,NULL,NULL,NULL);
        break;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            QueryChannels
// ARGUMENTS:           SrcImage
//                      RealImageChannel
//                      ImaginaryImageChannel
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Grab all channels from a two-channel image
// DEPENDENCIES:        blImage
//                      cvSplit
//                      std::complex
//-------------------------------------------------------------------
template<typename blType>
inline void QueryChannels(const blImage< std::complex<blType> >& SrcImage,
                          blImage<blType>& RealImageChannel,
                          blImage<blType>& ImaginaryImageChannel)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // If the individual channel sizes are
    // not the correct size then resize them
    if(Rows != RealImageChannel.size1() ||
       Cols != RealImageChannel.size2())
        RealImageChannel = blImage<blType>(Rows,Cols);

    if(Rows != ImaginaryImageChannel.size1() ||
       Cols != ImaginaryImageChannel.size2())
        ImaginaryImageChannel = blImage<blType>(Rows,Cols);

    // Call the opencv function to split the three-channel image
    cvSplit(SrcImage,
            RealImageChannel,
            ImaginaryImageChannel,
            NULL,
            NULL);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            QueryChannels
// ARGUMENTS:           SrcImage
//                      RedImageChannel
//                      GreenImageChannel
//                      BlueImageChannel
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Grab all channels from a three-channel image
// DEPENDENCIES:        blImage
//                      cvSplit
//-------------------------------------------------------------------
template<typename blType>
inline void QueryChannels(const blImage< blColor3<blType> >& SrcImage,
                          blImage<blType>& BlueImageChannel,
                          blImage<blType>& GreenImageChannel,
                          blImage<blType>& RedImageChannel)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // If the individual channel sizes are
    // not the correct size then resize them
    if(Rows != BlueImageChannel.size1() ||
       Cols != BlueImageChannel.size2())
        BlueImageChannel = blImage<blType>(Rows,Cols);

    if(Rows != GreenImageChannel.size1() ||
       Cols != GreenImageChannel.size2())
        GreenImageChannel = blImage<blType>(Rows,Cols);

    if(Rows != RedImageChannel.size1() ||
       Cols != RedImageChannel.size2())
        RedImageChannel = blImage<blType>(Rows,Cols);

    // Call the opencv function to split the three-channel image
    cvSplit(SrcImage,
            BlueImageChannel,
            GreenImageChannel,
            RedImageChannel,
            NULL);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            QueryChannels
// ARGUMENTS:           SrcImage
//                      RedImageChannel
//                      GreenImageChannel
//                      BlueImageChannel
//                      AlphaImageChannel
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Grab all channels from a four-channel image
// DEPENDENCIES:        blImage
//                      cvSplit
//-------------------------------------------------------------------
template<typename blType>
inline void QueryChannels(const blImage< blColor3<blType> >& SrcImage,
                          blImage<blType>& BlueImageChannel,
                          blImage<blType>& GreenImageChannel,
                          blImage<blType>& RedImageChannel,
                          blImage<blType>& AlphaImageChannel)
{
    // Get the size of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // If the individual channel sizes are
    // not the correct size then resize them
    if(Rows != BlueImageChannel.size1() ||
       Cols != BlueImageChannel.size2())
        BlueImageChannel = blImage<blType>(Rows,Cols);

    if(Rows != GreenImageChannel.size1() ||
       Cols != GreenImageChannel.size2())
        GreenImageChannel = blImage<blType>(Rows,Cols);

    if(Rows != RedImageChannel.size1() ||
       Cols != RedImageChannel.size2())
        RedImageChannel = blImage<blType>(Rows,Cols);

    if(Rows != AlphaImageChannel.size1() ||
       Cols != AlphaImageChannel.size2())
        AlphaImageChannel = blImage<blType>(Rows,Cols);

    // Call the opencv function to split the three-channel image
    cvSplit(SrcImage,
            BlueImageChannel,
            GreenImageChannel,
            RedImageChannel,
            AlphaImageChannel);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            InsertChannel
// ARGUMENTS:           ImageChannel
//                      DstImage
//                      WhichChannel
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Insert a grayscale image as a channel
//                      into an existing two-channel image
// DEPENDENCIES:        blImage
//                      cvMerge
//                      std::complex
//-------------------------------------------------------------------
template<typename blType>
inline void InsertChannel(const blImage<blType>& ImageChannel,
                          blImage< std::complex<blType> >& DstImage,
                          const int& WhichChannel)
{
    // If the DstImage is of the same size, then
    // we can simply use cvMerge to insert the
    // channel into it.
    if(ImageChannel.size1() == DstImage.size1() &&
       ImageChannel.size2() == DstImage.size2())
    {
        switch(WhichChannel)
        {
        case 0:
            cvMerge(ImageChannel,
                    NULL,
                    NULL,
                    NULL,
                    DstImage);
            break;

        case 1:
            cvMerge(NULL,
                    ImageChannel,
                    NULL,
                    NULL,
                    DstImage);
            break;

        default:

            // Error
            // In this case the wrong channel index
            // was specified and thus we insert
            // the channel as the blue channel
            cvMerge(ImageChannel,
                    NULL,
                    NULL,
                    NULL,
                    DstImage);
            break;
        }
    }
    else
    {
        // In this case, we know that the sizes
        // of the image channel and the DstImage don't
        // match, so we have to insert the channel
        // pixel by pixel (slower than cvMerge)

        // Calculate the smallest size (RowsMin,ColsMin)
        int RowsMin = std::min(ImageChannel.size1(),
                               DstImage.size1());
        int ColsMin = std::min(ImageChannel.size2(),
                               DstImage.size2());

        // Temporary complex variable
        std::complex<blType> Color;

        // Step through and assign the values
        // to the corresponding channel
        for(int i = 0; i < RowsMin; ++i)
        {
            for(int j = 0; j < ColsMin; ++j)
            {
                // Get the current color from
                // the DstImage
                Color = DstImage[i][j];

                // Change the corresponding
                // image channel value
                switch(WhichChannel)
                {
                case 0:
                    Color.real() = ImageChannel[i][j];
                    break;

                case 1:
                    Color.imag() = ImageChannel[i][j];
                    break;

                default:
                    // Error
                    // In this case the wrong channel index
                    // was specified and thus we insert
                    // the channel as the first channel
                    Color.real() = ImageChannel[i][j];
                    break;
                }

                // Re-assign the color to DstImage
                DstImage[i][j] = Color;
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            InsertChannel
// ARGUMENTS:           ImageChannel
//                      DstImage
//                      WhichChannel
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Insert a grayscale image as a channel
//                      into an existing three-channel colored image
// DEPENDENCIES:        blImage
//                      cvMerge
//-------------------------------------------------------------------
template<typename blType>
inline void InsertChannel(const blImage<blType>& ImageChannel,
                          blImage< blColor3<blType> >& DstImage,
                          const int& WhichChannel)
{
    // If the DstImage is of the same size, then
    // we can simply use cvMerge to insert the
    // channel into it.
    if(ImageChannel.size1() == DstImage.size1() &&
       ImageChannel.size2() == DstImage.size2())
    {
        switch(WhichChannel)
        {
        case 0:
            cvMerge(ImageChannel,
                    NULL,
                    NULL,
                    NULL,
                    DstImage);
            break;

        case 1:
            cvMerge(NULL,
                    ImageChannel,
                    NULL,
                    NULL,
                    DstImage);
            break;

        case 2:
            cvMerge(NULL,
                    NULL,
                    ImageChannel,
                    NULL,
                    DstImage);
            break;

        default:
            // Error
            // In this case the wrong channel index
            // was specified and thus we insert
            // the channel as the blue channel
            cvMerge(ImageChannel,
                    NULL,
                    NULL,
                    NULL,
                    DstImage);
            break;
        }
    }
    else
    {
        // In this case, we know that the sizes
        // of the image channel and the DstImage don't
        // match, so we have to insert the channel
        // pixel by pixel (slower than cvMerge)

        // Calculate the smallest size (RowsMin,ColsMin)
        int RowsMin = std::min(ImageChannel.size1(),
                               DstImage.size1());
        int ColsMin = std::min(ImageChannel.size2(),
                               DstImage.size2());

        // Temporary color variable
        blColor3<blType> Color;

        // Step through and assign the values
        // to the corresponding channel
        for(int i = 0; i < RowsMin; ++i)
        {
            for(int j = 0; j < ColsMin; ++j)
            {
                // Get the current color from
                // the DstImage
                Color = DstImage[i][j];

                // Change the corresponding
                // image channel value
                switch(WhichChannel)
                {
                case 0:
                    Color.m_Blue = ImageChannel[i][j];
                    break;

                case 1:
                    Color.m_Green = ImageChannel[i][j];
                    break;

                case 2:
                    Color.m_Red = ImageChannel[i][j];
                    break;

                default:
                    // Error
                    // In this case the wrong channel index
                    // was specified and thus we insert
                    // the channel as the blue channel
                    Color.m_Blue = ImageChannel[i][j];
                    break;
                }

                // Re-assign the color to DstImage
                DstImage[i][j] = Color;
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            InsertChannel
// ARGUMENTS:           ImageChannel
//                      DstImage
//                      WhichChannel
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Insert a grayscale image as a channel
//                      into an existing four-channel colored image
// DEPENDENCIES:        blImage
//                      cvMerge
//-------------------------------------------------------------------
template<typename blType>
inline void InsertChannel(const blImage<blType>& ImageChannel,
                          blImage< blColor4<blType> >& DstImage,
                          const int& WhichChannel)
{
    // If the DstImage is of the same size, then
    // we can simply use cvMerge to insert the
    // channel into it.
    if(ImageChannel.size1() == DstImage.size1() &&
       ImageChannel.size2() == DstImage.size2())
    {
        switch(WhichChannel)
        {
        case 0:
            cvMerge(ImageChannel,
                    NULL,
                    NULL,
                    NULL,
                    DstImage);
            break;

        case 1:
            cvMerge(NULL,
                    ImageChannel,
                    NULL,
                    NULL,
                    DstImage);
            break;

        case 2:
            cvMerge(NULL,
                    NULL,
                    ImageChannel,
                    NULL,
                    DstImage);
            break;

        case 3:
            cvMerge(NULL,
                    NULL,
                    NULL,
                    ImageChannel,
                    DstImage);
            break;

        default:
            // Error
            // In this case the wrong channel index
            // was specified and thus we insert
            // the channel as the first channel
            cvMerge(ImageChannel,
                    NULL,
                    NULL,
                    NULL,
                    DstImage);
            break;
        }
    }
    else
    {
        // In this case, we know that the sizes
        // of the image channel and the DstImage don't
        // match, so we have to insert the channel
        // pixel by pixel (slower than cvMerge)

        // Calculate the smallest size (RowsMin,ColsMin)
        int RowsMin = std::min(ImageChannel.size1(),
                               DstImage.size1());
        int ColsMin = std::min(ImageChannel.size2(),
                               DstImage.size2());

        // Temporary color variable
        blColor4<blType> Color;

        // Step through and assign the values
        // to the corresponding channel
        for(int i = 0; i < RowsMin; ++i)
        {
            for(int j = 0; j < ColsMin; ++j)
            {
                // Get the current color from
                // the DstImage
                Color = DstImage[i][j];

                // Change the corresponding
                // image channel value
                switch(WhichChannel)
                {
                case 0:
                    Color.m_Blue = ImageChannel[i][j];
                    break;

                case 1:
                    Color.m_Green = ImageChannel[i][j];
                    break;

                case 2:
                    Color.m_Red = ImageChannel[i][j];
                    break;

                case 3:
                    Color.m_Alpha = ImageChannel[i][j];
                    break;

                default:
                    // Error
                    // In this case the wrong channel index
                    // was specified and thus we insert
                    // the channel as the first channel
                    Color.m_Blue = ImageChannel[i][j];
                    break;
                }

                // Re-assign the color to DstImage
                DstImage[i][j] = Color;
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            CombineChannels
// ARGUMENTS:           RealImageChannel
//                      ImaginaryImageChannel
//                      DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Combine two single image channels into
//                      one two-channel colored image.  If the
//                      channels are of different size, then DstImage
//                      will be of size (minRows,minCols), and the
//                      channels will be copied into the DstImage
//                      pixel by pixel (slower than if the
//                      channels are the same size)
// DEPENDENCIES:        blImage
//                      cvMerge
//                      std::min
//                      std::complex
//-------------------------------------------------------------------
template<typename blType>
inline void CombineChannels(const blImage<blType>& RealImageChannel,
                            const blImage<blType>& ImaginaryImageChannel,
                            blImage< std::complex<blType> >& DstImage)
{
    // Check the channels sizes and make
    // sure that they're all the same
    int Rows = RealImageChannel.size1();
    int Cols = RealImageChannel.size2();

    if(Rows == ImaginaryImageChannel.size1() &&
       Cols == ImaginaryImageChannel.size2())
    {
        // Now that we know the image channels are
        // all of equal size, we check to make
        // sure the destination image is of
        // the correct size
        if(DstImage.size1() != Rows || DstImage.size2() != Cols)
            DstImage.CreateImage(Rows,Cols);

        // Now we can combine the channels into the DstImage
        cvMerge(RealImageChannel,
                ImaginaryImageChannel,
                NULL,
                DstImage);

        return;
    }
    else
    {
        // In this case, the image channels are not of
        // equal size, and thus we have to do more work

        // Calculate the minimum
        // number of rows and cols
        int RowsMin,ColsMin;

        RowsMin = std::min(Rows,ImaginaryImageChannel.size1());
        ColsMin = std::min(Cols,ImaginaryImageChannel.size2());

        // If the DstImage is not already the
        // correct size, then resize it
        if(DstImage.size1() != RowsMin || DstImage.size2() != ColsMin)
            DstImage.CreateImage(RowsMin,ColsMin);

        // Temporary complex variable
        std::complex<blType> Color;

        // Step through and assign the values
        // to the corresponding channels
        for(int i = 0; i < RowsMin; ++i)
        {
            for(int j = 0; j < ColsMin; ++j)
            {
                Color.real() = RealImageChannel[i][j];
                Color.imag() = ImaginaryImageChannel[i][j];

                DstImage[i][j] = Color;
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            CombineChannels
// ARGUMENTS:           BlueImageChannel
//                      GreenImageChannel
//                      RedImageChannel
//                      DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Combine three single image channels into
//                      one three-channel colored image.  If the
//                      channels are of different size, then DstImage
//                      will be of size (minRows,minCols), and the
//                      channels will be copied into the DstImage
//                      pixel by pixel (slower than if the
//                      channels are the same size)
// DEPENDENCIES:        blImage
//                      cvMerge
//                      std::min
//-------------------------------------------------------------------
template<typename blType>
inline void CombineChannels(const blImage<blType>& BlueImageChannel,
                            const blImage<blType>& GreenImageChannel,
                            const blImage<blType>& RedImageChannel,
                            blImage< blColor3<blType> >& DstImage)
{
    // Check the channels sizes and make
    // sure that they're all the same
    int Rows = BlueImageChannel.size1();
    int Cols = BlueImageChannel.size2();

    if(Rows == GreenImageChannel.size1() &&
       Cols == GreenImageChannel.size2() &&
       Rows == RedImageChannel.size1() &&
       Cols == RedImageChannel.size2())
    {
        // Now that we know the image channels are
        // all of equal size, we check to make
        // sure the destination image is of
        // the correct size
        if(DstImage.size1() != Rows || DstImage.size2() != Cols)
            DstImage.CreateImage(Rows,Cols);

        // Now we can combine the channels into the DstImage
        cvMerge(BlueImageChannel,
                GreenImageChannel,
                RedImageChannel,
                NULL,
                DstImage);

        return;
    }
    else
    {
        // In this case, the image channels are not of
        // equal size, and thus we have to do more work

        // Find the size of each image channel
        int RowsBlue = BlueImageChannel.size1();
        int ColsBlue = BlueImageChannel.size2();
        int RowsGreen = GreenImageChannel.size1();
        int ColsGreen = GreenImageChannel.size2();
        int RowsRed = RedImageChannel.size1();
        int ColsRed = RedImageChannel.size2();

        // Now that we know the size of each channel, we
        // have to find the minimum number of rows and cols
        int RowsMin,ColsMin;

        RowsMin = std::min(RowsBlue,RowsGreen);
        RowsMin = std::min(RowsMin,RowsRed);

        ColsMin = std::min(ColsBlue,ColsGreen);
        ColsMin = std::min(ColsMin,ColsRed);

        // If the DstImage is not already the
        // correct size, then resize it
        if(DstImage.size1() != RowsMin || DstImage.size2() != ColsMin)
            DstImage.CreateImage(RowsMin,ColsMin);

        // Temporary color variable
        blColor3<blType> Color;

        // Step through and assign the values
        // to the corresponding channels
        for(int i = 0; i < RowsMin; ++i)
        {
            for(int j = 0; j < ColsMin; ++j)
            {
                Color.m_Blue = BlueImageChannel[i][j];
                Color.m_Green = GreenImageChannel[i][j];
                Color.m_Red = RedImageChannel[i][j];

                DstImage[i][j] = Color;
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            CombineChannels
// ARGUMENTS:           BlueImageChannel
//                      GreenImageChannel
//                      RedImageChannel
//                      AlphaImageChannel
//                      DstImage
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Combine four single image channels into
//                      one four-channel colored image.  If the
//                      channels are of different size, then DstImage
//                      will be of size (minRows,minCols), and the
//                      channels will be copied into the DstImage
//                      pixel by pixel (slower than if the
//                      channels are the same size)
// DEPENDENCIES:        blImage
//                      cvMerge
//                      std::min
//-------------------------------------------------------------------
template<typename blType>
inline void CombineChannels(const blImage<blType>& BlueImageChannel,
                            const blImage<blType>& GreenImageChannel,
                            const blImage<blType>& RedImageChannel,
                            const blImage<blType>& AlphaImageChannel,
                            blImage< blColor4<blType> >& DstImage)
{
    // Check the channels sizes and make
    // sure that they're all the same
    int Rows = BlueImageChannel.size1();
    int Cols = BlueImageChannel.size2();

    if(Rows == GreenImageChannel.size1() &&
       Cols == GreenImageChannel.size2() &&
       Rows == RedImageChannel.size1() &&
       Cols == RedImageChannel.size2() &&
       Rows == AlphaImageChannel.size1() &&
       Cols == AlphaImageChannel.size2())
    {
        // Now that we know the image channels are
        // all of equal size, we check to make
        // sure the destination image is of
        // the correct size
        if(DstImage.size1() != Rows || DstImage.size2() != Cols)
            DstImage.CreateImage(Rows,Cols);

        // Now we can combine the channels into the DstImage
        cvMerge(BlueImageChannel,
                GreenImageChannel,
                RedImageChannel,
                AlphaImageChannel,
                DstImage);

        return;
    }
    else
    {
        // In this case, the image channels are not of
        // equal size, and thus we have to do more work

        // Find the size of each image channel
        int RowsBlue = BlueImageChannel.size1();
        int ColsBlue = BlueImageChannel.size2();
        int RowsGreen = GreenImageChannel.size1();
        int ColsGreen = GreenImageChannel.size2();
        int RowsRed = RedImageChannel.size1();
        int ColsRed = RedImageChannel.size2();
        int RowsAlpha = AlphaImageChannel.size1();
        int ColsAlpha = AlphaImageChannel.size2();

        // Now that we know the size of each channel, we
        // have to find the minimum number of rows and cols
        int RowsMin,ColsMin;

        RowsMin = std::min(RowsBlue,RowsGreen);
        RowsMin = std::min(RowsMin,RowsRed);
        RowsMin = std::min(RowsMin,RowsAlpha);

        ColsMin = std::min(ColsBlue,ColsGreen);
        ColsMin = std::min(ColsMin,ColsRed);
        ColsMin = std::min(ColsMin,ColsAlpha);

        // If the DstImage is not already the
        // correct size, then resize it
        if(DstImage.size1() != RowsMin || DstImage.size2() != ColsMin)
            DstImage.CreateImage(RowsMin,ColsMin);

        // Temporary color variable
        blColor3<blType> Color;

        // Step through and assign the values
        // to the corresponding channels
        for(int i = 0; i < RowsMin; ++i)
        {
            for(int j = 0; j < ColsMin; ++j)
            {
                Color.m_Blue = BlueImageChannel[i][j];
                Color.m_Green = GreenImageChannel[i][j];
                Color.m_Red = RedImageChannel[i][j];
                Color.m_Alpha = AlphaImageChannel[i][j];

                DstImage[i][j] = Color;
            }
        }
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGECHANNELS_HPP
