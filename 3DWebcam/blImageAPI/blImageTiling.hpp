#ifndef BL_IMAGETILING_HPP
#define BL_IMAGETILING_HPP


//-------------------------------------------------------------------
// FILE:            blImageTiling.hpp
// CLASS:           None
// BASE CLASS:      None
// PURPOSE:         A collection of functions used to augment
//                  images in various ways to make one "big"
//                  image.
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    blImage -- To represent the matrix data
//                  cvTranspose -- Calculate the transpose
//                  cvAdd -- Add two matrices
//                  cvSub -- Subtract two matrices
//                  cvScale -- Multiply a matrix by a scalar
//                  cvGEMM -- Multiply two matrices
//                  cvPseudoInverse -- Calculate the pseudo-inverse
//                  cvAbsDiff -- Calculate the absolute difference
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
// FUNCTION:            AugmentTwoMatricesRowWise
// ARGUMENTS:           M1,M2
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Creates a matrix augmenting two
//                      matrices M1 and M2 row wise
// DEPENDENCIES:        blImage
//                      std::max
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> AugmentTwoMatricesRowWise(const blImage<blType>& M1,
                                                 const blImage<blType>& M2)
{
    // Get the size of the two matrices
    int Rows1 = M1.size1();
    int Cols1 = M1.size2();
    int Rows2 = M2.size1();
    int Cols2 = M2.size2();

    // The ending size of the augmented matrix is going to be
    // (Rows1 + Rows2) x MaxCols
    int Rows = Rows1 + Rows2;
    int Cols = std::max(Cols1,Cols2);

    // Create the augmented matrix
    blImage<blType> Result(Rows,Cols);

    // Assign elements from Matrix M1
    for(int i = 0; i < Rows1; ++i)
        for(int j = 0; j < Cols1; ++j)
            Result[i][j] = M1[i][j];

    // Assign elements from Matrix M2
    for(int i = Rows1; i < Rows; ++i)
        for(int j = 0; j < Cols2; ++j)
            Result[i][j] = M2[i-Rows1][j];

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            AugmentTwoMatricesRowWise
// ARGUMENTS:           M1,M2,DstMatrix
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Creates a matrix augmenting two
//                      matrices M1 and M2 row wise and
//                      storing the result into DstMatrix
//                      (Could be faster than previous
//                      function if DstMatrix is already
//                      the correct size)
// DEPENDENCIES:        blImage
//                      std::max
//-------------------------------------------------------------------
template<typename blType>
inline void AugmentTwoMatricesRowWise(const blImage<blType>& M1,
                                      const blImage<blType>& M2,
                                      blImage<blType>& DstMatrix)
{
    // Get the size of the two matrices
    int Rows1 = M1.size1();
    int Cols1 = M1.size2();
    int Rows2 = M2.size1();
    int Cols2 = M2.size2();

    // The ending size of the augmented matrix is going to be
    // (Rows1 + Rows2) x MaxCols
    int Rows = Rows1 + Rows2;
    int Cols = std::max(Cols1,Cols2);

    // Let's make sure that DstMatrix is
    // already the correct size
    if(DstMatrix.size1() != Rows ||
       DstMatrix.size2() != Cols)
    {
        DstMatrix.CreateImage(Rows,Cols);
    }

    // Assign elements from Matrix M1
    for(int i = 0; i < Rows1; ++i)
        for(int j = 0; j < Cols1; ++j)
            DstMatrix[i][j] = M1[i][j];

    // Assign elements from Matrix M2
    for(int i = Rows1; i < Rows; ++i)
        for(int j = 0; j < Cols2; ++j)
            DstMatrix[i][j] = M2[i-Rows1][j];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            AugmentTwoMatricesColumnWise
// ARGUMENTS:           M1,M2
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Creates a matrix augmenting two
//                      matrices M1 and M2 column wise
// DEPENDENCIES:        blImage
//                      std::max
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> AugmentTwoMatricesColumnWise(const blImage<blType>& M1,
                                                    const blImage<blType>& M2)
{
    // Get the size of the two matrices
    int Rows1 = M1.size1();
    int Cols1 = M1.size2();
    int Rows2 = M2.size1();
    int Cols2 = M2.size2();

    // The ending size of the augmented matrix is going to be
    // MaxRows x (Cols1 + Cols2)
    int Rows = std::max(Rows1,Rows2);
    int Cols = Cols1 + Cols2;

    // Create the augmented matrix
    blImage<blType> Result(Rows,Cols);

    // Assign elements from Matrix M1
    for(int i = 0; i < Rows1; ++i)
        for(int j = 0; j < Cols1; ++j)
            Result[i][j] = M1[i][j];

    // Assign elements from Matrix M2
    for(int i = 0; i < Rows2; ++i)
        for(int j = Cols1; j < Cols; ++j)
            Result[i][j] = M2[i][j-Cols1];

    return Result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            AugmentTwoMatricesColumnWise
// ARGUMENTS:           M1,M2,DstMatrix
// TEMPLATE ARGUMENTS:  blType
// PURPOSE:             Creates a matrix augmenting two
//                      matrices M1 and M2 column wise and
//                      storing the result into DstMatrix
//                      (Could be faster than previous
//                      function if DstMatrix is already
//                      the correct size)
// DEPENDENCIES:        blImage
//                      std::max
//-------------------------------------------------------------------
template<typename blType>
inline blImage<blType> AugmentTwoMatricesColumnWise(const blImage<blType>& M1,
                                                    const blImage<blType>& M2,
                                                    blImage<blType>& DstMatrix)
{
    // Get the size of the two matrices
    int Rows1 = M1.size1();
    int Cols1 = M1.size2();
    int Rows2 = M2.size1();
    int Cols2 = M2.size2();

    // The ending size of the augmented matrix is going to be
    // MaxRows x (Cols1 + Cols2)
    int Rows = std::max(Rows1,Rows2);
    int Cols = Cols1 + Cols2;

    // Let's make sure that DstMatrix is
    // already the correct size
    if(DstMatrix.size1() != Rows ||
       DstMatrix.size2() != Cols)
    {
        DstMatrix.CreateImage(Rows,Cols);
    }

    // Assign elements from Matrix M1
    for(int i = 0; i < Rows1; ++i)
        for(int j = 0; j < Cols1; ++j)
            DstMatrix[i][j] = M1[i][j];

    // Assign elements from Matrix M2
    for(int i = 0; i < Rows2; ++i)
        for(int j = Cols1; j < Cols; ++j)
            DstMatrix[i][j] = M2[i][j-Cols1];
}
//-------------------------------------------------------------------


#endif // BL_IMAGETILING_HPP
