#!/bin/sh

CMAKE=/usr/bin/cmake

ANDROID_NDK=${HOME}/android/android-ndk-r19c
CMAKE_TOOLCHAIN_FILE=${ANDROID_NDK}/build/cmake/android.toolchain.cmake

mkdir -p lib/{arm64-v8a,armeabi-v7a,x86,x86_64}

cd nng-1.1.1

if false; then
echo "Building nng-1.1.1 armeabi-v7a ..."

mkdir -p build-armeabi-v7a && cd build-armeabi-v7a

${CMAKE} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
         -DANDROID_NDK=${ANDROID_NDK}                   \
         -DANDROID_ABI="armeabi-v7a with NEON"          \
         -DANDROID_NATIVE_API_LEVEL=android-23          \
         -DCMAKE_BUILD_TYPE=Release                     \
	 -DCMAKE_SYSTEM_NAME=Android                    \
	 -DCMAKE_INSTALL_PREFIX=/usr                    \
	 -DNNG_TESTS=ON                                 \
	 -DNNG_ENABLE_NNGCAT=ON                         \
         ..

${CMAKE} --build . || exit 1

cd ..

exit 0
fi

for ANDROID_ABI in arm64-v8a  armeabi-v7a  x86  x86_64
do

echo "Building nng-1.1.1 ${ANDROID_ABI} ..."

mkdir -p build-${ANDROID_ABI} && cd build-${ANDROID_ABI}

${CMAKE} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
         -DANDROID_NDK=${ANDROID_NDK}                   \
         -DANDROID_ABI=${ANDROID_ABI}                   \
         -DANDROID_NATIVE_API_LEVEL=android-23          \
         -DCMAKE_BUILD_TYPE=Release                     \
	 -DCMAKE_SYSTEM_NAME=Android                    \
	 -DCMAKE_INSTALL_PREFIX=/usr                    \
	 -DNNG_TESTS=ON                                 \
	 -DNNG_ENABLE_NNGCAT=ON                         \
         ..

${CMAKE} --build . || exit 1

cp -a libnng.a ../../lib/${ANDROID_ABI}

cd ..

echo "Done."
echo

done

echo "Copy include files"
cd build-${ANDROID_ABI}
make install DESTDIR=../../rootfs
cd ../..
cp -a rootfs/usr/include/nng include
rm -rf rootfs

