#ifndef AL_MOVIE_AUDIO_FACTORY_H
#define AL_MOVIE_AUDIO_FACTORY_H

#include <thread>
#include "audiofactory.hpp"
#include "gameplay.h"
#include <condition_variable>
#include <mutex>

class ALMovieAudioFactory : public Video::MovieAudioFactory
{
    virtual std::shared_ptr<Video::MovieAudioDecoder> createDecoder(Video::VideoState* videoState);
};



class ALMovieAudioDecoder : public Video::MovieAudioDecoder
{
public:
#define NUM_BUFFERS 4096
    virtual void adjustAudioSettings(AVSampleFormat& sampleFormat, uint64_t& channelLayout, int& sampleRate) override;

    virtual void setupFormat() override;

    // set listener's postion
    void setListenerPosition(float x, float y, float z);

    // set listener's orientation
    void setListenerOrientation(float vec[6]);

    //set audio source position
    void setSourcePosition(float x, float y, float z);
    void setSourceVelocity(float x, float y, float z);

    ALMovieAudioDecoder(Video::VideoState* videoState);
    ~ALMovieAudioDecoder();

    void audioPlay_thread();

    bool updateQueueBufferData();
private:

    ALuint _sourceId;
    ALuint _bufferId;

    std::thread  _audioThread;
    std::thread  _playAudioThread;
    std::mutex   _audioBufferMutex;
    std::condition_variable _audioBufferCond;
    ALCcontext *_context;
    ALCdevice *_device;
    ALenum _alformat;
    //ALuint _buffers[NUM_BUFFERS];
};


#endif
