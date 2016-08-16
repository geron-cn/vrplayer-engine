#include <cstring>
#include <sys/stat.h>
#include "FileUtils.h"
#include "unzip.h"


namespace vrlive {

FileUtils *FileUtils::getInstance()
{
    if(_fileUtilInst == nullptr)
    {
        _fileUtilInst = new FileUtils();
    }
    return _fileUtilInst;
}

void FileUtils::destoryInstace()
{
    if(_fileUtilInst != nullptr)
    {
        delete _fileUtilInst;
        _fileUtilInst = nullptr;
    }
}

Data FileUtils::reloadFileContent(const std::string &filename)
{
    removeDataFromCache(filename);
    return getFileContent(filename, true);
}

Data FileUtils::reloadZipFileContent(const std::string &filename)
{
    removeDataFromCache(filename);
    return getZipFileContent(filename, true);
}

Data FileUtils::getFileContent(const std::string &filename, bool cacheData)
{
    Data data;
    auto readcall = std::bind(&FileUtils::readFileData, this, filename, std::placeholders::_1);
    readFileDataHelper(filename, data, cacheData, readcall);
    return data;
}

Data FileUtils::getZipFileContent(const std::string &filename, bool cacheData)
{
    Data data;
    auto readcall = std::bind(&FileUtils::readZipFileData, this, filename, std::placeholders::_1);
    readFileDataHelper(filename, data, cacheData, readcall);
    return data;
}

Data FileUtils::getDataFromCache(const std::string &filename) const
{
    Data data;
    auto iter = _fileDatas.find(filename);
    if(iter != _fileDatas.end())
    {
       data = iter->second;
    }
    return data;
}

void FileUtils::removeDataFromCache(const std::string &filename)
{
    auto iter = _fileDatas.find(filename);
    if(iter != _fileDatas.end())
    {
        _fileDatas.erase(iter);
    }
}

FileUtils::~FileUtils()
{
    _fileDatas.clear();
}

FileUtils* FileUtils::_fileUtilInst = nullptr;

FileUtils::FileUtils()
{
    _fileDatas.clear();
}

FileUtils::Status FileUtils::readFileData(const std::string& filename, Data &data)
{
    if(filename.empty())
        return Status::NotExists;

    const char* fullpath = resolvePath(filename).c_str();
    unsigned char* dataBytes = nullptr;
    size_t size = 0;
    size_t readsize = 0;
#ifdef __ANDROID__
    if(filename[0] != '/')
    {
        LOG("%s", fullpath);
        if( nullptr == __assetManager)
        {
            LOG("asset mananger is null");
            return Status::OpenFailed;
        }
        
        AAsset* asset = AAssetManager_open(__assetManager, fullpath, AASSET_MODE_UNKNOWN);
        if( nullptr == asset)
        {
            LOG("open path failed");
            return Status::OpenFailed;
        }
        size = AAsset_getLength(asset);
        dataBytes = (unsigned char*)malloc(size + 1);
        memset(dataBytes, 0, size);
        dataBytes[size] = '\0';
        readsize = AAsset_read(asset, (char*)dataBytes, size);
        AAsset_close(asset);
   }
   else
#endif
   {
        FILE *fp = fopen(fullpath, "rb");
        if(!fp)
        {
            LOG("file: %s open failed!", filename.c_str());
            return Status::OpenFailed;
        }

        #if defined(_MSC_VER)
            auto descriptor = _fileno(fp);
        #else
            auto descriptor = fileno(fp);
        #endif

        struct stat statBuf;
        if (fstat(descriptor, &statBuf) == -1) {
            fclose(fp);
            return Status::ReadFailed;
        }
        size = statBuf.st_size;

        dataBytes = (unsigned char*)malloc(size + 1);
        memset(dataBytes, 0, size);
        readsize = fread(dataBytes, 1, size, fp);
        dataBytes[size] = '\0';
        fclose(fp);
    }
    
    auto retSatus = Status::OK;
    auto dataSize = size;
    if (readsize < size) {
        dataSize = readsize;
        retSatus = Status::ReadFailed;
    }
    if(!data.isNull())
        data.clear();
    data.fastSet(dataBytes, dataSize);
    return retSatus;
}

FileUtils::Status FileUtils::readZipFileData(const std::string& zipfilename, Data &data)
{
    Status retStatus;
    unzFile file = nullptr;
    unsigned char* buffer = nullptr;
    ssize_t size = 0;
    file = unzOpen(zipfilename.c_str());

    do
    {
#define IF_BREAK(CON,STA) \
    if(CON)\
    { \
    retStatus = STA; \
    break;\
    }

        IF_BREAK(!file, FileUtils::Status::OpenFailed);

        int zret = unzLocateFile(file, zipfilename.c_str(), 1);
        IF_BREAK(zret != UNZ_OK, FileUtils::Status::ReadFailed);
        char zfilePath[260];
        unz_file_info fileInfo;
        zret = unzGetCurrentFileInfo(file, &fileInfo, zfilePath, sizeof(zfilePath), nullptr, 0, nullptr, 0);
        IF_BREAK(zret != UNZ_OK, FileUtils::Status::ReadFailed);
        zret = unzOpenCurrentFile(file);
        IF_BREAK(zret != UNZ_OK, FileUtils::Status::ReadFailed);
        buffer = (unsigned char*)malloc(fileInfo.uncompressed_size + 1);
        zret = unzReadCurrentFile(file, buffer, static_cast<unsigned>(fileInfo.uncompressed_size));
        buffer[fileInfo.uncompressed_size] = '\0';
        IF_BREAK(zret != 0 && zret != (int)fileInfo.uncompressed_size, FileUtils::Status::ReadFailed);
#undef IF_BREAK
        size = fileInfo.compressed_size;
        unzCloseCurrentFile(file);
    }while(0);

    if(file)
    {
        unzClose(file);
        if(buffer != nullptr)
        {
            free(buffer);
            buffer = nullptr;
        }
    }
    if(! data.isNull())
        data.clear();
    data.fastSet(buffer, size);
    return retStatus;
}

void FileUtils::readFileDataHelper(const std::string &filename, Data &data, bool cacheData, ReadDataFunc  readCallFunc)
{
    if(filename.empty())
        return;

    if(cacheData && _fileDatas.find(filename) != _fileDatas.end())
    {
        data = getDataFromCache(filename);
    }

    auto status = readCallFunc(data);
    if(status == Status::OK && cacheData)
    {
        _fileDatas.insert(std::make_pair(filename, data));
    }
}

#ifdef __ANDROID__
    AAssetManager* FileUtils::__assetManager = nullptr;
    const std::string FileUtils::resolvePath(const std::string& filepath)
    {
        return filepath;
    }
#endif
}
