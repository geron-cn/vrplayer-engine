ndk-build  NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk
echo "build finished"
cp -Rfv libs/* ../../../../vrlib-android/vrlib/src/main/libs/
echo "copied *.so to vrlib"
