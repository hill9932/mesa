#include "decode_ndpi.h"

struct ndpi_detection_module_struct * c_ndpi_decoder::g_module = nullptr;

c_ndpi_decoder::c_ndpi_decoder()
{

}

c_ndpi_decoder::~c_ndpi_decoder()
{

}

bool c_ndpi_decoder::setup_detection()
{
    if (g_module)   return true;

    NDPI_PROTOCOL_BITMASK all;
    NDPI_BITMASK_SET_ALL(all);
    g_module = ndpi_init_detection_module();
    ndpi_set_protocol_detection_bitmask2(g_module, &all);

    set_ndpi_malloc(memory_wrapper);
    set_ndpi_free(free_wrapper);
//    set_ndpi_flow_malloc(memory_wrapper);
//    set_ndpi_flow_free(free_wrapper);

    return g_module != nullptr;
}

void* c_ndpi_decoder::memory_wrapper(size_t _size)
{
    return malloc(_size);
}

void  c_ndpi_decoder::free_wrapper(void* _freeable)
{
    free(_freeable);
}