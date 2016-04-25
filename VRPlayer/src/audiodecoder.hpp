#ifndef VIDEOPLAYER_AUDIODECODER_H
#define VIDEOPLAYER_AUDIODECODER_H

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
#include <stdint.h>

#include <new>
#include <memory>
#include "gameplay.h"


#ifdef __cplusplus
extern "C" {
#endif

    #include "libavutil/avutil.h"
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswresample/swresample.h"

    #if AV_VERSION_INT(52, 2, 0) <= AV_VERSION_INT(LIBAVUTIL_VERSION_MAJOR, \
        LIBAVUTIL_VERSION_MINOR, LIBAVUTIL_VERSION_MICRO)
        #include <libavutil/channel_layout.h>
    #endif
    
#ifdef __cplusplus
}
#endif


#ifdef _WIN32
#ifndef __GNUC__

#include <BaseTsd.h>
typedef SSIZE_T ssize_t;

#endif
#endif

namespace Video
{

    // Moved to implementation file, so that HAVE_SWRESAMPLE is used at library compile time only
    struct AudioResampler
    {
        AudioResampler()
        : mSwr(NULL)
        {
        }
        
        ~AudioResampler()
        {
            swr_free(&mSwr);
        }
        
        SwrContext* mSwr;
    };


struct VideoState;

class MovieAudioDecoder
{
protected:
    VideoState *mVideoState;
    AVStream *mAVStream;
    enum AVSampleFormat mOutputSampleFormat;
    uint64_t mOutputChannelLayout;
    int mOutputSampleRate;
    ssize_t mFramePos;
    ssize_t mFrameSize;
    double mAudioClock;

protected:
    struct AutoAVPacket : public AVPacket {
        AutoAVPacket(int size=0)
        {
            if(av_new_packet(this, size) < 0)
            {
                GP_ERROR("fail to allocate memory");
            }
        }
        ~AutoAVPacket()
        { av_free_packet(this); }
    };


    std::auto_ptr<AudioResampler> mAudioResampler;

    uint8_t *mDataBuf;
    uint8_t **mFrameData;
    int mDataBufLen;

    AutoAVPacket mPacket;
    AVFrame *mFrame;

    /* averaging filter for audio sync */
    double mAudioDiffAccum;
    const double mAudioDiffAvgCoef;
    const double mAudioDiffThreshold;
    int mAudioDiffAvgCount;

    /* Add or subtract samples to get a better sync, return number of bytes to
     * skip (negative means to duplicate). */
    int synchronize_audio();

    /// @param sample_skip If seeking happened, the sample_skip variable will be reset to 0.
    int audio_decode_frame(AVFrame *frame, int &sample_skip);

public:
    MovieAudioDecoder(VideoState *is);
    virtual ~MovieAudioDecoder();

    int getOutputSampleRate() const;
    AVSampleFormat getOutputSampleFormat() const;
    uint64_t getOutputChannelLayout() const;

    virtual void setupFormat();

    /// Adjust the given audio settings to the application's needs. The data given by the read() function will
    /// be in the desired format written to this function's parameters.
    /// @par Depending on the application, we may want either fixed settings, or a "closest supported match"
    /// for the input that does not incur precision loss (e.g. planar -> non-planar format).
    virtual void adjustAudioSettings(AVSampleFormat& sampleFormat, uint64_t& channelLayout, int& sampleRate) = 0;

    /// Return the current offset in seconds from the beginning of the audio stream.
    /// @par An internal clock exists in the mAudioClock member, and is used in the default implementation. However,
    /// for an accurate clock, it's best to also take the current offset in the audio buffer into account.
    virtual double getAudioClock();

    /// This is the main interface to be used by the user's audio library.
    /// @par Request filling the \a stream with \a len number of bytes.
    /// @return The number of bytes read (may not be the requested number if we arrived at the end of the audio stream)
    size_t read(unsigned char *stream, size_t len);
};

}

#endif
