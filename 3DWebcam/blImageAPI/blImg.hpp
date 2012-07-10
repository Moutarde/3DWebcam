#ifndef BL_IMG_HPP
#define BL_IMG_HPP


//-------------------------------------------------------------------
// FILE:            blImg.hpp
// CLASS:           blImg
// BASE CLASS:      None
//
// PURPOSE:         A base class used to wrap OpenCV's image
//                  class IplImage with a boost::shared_ptr
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    IplImage -- From opencv library
//                  boost::shared_ptr -- From boost c++ libraries
//                  IPL_DEPTH_1U -- For bool images
//                  IPL_DEPTH_8U -- For unsigned char images
//                  IPL_DEPTH_8S -- For char images
//                  IPL_DEPTH_16U -- For unsigned short images
//                  IPL_DEPTH_16S -- For short images
//                  IPL_DEPTH_32S -- For int images
//                  IPL_DEPTH_32F -- For float images
//                  IPL_DEPTH_64F -- For double images
//
// NOTES:
//
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
template<typename blType>
class blImg
{
public: // Constructors and destructors

    // Default constructor
    blImg(const int& NumOfRows = 1,const int& NumOfCols = 1);

    // Copy constructor
    blImg(const blImg<blType>& Image);

    // Destructor
    ~blImg()
    {
    }

public: // Assignment operator

    // Overloaded assignment operator
    blImg<blType>&      operator=(const blImg<blType>& Image);

public: // Public functions

    // Overloaded [] operator (For efficient and easy image access)
    blType*             operator[](const int& RowIndex);
    const blType*       operator[](const int& RowIndex)const;

    // Function used to create an empty image
    void                CreateImage(const int& NumOfRows,
                                    const int& NumOfCols);

    // Function used to convert the scale of the image
    void                ConvertScale(const IplImage* Src,IplImage* Dst);

    // Function used to convert this object to a pure OpenCV IplImage
    operator IplImage* const()
    {
        if(m_Image.use_count() > 0)
            return m_Image.get();
        else
            return NULL;
    }

    // Function used to convert this const object to a pure OpenCV IplImage
    operator const IplImage* const()const
    {
        if(m_Image.use_count() > 0)
            return m_Image.get();
        else
            return NULL;
    }

    // Function used to get the depth of the image
    const int           GetDepth()const;

    // Function used to get the number of channels of the image
    const int           GetNumOfChannels()const;

    // Function used to get raw image data pointer
    const char*         GetImageData()const;

    // Function used to get raw image
    // data pointer cast to the data
    // type corresponding to this image
    const blType*       GetImageDataCast()const;

    // Functions used to get the min/max values representable in the image
    const int           GetRangeMin()const;
    const int           GetRangeMax()const;

    // Functions used to get the sizes of the image
    const int&          size1()const;
    const int&          size2()const;

    // Function used to get the
    // size of the image in bytes
    const int&          GetSizeInBytes()const;

    // Function used to get
    // the width step
    const int&          GetWidthStep()const;

    // Funtion used to get
    // whether the image origin
    // is top-left (0) or
    // bottom-left (1)
    const int&          GetOrigin()const;

    // Function used to get
    // the data order, whether
    // the channels are interleaved (0)
    // or separate (1)
    const int&          GetDataOrder()const;

protected: // Protected variables

