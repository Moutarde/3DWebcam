#ifndef BL_IMAGE_HPP
#define BL_IMAGE_HPP


//-------------------------------------------------------------------
// FILE:            blImage.hpp
// CLASS:           blImage
// BASE CLASS:      blImg
//
// PURPOSE:         Based on blImg, adds more functionality to the
//                  OpenCV IplImage wrap
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blImg
//                  CvScalar -- To covert between different type images
//
// NOTES:
// DATE CREATED:    Jun/03/2010
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
#include <QImage>
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
class blImage : public blImg<blType>
{
public: // Constructors and destructors

    // Default constructor
    blImage(const int& Rows = 1,const int& Cols = 1);

    // Copy constructor
    blImage(const blImage<blType>& Image);

    // Copy constructor from a different type image
    template<typename blType2>
    blImage(const blImage<blType2>& Image,const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    // Constructor from a raw IplImage*
    blImage(const IplImage* Img,const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    // Constructor from a raw CvMat*
    blImage(const CvMat* Img,const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    // Construct image from raw 2D array
    template<int NumOfRows,int NumOfCols>
    blImage(const blType (&MatrixArray)[NumOfRows][NumOfCols]);

    // Destructor
    ~blImage()
    {
    }

public: // Public functions

    // Function used to wrap an existing
    // IplImage with a blImage and NULLIFY
    // the original IplImage pointer
    void            WrapIplImage(IplImage*& Img,
                                 const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    // Function used to load an IplImage into this object
    void            LoadImage(const IplImage* Img,
                              const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    // Function used to load a CvMat into this object
    void            LoadMatrix(const CvMat* Img,
                               const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    // Function used to load an image from file
    void            LoadImageFromFile(const string& Filename,
                                      const bool& ShouldImageBeFlipped = false,
                                      const int& ImageFlipMode = 0,
                                      const int& HowToReadImageColorAndDepth =
                                      CV_LOAD_IMAGE_ANYDEPTH |
                                      CV_LOAD_IMAGE_ANYCOLOR,
                                      const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType>::blImage(const int& Rows,const int& Cols)
                                : blImg<blType>(Rows,Cols)
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType>::blImage(const blImage<blType>& Image) : blImg<blType>(Image)
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
template<typename blType2>
inline blImage<blType>::blImage(const blImage<blType2>& Image,
                                const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
                                : blImg<blType>()
{
    // Since we have a different type image, we
    // have to do some extra work, but we do want
    // to copy image and not link
    LoadImage(Image,InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType>::blImage(const IplImage* Img,
                                const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
                                : blImg<blType>()
{
    LoadImage(Img,InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType>::blImage(const CvMat* Img,
                                const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
                                : blImg<blType>()
{
    this->LoadMatrix(Img,InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
template<int NumOfRows,int NumOfCols>
inline blImage<blType>::blImage(const blType (&MatrixArray)[NumOfRows][NumOfCols])
                                : blImg<blType>(NumOfRows,NumOfCols)
{
    // Fill the image values using the passed constant array
    for(int i = 0; i < NumOfRows; ++i)
        for(int j = 0; j < NumOfCols; ++j)
            (*this)[i][j] = MatrixArray[i][j];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blImage<blType>::WrapIplImage(IplImage*& Img,
                                          const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    // First we have to check
    // if the passed image pointer
    // is NULL
    if(Img == NULL)
    {
        // Error -- Tried to wrap an
        //          IplImage pointer
        //          that was NULL
        return;
    }

    // Since blImage has a statically
    // determined depth and number of
    // channels, if we want to wrap
    // an existing IplImage pointer, we
    // have to check if both the depth
    // and number of channels match this
    // blImage
    if(Img->depth == this->GetDepth() &&
       Img->nChannels == this->GetNumOfChannels())
    {
        // In this case they match, therefore
        // we can simply link to the existing
        // image pointer and then NULLIFY it
        this->m_Image = boost::shared_ptr<IplImage>(Img,ReleaseImage());
        Img = NULL;
    }
    else
    {
        // In this case they don't match,
        // therefore we first copy the
        // existing image pointer to this
        // blImage and then release the original
        // image and NULLIFY the pointer
        this->LoadImage(Img,InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
        cvReleaseImage(&Img);
        Img = NULL;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blImage<blType>::LoadMatrix(const CvMat* Img,
                                        const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    // First we need to check
    // the validity of the
    // CvMat* pointer
    if(Img == NULL)
    {
        // Error -- Tried to load a
        //          CvMat* structure
        //          into this image
        //          using a NULL pointer.
        //          In this case we
        //          simply default
        //          this image to
        //          size 1x1 and return
        this->CreateImage(1,1);
        return;
    }

    // Now that we know we don't
    // have a NULL pointer, we
    // create an IplImage header
    // from the CvMat image
    IplImage MatToImg;
    cvGetImage(Img,&MatToImg);

    // Now we can finally load
    // the matrix into this image
    this->LoadImage(&MatToImg,InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blImage<blType>::LoadImage(const IplImage* Img,
                                       const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    // Because the image has some default depth and number of channels,
    // we have to check if it corresponds to this object's image depth and number
    // of channels, otherwise we have to do some extra work
    if(Img->depth == this->GetDepth() &&
       Img->nChannels == this->GetNumOfChannels())
    {
        this->m_Image = boost::shared_ptr<IplImage>(cvCloneImage(Img),ReleaseImage());
    }
    else
    {
        // In this case, the depths or channels don't match, so we
        // have to do some extra work.  We have to create a new image
        // with the correct depth and number of channels and then
        // copy the passed image into this one
        this->CreateImage(Img->height,Img->width);

        // Check for zero size
        if(Img->width == 0 || Img->height == 0)
            return;

        // If we created the image successfully then copy the images
        if(this->m_Image.use_count() > 0)
        {
            // Check the number of channels and call the appropriate functions
            if(this->GetNumOfChannels() == Img->nChannels)
            {
                // At this point we know that the number of channels match, but
                // that the depths are different, so we use the cvConvertScale
                // function to convert the image to the correct depth
                this->ConvertScale(Img,(*this));
            }
            else if(this->GetNumOfChannels() == 1 && Img->nChannels == 3)
            {
                if(this->GetDepth() == Img->depth)
                {
                    if(Img->depth == IPL_DEPTH_8U ||
                       Img->depth == IPL_DEPTH_16U ||
                       Img->depth == IPL_DEPTH_32F)
                    {
                        cvCvtColor(Img,(*this),CV_BGR2GRAY);
                    }
                    else
                    {
                        CvScalar Color;
                        for(int i = 0; i < this->size1(); ++i)
                        {
                            for(int j = 0; j < this->size2(); ++j)
                            {
                                Color = cvGet2D(Img,i,j);
                                (*this)[i][j] = 0.114*Color.val[0] + 0.587*Color.val[1] + 0.299*Color.val[2];
                            }
                        }
                    }
                }
                else
                {
                    // The depths are different so we have to create an intermediary
                    // image to take care of the two conversions

                    // Create a new temporary image from the loaded one
                    IplImage* Img2 = cvCreateImage(cvSize(Img->width,Img->height),Img->depth,1);

                    // Convert the loaded image into this new image (to grayscale)
                    if(Img->depth == IPL_DEPTH_8U ||
                       Img->depth == IPL_DEPTH_16U ||
                       Img->depth == IPL_DEPTH_32F)
                    {
                        cvCvtColor(Img,Img2,CV_BGR2GRAY);
                    }
                    else
                    {
                        CvScalar Color;
                        for(int i = 0; i < this->size1(); ++i)
                        {
                            for(int j = 0; j < this->size2(); ++j)
                            {
                                Color = cvGet2D(Img,i,j);
                                cvSet2D(Img2,i,j,cvScalar(0.114*Color.val[0] + 0.587*Color.val[1] + 0.299*Color.val[2]));
                            }
                        }
                    }

                    // Convert the new grayscale image to this object's image
                    this->ConvertScale(Img2,(*this));

                    // Clean up the resources
                    if(Img2)
                        cvReleaseImage(&Img2);
                }
            }
            else if((this->GetNumOfChannels() == 3 && Img->nChannels == 1) ||
                    (this->GetNumOfChannels() == 4 && Img->nChannels == 1))
            {
                if(this->GetDepth() == Img->depth)
                {
                    // Depths are the same so we just use the convert color function
                    if(Img->depth == IPL_DEPTH_8U ||
                       Img->depth == IPL_DEPTH_16U ||
                       Img->depth == IPL_DEPTH_32F)
                    {
                        cvCvtColor(Img,(*this),CV_GRAY2RGB);
                    }
                    else
                    {
                        CvScalar Color;
                        for(int i = 0; i < this->size1(); ++i)
                        {
                            for(int j = 0; j < this->size2(); ++j)
                            {
                                Color = cvGet2D(Img,i,j);
                                cvSet2D((*this),i,j,cvScalar(Color.val[0],Color.val[0],Color.val[0]));
                            }
                        }
                    }
                }
                else
                {
                    // The depths are different so we have to create an intermediary
                    // image to take care of the two conversions

                    // Create a new temporary image from the loaded one
                    IplImage* Img2 = cvCreateImage(cvSize(Img->width,Img->height),Img->depth,this->GetNumOfChannels());

                    // Convert the loaded image into this new image (to grayscale)
                    // Depths are the same so we just use the convert color function
                    if(Img->depth == IPL_DEPTH_8U ||
                       Img->depth == IPL_DEPTH_16U ||
                       Img->depth == IPL_DEPTH_32F)
                    {
                        cvCvtColor(Img,Img2,CV_GRAY2RGB);
                    }
                    else
                    {
                        CvScalar Color;
                        for(int i = 0; i < this->size1(); ++i)
                        {
                            for(int j = 0; j < this->size2(); ++j)
                            {
                                Color = cvGet2D(Img,i,j);
                                cvSet2D(Img2,i,j,cvScalar(Color.val[0],Color.val[0],Color.val[0]));
                            }
                        }
                    }

                    // Convert the new grayscale image to this object's image
                    this->ConvertScale(Img2,(*this));

                    // Clean up the resources
                    if(Img2)
                        cvReleaseImage(&Img2);
                }
            }
            else if((this->GetNumOfChannels() == 1 && Img->nChannels == 2))
            {
                // At this point we know that we were passed a
                // complex type image and have to save it to
                // a single-channel image

                // Color variable used to get the pixel color
                CvScalar Color;

                // Loop through the image pixel by pixel and convert
                for(int i = 0; i < this->size1(); ++i)
                {
                    for(int j = 0; j < this->size2(); ++j)
                    {
                        // Get the color
                        Color = cvGet2D(Img,i,j);

                        // Depending on the passed parameter assign the pixel's value
                        switch(InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
                        {
                        case 0:

                            // In this case we just copy the real value
                            cvSet2D((*this),i,j,cvScalar(Color.val[0]));

                            break;

                        case 1:

                            // In this case we just copy the imaginary value
                            cvSet2D((*this),i,j,cvScalar(Color.val[1]));

                            break;

                        case 2:

                            // In this case, we assign the absolute value to this image's pixel
                            cvSet2D((*this),i,j,cvScalar(std::sqrt(Color.val[0]*Color.val[0] + Color.val[1]*Color.val[1])));

                            break;

                        default:

                            // As a default we just copy the real value
                            cvSet2D((*this),i,j,cvScalar(Color.val[0]));

                            break;
                        }
                    }
                }
            }
            else if((this->GetNumOfChannels() == 3 && Img->nChannels == 2) ||
                    (this->GetNumOfChannels() == 4 && Img->nChannels == 2))
            {
                // At this point we know that we were passed a
                // complex type image and have to save it to
                // a three-channel or four-channel image

                // Color variable used to get the pixel color
                CvScalar Color;

                // Loop through the image pixel by pixel and convert
                for(int i = 0; i < this->size1(); ++i)
                {
                    for(int j = 0; j < this->size2(); ++j)
                    {
                        // Get the color
                        Color = cvGet2D(Img,i,j);

                        // Variable used to calculate the absolute value
                        double AbsValue;

                        // Depending on the passed parameter assign the pixel's value
                        switch(InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
                        {
                        case 0:

                            // In this case we just copy the real value
                            cvSet2D((*this),i,j,cvScalar(Color.val[0],Color.val[0],Color.val[0]));

                            break;

                        case 1:

                            // In this case we just copy the imaginary value
                            cvSet2D((*this),i,j,cvScalar(Color.val[1],Color.val[1],Color.val[1]));

                            break;

                        case 2:

                            // In this case, we assign the absolute value to this image's pixel
                            AbsValue = std::sqrt(Color.val[0]*Color.val[0] + Color.val[1]*Color.val[1]);
                            cvSet2D((*this),i,j,cvScalar(AbsValue,AbsValue,AbsValue));

                            break;

                        default:

                            // As a default we just copy the real value
                            cvSet2D((*this),i,j,cvScalar(Color.val[0],Color.val[0],Color.val[0]));

                            break;
                        }
                    }
                }
            }
            else if(this->GetNumOfChannels() == 2 && Img->nChannels == 1)
            {
                // At this point we know that we were passed a
                // single channel image but have to save it
                // to a complex image

                // Color variable used to get the pixel color
                CvScalar Color;

                // Loop through the image pixel by pixel and convert
                for(int i = 0; i < this->size1(); ++i)
                {
                    for(int j = 0; j < this->size2(); ++j)
                    {
                        // Get the color
                        Color = cvGet2D(Img,i,j);

                        // Assign the absolute value to this image's pixel
                        cvSet2D((*this),i,j,cvScalar(Color.val[0]));
                    }
                }
            }
            else if((this->GetNumOfChannels() == 2 && Img->nChannels == 3) ||
                    (this->GetNumOfChannels() == 2 && Img->nChannels == 4))
            {
                // At this point we know that we were passed a
                // three-channel image, so we just assign the
                // average value as the real values of our complex image

                // Color variable used to get the pixel color
                CvScalar Color;

                // Loop through the image pixel by pixel and convert
                for(int i = 0; i < this->size1(); ++i)
                {
                    for(int j = 0; j < this->size2(); ++j)
                    {
                        // Get the color
                        Color = cvGet2D(Img,i,j);

                        // Assign the average value to this image's pixel
                        cvSet2D((*this),i,j,cvScalar((Color.val[0] + Color.val[1] + Color.val[2])/3.0f));
                    }
                }
            }
        }
        else
        {
            // Error
        }
    }

    return;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blImage<blType>::LoadImageFromFile(const string& Filename,
                                               const bool& ShouldImageBeFlipped,
                                               const int& ImageFlipMode,
                                               const int& HowToReadImageColorAndDepth,
                                               const int& InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    // Load image from file
    IplImage* img = NULL;
    img = cvLoadImage(Filename.c_str(),HowToReadImageColorAndDepth);

    // Check if the image successfully loaded
    if(img == NULL)
    {
        // Error
        return;
    }

    // Flip image if being asked to
    if(ShouldImageBeFlipped)
        cvFlip(img,img,ImageFlipMode);

    // Load the file image into this one (we want
    // to copy the image and not just link to it)
    LoadImage(img,InCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

    // Release the image that was loaded from file
    cvReleaseImage(&img);

    return;
}
//-------------------------------------------------------------------

#endif // BL_IMAGE_HPP
