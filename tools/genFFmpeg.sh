SCRIPURL=$(cd `dirname $0`; pwd)
cd $SCRIPURL
FRAMEWORK=VRLiveFramework
SCHEMENAME=VRLiveFramework
BUILD=build
FRAMEWORK_PATH=$FRAMEWORK.framework
rm -Rf $BUILD
rm $FRAMEWORK_PATH.tar.gz
xcodebuild archive -project $FRAMEWORK.xcodeproj -scheme $SCHEMENAME -sdk iphoneos SYMROOT=$BUILD
xcodebuild build -project $FRAMEWORK.xcodeproj -target $SCHEMENAME -sdk iphonesimulator SYMROOT=$BUILD
cp -RL $BUILD/Release-iphoneos $BUILD/Release-universal
lipo -create $BUILD/Release-iphoneos/$FRAMEWORK_PATH/$FRAMEWORK $BUILD/Release-iphonesimulator/$FRAMEWORK_PATH/$FRAMEWORK -output $BUILD/Release-universal/$FRAMEWORK_PATH/$FRAMEWORK
tar -czv -C $BUILD/Release-universal -f $FRAMEWORK.framework.tar.gz $FRAMEWORK_PATH