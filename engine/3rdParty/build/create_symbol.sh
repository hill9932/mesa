#!/bin/bash
set -x

CWD=$(cd `dirname $0`; pwd)
ROOT_DIR=$CWD/..

echo "Current dir is " $ROOT_DIR

rm -f $ROOT_DIR/dpdk
rm -f $ROOT_DIR/wireshark
rm -f $ROOT_DIR/glib
rm -f $ROOT_DIR/log4cplus
rm -f $ROOT_DIR/gtest
rm -f $ROOT_DIR/jsoncpp
rm -f $ROOT_DIR/memcached
rm -f $ROOT_DIR/lua
rm -f $ROOT_DIR/popt
rm -f $ROOT_DIR/thrift
rm -f $ROOT_DIR/tinyxml
rm -f $ROOT_DIR/ndpi


ln -sf /usr/include/glib-2.0			$ROOT_DIR/glib
ln -sf src/dpdk/dpdk-stable-16.11.1 	$ROOT_DIR/dpdk
ln -sf src/wireshark/wireshark-2.0.5	$ROOT_DIR/wireshark
ln -sf src/log4cplus-1.1.2				$ROOT_DIR/log4cplus
ln -sf src/gtest-1.7.0					$ROOT_DIR/gtest
ln -sf src/jsoncpp-0.7.0				$ROOT_DIR/jsoncpp
ln -sf src/libmemcached-1.0.18			$ROOT_DIR/memcached
ln -sf src/lua-5.3.4					$ROOT_DIR/lua
ln -sf src/popt							$ROOT_DIR/popt
ln -sf src/thrift-0.9.1					$ROOT_DIR/thrift
ln -sf src/tinyxml						$ROOT_DIR/tinyxml
ln -sf src/ndpi/nDPI-dev				$ROOT_DIR/ndpi