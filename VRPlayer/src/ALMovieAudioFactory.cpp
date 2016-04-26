#include "ALMovieAudioFactory.h"

#include <stdexcept>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif
#include "libavutil/samplefmt.h"
#include "libswresample/swresample.h"
#ifdef __cplusplus
}
#endif
        
#include "videostate.hpp"
namespace Video
{
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
        alGenSources(1, &_sourceId);
        alSourcei(_sourceId, AL_LOOPING, AL_FALSE);
        alSourcef(_sourceId, AL_SOURCE_TYPE, AL_STREAMING);
        alSourcef(_sourceId, AL_GAIN, 1.0f);
        
        _audioThread = std::thread(std::bind(&ALMovieAudioDecoder::audioPlay_thread, this), false);
    }

    ALMovieAudioDecoder::ALMovieAudioDecoder(Video::VideoState* videoState)
        : MovieAudioDecoder(videoState)
        , _sourceId(0)
        , _device(nullptr)
        , _context(nullptr)
    {
        _device = alcOpenDevice(NULL);
        if(!_device)
        {
            fprintf(stderr, "OpenAL: could not open device - exiting\n");
            return;
        }
        _context = alcCreateContext(_device, NULL);
        if(!_context)
        {
            fprintf(stderr, "OpenAL: could not create context - exiting\n");
            return;
        }
        if(alcMakeContextCurrent(_context) == ALC_FALSE)
        {
            fprintf(stderr, "OpenAL: could not make context current - exiting\n");
            return;
        }
    }

    ALMovieAudioDecoder::~ALMovieAudioDecoder()
    {
        updateQueueBufferData();
        if(_sourceId != 0)
            alDeleteSources(1, &_sourceId);
        alcMakeContextCurrent(NULL);
        if(_context != nullptr)
        {
            alcDestroyContext(_context);
        }
        if(_device != nullptr)
            alcCloseDevice(_device);
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

    void ALMovieAudioDecoder::setSourceGain(float voiceL)
    {
        alSourcef(_sourceId, AL_GAIN, voiceL);
    }

    float ALMovieAudioDecoder::getSourceGain() const
    {
        ALfloat gain = 0.0f;
        alGetSourcef(_sourceId, AL_GAIN, &gain);
        return gain;
    }

    void ALMovieAudioDecoder::audioPlay_thread()
    {
        ALint queued = 0;
        int readedSize = 0;
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
                alBufferData(_bufferId, AL_FORMAT_STEREO16, stream, readedSize/*audioFrame->linesize[0]*/, frame->sample_rate);
                alSourceQueueBuffers(_sourceId, 1, &_bufferId);
                updateQueueBufferData();
                ALenum err2 = alGetError();
                if (err2 != AL_NO_ERROR)
                {
                    printf("Error play buffers and release buffers:(\n");
                }
            }
        }
    }

    bool ALMovieAudioDecoder::updateQueueBufferData()
    {
        ALint state = 0;
        int processed = 0;
        alGetSourcei(_sourceId, AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING)
        {
            alSourcePlay(_sourceId);
            return false;
        }

        alGetSourcei(_sourceId, AL_BUFFERS_PROCESSED, &processed);
        while (processed--)
        {
            ALuint bufferId;
            alSourceUnqueueBuffers(_sourceId, 1, &bufferId);
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

        mOutputSampleRate = inputSampleRate;//  44100 48000;
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
}
