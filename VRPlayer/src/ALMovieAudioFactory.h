#ifndef AL_MOVIE_AUDIO_FACTORY_H
#define AL_MOVIE_AUDIO_FACTORY_H

//#include <thread>
#include "audiofactory.hpp"
#include "openal/al.h"
#include "openal/alc.h"
#include <mutex>

class ALMovieAudioFactory : public Video::MovieAudioFactory
{
    virtual std::shared_ptr<Video::MovieAudioDecoder> createDecoder(Video::VideoState* videoState);
};



class ALMovieAudioDecoder : public Video::MovieAudioDecoder
{
public:
    
    virtual void adjustAudioSettings(AVSampleFormat& sampleFormat, uint64_t& channelLayout, int& sampleRate) override;

    virtual void setupFormat() override;

    // set listener's postion
    void setListenerPosition(float x, float y, float z);

    // set listener's orientation
    void setListenerOrientation(float vec[6]);

    // set audio source position
    void setSourcePosition(float x, float y, float z);
    
    // set audio speed
    void setSourceVelocity(float x, float y, float z);
    
    // set audio's gain, 1.0 is the loudest
    void  setSourceGain(float voiceL);
    float getSourceGain() const;
    
    ALMovieAudioDecoder(Video::VideoState* videoState);
    ~ALMovieAudioDecoder();

    void audioPlay_thread();

    bool updateQueueBufferData();
private:

    ALuint _sourceId;
    ALuint _bufferId;
    
#define BUFFER_SIZE 4096
    unsigned char stream[BUFFER_SIZE];
    std::thread  _audioThread;
    ALCcontext *_context;
    ALCdevice *_device;
    ALenum     _alformat;
};


#endif
