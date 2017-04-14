#ifndef __HILUO_STREAMSHAPE_INCLUDE_H__
#define __HILUO_STREAMSHAPE_INCLUDE_H__

#include "common.h"
#include <vector>

using namespace std;

template<class T, int SIZE = 20>
class CDataShaper
{
public:
    CDataShaper()
    {
        memset(m_window, NULL, SIZE * sizeof(T*));
        m_next_seq  = -1;
        m_toggle    = false;
    }

    struct Data_t
    {
        u_int32 m_seq_number;
        T*      m_data;
    };

    int insert(u_int32 _seq_number, T* _data, vector<Data_t>& _dataVec)
    {
        if (!isValid(_seq_number)) return 0;

        u_int32 index = _seq_number % SIZE;
        if (m_window[index]) 
        { 
            assert(false);  
            RM_LOG_ERROR("Duplicated packet happen: " << _seq_number);
            return 0;
        }
        m_window[index] = _data;

        if (m_next_seq == -1)
        {
            m_next_seq = _seq_number;
        }
        else
        {
            //
            // output the continuous blocks
            //
            int i = m_next_seq % SIZE;
            for (; i < i + SIZE; ++i)
            {
                int j = i % SIZE;
                if (m_window[j])
                {
                    Data_t data = { m_next_seq, m_window[j]};
                    _dataVec.push_back(data);
                    m_window[j] = NULL;
                    ++m_next_seq;
                    m_toggle = true;
                }
                else
                    break;
            }
        }

        return _dataVec.size();
    }

private:
    /**
     * @Function: a valid sequence should be in the range [m_next_seq, m_next_seq + SIZE]
     *  if hasn't output any sequence, a valid sequence may in [m_next_seq - SIZE, m_next_seq + SIZE]
     **/
    bool isValid(u_int32 _seq_number)
    {
        int diff = _seq_number > m_next_seq ? _seq_number - m_next_seq : m_next_seq - _seq_number;
        bool ret = !m_toggle ? true : (_seq_number >= m_next_seq && diff < SIZE);
        if (ret && m_next_seq != -1) m_next_seq = MyMin(_seq_number, m_next_seq);

        return ret;
    }

private:
    T* m_window[SIZE];
    u_int32 m_next_seq;
    bool    m_toggle;   // whether has output blocks
};

#endif
