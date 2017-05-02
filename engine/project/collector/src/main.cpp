#include "main.h"
#include "file_util.h"


RM_LOG_DEFINE(TEXT("collector"));


static int lcore_hello(__attribute__((unused)) void *arg)
{
	unsigned lcore_id;
	lcore_id = rte_lcore_id();
	printf("hello from core %u\n", lcore_id);
	return 0;
}


int main(int argc, char **argv)
{
    tstring app_path = FileUtil::GetAppDir();
    tstring filename = app_path + "../configure/project_log4cplus.properties";

    int ret = InitLog(filename.c_str(), "collector");
    if (!ret)
    {
        PRINT_MSG("Fail to init log.");
        return ret;
    }
    L4C_LOG_INFO("welcome to collector.");
        
    c_wireshark_decoder decoder;
    filename = app_path + "1.pcap";
    decoder.decode_file(filename.c_str(), PRINT_XML);

	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_panic("Cannot init EAL\n");

    unsigned lcore_id;
	RTE_LCORE_FOREACH_SLAVE(lcore_id) {
		rte_eal_remote_launch(lcore_hello, NULL, lcore_id);
	}

    int devCount  = rte_eth_dev_count();
    if (devCount <= 0)
    {
        L4C_LOG_ERROR("No Ethernet devices available.");
    }
    L4C_LOG_ERROR("Detect " << devCount << " eth devices.");

	rte_eal_mp_wait_lcore();

	return 0;
}
