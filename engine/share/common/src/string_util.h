#ifndef __HL_STRING_UTIL_H_INCLUDE__
#define __HL_STRING_UTIL_H_INCLUDE__

#include "common.h"
#include <vector>
#include <memory>


namespace LabSpace
{
    namespace Common
    {
        struct StrUtil
        {
            /**
            * @Function: Split string specified by delim and put the substring into vector
            **/
            static int StrSplit(const tstring& _src, const tstring& _delim, vector<tstring>& _out);

            /**
            * @Function: Split string by substring
            **/
            static int StrSubSplit(const tstring& _src, const tstring& _delim, vector<tstring>& _out);

            /**
            * @Function: Remove the space chars at the front and end of a string
            **/
            static tstring StrTrim(const tstring& _src);

            /**
            * @Function: Replace the _oldTokens in _src with newToken
            **/
            static tstring StrReplace(const tstring& _src, const tstring& _oldTokens, tchar _newToken);

            /**
            * @Function: Convert a command string into argv[]
            * @Param _cmd: Contain the execute command string, such as "cmd.exe param1"
            * @Param _argv [out]: keep the params
            * @Param _argc [in, out]: keep the size of _argv and return the size of params
            * @Return the duplicated buffer, should be freed
            **/
            static std::shared_ptr<tchar> Str2Argv(const tchar* _cmd, tchar** _argv, int& _argc);

            static int  GetXValue(char _x);
            static byte AtoX(const char* _xcode);

            /**
            * @Function: convert the binary values into string
            * @Param _len [in]: the length of _p
            * @Return the memory to keep the string, user has to free it
            **/
            static std::shared_ptr<tchar> Bin2Str(const byte* _p, size_t _len);
            static size_t convert2Hex(char buf[], const void* _value);
            static u_int64 HexStr2Dec(const tchar* _value);

            static std::string  AddValue(const string& _value, u_int64 _count);
            static std::string	MinusValue(const string& _value, u_int64 _count);
            static void			AddValueR(string& _value, u_int64 _count);
            static void			MinusValueR(string& _value, u_int64 _count);
            static void			AddValueR(byte* _value, int _valueSize, u_int64 _count);
            static void			MinusValueR(byte* _value, int _valueSize, u_int64 _count);

            /**
            * @Function: Get the file path exclude the parameters
            **/
            static tstring GetFilePath(const tstring& _exePath);

            /**
             * @Function: Get the process name from command line
             **/
            static tstring GetFileName(const tstring& _exePath);

            static tstring GetFileDir(const tstring& _path);

            /**
             * @Function: Check the regular expression
             **/
            static bool IsMatch(const tstring& _src, const tstring& _express);
            static bool IsAllNumber(const tstring& _src);

            /**
             * @Function: only allow '0-9', 'a-z', 'A-Z', '.', ':', '/', '\', '_'
             **/
            static bool IsValidPath(const tstring& _src);
        };
    }
}


#endif
