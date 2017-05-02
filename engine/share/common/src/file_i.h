/**
* @Function:
*  Define the interface of file class
* @Memo:
*  Create by hill, 4/16/2017
*/
#pragma once
#include "file_def.h"
#include "stdString.h"


namespace LabSpace
{
    namespace Common
    {
        class IFile
        {
        public:
            IFile()
            {
                m_fileType = FileType::FILE_TYPE_UNSUPPORT;
            }

            virtual ~IFile() {}
            virtual FileType::FILE_TYPE_e getFileType()     { return m_fileType; }
            virtual string  getFileName()                   { return m_fileName; }
            virtual int     getFileHeaderSize()             { return FILE_HEADERS[m_fileType].size; }

            virtual u_int64 getOffset()                     { return m_offset; }
            virtual u_int64 getFileSize()                   { return m_fileSize; }
            virtual int     setFilePos(u_int64  _filePos)   { m_offset = _filePos; return 0; }

            virtual int     open(const tchar* _fileName, int _access, int _flag, bool _asyn, bool _directIO) = 0;
            virtual int     close()                                 = 0;
            virtual int     read(PPER_FILEIO_INFO_t  _readBlock)    = 0;
            virtual int     write(PPER_FILEIO_INFO_t _writeBlock)   = 0;

            virtual int     rename(const tchar* _fileName)          = 0;
            virtual int     setFileSize(u_int64 _fileSize)          = 0;
            virtual int     flush()         = 0;
            virtual bool    isValid()       = 0;
            virtual bool    needPadding()   = 0;

        protected:
            FileType::FILE_TYPE_e   m_fileType;
            std::string             m_fileName;
            u_int64                 m_offset;
            u_int64                 m_fileSize;
        };
    }
}
