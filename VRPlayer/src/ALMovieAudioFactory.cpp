#include "ALMovieAudioFactory.h"

#include <stdexcept>
#include <chrono>
#include "libavutil/samplefmt.h"
#include <libswresample/swresample.h>
#include "videostate.hpp"

std::shared_ptr<Video::MovieAudioDecoder> ALMovieAudioFactory::createDecoder(Video::VideoState* videoState)
{
    std::shared_ptr<Video::MovieAudioDecoder> ptr(new ALMovieAudioDecoder(videoState));
    return ptr;
}

void ALMovieAudioDecoder::adjustAudioSettings(AVSampleFormat& sampleFormat, uint64_t& channelLayout, int& sampleRate)
{
    // check eax
    auto isEAX = alIsExtensionPresent("EAX2.0");
    // Generate Buffers
    alGetError(); // clear error code
    if (alGetError() != AL_NO_ERROR)
    {
        printf("could not create buffers!\n");
        return;
    }
    //// get al format 
    //switch (mAVStream->codec->sample_fmt)
    //{
    //case AV_SAMPLE_FMT_U8P:
    //    if(mAVStream->codec->channels == 1)
    //        _alformat = AL_FORMAT_MONO8;
    //    else if (mAVStream->codec->channels == 2)
    //        _alformat = AL_FORMAT_STEREO8;
    //case AV_SAMPLE_FMT_S16:
    //case  AV_SAMPLE_FMT_S16P:
    //    if(mAVStream->codec->channels == 1)
    //        _alformat = AL_FORMAT_MONO16;
    //    else if (mAVStream->codec->channels == 2)
    //        _alformat = AL_FORMAT_STEREO16;
    //case AV_SAMPLE_FMT_FLTP:
    //    //alformat = AL_FORMAT_STEREO_FLOAT32;
    //default:
    //    break;
    //}
    //
    //if (!_alformat) {
    //    printf("OpenAL can't open this format of sound.\n");
    //    return;
    //}

    /*auto dataSize = av_samples_get_buffer_size(NULL, self->mAVStream->codec->channels,
        decodedFrame->nb_samples, self->mAVStream->codec->sample_fmt, 1); */   //@TODO need use datadize to limit bufflen
    alGenSources(1, &_sourceId);
    alSourcei(_sourceId, AL_LOOPING, AL_FALSE);
    alSourcef(_sourceId, AL_SOURCE_TYPE, AL_STREAMING);
    alSourcef(_sourceId, AL_GAIN, 1.0f);
    
    _audioThread = std::thread(std::bind(&ALMovieAudioDecoder::audioPlay_thread, this));
   // updateQueueBufferData();
}

ALMovieAudioDecoder::ALMovieAudioDecoder(Video::VideoState* videoState)
    : MovieAudioDecoder(videoState)
    , _sourceId(0)
    , _device(nullptr)
    , _context(nullptr)
{
}

ALMovieAudioDecoder::~ALMovieAudioDecoder()
{
    //delete our source  
    updateQueueBufferData();
    alDeleteSources(1, &_sourceId);
    
}

void ALMovieAudioDecoder::setListenerPosition(float x, float y, float z)
{
    alListener3f(AL_POSITION, x, y, z);
}

void ALMovieAudioDecoder::setListenerOrientation(float vec[6])
{
    alListenerfv(AL_ORIENTATION, vec);
}

void ALMovieAudioDecoder::setSourcePosition(float x, float y, float z)
{
    alSource3f(_sourceId, AL_POSITION, x, y, z);
}

void ALMovieAudioDecoder::setSourceVelocity(float x, float y, float z)
{
    alSource3f(_sourceId, AL_VELOCITY, x, y, z);
}

