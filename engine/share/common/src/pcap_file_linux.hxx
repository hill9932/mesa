#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int c_pcap_file::setFileSize(u_int64 _fileSize)
{
    if (INVALID_HANDLE_VALUE == m_fileHandle) return -1;

    int z = truncate(m_fileName.c_str(), _fileSize);
    ON_ERROR_PRINT_LASTMSG_S_AND_DO(z, !=, 0, m_fileName, return -1);

    m_fileSize = _fileSize;
    return z;
}

int c_pcap_file::createMapFile(const tchar* _fileName, u_int64 _fileSize)
{
    m_fileHandle = ::open(_fileName,
                     _fileSize > 0 ? O_CREAT | O_RDWR : O_RDONLY, 
                     S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); 
    ON_ERROR_PRINT_LASTMSG_S_AND_DO(m_fileHandle, == , -1, _fileName, return -1);

    u_int32 low32Size = 0;
    if (_fileSize != 0 /*&& _fileSize != m_fileSize*/) 
        setFileSize(_fileSize);
    else
    {
        m_fileSize = 0;
        low32Size = getSize((u_int32*)&m_fileSize);
        m_fileSize <<= 32;
        m_fileSize += low32Size;
    }

    //lseek64(m_hFile, _fileSize, SEEK_SET);     

    m_data = (byte*)mmap(NULL, 
                        m_fileSize,
                        _fileSize > 0 ? PROT_READ | PROT_WRITE : PROT_READ,
                        MAP_SHARED, 
                        m_fileHandle, 
                        0);
    if (m_data == MAP_FAILED)
    {
        LOG_LAST_ERRORMSG_S(_fileName);
        m_data = NULL;
    }

    return 0;
}

int c_pcap_file::close()
{
    if (!isValid()) return -1;

    flush();
    int z = munmap(m_data, m_fileSize);
    CloseHandle(m_fileHandle);
    m_data = NULL;

    return z;
}

int c_pcap_file::flush()
{
    if (!isValid())   return -1;

    return msync(m_data, m_fileSize, 0);
}

