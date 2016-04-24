cd ../OpenALSoft/build

echo "build for mac ..."
cmake .. -DCMAKE_INSTALLPREFIX=osxlibs -DLIBTYPE=STATIC
make && make install

echo "build for android"
cmake .. -DCMAKE_TOOLCHAIN_FILE=../XCompile-Android.txt -DHOST=arm-linux-androideabi -DCMAKE_INSTALLPREFIX=anroidlibs -DLIBTYPE=STATIC
make && make install