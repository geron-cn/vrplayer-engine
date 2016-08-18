//
//  FileUtils-IOS.cpp
//  VRLive
//
//  Created by gero on 8/7/16.
//  Copyright © 2016 yangxiao. All rights reserved.
//
#import <Foundation/Foundation.h>

#include "FileUtils.h"

namespace vrlive {
    const std::string FileUtils::resolvePath(const std::string& filepath)
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
}
