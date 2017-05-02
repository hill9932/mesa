/**
 * @Function:
 *  Use wireshark to decode packets
 * @Memo:
 *  Create by hill, 4/11/2017
 */
#pragma once
 
#define HAVE_STDARG_H 1
#define WS_MSVC_NORETURN

#ifdef __cplusplus
extern "C" {
#endif

#include <epan/epan.h>
#include <epan/print.h>
#include <epan/timestamp.h>
#include <epan/prefs.h>
#include <epan/column.h>
#include <epan/epan-int.h>
#include <epan/epan_dissect.h>
#include <epan/proto.h>
#include <epan/ftypes/ftypes.h>
#include <epan/asm_utils.h>
#include <wsutil/privileges.h>
#include <wiretap/wtap.h>
#include <frame_tvbuff.h>

#ifdef __cplusplus
}
#endif

#include "decode_define.h"

#include <cassert>
#include <string>
#include <iostream>


class c_wireshark_decoder
{
public:
    c_wireshark_decoder();
    ~c_wireshark_decoder();

    int decode_file(const char* _filename, print_type_e _print_type);

private:
    int     init_epan(const gchar* filename);
    void    print_each_packet_xml();
    void    print_each_packet_text();
    void    set_timestamp();
    void    clean();

    e_prefs* get_prefs();
    gboolean read_packet(epan_dissect_t **edt_r);

    static const nstime_t* get_frame_ts(void *data, guint32 frame_num);

private:
    std::string     m_filename;
    capture_file    m_cfile;

    uint32_t        m_cum_bytes;
    int64_t         m_data_offset;
};

