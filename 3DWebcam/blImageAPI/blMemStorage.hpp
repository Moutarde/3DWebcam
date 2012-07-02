#ifndef BL_MEMSTORAGE_HPP
#define BL_MEMSTORAGE_HPP


//-------------------------------------------------------------------
// FILE:            blMemStorage.hpp
// CLASS:           blMemStorage
// BASE CLASS:      None
//
// PURPOSE:         A base class used to wrap OpenCV's CvMemStorage
//                  with boost::shared_ptr
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blColor3 -- To represent color
//                  CvMemStorage -- The memory storage
//                  boost::shared_ptr -- To allow for automatic
//                                       garbage collection of
//                                       memory storage
//
//                  cvCreateMemStorage -- To create the memory storage
//
// NOTES:
//
// DATE CREATED:    Nov/05/2010
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
class blMemStorage
{
public: // Constructors and destructors

    // Default constructor
    blMemStorage(const int& BlockSize = 0);

    // Copy constructor
    blMemStorage(const blMemStorage& MemStorage);

    // Constructor from CvMemStorage
    blMemStorage(CvMemStorage* MemStorage);

    // Destructor
    ~blMemStorage()
    {
    }

public: // Public functions

    // Function used to get
    // the memory storage
    const boost::shared_ptr<CvMemStorage>&	GetMemStorage()const;

    // Function used to create
    // the memory storage
    void        CreateMemStorage(const int& BlockSize = 0);

    // Function used to get the
    // size of the memory storage
    const int   size()const;

    // Function used to get the free
    // space for this memory storage
    const int   FreeSpace()const;

    // Function used to convert this const object to
    // a pure OpenCV CvCvMemStorage storage
    operator CvMemStorage* const()
    {
        if(m_MemStorage.use_count() > 0)
            return m_MemStorage.get();
        else
            return NULL;
    }

    // Function used to convert this const object to
    // a pure OpenCV CvCvMemStorage storage
    operator const CvMemStorage* const()const
    {
        if(m_MemStorage.use_count() > 0)
            return m_MemStorage.get();
        else
            return NULL;
    }

protected: // Protected variables

    // Memory storage
    boost::shared_ptr<CvMemStorage>     m_MemStorage;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blMemStorage::blMemStorage(const int& BlockSize)
{
    // Create the memory storage
    CreateMemStorage(BlockSize);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blMemStorage::blMemStorage(const blMemStorage& MemStorage)
{
    // In this case we simply link to the memory storage
    m_MemStorage = MemStorage.GetMemStorage();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blMemStorage::blMemStorage(CvMemStorage* MemStorage)
{
    // If the mem storage is not NULL
    // then store it inside this object
    if(MemStorage != NULL)
        m_MemStorage = boost::shared_ptr<CvMemStorage>(MemStorage,ReleaseMemStorage());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const boost::shared_ptr<CvMemStorage>& blMemStorage::GetMemStorage()const
{
    return m_MemStorage;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blMemStorage::CreateMemStorage(const int& BlockSize)
{
    m_MemStorage = boost::shared_ptr<CvMemStorage>(cvCreateMemStorage(BlockSize),ReleaseMemStorage());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const int blMemStorage::size()const
{
    return m_MemStorage->block_size;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const int blMemStorage::FreeSpace()const
{
    return m_MemStorage->free_space;
}
//-------------------------------------------------------------------


#endif // BL_MEMSTORAGE_HPP
