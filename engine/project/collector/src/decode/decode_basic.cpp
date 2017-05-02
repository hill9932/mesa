#include "decode_basic.h"
#include "pcap_file.h"
#include "log_.h"

capture_file    cfile;

c_wireshark_decoder::c_wireshark_decoder()
{
    m_cum_bytes     = 0;
    m_data_offset   = 0;
}

c_wireshark_decoder::~c_wireshark_decoder()
{
    clean();
}

int c_wireshark_decoder::decode_file(const char* _filename, print_type_e _print_type)
{
    if (0 != init_epan(_filename)) 
    {
        L4C_LOG_ERROR("Fail to init epan with file " << _filename);
        return -1;
    }
    m_filename = _filename;
        
    c_pcap_file pcap_file;
    if (0 != pcap_file.open(_filename,
        FileAccessMode::ACCESS_READ,
        FileAccessOption::FILE_OPEN_EXISTING,
        false,
        false))
    {
        u_int32 err = Util::GetLastSysError(); 
        L4C_LOG_ERROR("Fail to open file " << _filename << ": " << Util::GetLastSysErrorMessage(err));
        return -1;
    }

    byte* pktData = nullptr;
    packet_header_t newPktHeader;
    int pktSize = pcap_file.getNextPacket(newPktHeader, pktData);

    epan_dissect_t *edt = m_cfile.edt;
    wtap* wth           = m_cfile.wth;
    while (pktSize > 0) 
    {
        int                err;
        gchar             *err_info = nullptr;
        struct wtap_pkthdr *whdr    = wtap_phdr(m_cfile.wth);
        guchar             *buf     = wtap_buf_ptr(m_cfile.wth);

        m_cfile.count++;

        whdr->caplen    = newPktHeader.caplen;
        whdr->len       = newPktHeader.len;
        whdr->ts.secs   = newPktHeader.ts.tv_sec;
        whdr->ts.nsecs  = newPktHeader.ts.tv_nsec;
        whdr->pkt_encap = 1;    // wth->file_encap;  // 1
  //      whdr->pkt_tsprec    = wth->file_tsprec;
        whdr->presence_flags = 7;

        frame_data fdlocal;
        frame_data_init(&fdlocal, m_cfile.count, whdr, m_data_offset, m_cum_bytes);
        frame_data_set_before_dissect(&fdlocal, &m_cfile.elapsed_time, &m_cfile.ref, m_cfile.prev_dis);
        m_cfile.ref = &fdlocal;

        epan_dissect_run(edt, m_cfile.cd_t, &(m_cfile.phdr), frame_tvbuff_new(&fdlocal, buf), &fdlocal, &m_cfile.cinfo);

        frame_data_set_after_dissect(&fdlocal, &m_cum_bytes);
        m_cfile.prev_cap = m_cfile.prev_dis = frame_data_sequence_add(m_cfile.frames, &fdlocal);
        frame_data_destroy(&fdlocal);

        write_pdml_proto_tree(edt, stdout);
    }

    return 0;
}

int c_wireshark_decoder::init_epan(const gchar* filename)
{
    int          err = 0;
    gchar       *err_info = nullptr;
    e_prefs     *prefs_p;

    init_process_policies();

    epan_init(register_all_protocols, register_all_protocol_handoffs, nullptr, nullptr);

    cap_file_init(&m_cfile);
    m_cfile.filename = const_cast<gchar*>(filename);

    m_cfile.wth = wtap_open_offline(m_cfile.filename, WTAP_TYPE_AUTO, &err, &err_info, TRUE);
    if (m_cfile.wth == nullptr)
        goto fail;

    m_cfile.count = 0;
    m_cfile.epan = epan_new();
    m_cfile.epan->data = &m_cfile;
    m_cfile.epan->get_frame_ts = get_frame_ts;
    m_cfile.edt = epan_dissect_new(m_cfile.epan, TRUE, TRUE);

    set_timestamp();
    m_cfile.frames = new_frame_data_sequence();

    prefs_p = get_prefs();

    build_column_format_array(&m_cfile.cinfo, prefs_p->num_cols, TRUE);

    cfile = m_cfile;
    return 0;

fail:
    clean();
    return err;
}

void c_wireshark_decoder::clean()
{
    if (m_cfile.frames != nullptr) 
    {
        free_frame_data_sequence(m_cfile.frames);
        m_cfile.frames = nullptr;
    }

    if (m_cfile.wth != nullptr) 
    {
        wtap_close(m_cfile.wth);
        m_cfile.wth = nullptr;
    }

    if (m_cfile.epan != nullptr)
        epan_free(m_cfile.epan);

    if (m_cfile.edt != nullptr)
        epan_dissect_free(m_cfile.edt);

    epan_cleanup();
}


