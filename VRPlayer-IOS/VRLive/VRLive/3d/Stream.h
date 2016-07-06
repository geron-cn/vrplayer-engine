//
// Created by tony
//
#ifndef STREAM_H_
#define STREAM_H_

#include <vector>
#include "Ref.h"
#include <string>

namespace vrlive {
    class Stream : public Ref
    {
    public:
        /**
         * Closes the stream.
         */
        virtual void close() = 0;
        
        /**
         * Reads an array of elements.
         *
         * @param ptr   The pointer to the memory to copy into.
         *              The available size should be at least bytes.
         * @param size  The size of each element to be read, in bytes.
         * @param count The number of elements to read.
         *
         * @return The number of elements read.
         */
        virtual size_t read(void* ptr, size_t size, size_t count) = 0;
        
        /**
         * Reads a line from the stream.
         */
        virtual char* readLine(int num,char* line) = 0;
        
        /**
         * Writes an array of elements.
         *
         * @param ptr   The pointer to the array of elements to be written.
         * @param size  The size of each element to be written, in bytes.
         * @param count The number of elements to write.
         *
         * @return The number of elements written.
         */
        virtual size_t write(const void* ptr, size_t size, size_t count) = 0;
        
        /**
         * Returns true if the end of the stream has been reached.
         */
        virtual bool eof() = 0;
        
        /**
         * Returns the length of the stream in bytes.
         */
        virtual size_t length() = 0;
        
        /**
         * Returns the position of the file pointer.
         */
        virtual long int tell() = 0;
        
        /**
         * Sets the position of the file pointer.
         */
        virtual bool seek(long int offset, int origin) = 0;
        
        /**
         * Sets the file pointer at the start of the file.
         */
        virtual bool rewind() = 0;
        
        /**
         * read binary typed value.
         */
        template<typename T> bool read(T* ptr);
        template<typename T> bool readArray(unsigned int* length, std::vector<T>* values);
        
        /**
         * first read length, then read string text
         */
        virtual std::string readString();
        virtual bool readMatrix(float* m);
        
        /**
         *  write binary typed value
         */
        template<typename T> bool write(const T& ptr);
        template<typename T> bool writeArray(const std::vector<T>& values);
        
        /**
         *  first write length, then write string text, DO NOT call writeString for text file, call write(string) instead
         */
        virtual bool writeString(const std::string& str);
        virtual bool writeMatrix(const float* m);
        virtual bool write(const std::string& str);
        
    };
    
    class MemoryStream : public Stream
    {
    public:
        static MemoryStream* create(char* lpbuffer, unsigned int length);
        
        virtual void close();
        
        /**
         * Reads an array of elements.
         */
        virtual size_t read(void* ptr, size_t size, size_t count);
        
        /**
         * Reads a line from the stream.
         */
        virtual char* readLine(int num,char* line);
        
        /**
         * Writes an array of elements.
         */
        virtual size_t write(const void* ptr, size_t size, size_t count);
        
        /**
         * Returns true if the end of the stream has been reached.
         */
        virtual bool eof();
        
        /**
         * Returns the length of the stream in bytes.
         */
        virtual size_t length();
        
        /**
         * Returns the position of the file pointer.
         */
        virtual long int tell();
        
        /**
         * Sets the position of the file pointer.
         */
        virtual bool seek(long int offset, int origin);
        
        /**
         * Sets the file pointer at the start of the file.
         */
        virtual bool rewind();
        
    private:
        MemoryStream(char* lpbuffer, unsigned int len);
        MemoryStream();
        virtual ~MemoryStream();
        
        long int m_position;
        size_t  m_length;
        char* m_buffer;
    };
}

#endif
