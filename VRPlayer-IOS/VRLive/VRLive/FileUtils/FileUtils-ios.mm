//
//  FileUtils-IOS.cpp
//  VRLive
//
//  Created by gero on 8/7/16.
//  Copyright © 2016 yangxiao. All rights reserved.
//
#import <Foundation/Foundation.h>

#include "FileUtils.h"
#include "FileSystem.h"

namespace vrlive {
    std::string _resolvePath(const std::string& filepath)
    {
        if(filepath.empty())
            return "";
        
        if(filepath[0] == '/') // absolutepath
        {
            return filepath;
        }
        
        size_t pos = filepath.find_last_of("/");
        std::string dirpath = "";
        std::string filename = filepath;
        if(pos != std::string::npos)
        {
            dirpath = filepath.substr(0, pos + 1);
            filename = filepath.substr(pos + 1);
        }
        
        NSBundle* mainbundle = [NSBundle mainBundle];
        NSString* fullpath = [NSString stringWithUTF8String:filepath.c_str()];
        if( mainbundle != nil)
            fullpath = [mainbundle pathForResource:[NSString stringWithUTF8String:filename.c_str()] ofType:nil inDirectory:[NSString stringWithUTF8String:dirpath.c_str()]];
        
        if (fullpath == nil)
            return std::string("");
        
        return [fullpath UTF8String];
    }
    const std::string FileUtils::resolvePath(const std::string& filepath)
    {
        return _resolvePath(filepath);
    }
    
    void FileSystem::getFullPath(const char* path, std::string& fullPath)
    {
        fullPath = _resolvePath(std::string(path));
    }
}