gboolean c_wireshark_decoder::read_packet(epan_dissect_t **edt_r)
{
    epan_dissect_t    *edt;
    int                err;
    gchar             *err_info = nullptr;

    struct wtap_pkthdr *whdr = wtap_phdr(m_cfile.wth);
    guchar             *buf  = wtap_buf_ptr(m_cfile.wth);

    if (wtap_read(m_cfile.wth, &err, &err_info, &m_data_offset))
    {
        m_cfile.count++;

        frame_data fdlocal;
        frame_data_init(&fdlocal, m_cfile.count, whdr, m_data_offset, m_cum_bytes);

        edt = epan_dissect_new(m_cfile.epan, TRUE, TRUE);

        frame_data_set_before_dissect(&fdlocal, &m_cfile.elapsed_time, &m_cfile.ref, m_cfile.prev_dis);
        m_cfile.ref = &fdlocal;

        epan_dissect_run(edt, m_cfile.cd_t, &(m_cfile.phdr), frame_tvbuff_new(&fdlocal, buf), &fdlocal, &m_cfile.cinfo);

        frame_data_set_after_dissect(&fdlocal, &m_cum_bytes);

        m_cfile.prev_cap = m_cfile.prev_dis = frame_data_sequence_add(m_cfile.frames, &fdlocal);

        //free space
        frame_data_destroy(&fdlocal);

        *edt_r = edt;
        return true;
    }
    return false;
}

void c_wireshark_decoder::print_each_packet_xml()
{
    epan_dissect_t *edt;

    while (read_packet(&edt)) 
    {
        write_pdml_proto_tree(edt, stdout);
        epan_dissect_free(edt);
        edt = nullptr;
    }
}

void c_wireshark_decoder::print_each_packet_text()
{
    epan_dissect_t *edt;
    print_stream_t *print_stream;
    print_args_t    print_args;

    print_stream = print_stream_text_stdio_new(stdout);

    print_args.print_hex = TRUE;
    print_args.print_dissections = print_dissections_expanded;

    while (read_packet(&edt)) 
    {
        proto_tree_print(&print_args, edt, nullptr, print_stream);
        epan_dissect_free(edt);
        edt = nullptr;
    }
}

void c_wireshark_decoder::set_timestamp()
{
    switch (wtap_file_tsprec(m_cfile.wth))
    {
        case(WTAP_TSPREC_SEC) :
            timestamp_set_precision(TS_PREC_FIXED_SEC);
            break;

        case(WTAP_TSPREC_DSEC) :
            timestamp_set_precision(TS_PREC_FIXED_DSEC);
            break;

        case(WTAP_TSPREC_CSEC) :
            timestamp_set_precision(TS_PREC_FIXED_CSEC);
            break;

        case(WTAP_TSPREC_MSEC) :
            timestamp_set_precision(TS_PREC_FIXED_MSEC);
            break;

        case(WTAP_TSPREC_USEC) :
            timestamp_set_precision(TS_PREC_FIXED_USEC);
            break;

        case(WTAP_TSPREC_NSEC) :
            timestamp_set_precision(TS_PREC_FIXED_NSEC);
            break;

        default:
            g_assert_not_reached();
    }
}

const nstime_t* c_wireshark_decoder::get_frame_ts(void *data, guint32 frame_num)
{
    capture_file *cf = (capture_file *)data;

    if (cf->ref && cf->ref->num == frame_num)
        return &(cf->ref->abs_ts);

    if (cf->prev_dis && cf->prev_dis->num == frame_num)
        return &(cf->prev_dis->abs_ts);

    if (cf->prev_cap && cf->prev_cap->num == frame_num)
        return &(cf->prev_cap->abs_ts);

    if (cf->frames) 
    {
        frame_data *fd = frame_data_sequence_find(cf->frames, frame_num);
        return (fd) ? &fd->abs_ts : nullptr;
    }

    return nullptr;
}

e_prefs* c_wireshark_decoder::get_prefs()
{
    e_prefs     *prefs_p;
    char        *gpf_path, *pf_path;
    int          gpf_read_errno, gpf_open_errno;
    int          pf_open_errno, pf_read_errno;

    prefs_p = read_prefs(
        &gpf_open_errno, 
        &gpf_read_errno, 
        &gpf_path,
        &pf_open_errno, 
        &pf_read_errno, 
        &pf_path);

    return prefs_p;
}

