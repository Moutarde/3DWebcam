#ifndef BL_IMAGESERIALIZATION_HPP
#define BL_IMAGESERIALIZATION_HPP


//-------------------------------------------------------------------
// FILE:            blImageSerialization.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of simple functions used to
//                  serialize/unserialize images to/from a
//                  file storage
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    - Opencv cxcore library
//
// NOTES:
//
// DATE CREATED:    Jun/05/2011
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
// FUNCTION:            DetermineTypeOfImage
//
// ARGUMENTS:           SrcImage
//
// TEMPLATE ARGUMENTS:  None
//
// PURPOSE:             Build a string used to represent
//                      the type of image when saving
//                      the image to disk
//
// DEPENDENCIES:        IplImage
//-------------------------------------------------------------------
inline string DetermineTypeOfImage(const IplImage* SrcImage)
{
    string TypeOfImage = "";

    // Check to see if the image is NULL
    if(SrcImage == NULL)
    {
        // Error -- Tried to determine
        //          the type of a NULL
        //          image
        return TypeOfImage;
    }

    // Check the number of channels
    switch(SrcImage->nChannels)
    {
    case 2:
        TypeOfImage.append("2");
        break;
    case 3:
        TypeOfImage.append("3");
        break;
    case 4:
        TypeOfImage.append("4");
        break;
    default:
        // In this case we
        // have one channel so
        // we do not have to write it
        break;
    }

    // Check the image depth
    switch(SrcImage->depth)
    {
    case IPL_DEPTH_1U:
        TypeOfImage.append("u");
        break;
    case IPL_DEPTH_8U:
        TypeOfImage.append("u");
        break;
    case IPL_DEPTH_8S:
        TypeOfImage.append("c");
        break;
    case IPL_DEPTH_16U:
        TypeOfImage.append("w");
        break;
    case IPL_DEPTH_16S:
        TypeOfImage.append("s");
        break;
    case IPL_DEPTH_32S:
        TypeOfImage.append("i");
        break;
    case IPL_DEPTH_32F:
        TypeOfImage.append("f");
        break;
    case IPL_DEPTH_64F:
        TypeOfImage.append("d");
        break;
    default:
        TypeOfImage.append("u");
        break;
    }

    return TypeOfImage;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            DetermineDepthAndNumOfChannels
//
// ARGUMENTS:           TypeOfImage
//                      Depth
//                      NumOfChannels
//
// TEMPLATE ARGUMENTS:  None
//
// PURPOSE:             Determine the depth and number of
//                      channels of an image from a string
//                      when loading an image from file
//
// DEPENDENCIES:        IplImage
//-------------------------------------------------------------------
inline void DetermineDepthAndNumOfChannels(const string& TypeOfImage,
                                           int& Depth,
                                           int& NumOfChannels)
{
    // We simply check the type
    // of image and determine the
    // appropriate depth and number
    // of channels
    if(TypeOfImage == "u")
    {
        Depth = IPL_DEPTH_8U;
        NumOfChannels = 1;
    }
    else if(TypeOfImage == "c")
    {
        Depth = IPL_DEPTH_8S;
        NumOfChannels = 1;
    }
    else if(TypeOfImage == "w")
    {
        Depth = IPL_DEPTH_16U;
        NumOfChannels = 1;
    }
    else if(TypeOfImage == "s")
    {
        Depth = IPL_DEPTH_16S;
        NumOfChannels = 1;
    }
    else if(TypeOfImage == "i")
    {
        Depth = IPL_DEPTH_32S;
        NumOfChannels = 1;
    }
    else if(TypeOfImage == "f")
    {
        Depth = IPL_DEPTH_32F;
        NumOfChannels = 1;
    }
    else if(TypeOfImage == "d")
    {
        Depth = IPL_DEPTH_64F;
        NumOfChannels = 1;
    }
    else if(TypeOfImage == "2u")
    {
        Depth = IPL_DEPTH_8U;
        NumOfChannels = 2;
    }
    else if(TypeOfImage == "2c")
    {
        Depth = IPL_DEPTH_8S;
        NumOfChannels = 2;
    }
    else if(TypeOfImage == "2w")
    {
        Depth = IPL_DEPTH_16U;
        NumOfChannels = 2;
    }
    else if(TypeOfImage == "2s")
    {
        Depth = IPL_DEPTH_16S;
        NumOfChannels = 2;
    }
    else if(TypeOfImage == "2i")
    {
        Depth = IPL_DEPTH_32S;
        NumOfChannels = 2;
    }
    else if(TypeOfImage == "2f")
    {
        Depth = IPL_DEPTH_32F;
        NumOfChannels = 2;
    }
    else if(TypeOfImage == "2d")
    {
        Depth = IPL_DEPTH_64F;
        NumOfChannels = 2;
    }
    else if(TypeOfImage == "3u")
    {
        Depth = IPL_DEPTH_8U;
        NumOfChannels = 3;
    }
    else if(TypeOfImage == "3c")
    {
        Depth = IPL_DEPTH_8S;
        NumOfChannels = 3;
    }
    else if(TypeOfImage == "3w")
    {
        Depth = IPL_DEPTH_16U;
        NumOfChannels = 3;
    }
    else if(TypeOfImage == "3s")
    {
        Depth = IPL_DEPTH_16S;
        NumOfChannels = 3;
    }
    else if(TypeOfImage == "3i")
    {
        Depth = IPL_DEPTH_32S;
        NumOfChannels = 3;
    }
    else if(TypeOfImage == "3f")
    {
        Depth = IPL_DEPTH_32F;
        NumOfChannels = 3;
    }
    else if(TypeOfImage == "3d")
    {
        Depth = IPL_DEPTH_64F;
        NumOfChannels = 3;
    }
    else if(TypeOfImage == "4u")
    {
        Depth = IPL_DEPTH_8U;
        NumOfChannels = 4;
    }
    else if(TypeOfImage == "4c")
    {
        Depth = IPL_DEPTH_8S;
        NumOfChannels = 4;
    }
    else if(TypeOfImage == "4w")
    {
        Depth = IPL_DEPTH_16U;
        NumOfChannels = 4;
    }
    else if(TypeOfImage == "4s")
    {
        Depth = IPL_DEPTH_16S;
        NumOfChannels = 4;
    }
    else if(TypeOfImage == "4i")
    {
        Depth = IPL_DEPTH_32S;
        NumOfChannels = 4;
    }
    else if(TypeOfImage == "4f")
    {
        Depth = IPL_DEPTH_32F;
        NumOfChannels = 4;
    }
    else if(TypeOfImage == "4d")
    {
        Depth = IPL_DEPTH_64F;
        NumOfChannels = 4;
    }
    else
    {
        // Error -- We did not recognize the
        //          image type from the string
        //          so we simply assume a single
        //          channel unsigned char image
        Depth = IPL_DEPTH_8U;
        NumOfChannels = 1;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            WriteImageToFileStorage
//
// ARGUMENTS:           FileStorage
//                      SrcImage
//                      ImageTag
//
// TEMPLATE ARGUMENTS:  None
//
// PURPOSE:             Serialize an image and write it to a
//                      file storage using a specified image
//                      tag
//
// DEPENDENCIES:        IplImage
//-------------------------------------------------------------------
inline void WriteImageToFileStorage(CvFileStorage* FileStorage,
                                    const IplImage* SrcImage,
                                    const string& ImageTag)
{
    // Here we check if the
    // file storage is NULL
    if(FileStorage == NULL)
    {
        // Error -- File storage used to
        //          store image into is
        //          not open
        return;
    }

    // Now we check if the image
    // we're supposed to save is
    // NULL
    if(SrcImage == NULL)
    {
        // Error -- Tried to save an image
        //          to a file storage but
        //          the image is NULL
        return;
    }

    // First:       Create the image node
    cvStartWriteStruct(FileStorage,ImageTag.c_str(),CV_NODE_MAP,"opencv-image");

    // Second:      Store the width of the image
    cvWriteInt(FileStorage,"width",SrcImage->width);

    // Third:       Store the height of the image
    cvWriteInt(FileStorage,"height",SrcImage->height);

    // Fourth:      Store whether the image's
    //              origin is top-left or
    //              bottom-left
    if(SrcImage->origin == 0)
        cvWriteString(FileStorage,"origin","top-left");
    else
        cvWriteString(FileStorage,"origin","bottom-left");

    // Fifth:       Store whether the image
    //              is interleaved or has
    //              separate color channels
    if(SrcImage->dataOrder == 0)
        cvWriteString(FileStorage,"layout","interleaved");
    else
        cvWriteString(FileStorage,"layout","separate");

    // Sixth:       Now we have to store the
    //              type of image, meaning the
    //              number of channels and its
    //              depth
    string TypeOfImage = DetermineTypeOfImage(SrcImage);
    cvWriteString(FileStorage,"dt",TypeOfImage.c_str());

    // Seventh:     Finally we get to the data
    //              here we write the data
    //              as a string
    cvStartWriteStruct(FileStorage,"data",CV_NODE_SEQ,NULL);
    cvWriteRawData(FileStorage,SrcImage->imageData,SrcImage->height*SrcImage->width,TypeOfImage.c_str());
    cvEndWriteStruct(FileStorage);

    // Eighth:      Here we simply close
    //              the image node we
    //              opened earlier
    cvEndWriteStruct(FileStorage);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            ReadImageFromFileStorage
//
// ARGUMENTS:           FileStorage
//                      SrcImage
//                      ImageTag
//
// TEMPLATE ARGUMENTS:  None
//
// PURPOSE:             Read an image from a file storage using
//                      a specified image tag, and unserialize
//                      it and store it into an IplImage
//
// DEPENDENCIES:        IplImage
//-------------------------------------------------------------------
inline void ReadImageFromFileStorage(CvFileStorage* FileStorage,
                                     CvFileNode* Map,
                                     IplImage*& DstImage,
                                     const string& ImageTag)
{
    // Here we check if the
    // file storage is opened
    if(FileStorage == NULL)
    {
        // Error -- File storage used to
        //          read image from is
        //          not open
        return;
    }

    CvFileNode* ImageNode = NULL;

    // We check if we're inside the
    // blImage node that contains
    // our image
    string ImageNodeName = "";
    if(Map != NULL)
        ImageNodeName = cvGetFileNodeName(Map);

    // If we're not in the image
    // node, we search for one
    if(ImageNodeName != ImageTag)
    {
        ImageNode = cvGetFileNodeByName(FileStorage,Map,ImageTag.c_str());

        // If we failed to find a node, then
        // we simply quit since the file has
        // to be the wrong format
        if(ImageNode == NULL)
        {
            // Error -- We failed to
            //          find a node that
            //          contains an image,
            //          the file format is
            //          wrong
            return;
        }
    }
    else
        ImageNode = Map;

    CvFileNode* CurrentFileNode = NULL;

    // Find the width node
    CurrentFileNode = cvGetFileNodeByName(FileStorage,ImageNode,"width");
    if(CurrentFileNode == NULL)
    {
        // Error -- We failed to find
        //          the width node
        return;
    }

    // Read the width of the image
    int Cols = cvReadInt(CurrentFileNode);

    // Find the height node
    CurrentFileNode = cvGetFileNodeByName(FileStorage,ImageNode,"height");
    if(CurrentFileNode == NULL)
    {
        // Error -- We failed to find
        //          the height node
        return;
    }

    // Read the height of the image
    int Rows = cvReadInt(CurrentFileNode);

    // Find the type of image node
    CurrentFileNode = cvGetFileNodeByName(FileStorage,ImageNode,"dt");
    if(CurrentFileNode == NULL)
    {
        // Error -- We failed to find
        //          the type of image node
        return;
    }

    // Read the type of data
    string TypeOfImage = cvReadString(CurrentFileNode,"u");

    // From the TypeOfImage string, we
    // determine the depth and number
    // of channels of the image
    int Depth,NumOfChannels;
    DetermineDepthAndNumOfChannels(TypeOfImage,Depth,NumOfChannels);

    // Find the data node
    CurrentFileNode = cvGetFileNodeByName(FileStorage,ImageNode,"data");
    if(CurrentFileNode == NULL)
    {
        // Error -- We failed to find
        //          the data node
        return;
    }

    // Before we read the data, we check
    // if the image already exists, and if
    // it does, we release it before creating
    // the new one
    if(DstImage != NULL)
    {
        cvReleaseImage(&DstImage);
        DstImage = NULL;
    }

    // Now we create the destination image
    DstImage = cvCreateImage(cvSize(Cols,Rows),Depth,NumOfChannels);

    // Check if the destination image
    // was successfully created
    if(DstImage == NULL)
    {
        // Error -- Failed to make a temporary
        //          image and thus we quite
        return;
    }

    // Here we let opencv
    // load the date into it
    cvReadRawData(FileStorage,CurrentFileNode,DstImage->imageData,TypeOfImage.c_str());

    // We're done
    return;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            ReadImageFromFileStorage
//
// ARGUMENTS:           FileStorage
//                      SrcImage
//                      ImageTag
//
// TEMPLATE ARGUMENTS:  blType
//
// PURPOSE:             This function, just like the one above,
//                      reads an image from a file storage using
//                      a specified image tag, but unserializes it
//                      and stores it inside a blImage of a specified
//                      type, where the type is assumed by the
//                      type of the blImage
//
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType>
inline void ReadImageFromFileStorage(CvFileStorage* FileStorage,
                                     CvFileNode* Map,
                                     blImage<blType>& DstImage,
                                     const string& ImageTag)
{
    // First we load the image into
    // a generic IplImage pointer
    IplImage* Img = NULL;
    ReadImageFromFileStorage(FileStorage,Map,Img,ImageTag);

    // We then simply wrap the
    // loaded image by the
    // DstImage blImage passed to
    // this function
    DstImage.WrapIplImage(Img);
}
//-------------------------------------------------------------------


#endif // BL_IMAGESERIALIZATION_HPP
