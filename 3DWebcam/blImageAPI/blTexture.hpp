#ifndef BL_TEXTURE_HPP
#define BL_TEXTURE_HPP


//-------------------------------------------------------------------
// FILE:            blTexture.hpp
// CLASS:           blTexture
// BASE CLASS:      None
// PURPOSE:         A wrapper for an opengl texture I wrote that
//                  generate textures from opencv IplImage or
//                  from image files
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    blImage -- To load opencv images to opengl textures
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
class blTexture
{
public: // Constructors and destructors

    // Default constructor
    blTexture();

    // Copy constructor
    blTexture(const blTexture& Texture);

    // Destructor
    ~blTexture()
    {
        // Release the texture
        this->ReleaseTexture();
    }

public: // Public functions

    // Function used to check whether the texture we're
    // holding is a valid openGL texture object
    const bool                      IsTexture()const;

    // Function used to get the actual texture
    const GLuint&                   GetTexture()const;

    // Function used to release the texture object
    void                            ReleaseTexture();

    // Functions used to get
    // the texture size
    const int                       GetWidth()const;
    const int                       GetHeight()const;

    // Function used to create an empty the texture
    template<typename blType>
    void                            CreateEmptyTexture(const int& Width,
                                                       const int& Height);

    // Function used to load an
    // image to a texture
    template<typename blType>
    void    LoadImageToTexture(const blImage<blType>& Image,
                               const bool& IsTextureToBeReused,
                               const bool& AreMipmapsToBeGenerated);

    // Function used to load an image
    // from a file to a texture
    template<typename blType>
    void    LoadImageFromFileToTexture(const string& Filename,
                                       const bool& IsTextureToBeReused,
                                       const bool& AreMipmapsToBeGenerated,
                                       const bool& ShouldImageBeFlipped = false,
                                       const int& ImageFlipMode = 0,
                                       const int& HowToReadImageColorAndDepth =
                                       CV_LOAD_IMAGE_ANYDEPTH |
                                       CV_LOAD_IMAGE_ANYCOLOR);

    // Function used to load
    // a texture to an image
    template<typename blType>
    void    SaveTextureToImage(blImage<blType>& Image,
                               const GLint& Level = 0);

    // Function used to save
    // a texture to a file
    template<typename blType>
    void    SaveTextureToFile(const string& Filepath,
                              const GLint& Level = 0);

    // Functions used to render to
    // this texture object
    void                            CopyBufferToThisTexture();

protected: // Protected variables

