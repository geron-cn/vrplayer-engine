#include "CameraCursor.h"
#include "GLProgram.h"

namespace vrlive {
    
    static char* vs = "";
    static char* fs = "";
    
    CameraCursor* CameraCursor::create(const std::string& imagePath, float width, float height)
    {
        CameraCursor* cursor = new CameraCursor;
        
        if (cursor->init(width, height) == false)
        {
            delete cursor;
            cursor = NULL;
        }
        
        return cursor;
    }
    
    bool CameraCursor::init(float width, float height)
    {
        GLProgram* program = GLProgram::create(vs, fs);
        if (program == nullptr)
            return false;
        
        //
        
        return true;
    }
    
    CameraCursor::CameraCursor()
    : _program(nullptr)
    {
        
    }
    
    CameraCursor::~CameraCursor()
    {
        if (_program)
        {
            _program->release();
            _program = nullptr;
        }
    }

    void CameraCursor::draw()
    {
        
    }
}

