#ifndef BL_IMAGEOPERATORS_HPP
#define BL_IMAGEOPERATORS_HPP


//-------------------------------------------------------------------
// FILE:            blImageOperators.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of overloaded operators and functions
//                  I developed to handle images just like matrices, so
//                  as to make code very readable
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blImage -- To represent the matrix data
//                  cvTranspose -- Calculate the transpose
//                  cvAdd -- Add two matrices
//                  cvSub -- Subtract two matrices
//                  cvScale -- Multiply a matrix by a scalar
//                  cvGEMM -- Multiply two matrices
//                  cvPseudoInverse -- Calculate the pseudo-inverse
//                  cvAbsDiff -- Calculate the absolute difference
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
// FUNCTION:            operator<<
// ARGUMENTS:           os,M
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Output the image matrix to a stream
// DEPENDENCIES:        blImage
//                      iostream
//-------------------------------------------------------------------
template<typename blType>
inline ostream& operator<<(ostream& os,const blImage<blType>& M)
{
    // We output the matrix using
    // a super simple format:
    // Rows
    // Cols
    // Data
    os << M.size1() << "\n";
    os <<  M.size2() << "\n";

    for(int i = 0; i < M.size1(); ++i)
        for(int j = 0; j < M.size2(); ++j)
            os << M[i][j] << " ";

    os << "\n";

    return os;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator>>
// ARGUMENTS:           is,M
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Input the image from is to M
// DEPENDENCIES:        blImage
//                      iostream
//-------------------------------------------------------------------
template<typename blType>
inline istream& operator>>(istream& is,blImage<blType>& M)
{
    // Here we assume we're at the
    // point where we can simply read
    // the image following our super simple
    // format
    int Rows,Cols;

    if(!(is >> Rows))
    {
        // Error -- Could not read the
        //          number of rows
        return is;
    }

    if(!(is >> Cols))
    {
        // Error -- Could not read the
        //          number of columns
        return is;
    }

    // Here we size the image
    M.CreateImage(Rows,Cols);

    blType Value;

    // Now we read the data points
    for(int i = 0; i < Rows; ++i)
    {
        for(int j = 0; j < Cols; ++j)
        {
            if(!(is >> Value))
            {
                // Error -- Could not read
                //          data points
                return is;
            }
            M[i][j] = Value;
        }
    }

    return is;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator-
// ARGUMENTS:           M
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             The negative of a matrix M
// DEPENDENCIES:        blImage
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> operator-(const blImage<blType>& M)
{
    // Get the matrix size
    int Rows = M.size1();
    int Cols = M.size2();

    // Create the resulting matrix
    blImage<blType> Result(Rows,Cols);

    // Loop through and negate all values
    for(int i = 0; i < Rows; ++i)
        for(int j = 0; j < Cols; ++j)
            Result[i][j] = -M[i][j];

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            Transpose
// ARGUMENTS:           M
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Transpose of matrix M
// DEPENDENCIES:        blImage
//                      cvTranspose
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> Transpose(const blImage<blType>& M)
{
    // Create the resulting transposed matrix
    blImage<blType> Result(M.size2(),M.size1());

    // Calculate the transpose
    cvTranspose(M,Result);

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            inv
// ARGUMENTS:           M
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Calculate the inverse or pseudo-inverse
//                      of the passed matrix
// DEPENDENCIES:        blImage
//                      cvPseudoInverse
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> inv(const blImage<blType>& M)
{
    // Create the result matrix
    blImage<blType> Result(M.size2(),M.size1());

    // Invert the matrix
    cvPseudoInverse(M,Result);

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator+
// ARGUMENTS:           M1,M2
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Add two matrices (images) using OpenCV
// DEPENDENCIES:        blImage
//                      cvAdd
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> operator+(const blImage<blType>& M1,
                                 const blImage<blType>& M2)
{
    // Check the sizes otherwise return M1
    if(M1.size1() != M2.size1() && M1.size2() != M2.size2())
    {
        // Error
        return M1;
    }

    // If we got this far it is because the matrices are
    // the correct size so we just let OpenCV add them together
    blImage<blType> Result(M1.size1(),M1.size2());
    cvAdd(M1,M2,Result);

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator+
// ARGUMENTS:           M1,M2
// TEMPLATE ARGUMENTS:  blType,bltype2
// PURPOSE:             Add two matrices (images) using OpenCV with
//                      the two matrices being of different type
// DEPENDENCIES:        blImage
//                      operator+
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline blImage<blType> operator+(const blImage<blType>& M1,
                                 const blImage<blType2>& M2)
{
    // Check the sizes otherwise return M1
    if(M1.size1() != M2.size1() && M1.size2() != M2.size2())
    {
        // Error
        return M1;
    }

    // Copy the different type image
    blImage<blType> M2newtype = M2;

    // Now add the two matrices
    return (M1 + M2newtype);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator-
// ARGUMENTS:           M1,M2
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Subtract two matrices (images) using OpenCV
// DEPENDENCIES:        blImage
//                      cvSub
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> operator-(const blImage<blType>& M1,
                                 const blImage<blType>& M2)
{
    // Check the sizes otherwise return M1
    if(M1.size1() != M2.size1() && M1.size2() != M2.size2())
    {
        // Error
        return M1;
    }

    // If we got this far it is because the matrices are
    // the correct size so we just let OpenCV add them together
    blImage<blType> Result(M1.size1(),M1.size2());
    cvSub(M1,M2,Result);

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator-
// ARGUMENTS:           M1,M2
// TEMPLATE ARGUMENTS:  blType,bltype2
// PURPOSE:             Subtract two matrices (images) using OpenCV
//                      with the two matrices being of different type
// DEPENDENCIES:        blImage
//                      operator-
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline blImage<blType> operator-(const blImage<blType>& M1,
                                 const blImage<blType2>& M2)
{
    // Check the sizes otherwise return M1
    if(M1.size1() != M2.size1() && M1.size2() != M2.size2())
    {
        // Error
        return M1;
    }

    // Copy the different type image
    blImage<blType> M2newtype = M2;

    // Now add the two matrices
    return (M1 - M2newtype);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator*
// ARGUMENTS:           M1,M2
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Generalized multiplication of two
//                      matrices M1 and M2 using OpenCV
// DEPENDENCIES:        blImage
//                      cvScale
//                      cvGEMM
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> operator*(const blImage<blType>& M1,
                                 const blImage<blType>& M2)
{
    // First we get the sizes of the matrices
    int Rows1 = M1.size1();
    int Cols1 = M1.size2();
    int Rows2 = M2.size1();
    int Cols2 = M2.size2();

    // Let's check the sizes and make sure that the
    // matrices can indeed be multiplied together
    if(Cols1 != Rows2)
    {
        // This means that the matrices don't have the
        // correct sizes for matrix multiplication, so
        // let's check if one of them is a scalar
        if(Rows1 == Cols1 && Rows1 == 1)
        {
            blImage<blType> Result(Rows2,Cols2);
            cvScale(M2,Result,M1(0,0));

            return Result;
        }
        else if(Rows2 == Cols2 && Rows2 == 1)
        {
            blImage<blType> Result(Rows1,Cols1);
            cvScale(M1,Result,M2(0,0));

            return Result;
        }

        // If we got here then this means that the
        // matrices are the wrong sizes and that we
        // cannot perform multiplication, so we record
        // the error and return M1 as the result

        // Error
        return M1;
    }

    // If we make it to this point then this means that the
    // matrices are the correct sizes, so we just call OpenCV's
    // cvGEMM generalized matrix multiplication algorithm
    blImage<blType> Result(Rows1,Cols2);
    cvGEMM(M1,M2,1,NULL,0,Result);

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator*
// ARGUMENTS:           M1,M2
// TEMPLATE ARGUMENTS:  blType,blType2
// PURPOSE:             Generalized multiplication of two
//                      matrices M1 and M2 using OpenCV, but
//                      with M1 and M2 being of different type
// DEPENDENCIES:        blImage
//                      operator*
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline blImage<blType> operator*(const blImage<blType>& M1,
                                 const blImage<blType2>& M2)
{
    // Copy the different type image
    blImage<blType> M2newtype = M2;

    // Multiply the two images
    return (M1*M2newtype);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator*
// ARGUMENTS:           M1,Scalar
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Multiplication by a scalar with the
//                      scalar following the matrix
// DEPENDENCIES:        blImage
//                      cvScale
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> operator*(const blImage<blType>& M1,
                                 const blType& Scalar)
{
    // Create the resulting matrix
    blImage<blType> Result(M1.size1(),M1.size2());

    // Multiply the elements of the matrix by Scalar
    cvScale(M1,Result,Scalar);

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator*
// ARGUMENTS:           Scalar,M2
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Multiplication by a scalar with the
//                      scalar preceding the matrix
// DEPENDENCIES:        blImage
//                      cvScale
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> operator*(const blType& Scalar,
                                 const blImage<blType>& M2)
{
    // Create the resulting matrix
    blImage<blType> Result(M2.size1(),M2.size2());

    // Multiply the elements of the matrix by Scalar
    cvScale(M2,Result,Scalar);

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator/
// ARGUMENTS:           M1,M2
// TEMPLATE ARGUMENTS:  blType,blType2
// PURPOSE:             Multiply matrix M1 by inverse of matrix M2,
//                      where M1 and M2 could be of different type
// DEPENDENCIES:        blImage
//                      inv
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline blImage<blType> operator/(const blImage<blType>& M1,
                                 const blImage<blType2>& M2)
{
    // We divide two matrices by multiplying the first
    // matrix M1 by the inverse of the second matrix M2
    return (M1*inv(M2));
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator/
// ARGUMENTS:           M1,Scalar
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Division of a matrix by a scalar
// DEPENDENCIES:        blImage
//                      cvScale
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> operator/(const blImage<blType>& M1,
                                 const blType& Scalar)
{
    // Create the resulting matrix
    blImage<blType> Result(M1.size1(),M1.size2());

    // Divide the elements of the matrix by Scalar
    cvScale(M1,Result,blType(1)/Scalar);

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator/
// ARGUMENTS:           Scalar,M2
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Multiplication by a scalar with the
//                      scalar preceding the matrix
// DEPENDENCIES:        blImage
//                      inv
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> operator/(const blType& Scalar,
                                 const blImage<blType>& M2)
{
    // We multiply the scalar value to the inverse of M2
    return (Scalar*inv(M2));
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            AbsDiff
// ARGUMENTS:           Src1,Src2,Dst
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             The absolute difference of (Src2 - Src1)
//                      which is stored into the Dst Image
// DEPENDENCIES:        blImage
//                      cvAbsDiff
//-------------------------------------------------------------------
template<typename blType>
inline void AbsDiff(const blImage<blType>& Src1,
                    const blImage<blType>& Src2,
                    blImage<blType>& Dst)
{
    // Get the sizes of the images
    int Rows1 = Src1.size1();
    int Cols1 = Src1.size2();
    int Rows2 = Src2.size1();
    int Cols2 = Src2.size2();

    // Check if the two source images are of
    // the same size otherwise return
    if(Rows1 != Rows2 || Cols1 != Cols2)
    {
        // Error
        return;
    }

    // Check the size of the resulting matrix to
    // make sure it is (Rows1 x Cols1)
    if(Dst.size1() != Rows1 || Dst.size2() != Cols1)
        Dst = blImage<blType>(Rows1,Cols1);

    cvAbsDiff(Src1,Src2,Dst);
}
//-------------------------------------------------------------------


#endif // BL_IMAGEOPERATORS_HPP
