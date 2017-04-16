#!/bin/bash
set -x

CWD=$(cd `dirname $0`; pwd)
ROOT_DIR=$CWD/..
THIRD_PARTY_DIR=$ROOT_DIR/3rdParty

echo "Current dir is " $ROOT_DIR

rm -f $THIRD_PARTY_DIR/dpdk
rm -f $THIRD_PARTY_DIR/wireshark
rm -f $THIRD_PARTY_DIR/glib
rm -f $THIRD_PARTY_DIR/log4cplus
rm -f $THIRD_PARTY_DIR/gtest
rm -f $THIRD_PARTY_DIR/jsoncpp
rm -f $THIRD_PARTY_DIR/memcached
rm -f $THIRD_PARTY_DIR/lua
rm -f $THIRD_PARTY_DIR/popt
rm -f $THIRD_PARTY_DIR/thrift
rm -f $THIRD_PARTY_DIR/tinyxml
rm -f $THIRD_PARTY_DIR/ndpi


ln -sf /usr/include/glib-2.0			$THIRD_PARTY_DIR/glib
ln -sf src/dpdk/dpdk-stable-16.11.1 	$THIRD_PARTY_DIR/dpdk
ln -sf src/wireshark/wireshark-2.0.5	$THIRD_PARTY_DIR/wireshark
ln -sf src/log4cplus-1.1.2				$THIRD_PARTY_DIR/log4cplus
ln -sf src/gtest-1.7.0					$THIRD_PARTY_DIR/gtest
ln -sf src/jsoncpp-0.7.0				$THIRD_PARTY_DIR/jsoncpp
ln -sf src/libmemcached-1.0.18			$THIRD_PARTY_DIR/memcached
ln -sf src/lua-5.3.4					$THIRD_PARTY_DIR/lua
ln -sf src/popt							$THIRD_PARTY_DIR/popt
ln -sf src/thrift-0.9.1					$THIRD_PARTY_DIR/thrift
ln -sf src/tinyxml						$THIRD_PARTY_DIR/tinyxml
ln -sf src/ndpi/nDPI-1.8				$THIRD_PARTY_DIR/ndpi