    // Texture object
    GLuint                          m_Texture;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blTexture::blTexture()
{
    // Default the texture object to 0
    m_Texture = 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blTexture::blTexture(const blTexture& Texture)
{
    // Copy the texture object
    m_Texture = Texture.GetTexture();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blTexture::ReleaseTexture()
{
   // Delete the texture if it has been created
    if(glIsTexture(m_Texture))
        glDeleteTextures(1,&m_Texture);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const bool blTexture::IsTexture()const
{
    return glIsTexture(m_Texture);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const GLuint& blTexture::GetTexture()const
{
    return m_Texture;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blTexture::CreateEmptyTexture(const int& Width,
                                          const int& Height)
{
    // Let's create an empty image of the correct size
    blImage< blColor3<float> > TempImage(Height,Width);

    // Let's create the texture from the image
    LoadImageToTexture<blType>(TempImage,true,false);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blTexture::SaveTextureToFile(const string& Filepath,
                                         const GLint& Level)
{
    // We first create a temporary
    // image and save this texture
    // to that image
    blImage<blType> TempImage;
    this->SaveTextureToImage(TempImage,Level);

    // Finally we save the image
    // at the file path passed to
    // this function
    cvSaveImage(Filepath.c_str(),TempImage);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blTexture::SaveTextureToImage(blImage<blType>& Image,
                                          const GLint& Level)
{
    // Step 1:  Check if this texture
    //          is a valid texture object
    if(!glIsTexture(m_Texture))
    {
        // Error -- This texture is
        //          not a valid texture
        //          object, so we simply
        //          quit
        return;
    }

    // Step 2:  Bind to this texture object
    glBindTexture(GL_TEXTURE_2D,m_Texture);

    // Step 3:  Determine the pixel format
    GLenum ChannelFormat;

    switch(Image.GetNumOfChannels())
    {
    case 1:
        ChannelFormat = GL_LUMINANCE;
        break;
    case 3:
        ChannelFormat = GL_BGR_EXT;
        break;
    case 4:
        ChannelFormat = GL_BGRA_EXT;
        break;
    default:
        // Error -- Did not understand format, so we
        //          default the format to GL_LUMINANCE
        ChannelFormat = GL_LUMINANCE;
        break;
    }

    // Step 4:  Determine the depth format
    GLenum DepthFormat;

    switch(Image.GetDepth())
    {
    case IPL_DEPTH_8U:
        DepthFormat = GL_UNSIGNED_BYTE;
        break;
    case IPL_DEPTH_8S:
        DepthFormat = GL_BYTE;
        break;
    case IPL_DEPTH_16U:
        DepthFormat = GL_UNSIGNED_SHORT;
        break;
    case IPL_DEPTH_16S:
        DepthFormat = GL_SHORT;
        break;
    case IPL_DEPTH_32S:
        DepthFormat = GL_INT;
        break;
    case IPL_DEPTH_32F:
        DepthFormat = GL_FLOAT;
        break;
    case IPL_DEPTH_64F:
        // Error -- Most GPUs don't support double and so
        //          the texture won't load, for this I
        //          make the depth format float.  Note that
        //          the texture will be streched
        DepthFormat = GL_FLOAT;
        break;
    default:
        // Error -- Did not understand format, so we
        //          default the format to GL_UNSIGNED_BYTE
        DepthFormat = GL_UNSIGNED_BYTE;
        break;
    }

    // Step 5:  We need to make sure that
    //          the image is the correct size
    if(Image.size1() != this->GetHeight() ||
       Image.size2() != this->GetWidth())
    {
        Image.CreateImage(this->GetHeight(),
                          this->GetWidth());
    }

    // Step 6:  Finally we get the image data
    glGetTexImage(GL_TEXTURE_2D,
                  Level,
                  ChannelFormat,
                  DepthFormat,
                  Image[0]);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blTexture::LoadImageToTexture(const blImage<blType>& Image,
                                          const bool& IsTextureToBeReused,
                                          const bool& AreMipmapsToBeGenerated)
{
    // Set the color format depending on
    // the number of channels of the image
    GLenum ChannelFormat;

    switch(Image.GetNumOfChannels())
    {
    case 1:
        ChannelFormat = GL_LUMINANCE;
        break;
    case 3:
        ChannelFormat = GL_BGR_EXT;
        break;
    case 4:
        ChannelFormat = GL_BGRA_EXT;
        break;
    default:
        // Error -- Did not understand format, so we
        //          default the format to GL_LUMINANCE
        ChannelFormat = GL_LUMINANCE;
        break;
    }

    // Set the format of the data
    // depending on the image depth
    GLenum DepthFormat;

    switch(Image.GetDepth())
    {
    case IPL_DEPTH_8U:
        DepthFormat = GL_UNSIGNED_BYTE;
        break;
    case IPL_DEPTH_8S:
        DepthFormat = GL_BYTE;
        break;
    case IPL_DEPTH_16U:
        DepthFormat = GL_UNSIGNED_SHORT;
        break;
    case IPL_DEPTH_16S:
        DepthFormat = GL_SHORT;
        break;
    case IPL_DEPTH_32S:
        DepthFormat = GL_INT;
        break;
    case IPL_DEPTH_32F:
        DepthFormat = GL_FLOAT;
        break;
    case IPL_DEPTH_64F:
        // Error -- Most GPUs don't support double and so
        //          the texture won't load, for this I
        //          make the depth format float.  Note that
        //          the texture will be streched
        DepthFormat = GL_FLOAT;
        break;
    default:
        // Error -- Did not understand format, so we
        //          default the format to GL_UNSIGNED_BYTE
        DepthFormat = GL_UNSIGNED_BYTE;
        break;
    }

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Temporary texture object
    GLuint TempTexture;

    // If the current texture object is NOT to
    // be reused we have to generate another one
    if(!IsTextureToBeReused || m_Texture == 0)
    {
        // If the current texture object
        // is a valid texture then delete it
        if(glIsTexture(m_Texture))
            glDeleteTextures(1,&m_Texture);

        // Now we generate the new texture object
        glGenTextures(1,&m_Texture);
    }
    else
    {
        // Now we know that we want to reuse the
        // current texture object, but if it's
        // not a valid texture object, then we
        // have to generate a new one anyway
        if(!glIsTexture(m_Texture))
            glGenTextures(1,&m_Texture);
    }

    // Bind to the texture object
    glBindTexture(GL_TEXTURE_2D,m_Texture);

    // Set the texturing environment variables
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glPixelStorei(GL_UNPACK_ALIGNMENT,2);
    glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    // If asked to, here we generate the mipmaps
    if(AreMipmapsToBeGenerated)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D,
                          GL_RGBA,
                          Image.size2(),
                          Image.size1(),
                          ChannelFormat,
                          DepthFormat,
                          Image.GetImageDataCast());
    }
    else
    {
        if(IsTextureToBeReused)
        {
            // Make sure that the texture has the
            // correct size depth and channel formats
            int Width;
            int Height;
            int nChannels;
            int depth;
            GLint TextureDepthFormat;
            GLint TextureChannelFormat;

            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&Width);
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&Height);
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_DEPTH,&TextureDepthFormat);
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_INTERNAL_FORMAT,&TextureChannelFormat);

            // Get the texture channel format
            switch(TextureChannelFormat)
            {
            case GL_LUMINANCE:
                nChannels = 1;
                break;
            case GL_BGR_EXT:
                nChannels = 3;
                break;
            case GL_BGRA_EXT:
                nChannels = 4;
                break;
            default:
                nChannels = 1;
                break;
            }

            // Get the texture depth format
            switch(TextureDepthFormat)
            {
            case GL_UNSIGNED_BYTE:
                depth = IPL_DEPTH_8U;
                break;
            case IPL_DEPTH_8S:
                depth = GL_BYTE;
                break;
            case GL_UNSIGNED_SHORT:
                depth = IPL_DEPTH_16U;
                break;
            case GL_SHORT:
                depth = IPL_DEPTH_16S;
                break;
            case GL_INT:
                depth = IPL_DEPTH_32S;
                break;
            case GL_FLOAT:
                depth = IPL_DEPTH_32F;
                break;
            case GL_DOUBLE:
                // I know most GPUs don't support double, but
                // i'm leaving this in just in case
                depth = IPL_DEPTH_64F;
                break;
            default:
                depth = IPL_DEPTH_8U;
                break;
            }

            if(Width == Image.size2() &&
               Height == Image.size1() &&
               depth == Image.GetDepth() &&
               nChannels == Image.GetNumOfChannels())
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexSubImage2D(GL_TEXTURE_2D,
                                0,0,0,
                                Width,
                                Height,
                                ChannelFormat,
                                DepthFormat,
                                Image.GetImageDataCast());
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_RGBA,
                             Image.size2(),
                             Image.size1(),
                             0,
                             ChannelFormat,
                             DepthFormat,
                             Image.GetImageDataCast());
            }
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         Image.size2(),
                         Image.size1(),
                         0,
                         ChannelFormat,
                         DepthFormat,
                         Image.GetImageDataCast());
        }
    }

