SCRIPDIR=$(cd `dirname $0`; pwd)
cd $SCRIPDIR/../VRLiveFramework
FRAMEWORK=VRLiveFramework
SCHEMENAME=VRLiveFramework
BUILD=build
FRAMEWORK_PATH=$FRAMEWORK.framework
echo " clean builded ..."
rm -Rf $BUILD
rm $SCRIPDIR/products/$FRAMEWORK_PATH.tar.gz
echo " build iphoneos ..."
xcodebuild archive -project $FRAMEWORK.xcodeproj -scheme $SCHEMENAME -sdk iphoneos SYMROOT=$BUILD
echo " build iphonesimulator ..."
xcodebuild build -project $FRAMEWORK.xcodeproj -target $SCHEMENAME -sdk iphonesimulator SYMROOT=$BUILD
cp -RL $BUILD/Release-iphoneos $BUILD/Release-universal
echo " create universal ..."
lipo -create $BUILD/Release-iphoneos/$FRAMEWORK_PATH/$FRAMEWORK $BUILD/Release-iphonesimulator/$FRAMEWORK_PATH/$FRAMEWORK -output $BUILD/Release-universal/$FRAMEWORK_PATH/$FRAMEWORK

PRODUCT_DIR=$SCRIPDIR/products
if [ ! -d "$PRODUCT_DIR"]; then 
mkdir "$PRODUCT_DIR" 
fi 
tar -czv -C $BUILD/Release-universal -f $PRODUCT_DIR/$FRAMEWORK.framework.tar.gz $FRAMEWORK_PATH