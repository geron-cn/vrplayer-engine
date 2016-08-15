#include "Stream.h"

namespace vrlive {
    
    std::string Stream::readString()
    {
        unsigned int length;
        if(read(&length, 4, 1) != 1)
        {
            return std::string();
        }
        
        std::string str;
        if (length > 0)
        {
            str.resize(length);
            if (read(&str[0], 1, length) != length)
            {
                return std::string();
            }
        }
        return str;
    }
    
    bool Stream::readMatrix(float* m)
    {
        return (read(m, sizeof(float), 16) == 16);
    }
    
    bool Stream::writeString(const std::string& str)
    {
        unsigned int length = str.length();
        if (write(&length, 4, 1) != 1)
        {
            return false;
        }
        
        if (length > 0)
        {
            return write(str);
        }
        return true;
    }
    
    bool Stream::writeMatrix(const float* m)
    {
        return write(m, sizeof(float), 16) == 16;
    }
    
    bool Stream::write(const std::string& str)
    {
        return write(str.c_str(), str.length(), 1) == 1;
    }
    
    ///..............
    MemoryStream::MemoryStream()
    {
        m_buffer = NULL;
        m_position = 0;
        m_length = 0;
    }
    
    MemoryStream::MemoryStream(char* lpbuffer, unsigned int length)
    {
        m_position = 0;
        m_buffer  = lpbuffer;
        m_length = length;
    };
    
    MemoryStream::~MemoryStream()
    {
        // should free the memory of the buffer by who created the buffer         
        //close();
    }
    
    MemoryStream* MemoryStream::create(char* lpbuffer, unsigned int length)
    {
        if (lpbuffer)
        {
            MemoryStream* stream = new MemoryStream(lpbuffer, length);
            
            return stream;
        }
        return NULL;
    }
    
    void MemoryStream::close()
    {
        delete[] (m_buffer);
    }
    
    size_t MemoryStream::read(void* ptr, size_t size, size_t count)
    {
        if (!m_buffer || eof())
            return 0;
        
        size_t validCount;
        size_t validLength = m_length - m_position;
        size_t needLength = size*count;
        char* ptr1 = (char*)ptr;
        if(validLength <= needLength)
        {
            validCount = validLength/size;
            size_t readLength = size*validCount;
            memcpy(ptr1,(char*)m_buffer+m_position,readLength);
            ptr1 += readLength;
            m_position += readLength;
            readLength = validLength - readLength;
            if(readLength>0)
            {
                memcpy(ptr1,(char*)m_buffer+m_position,readLength);
                m_position += readLength;
                validCount+=1;
            }
        }
        else
        {
            memcpy(ptr1,(char*)m_buffer+m_position,needLength);
            m_position += needLength;
            validCount = count;
        }
        //*ptr1 = 0;
        
        return validCount;
    }
    
    char* MemoryStream::readLine(int num,char* line)
    {
        if (!m_buffer)
            return 0;
        
        //char* str = new char[num];
        char* buffer = (char*)m_buffer+m_position;
        
        char* p = line;
        char c;
        size_t readNum = 0;
        while((c=*buffer) != 10 && readNum < (size_t)num && m_position<(long int)m_length)
        {
            *p = c;
            p++;
            buffer++;
            m_position++;
            readNum++;
        }
        *p = '\0';
        
        return line;
    }
    
    size_t MemoryStream::write(const void* ptr, size_t size, size_t count)
    {
        if (!m_buffer)
            return 0;
        
        size_t validCount;
        size_t validLength = m_length - m_position;
        size_t needLength = size*count;
        const char* ptr1 = (const char*)ptr;
        if (validLength <= needLength)
        {
            validCount = validLength / size;
            size_t readLength = size * validCount;
            memcpy((char*)m_buffer + m_position, ptr1, readLength);
            ptr1 += readLength;
            m_position += readLength;
            readLength = validLength - readLength;
            if(readLength > 0)
            {
                memcpy((char*)m_buffer+m_position, ptr1, readLength);
                m_position += readLength;
                validCount += 1;
            }
        }
        else
        {
            memcpy((char*)m_buffer + m_position, ptr1, needLength);
            m_position += needLength;
            validCount = count;
        }
        
        return validCount;
    }
    
    bool MemoryStream::eof()
    {
        if (!m_buffer)
            return true;
        return ((size_t)tell()) >= length();
    }
    
    size_t MemoryStream::length()
    {
        return m_length;
    }
    
    long int MemoryStream::tell()
    {
        if (!m_buffer)
            return -1;
        return m_position;
    }
    
    bool MemoryStream::seek(long int offset, int origin)
    {
        if (!m_buffer)
            return false;
        
        if(origin == SEEK_CUR)
        {
            m_position += offset;
        }
        else if(origin == SEEK_SET)
        {
            m_position = offset;
        }
        else if(origin == SEEK_END)
        {
            m_position = m_length+offset;
        }
        else
            return false;
        
        return true;
    }
    
    bool MemoryStream::rewind()
    {
        if (m_buffer != NULL)
        {
            m_position = 0;
            return true;
        }
        return false;
    }
    
}
