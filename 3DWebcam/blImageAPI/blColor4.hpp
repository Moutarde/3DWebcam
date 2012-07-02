#ifndef BL_COLOR4_HPP
#define BL_COLOR4_HPP


//-------------------------------------------------------------------
// FILE:            blColor4.hpp
// CLASS:           blColor4
// BASE CLASS:      blColor3
// PURPOSE:         Based on blColor3, adds an alpha transparency value
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    blColor4
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
class blColor4 : public blColor3<blType>
{
public: // Constructors and destructors

    // Default constructor
    blColor4(const blType& DefaultValue = 0);

    // Constructor from four values
    blColor4(const blType& Blue,
             const blType& Green,
             const blType& Red,
             const blType& Alpha);

    // Copy constructor
    template<typename blType2>
    blColor4(const blColor4<blType2>& Color);

    // Construction from a CvScalar
    blColor4(const CvScalar& OpenCVColor);

    // Destructor
    ~blColor4()
    {
    }

public: // Overloaded operators

    const blColor4<blType>  operator-()const;
    blColor4<blType>&       operator+=(const blColor4<blType>& Color);
    blColor4<blType>&       operator-=(const blColor4<blType>& Color);
    blColor4<blType>&       operator*=(const blType& Scalar);
    blColor4<blType>&       operator/=(const blType& Scalar);

    const blColor4<blType>  operator+(const blColor4<blType>& Color)const;
    const blColor4<blType>  operator-(const blColor4<blType>& Color)const;
    const blColor4<blType>  operator*(const blType& Scalar)const;
    const blColor4<blType>  operator/(const blType& Scalar)const;

    const blColor4<blType>  operator*(const blColor4<blType>& Color)const;
    const blColor4<blType>  operator/(const blColor4<blType>& Color)const;

public: // Public functions

    // Functions used to convert this color
    // to a CvScalar
    operator CvScalar()
    {
        CvScalar Color;
        Color.val[0] = this->m_Blue;
        Color.val[1] = this->m_Green;
        Color.val[2] = this->m_Red;
        Color.val[3] = m_Alpha;
        return Color;
    }
    operator const CvScalar()const
    {
        CvScalar Color;
        Color.val[0] = this->m_Blue;
        Color.val[1] = this->m_Green;
        Color.val[2] = this->m_Red;
        Color.val[3] = m_Alpha;
        return Color;
    }

public: // Public variables

    // The alpha component
    blType              m_Alpha;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor4<blType>::blColor4(const blType& DefaultValue)
                                  : blColor3<blType>(DefaultValue)
{
    m_Alpha = 1;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor4<blType>::blColor4(const blType& Blue,
                                  const blType& Green,
                                  const blType& Red,
                                  const blType& Alpha)
                                  : blColor3<blType>(Blue,Green,Red)
{
    m_Alpha = Alpha;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
template<typename blType2>
inline blColor4<blType>::blColor4(const blColor4<blType2>& Color)
                                  : blColor3<blType>(Color)
{
    m_Alpha = Color.m_Alpha;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor4<blType>::blColor4(const CvScalar& OpenCVColor)
                                  : blColor3<blType>(OpenCVColor)
{
    m_Alpha = OpenCVColor.val[3];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor4<blType> blColor4<blType>::operator-()const
{
    return blColor4<blType>(-this->m_Blue,
                            -this->m_Green,
                            -this->m_Red,
                            -this->m_Alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor4<blType>& blColor4<blType>::operator+=(const blColor4<blType>& Color)
{
    this->m_Blue += Color.m_Blue;
    this->m_Green += Color.m_Green;
    this->m_Red += Color.m_Red;
    m_Alpha += Color.m_Alpha;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor4<blType>& blColor4<blType>::operator-=(const blColor4<blType>& Color)
{
    this->m_Blue -= Color.m_Blue;
    this->m_Green -= Color.m_Green;
    this->m_Red -= Color.m_Red;
    m_Alpha -= Color.m_Alpha;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor4<blType>& blColor4<blType>::operator*=(const blType& Scalar)
{
    this->m_Blue *= Scalar;
    this->m_Green *= Scalar;
    this->m_Red *= Scalar;
    m_Alpha *= Scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline blColor4<blType>& blColor4<blType>::operator/=(const blType& Scalar)
{
    this->m_Blue /= Scalar;
    this->m_Green /= Scalar;
    this->m_Red /= Scalar;
    m_Alpha /= Scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor4<blType> blColor4<blType>::operator+(const blColor4<blType>& Color)const
{
    return blColor4<blType>(this->m_Blue + Color.m_Blue,
                            this->m_Green + Color.m_Green,
                            this->m_Red + Color.m_Red,
                            m_Alpha + Color.m_Alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor4<blType> blColor4<blType>::operator-(const blColor4<blType>& Color)const
{
    return blColor4<blType>(this->m_Blue - Color.m_Blue,
                            this->m_Green - Color.m_Green,
                            this->m_Red - Color.m_Red,
                            m_Alpha - Color.m_Alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor4<blType> blColor4<blType>::operator*(const blColor4<blType>& Color)const
{
    return blColor4<blType>(this->m_Blue * Color.m_Blue,
                            this->m_Green * Color.m_Green,
                            this->m_Red * Color.m_Red,
                            m_Alpha * Color.m_Alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor4<blType> blColor4<blType>::operator/(const blColor4<blType>& Color)const
{
    return blColor4<blType>(this->m_Blue / Color.m_Blue,
                            this->m_Green / Color.m_Green,
                            this->m_Red / Color.m_Red,
                            m_Alpha / Color.m_Alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor4<blType> blColor4<blType>::operator*(const blType& Scalar)const
{
    return blColor4<blType>(this->m_Blue * Scalar,
                            this->m_Green * Scalar,
                            this->m_Red * Scalar,
                            m_Alpha * Scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline const blColor4<blType> blColor4<blType>::operator/(const blType& Scalar)const
{
    return blColor4<blType>(this->m_Blue / Scalar,
                            this->m_Green / Scalar,
                            this->m_Red / Scalar,
                            m_Alpha / Scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline ostream& operator<<(ostream& os,const blColor4<blType>& Color)
{
    os << Color.m_Blue << " ";
    os << Color.m_Green << " ";
    os << Color.m_Red << " ";
    os << Color.m_Alpha;
    return os;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline istream& operator>>(istream& is,blColor4<blType>& Color)
{
    is >> Color.m_Blue;
    is >> Color.m_Green;
    is >> Color.m_Red;
    is >> Color.m_Alpha;

    return is;
}
//-------------------------------------------------------------------


#endif // BL_COLOR4_HPP
