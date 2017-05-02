#ifndef __HL_FILEPOOL_INCLUDE_H__
#define __HL_FILEPOOL_INCLUDE_H__

#include "aio_.h"
#include "file_i.h"


namespace LabSpace
{
namespace Common
{
class CFilePool
{
public:
    typedef std::function<int(void*, PPER_FILEIO_INFO_t, u_int32)>  CompleteCallback;
    typedef std::function<int(void*, PPER_FILEIO_INFO_t, u_int32)>  WriteCompleteCallback;
    typedef std::function<int(void*, PPER_FILEIO_INFO_t, u_int32)>  ReadCompleteCallback;
    typedef std::function<int(void*, PPER_FILEIO_INFO_t, int)>      ErrorCallback;
    typedef int(*CallBack_Func) (void*, PPER_FILEIO_INFO_t);

public:
    /**
        * @Param _blockNum:    the number of fixed size block
        * @Param _threadNum:   the number of thread to handle finished io
        * @Param _context:     user defined parameter which will be the void* value in callback
        **/
    CFilePool();
    ~CFilePool();

    int  init(int _blockNum, int _threadNum, void* _context = NULL);
    int  attach(handle_t _file_handle, void* _context = NULL, u_int32 _flag = 0);

    int getFinishedRequest(vector<PPER_FILEIO_INFO_t>& _request, int _timeout);  // in milliseconds
    int getFinishedRequest();
    Aio_t getAioObject() { return m_IOService.m_aioHandler; }

    void setWriteCompleteCallback(WriteCompleteCallback _callback)
    {
        m_writeCallback = _callback;
    }

    void setReadCompleteCallback(ReadCompleteCallback  _callback)
    {
        m_readCallback = _callback;
    }

    void setErrorCallback(ErrorCallback _callback)
    {
        m_errorCallback = _callback;
    }

    void setAioCallback(CallBack_Func _callback)
    {
        m_aioCallback = _callback;
    }   

protected:
    virtual int onDataRead(void*, PPER_FILEIO_INFO_t, u_int32);
    virtual int onDataWritten(void*, PPER_FILEIO_INFO_t, u_int32);
    virtual int onError(void*, PPER_FILEIO_INFO_t, int);

public:
    WriteCompleteCallback m_writeCallback;
    ReadCompleteCallback  m_readCallback;
    ErrorCallback         m_errorCallback;
    CallBack_Func         m_aioCallback;

private:
    CAsyncIO        m_IOService;

    bool            m_isStop;
    void*           m_context;  // user will set this value to indicate more values
};
}
}

#endif
