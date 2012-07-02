#ifndef BL_COLOR3_HPP
#define BL_COLOR3_HPP


//-------------------------------------------------------------------
// FILE:            blColor3.hpp
// CLASS:           blColor3
// BASE CLASS:      None
// PURPOSE:         A simple and efficient color structure of three
//                  components saved in a Blue,Green,Red sequence
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    CvScalar -- From opencv library
// NOTES:
// DATE CREATED:    May/31/2010
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
template<typename blType>
class blColor3
{
public: // Constructors and destructors

    // Default constructor
    blColor3(const blType& DefaultValue = 0);

    // Constructor from three values
    blColor3(const blType& Blue,
             const blType& Green,
             const blType& Red);

    // Copy constructor
    template<typename blType2>
    blColor3(const blColor3<blType2>& Color);

    // Construction from a CvScalar
    blColor3(const CvScalar& OpenCVColor);

    // Destructor
    ~blColor3()
    {
    }

public: // Overloaded operators

    const blColor3<blType>  operator-()const;
    blColor3<blType>&       operator+=(const blColor3<blType>& Color);
    blColor3<blType>&       operator-=(const blColor3<blType>& Color);
    blColor3<blType>&       operator*=(const blType& Scalar);
    blColor3<blType>&       operator/=(const blType& Scalar);

    const blColor3<blType>  operator+(const blColor3<blType>& Color)const;
    const blColor3<blType>  operator-(const blColor3<blType>& Color)const;
    const blColor3<blType>  operator*(const blType& Scalar)const;
    const blColor3<blType>  operator/(const blType& Scalar)const;

    const blColor3<blType>  operator*(const blColor3<blType>& Color)const;
    const blColor3<blType>  operator/(const blColor3<blType>& Color)const;

public: // Public functions

    // Functions used to convert this color
    // to a CvScalar
    operator CvScalar()
    {
        return CV_RGB(m_Red,m_Green,m_Blue);
    }
    operator const CvScalar()const
    {
        return CV_RGB(m_Red,m_Green,m_Blue);
    }

public: // Public variables

    // The color components
    blType              m_Blue;
    blType              m_Green;
    blType              m_Red;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor3<blType>::blColor3(const blType& DefaultValue)
{
    m_Blue = DefaultValue;
    m_Green = DefaultValue;
    m_Red = DefaultValue;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor3<blType>::blColor3(const blType& Blue,
                                  const blType& Green,
                                  const blType& Red)
{
    m_Blue = Blue;
    m_Green = Green;
    m_Red = Red;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
template<typename blType2>
inline blColor3<blType>::blColor3(const blColor3<blType2>& Color)
{
    m_Blue = blType(Color.m_Blue);
    m_Green = blType(Color.m_Green);
    m_Red = blType(Color.m_Red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor3<blType>::blColor3(const CvScalar& OpenCVColor)
{
    m_Blue = OpenCVColor.val[0];
    m_Green = OpenCVColor.val[1];
    m_Red = OpenCVColor.val[2];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor3<blType> blColor3<blType>::operator-()const
{
    return blColor3<blType>(-m_Blue,-m_Green,-m_Red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor3<blType>& blColor3<blType>::operator+=(const blColor3<blType>& Color)
{
    m_Blue += Color.m_Blue;
    m_Green += Color.m_Green;
    m_Red += Color.m_Red;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor3<blType>& blColor3<blType>::operator-=(const blColor3<blType>& Color)
{
    m_Blue -= Color.m_Blue;
    m_Green -= Color.m_Green;
    m_Red -= Color.m_Red;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor3<blType>& blColor3<blType>::operator*=(const blType& Scalar)
{
    m_Blue *= Scalar;
    m_Green *= Scalar;
    m_Red *= Scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor3<blType>& blColor3<blType>::operator/=(const blType& Scalar)
{
    m_Blue /= Scalar;
    m_Green /= Scalar;
    m_Red /= Scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor3<blType> blColor3<blType>::operator+(const blColor3<blType>& Color)const
{
    return blColor3<blType>(m_Blue + Color.m_Blue,
                            m_Green + Color.m_Green,
                            m_Red + Color.m_Red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor3<blType> blColor3<blType>::operator-(const blColor3<blType>& Color)const
{
    return blColor3<blType>(m_Blue - Color.m_Blue,
                            m_Green - Color.m_Green,
                            m_Red - Color.m_Red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor3<blType> blColor3<blType>::operator*(const blColor3<blType>& Color)const
{
    return blColor3<blType>(m_Blue * Color.m_Blue,
                            m_Green * Color.m_Green,
                            m_Red * Color.m_Red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor3<blType> blColor3<blType>::operator/(const blColor3<blType>& Color)const
{
    return blColor3<blType>(m_Blue / Color.m_Blue,
                            m_Green / Color.m_Green,
                            m_Red / Color.m_Red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor3<blType> blColor3<blType>::operator*(const blType& Scalar)const
{
    return blColor3<blType>(m_Blue * Scalar,
                            m_Green * Scalar,
                            m_Red * Scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor3<blType> blColor3<blType>::operator/(const blType& Scalar)const
{
    return blColor3<blType>(m_Blue / Scalar,
                            m_Green / Scalar,
                            m_Red / Scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline ostream& operator<<(ostream& os,const blColor3<blType>& Color)
{
    os << Color.m_Blue << " ";
    os << Color.m_Green << " ";
    os << Color.m_Red;
    return os;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline istream& operator>>(istream& is,blColor3<blType>& Color)
{
    is >> Color.m_Blue;
    is >> Color.m_Green;
    is >> Color.m_Red;

    return is;
}
//-------------------------------------------------------------------


#endif // BL_COLOR3_HPP
