SCRIP_DIR=$(cd `dirname $0`; pwd)

if [ ! `which ant` ]
then
    echo " export ant first !"
    exit 1
fi

cd $SCRIP_DIR/../VRPlayer/android
TARGET=$1

case "$TARGET" in
    release|"")
    if [  -f "$ant.properties" ]; then  
        echo "need ant.properties sign keystore file!"
       exit 1
    fi  
        ant release
    ;;
    debug)
        ant debug
    ;;
esac