#define BUFFER_SIZE 4096
unsigned char stream[BUFFER_SIZE];
#define MIN(X, Y) (((X)<(Y))? (X) : (Y) )
#define MAX(X, Y) (((X)<(Y))? (Y) : (X) )
void ALMovieAudioDecoder::audioPlay_thread()
{
    bool buffered = false;
    ALint queued = 0;
    while (true)
    {
        if (mVideoState->mPaused)
        {
            alSourceStop(_sourceId);
            continue;
        }
        alGetSourcei(_sourceId, AL_BUFFERS_QUEUED, &queued);
        if (queued > 5)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            updateQueueBufferData();
            continue;
        }
        
        int readedSize = 0;
        auto frame = mFrame;
        readedSize = read(stream, BUFFER_SIZE);
        if (readedSize > 0)
        {
            if (readedSize < BUFFER_SIZE)
            {
                // we don't have enough data, which means EOF was reached
                // fill the rest of the buffer with silence
                size_t sampleSize = av_get_bytes_per_sample(mOutputSampleFormat);
                uint8_t* data[1];
                data[0] = stream + readedSize;
                av_samples_set_silence((uint8_t**)data, 0, (BUFFER_SIZE - readedSize) / sampleSize, 1, mOutputSampleFormat);
            }

            alGenBuffers(1, &_bufferId);
            //                 if (alGetError() != AL_NO_ERROR)
            //                 {
            //                     printf("Error generate buffering :(\n");
            //                     // return ;
            //                 }
            alBufferData(_bufferId, AL_FORMAT_STEREO16, stream, readedSize/*audioFrame->linesize[0]*/, frame->sample_rate);
            //                 if (alGetError() != AL_NO_ERROR)
            //                 {
            //                     printf( "Error assign buffering :(\n");
            //                    // return ;
            //                 }
            alSourceQueueBuffers(_sourceId, 1, &_bufferId);
            //                 if (alGetError() != AL_NO_ERROR)
            //                 {
            //                     printf( "Error queueing buffers :(\n");
            //                     //return ;
            //                 }
            
            updateQueueBufferData();
            ALenum err2 = alGetError();
            if (err2 != AL_NO_ERROR)
            {
                printf("Error play buffers and release buffers:(\n");
                //return ;
            }

           // std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
}

bool ALMovieAudioDecoder::updateQueueBufferData()
{
    ALint state = 0;
    int processed = 0;// , queued = 0;
    alGetSourcei(_sourceId, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
    {
        alSourcePlay(_sourceId);
        return false;
    }

    alGetSourcei(_sourceId, AL_BUFFERS_PROCESSED, &processed);
    //alGetSourcei(_sourceId, AL_BUFFERS_QUEUED, &queued);
    while (processed--)
    {
        ALuint bufferId;
        auto b = _audioBufferMutex.try_lock();
        alSourceUnqueueBuffers(_sourceId, 1, &bufferId);
        _audioBufferMutex.unlock();
        alDeleteBuffers(1, &bufferId);
    }
    return true;
}

void ALMovieAudioDecoder::setupFormat()
{
    AVSampleFormat inputSampleFormat = mAVStream->codec->sample_fmt;

    uint64_t inputChannelLayout = mAVStream->codec->channel_layout;
    if (inputChannelLayout == 0)
        inputChannelLayout = av_get_default_channel_layout(mAVStream->codec->channels);

    int inputSampleRate = mAVStream->codec->sample_rate;

    mOutputSampleRate = inputSampleRate;//  44100;
    mOutputSampleFormat = AV_SAMPLE_FMT_S16;
    mOutputChannelLayout = AV_CH_LAYOUT_STEREO;
    if (inputSampleFormat != mOutputSampleFormat
        || inputChannelLayout != mOutputChannelLayout
        || inputSampleRate != mOutputSampleRate)
    {
        mAudioResampler->mSwr = swr_alloc_set_opts(mAudioResampler->mSwr,
            mOutputChannelLayout,
            mOutputSampleFormat,
            mOutputSampleRate,
            inputChannelLayout,
            inputSampleFormat,
            inputSampleRate,
            0,                             // logging level offset
            NULL);                         // log context
        if (!mAudioResampler->mSwr)
            printf("Couldn't allocate SwrContext");
        if (swr_init(mAudioResampler->mSwr) < 0)
            printf("Couldn't initialize SwrContext");
    }

    adjustAudioSettings(mOutputSampleFormat, mOutputChannelLayout, mOutputSampleRate);
}
