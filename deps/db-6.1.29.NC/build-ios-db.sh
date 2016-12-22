#!/bin/bash

# Yay shell scripting! This script builds a static version of
# BerkeleyDB ${DB_VERSION} for iOS 7.0 that contains code for armv6, armv7, arm7s and i386.

set -x
set -o allexport

# Setup paths to stuff we need

DB_VERSION="6.1.29.NC"
DEVELOPER="/Applications/Xcode.app/Contents/Developer"

SDK_VERSION="10.1"
MIN_VERSION="9.3"

IPHONEOS_PLATFORM="${DEVELOPER}/Platforms/iPhoneOS.platform"
IPHONEOS_SDK="${IPHONEOS_PLATFORM}/Developer/SDKs/iPhoneOS${SDK_VERSION}.sdk"
IPHONEOS_CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
IPHONEOS_CXX="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++"

IPHONESIMULATOR_PLATFORM="${DEVELOPER}/Platforms/iPhoneSimulator.platform"
IPHONESIMULATOR_SDK="${IPHONESIMULATOR_PLATFORM}/Developer/SDKs/iPhoneSimulator${SDK_VERSION}.sdk"
IPHONESIMULATOR_CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
IPHONESIMULATOR_CXX="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++"

# Make sure things actually exist

if [ ! -d "$IPHONEOS_PLATFORM" ]; then
  echo "Cannot find $IPHONEOS_PLATFORM"
  exit 1
fi

if [ ! -d "$IPHONEOS_SDK" ]; then
  echo "Cannot find $IPHONEOS_SDK"
  exit 1
fi

if [ ! -d "$IPHONESIMULATOR_PLATFORM" ]; then
  echo "Cannot find $IPHONESIMULATOR_PLATFORM"
  exit 1
fi

if [ ! -d "$IPHONESIMULATOR_SDK" ]; then
  echo "Cannot find $IPHONESIMULATOR_SDK"
  exit 1
fi

# Clean up whatever was left from our previous build

rm -rf include lib
rm -rf /tmp/db-${DB_VERSION}-*
rm -rf /tmp/db-${DB_VERSION}-*.*-log

build()
{
   HOST=$1
   ARCH=$2
   GCC=$3
   SDK=$4
   EXTRA=$5
   #patch -p1 -i ../dbatom.patch
   cd "build_unix"
   export CFLAGS="-arch ${ARCH} -isysroot ${SDK} -DSTDC_HEADERS ${EXTRA}"
   export CXXFLAGS="-arch ${ARCH} -isysroot ${SDK} -DSTDC_HEADERS ${EXTRA}"
   export LDFLAGS="-arch ${ARCH} -isysroot ${SDK}"
   export CC=${GCC}
   ../dist/configure --host=${HOST} --enable-cxx --prefix="/tmp/db-${DB_VERSION}-${ARCH}"  &> "/tmp/db-${DB_VERSION}-${ARCH}.log"
	 make clean
   make -j4 &> "/tmp/db-${DB_VERSION}-${ARCH}.build-log"
   make install &> "/tmp/db-${DB_VERSION}-${ARCH}.install-log"
}

build "arm" "armv7" "${IPHONEOS_CC}" "${IPHONEOS_SDK}" ""
build "arm" "arm64" "${IPHONEOS_CC}" "${IPHONEOS_SDK}" ""
build "i386" "i386" "${IPHONESIMULATOR_CC}" "${IPHONESIMULATOR_SDK}" "-miphoneos-version-min=9.3"
build "x86_64" "x86_64" "${IPHONESIMULATOR_CC}" "${IPHONESIMULATOR_SDK}" "-miphoneos-version-min=9.3"

#

mkdir include
cp -r /tmp/db-${DB_VERSION}-i386/include/db*.h include/

mkdir lib
lipo \
	"/tmp/db-${DB_VERSION}-armv7/lib/libdb.a" \
	"/tmp/db-${DB_VERSION}-arm64/lib/libdb.a" \
	"/tmp/db-${DB_VERSION}-i386/lib/libdb.a" \
	"/tmp/db-${DB_VERSION}-x86_64/lib/libdb.a" \
	-create -output lib/libdb.a
lipo \
	"/tmp/db-${DB_VERSION}-armv7/lib/libdb_cxx.a" \
	"/tmp/db-${DB_VERSION}-arm64/lib/libdb_cxx.a" \
	"/tmp/db-${DB_VERSION}-i386/lib/libdb_cxx.a" \
	"/tmp/db-${DB_VERSION}-x86_64/lib/libdb_cxx.a" \
	-create -output lib/libdb_cxx.a

rm -rf "/tmp/db-${DB_VERSION}-*"
rm -rf "/tmp/db-${DB_VERSION}-*.*-log"