    return;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const int blTexture::GetWidth()const
{
    if(glIsTexture(m_Texture))
    {
        glBindTexture(GL_TEXTURE_2D,m_Texture);
        int Width;
        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&Width);
        return Width;
    }
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const int blTexture::GetHeight()const
{
    if(glIsTexture(m_Texture))
    {
        glBindTexture(GL_TEXTURE_2D,m_Texture);
        int Height;
        glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&Height);
        return Height;
    }
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline void blTexture::LoadImageFromFileToTexture(const string& Filename,
                                                  const bool& IsTextureToBeReused,
                                                  const bool& AreMipmapsToBeGenerated,
                                                  const bool& ShouldImageBeFlipped,
                                                  const int& ImageFlipMode,
                                                  const int& HowToReadImageColorAndDepth)
{
    // Let's create an empty image
    blImage<blType> Image;

    // Let's try to load the image
    // from the specified location
    Image.LoadImageFromFile(Filename,
                            ShouldImageBeFlipped,
                            ImageFlipMode);

    // Now that we have loaded an image
    // we just use the above function to
    // load it into this texture object
    LoadImageToTexture(Image,
                       IsTextureToBeReused,
                       AreMipmapsToBeGenerated);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blTexture::CopyBufferToThisTexture()
{
    // Check if the texture is valid
    if(!glIsTexture(m_Texture))
    {
        // Error -- Tried to render to
        //          a texture object
        //          without first
        //          initializing it
        return;
    }

    // Bind to the texture object
    glBindTexture(GL_TEXTURE_2D,m_Texture);

    // Store the rendered data
    // into this texture object
    glCopyTexSubImage2D(GL_TEXTURE_2D,      // Type of texture
                        0,                  // Level of detail (0 is base image)
                        0,                  // Texture x starting point
                        0,                  // Texture y starting point
                        0,                  // Window starting x coord
                        0,                  // Window starting y coord
                        this->GetWidth(),   // Width of image to copy
                        this->GetHeight()   // Height of image to copy
                       );
}
//-------------------------------------------------------------------


#endif // BL_TEXTURE_HPP