    // Image variable
    boost::shared_ptr<IplImage>     m_Image;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blImg<blType>::blImg(const int& NumOfRows,const int& NumOfCols)
{
    // Create an empty image to the correct size
    CreateImage(NumOfRows,NumOfCols);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blImg<blType>::blImg(const blImg<blType>& Image)
{
    // Clone the image and store
    // it inside the shared pointer
    this->m_Image = shared_ptr<IplImage>(cvCloneImage(Image),ReleaseImage());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blImg<blType>::CreateImage(const int& NumOfRows,
                                       const int& NumOfCols)
{
    // Create the empty image
    IplImage* Img = NULL;
    Img = cvCreateImage(cvSize(NumOfCols,NumOfRows),
                        this->GetDepth(),
                        this->GetNumOfChannels());

    // Check if the image was created successfully
    if(Img != NULL)
    {
        m_Image = boost::shared_ptr<IplImage>(Img,ReleaseImage());
    }
    else
    {
        // Error -- We failed to
        //          create an image
    }

    return;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blImg<blType>& blImg<blType>::operator=(const blImg<blType>& Image)
{
    // Clone the image and store
    // it inside the shared pointer
    this->m_Image = boost::shared_ptr<IplImage>(cvCloneImage(Image),ReleaseImage());

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blType* blImg<blType>::operator[](const int& RowIndex)
{
    return reinterpret_cast<blType*>(m_Image->imageData + m_Image->widthStep*RowIndex);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blType* blImg<blType>::operator[](const int& RowIndex)const
{
    return reinterpret_cast<blType*>(m_Image->imageData + m_Image->widthStep*RowIndex);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const int& blImg<blType>::size1()const
{
    return m_Image->height;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const int& blImg<blType>::size2()const
{
    return m_Image->width;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const int& blImg<blType>::GetSizeInBytes()const
{
    return m_Image->imageSize;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const int& blImg<blType>::GetWidthStep()const
{
    return m_Image->widthStep;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const int& blImg<blType>::GetOrigin()const
{
    return m_Image->origin;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const int& blImg<blType>::GetDataOrder()const
{
    return m_Image->dataOrder;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const char* blImg<blType>::GetImageData()const
{
    if(m_Image.use_count() > 0)
        return m_Image->imageData;
    else
    {
        // Error
        return NULL;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blType* blImg<blType>::GetImageDataCast()const
{
    if(m_Image.use_count() > 0)
        return (blType*)(m_Image->imageData);
    else
    {
        // Error
        return NULL;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Template specializations of the function GetDepth used to
// get the OpenCV depth value for this image
//-------------------------------------------------------------------
template<typename blType>
inline const int blImg<blType>::GetDepth()const{return IPL_DEPTH_8U;}

template<>
inline const int blImg<bool>::GetDepth()const{return IPL_DEPTH_1U;}

template<>
inline const int blImg<unsigned char>::GetDepth()const{return IPL_DEPTH_8U;}

template<>
inline const int blImg<char>::GetDepth()const{return IPL_DEPTH_8S;}

template<>
inline const int blImg<unsigned short>::GetDepth()const{return IPL_DEPTH_16U;}

template<>
inline const int blImg<short>::GetDepth()const{return IPL_DEPTH_16S;}

template<>
inline const int blImg<int>::GetDepth()const{return IPL_DEPTH_32S;}

template<>
inline const int blImg<float>::GetDepth()const{return IPL_DEPTH_32F;}

template<>
inline const int blImg<double>::GetDepth()const{return IPL_DEPTH_64F;}

template<>
inline const int blImg< std::complex<bool> >::GetDepth()const{return IPL_DEPTH_1U;}

template<>
inline const int blImg< std::complex<unsigned char> >::GetDepth()const{return IPL_DEPTH_8U;}

template<>
inline const int blImg< std::complex<char> >::GetDepth()const{return IPL_DEPTH_8S;}

template<>
inline const int blImg< std::complex<unsigned short> >::GetDepth()const{return IPL_DEPTH_16U;}

template<>
inline const int blImg< std::complex<short> >::GetDepth()const{return IPL_DEPTH_16S;}

template<>
inline const int blImg< std::complex<int> >::GetDepth()const{return IPL_DEPTH_32S;}

template<>
inline const int blImg< std::complex<float> >::GetDepth()const{return IPL_DEPTH_32F;}

template<>
inline const int blImg< std::complex<double> >::GetDepth()const{return IPL_DEPTH_64F;}

template<>
inline const int blImg< blColor3<bool> >::GetDepth()const{return IPL_DEPTH_1U;}

template<>
inline const int blImg< blColor3<unsigned char> >::GetDepth()const{return IPL_DEPTH_8U;}

template<>
inline const int blImg< blColor3<char> >::GetDepth()const{return IPL_DEPTH_8S;}

template<>
inline const int blImg< blColor3<unsigned short> >::GetDepth()const{return IPL_DEPTH_16U;}

template<>
inline const int blImg< blColor3<short> >::GetDepth()const{return IPL_DEPTH_16S;}

template<>
inline const int blImg< blColor3<int> >::GetDepth()const{return IPL_DEPTH_32S;}

template<>
inline const int blImg< blColor3<float> >::GetDepth()const{return IPL_DEPTH_32F;}

template<>
inline const int blImg< blColor3<double> >::GetDepth()const{return IPL_DEPTH_64F;}

template<>
inline const int blImg< blColor4<bool> >::GetDepth()const{return IPL_DEPTH_1U;}

template<>
inline const int blImg< blColor4<unsigned char> >::GetDepth()const{return IPL_DEPTH_8U;}

template<>
inline const int blImg< blColor4<char> >::GetDepth()const{return IPL_DEPTH_8S;}

template<>
inline const int blImg< blColor4<unsigned short> >::GetDepth()const{return IPL_DEPTH_16U;}

template<>
inline const int blImg< blColor4<short> >::GetDepth()const{return IPL_DEPTH_16S;}

template<>
inline const int blImg< blColor4<int> >::GetDepth()const{return IPL_DEPTH_32S;}

template<>
inline const int blImg< blColor4<float> >::GetDepth()const{return IPL_DEPTH_32F;}

template<>
inline const int blImg< blColor4<double> >::GetDepth()const{return IPL_DEPTH_64F;}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Template specializations of the function GetNumOfChannels used to
// get the number of channels of an IplImage
//-------------------------------------------------------------------
template<typename blType>
inline const int blImg<blType>::GetNumOfChannels()const{return 1;}

template<>
inline const int blImg< std::complex<bool> >::GetNumOfChannels()const{return 2;}

template<>
inline const int blImg< std::complex<unsigned char> >::GetNumOfChannels()const{return 2;}

template<>
inline const int blImg< std::complex<char> >::GetNumOfChannels()const{return 2;}

template<>
inline const int blImg< std::complex<unsigned short> >::GetNumOfChannels()const{return 2;}

template<>
inline const int blImg< std::complex<short> >::GetNumOfChannels()const{return 2;}

template<>
inline const int blImg< std::complex<int> >::GetNumOfChannels()const{return 2;}

template<>
inline const int blImg< std::complex<float> >::GetNumOfChannels()const{return 2;}

template<>
inline const int blImg< std::complex<double> >::GetNumOfChannels()const{return 2;}

template<>
inline const int blImg< blColor3<bool> >::GetNumOfChannels()const{return 3;}

template<>
inline const int blImg< blColor3<unsigned char> >::GetNumOfChannels()const{return 3;}

template<>
inline const int blImg< blColor3<char> >::GetNumOfChannels()const{return 3;}

template<>
inline const int blImg< blColor3<unsigned short> >::GetNumOfChannels()const{return 3;}

template<>
inline const int blImg< blColor3<short> >::GetNumOfChannels()const{return 3;}

template<>
inline const int blImg< blColor3<int> >::GetNumOfChannels()const{return 3;}

template<>
inline const int blImg< blColor3<float> >::GetNumOfChannels()const{return 3;}

template<>
inline const int blImg< blColor3<double> >::GetNumOfChannels()const{return 3;}

template<>
inline const int blImg< blColor4<bool> >::GetNumOfChannels()const{return 4;}

template<>
inline const int blImg< blColor4<unsigned char> >::GetNumOfChannels()const{return 4;}

template<>
inline const int blImg< blColor4<char> >::GetNumOfChannels()const{return 4;}

template<>
inline const int blImg< blColor4<unsigned short> >::GetNumOfChannels()const{return 4;}

template<>
inline const int blImg< blColor4<short> >::GetNumOfChannels()const{return 4;}

template<>
inline const int blImg< blColor4<int> >::GetNumOfChannels()const{return 4;}

template<>
inline const int blImg< blColor4<float> >::GetNumOfChannels()const{return 4;}

template<>
inline const int blImg< blColor4<double> >::GetNumOfChannels()const{return 4;}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Template specializations of the function GetRangeMin
// used to get the minimum value of the image pixel representable
// by the image depth
//-------------------------------------------------------------------
template<typename blType>
inline const int blImg<blType>::GetRangeMin()const{return 0;}

template<>
inline const int blImg<bool>::GetRangeMin()const{return std::numeric_limits<bool>::min();}

template<>
inline const int blImg<unsigned char>::GetRangeMin()const{return std::numeric_limits<unsigned char>::min();}

template<>
inline const int blImg<char>::GetRangeMin()const{return std::numeric_limits<char>::min();}

template<>
inline const int blImg<unsigned short>::GetRangeMin()const{return std::numeric_limits<unsigned short>::min();}

template<>
inline const int blImg<short>::GetRangeMin()const{return std::numeric_limits<short>::min();}

template<>
inline const int blImg<int>::GetRangeMin()const{return std::numeric_limits<int>::min();}

template<>
inline const int blImg<float>::GetRangeMin()const{return 0;}

template<>
inline const int blImg<double>::GetRangeMin()const{return 0;}

template<>
inline const int blImg< std::complex<bool> >::GetRangeMin()const{return std::numeric_limits<bool>::min();}

template<>
inline const int blImg< std::complex<unsigned char> >::GetRangeMin()const{return std::numeric_limits<unsigned char>::min();}

template<>
inline const int blImg< std::complex<char> >::GetRangeMin()const{return std::numeric_limits<char>::min();}

template<>
inline const int blImg< std::complex<unsigned short> >::GetRangeMin()const{return std::numeric_limits<unsigned short>::min();}

template<>
inline const int blImg< std::complex<short> >::GetRangeMin()const{return std::numeric_limits<short>::min();}

template<>
inline const int blImg< std::complex<int> >::GetRangeMin()const{return std::numeric_limits<int>::min();}

template<>
inline const int blImg< std::complex<float> >::GetRangeMin()const{return 0;}

template<>
inline const int blImg< std::complex<double> >::GetRangeMin()const{return 0;}

template<>
inline const int blImg< blColor3<bool> >::GetRangeMin()const{return std::numeric_limits<bool>::min();}

template<>
inline const int blImg< blColor3<unsigned char> >::GetRangeMin()const{return std::numeric_limits<unsigned char>::min();}

template<>
inline const int blImg< blColor3<char> >::GetRangeMin()const{return std::numeric_limits<char>::min();}

template<>
inline const int blImg< blColor3<unsigned short> >::GetRangeMin()const{return std::numeric_limits<unsigned short>::min();}

template<>
inline const int blImg< blColor3<short> >::GetRangeMin()const{return std::numeric_limits<short>::min();}

template<>
inline const int blImg< blColor3<int> >::GetRangeMin()const{return std::numeric_limits<int>::min();}

template<>
inline const int blImg< blColor3<float> >::GetRangeMin()const{return 0;}

template<>
inline const int blImg< blColor3<double> >::GetRangeMin()const{return 0;}

template<>
inline const int blImg< blColor4<bool> >::GetRangeMin()const{return std::numeric_limits<bool>::min();}

template<>
inline const int blImg< blColor4<unsigned char> >::GetRangeMin()const{return std::numeric_limits<unsigned char>::min();}

template<>
inline const int blImg< blColor4<char> >::GetRangeMin()const{return std::numeric_limits<char>::min();}

template<>
inline const int blImg< blColor4<unsigned short> >::GetRangeMin()const{return std::numeric_limits<unsigned short>::min();}

template<>
inline const int blImg< blColor4<short> >::GetRangeMin()const{return std::numeric_limits<short>::min();}

template<>
inline const int blImg< blColor4<int> >::GetRangeMin()const{return std::numeric_limits<int>::min();}

template<>
inline const int blImg< blColor4<float> >::GetRangeMin()const{return 0;}

template<>
inline const int blImg< blColor4<double> >::GetRangeMin()const{return 0;}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// Template specializations of the function GetRangeMax
// used to get the maximum value of the image pixel representable
// by the image depth
//-------------------------------------------------------------------
template<typename blType>
inline const int blImg<blType>::GetRangeMax()const{return 1;}

template<>
inline const int blImg<bool>::GetRangeMax()const{return std::numeric_limits<bool>::max();}

template<>
inline const int blImg<unsigned char>::GetRangeMax()const{return std::numeric_limits<unsigned char>::max();}

template<>
inline const int blImg<char>::GetRangeMax()const{return std::numeric_limits<char>::max();}

template<>
inline const int blImg<unsigned short>::GetRangeMax()const{return std::numeric_limits<unsigned short>::max();}

template<>
inline const int blImg<short>::GetRangeMax()const{return std::numeric_limits<short>::max();}

template<>
inline const int blImg<int>::GetRangeMax()const{return std::numeric_limits<int>::max();}

template<>
inline const int blImg<float>::GetRangeMax()const{return 1;}

template<>
inline const int blImg<double>::GetRangeMax()const{return 1;}

template<>
inline const int blImg< std::complex<bool> >::GetRangeMax()const{return std::numeric_limits<bool>::max();}

template<>
inline const int blImg< std::complex<unsigned char> >::GetRangeMax()const{return std::numeric_limits<unsigned char>::max();}

template<>
inline const int blImg< std::complex<char> >::GetRangeMax()const{return std::numeric_limits<char>::max();}

template<>
inline const int blImg< std::complex<unsigned short> >::GetRangeMax()const{return std::numeric_limits<unsigned short>::max();}

template<>
inline const int blImg< std::complex<short> >::GetRangeMax()const{return std::numeric_limits<short>::max();}

template<>
inline const int blImg< std::complex<int> >::GetRangeMax()const{return std::numeric_limits<int>::max();}

template<>
inline const int blImg< std::complex<float> >::GetRangeMax()const{return 1;}

template<>
inline const int blImg< std::complex<double> >::GetRangeMax()const{return 1;}

template<>
inline const int blImg< blColor3<bool> >::GetRangeMax()const{return std::numeric_limits<bool>::max();}

template<>
inline const int blImg< blColor3<unsigned char> >::GetRangeMax()const{return std::numeric_limits<unsigned char>::max();}

template<>
inline const int blImg< blColor3<char> >::GetRangeMax()const{return std::numeric_limits<char>::max();}

template<>
inline const int blImg< blColor3<unsigned short> >::GetRangeMax()const{return std::numeric_limits<unsigned short>::max();}

template<>
inline const int blImg< blColor3<short> >::GetRangeMax()const{return std::numeric_limits<short>::max();}

template<>
inline const int blImg< blColor3<int> >::GetRangeMax()const{return std::numeric_limits<int>::max();}

template<>
inline const int blImg< blColor3<float> >::GetRangeMax()const{return 1;}

template<>
inline const int blImg< blColor3<double> >::GetRangeMax()const{return 1;}

template<>
inline const int blImg< blColor4<bool> >::GetRangeMax()const{return std::numeric_limits<bool>::max();}

template<>
inline const int blImg< blColor4<unsigned char> >::GetRangeMax()const{return std::numeric_limits<unsigned char>::max();}

template<>
inline const int blImg< blColor4<char> >::GetRangeMax()const{return std::numeric_limits<char>::min();}

template<>
inline const int blImg< blColor4<unsigned short> >::GetRangeMax()const{return std::numeric_limits<unsigned short>::max();}

template<>
inline const int blImg< blColor4<short> >::GetRangeMax()const{return std::numeric_limits<short>::max();}

template<>
inline const int blImg< blColor4<int> >::GetRangeMax()const{return std::numeric_limits<int>::max();}

template<>
inline const int blImg< blColor4<float> >::GetRangeMax()const{return 1;}

template<>
inline const int blImg< blColor4<double> >::GetRangeMax()const{return 1;}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blImg<blType>::ConvertScale(const IplImage* Src,IplImage* Dst)
{
    double SrcMin = 0;
    double SrcMax = 1;
    double DstMin = 0;
    double DstMax = 1;

    // Determine the minimum and
    // maximum rapresentable numbers of
    // the source image
    switch(Src->depth)
    {
        case IPL_DEPTH_1U:
            SrcMin = 0;
            SrcMax = 1;
            break;
        case IPL_DEPTH_8U:
            SrcMin = numeric_limits<unsigned char>::min();
            SrcMax = numeric_limits<unsigned char>::max();
            break;
        case IPL_DEPTH_8S:
            SrcMin = numeric_limits<char>::min();
            SrcMax = numeric_limits<char>::max();
            break;
        case IPL_DEPTH_16U:
            SrcMin = numeric_limits<unsigned short>::min();
            SrcMax = numeric_limits<unsigned short>::max();
            break;
        case IPL_DEPTH_16S:
            SrcMin = numeric_limits<short>::min();
            SrcMax = numeric_limits<short>::max();
            break;
        case IPL_DEPTH_32S:
            SrcMin = numeric_limits<int>::min();
            SrcMax = numeric_limits<int>::max();
            break;
        case IPL_DEPTH_32F:
            SrcMin = 0;
            SrcMax = 1;
            break;
        case IPL_DEPTH_64F:
            SrcMin = 0;
            SrcMax = 1;
            break;
    }

    // Determine the minimum and
    // maximum rapresentable numbers of
    // the destination image
    switch(Dst->depth)
    {
        case IPL_DEPTH_1U:
            DstMin = 0;
            DstMax = 1;
            break;
        case IPL_DEPTH_8U:
            DstMin = 0;
            DstMax = 255;
            break;
        case IPL_DEPTH_8S:
            DstMin = numeric_limits<char>::min();
            DstMax = numeric_limits<char>::max();
            break;
        case IPL_DEPTH_16U:
            DstMin = numeric_limits<unsigned short>::min();
            DstMax = numeric_limits<unsigned short>::max();
            break;
        case IPL_DEPTH_16S:
            DstMin = numeric_limits<short>::min();
            DstMax = numeric_limits<short>::max();
            break;
        case IPL_DEPTH_32S:
            DstMin = numeric_limits<int>::min();
            DstMax = numeric_limits<int>::max();
            break;
        case IPL_DEPTH_32F:
            DstMin = 0;
            DstMax = 1;
            break;
        case IPL_DEPTH_64F:
            DstMin = 0;
            DstMax = 1;
            break;
    }

    // Calculate the Scale and shift values
    double Scale = (DstMax - DstMin)/(SrcMax - SrcMin);
    double Shift = DstMin - Scale*SrcMin;

    cvConvertScale(Src,Dst,Scale,Shift);
}
//-------------------------------------------------------------------


#endif // BL_IMG_HPP
