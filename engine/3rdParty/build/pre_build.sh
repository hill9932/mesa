#!/bin/bash
set -x

CWD=$(cd `dirname $0`; pwd)
ROOT_OF_SOURCE=$CWD/..
ROOT_OF_LIB_OUTPUT=$CWD/../lib
ROOT_OF_BIN_OUTPUT=$CWD/../bin

COMMAND="all"
if [ $# -ge 1 ]; then
    COMMAND=$1
fi


install_packages()
{
    sudo apt-get update
    PACKAGES="cmake libglib2.0-dev"

    for package in ${PACKAGES}
    do
        sudo apt-get install -y ${PACKAGES}
        if [ $? -ne 0 ]
        then
            echo "install $package failed"
            echo "please contact the administrator."
            exit;
        fi
    done
}


build_log4cplus() 
{
	echo "Building log4cplus ..."
	
    LIB_ROOT_DIR=$ROOT_OF_SOURCE/log4cplus
    cd $LIB_ROOT_DIR;
    chmod +x configure;
    ./configure CXXFLAGS="-fPIC -std=c++11" --with-gnu-ld=yes --prefix=$ROOT_OF_LIB_OUTPUT --libdir=$ROOT_OF_LIB_OUTPUT --enable-shared=no --enable-static=yes && 
	make && make install
	
    if [ $? != 0 ]
    then
        echo "log4cplus installation failed."
        exit;
    fi
	
	echo "Build log4cplus finished."
}


build_lua()
{
	echo "Building lua ..."
	
	LIB_ROOT_DIR=$ROOT_OF_SOURCE/lua
	cd $LIB_ROOT_DIR;
	make linux && cp $LIB_ROOT_DIR/src/liblua.a $ROOT_OF_LIB_OUTPUT
	
	echo "Build lua finished"
}

build_ndpi()
{
	echo "Building ndpi ..."
	
	LIB_ROOT_DIR=$ROOT_OF_SOURCE/ndpi
    cd $LIB_ROOT_DIR;
    chmod +x autogen.sh;
	./autogen.sh && ./configure --prefix=$ROOT_OF_LIB_OUTPUT --libdir=$ROOT_OF_LIB_OUTPUT --enable-shared=no --enable-static=yes && 
	make && make install
	
	echo "Build ndpi finished."
}


build_gtest()
{
	echo "Building gtest ..."
	
    LIB_ROOT_DIR=$ROOT_OF_SOURCE/gtest
    cd $LIB_ROOT_DIR;
    chmod +x configure;
    ./configure CXXFLAGS="-fPIC -std=c++11" --with-gnu-ld=yes --prefix=$ROOT_OF_LIB_OUTPUT --libdir=$ROOT_OF_LIB_OUTPUT --enable-shared=no --enable-static=yes && 
	make && make install
	
    if [ $? != 0 ]
    then
        echo "gtest installation failed."
        exit;
    fi
	
	echo "Build gtest finished."
}


build_dpdk() 
{
	echo "Building dpdk ..."

	LIB_ROOT_DIR=$ROOT_OF_SOURCE/dpdk
	LIB_TARGET_DIR=$ROOT_OF_LIB_OUTPUT/$RTE_TARGET
	
	export RTE_SDK=$LIB_ROOT_DIR
    export RTE_TARGET=x86_64-native-linuxapp-gcc
	export RTE_SDK_BIN=$RTE_SDK/$RTE_TARGET
	
    chmod +x $RTE_SDK/scripts/*.sh
    if [ $? -ne 0 ]
    then
        echo "DPDK chmod scripts/*.sh failed"
    fi

    cd $RTE_SDK
    make config T=$RTE_TARGET O=$RTE_TARGET
    if [ $? -ne 0 ]
    then
        rm -rf $RTE_TARGET
        echo "DPDK make config failed"
        exit;
    fi

	#cd $LIB_TARGET_DIR
    make install T=$RTE_TARGET CONFIG_RTE_BUILD_COMBINE_LIBS=y CONFIG_RTE_BUILD_SHARED_LIB=n EXTRA_CFLAGS=-fPIC
    if [ $? -ne 0 ]
    then
        rm -rf $RTE_TARGET
        echo "DPDK make install failed"
        exit;
    fi
	
	echo "Build dpdk finished."
}
	
	
build_wireshark()
{
	echo "Building wireshark ..."
	
	LIB_ROOT_DIR=$ROOT_OF_SOURCE/wireshark
	
	cd $LIB_ROOT_DIR &&	
	chmod a+x configure && 
	chmod a+x autogen.sh &&
	./autogen.sh &&
	./configure --without-qt --without-gtk3 --without-lua --disable-wireshark --bindir=$ROOT_OF_BIN_OUTPUT --libdir=$ROOT_OF_LIB_OUTPUT && 
	make && make install 
	
	echo "Build wireshark finished."
}	
	

main() 
{
	./create_symbol.sh
	
	case $COMMAND in		
	build_log4cplus)
		build_log4cplus
		;;
		
	build_lua)
		build_lua
		;;
	
	build_ndpi)
		build_ndpi
		;;
		
	build_dpdk)
		build_dpdk
		;;
		
	build_wireshark)
		build_wireshark
		;;
		
	all)
		install_packages
		build_log4cplus	
		build_lua
		build_wireshark
		build_dpdk
		build_ndpi
		;;
	*)
		echo "[Usage:] $0 all/build_dpdk"
		;;
	esac
}


main	
