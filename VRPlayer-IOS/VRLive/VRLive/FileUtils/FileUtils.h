#ifndef __FILEUTILS_H_
#define __FILEUTILS_H_

#include <map>
#include <functional>
#include <string>
#include "Data.h"

namespace vrlive {

class FileUtils
{
public:

    enum class Status
       {
           OK = 0,
           NotExists = 1, // File not exists
           OpenFailed = 2, // Open file failed.
           ReadFailed = 3, // Read failed
           NotInitialized = 4, // FileUtils is not initializes
           TooLarge = 5, // The file is too large (great than 2^32-1)
           ObtainSizeFailed = 6 // Failed to obtain the file size.
       };

    static FileUtils* getInstance();
    static void destoryInstace();

    // remove cache data from cache, and reload data
    void reloadFileContent(const std::string& filename, Data &data);
    void reloadZipFileContent(const std::string& filename, Data &data);

    /**
     * @brief getFileContent, get file content
     * @param filename, absolute path of file
     * @param data, the result content, will clear(free) the data's old bytes
     * @param cacheData, while true, check data from cache first,
     *        then read data and cache data while not in cache.
     *        while false, read data directly from path
     * @return
     */
    virtual void getFileContent(const std::string& filename, Data &data, bool cacheData);

    /**
     * @brief getZipFileContent, get a zip file content
     * @param filename
     * @param data, the result content, will clear(free) the data's old bytes
     * @param cacheData
     * @return
     */
    virtual void getZipFileContent(const std::string& filename, Data &data, bool cacheData);

    /**
     * @brief getDataFromCache, get data from cache
     * @param filename
     * @return return a NUll Data when filename not in cache
     */
    virtual Data getDataFromCache(const std::string& filename) const;

    virtual void removeDataFromCache(const std::string& filename);

    virtual ~FileUtils();
protected:
    FileUtils();

    virtual FileUtils::Status readFileData(const std::string& filename, Data& data);

    virtual FileUtils::Status readZipFileData(const std::string& zipfilename, Data& data);
    
    virtual const std::string resolvePath(const std::string& filepath);
private:
    static FileUtils* _fileUtilInst;
    std::map<std::string, Data> _fileDatas;

    typedef std::function<FileUtils::Status(Data&)> ReadDataFunc;
    void readFileDataHelper(const std::string& filename, Data &data, bool cacheData, ReadDataFunc readCallFunc);
};
}
#endif // __FILEUTILS_H_
