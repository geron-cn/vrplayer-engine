//
// Created by tony
//
#ifndef GLPROGRAM_H_
#define GLPROGRAM_H_
#include <string>
#include <map>

#include "Base.h"
#include "Vector3.h"
#include "Ref.h"

namespace vrlive {
    class GLProgram : public Ref
    {
    public:
        
        
        static GLProgram* create(const std::string& vs, const std::string& fs);
        
        
        void bind();
        
        void unbind();
        
        GLuint getAttribute(const std::string& attributName);
        
        GLuint getUniform(const std::string& uniformName);
        
    protected:
        /**
         * Constructor.
         */
        GLProgram();
        
        /**
         * Destructor.
         */
        ~GLProgram();
        
        GLuint _program;
        
        std::map<std::string, GLuint> _attributes;
        std::map<std::string, GLuint> _uniforms;
    };
    
    class GLProgramCache
    {
    public:
        static std::string PositionTexCoord;
        static std::string PositionTexCoordFlipY;
        static GLProgramCache* getInstance();
        
        static void destroyInstance();
        
        GLProgram* createOrGet(const std::string& key);
        
    protected:
        
        static GLProgram* loadDefProgram(const std::string& key);
        GLProgramCache();
        ~GLProgramCache();
        
        static GLProgramCache* _instance;
        
        std::map<std::string, GLProgram*> _programMap;
    };
}

#endif
