#ifndef VIDEOPLAYER_VIDEOSTATE_H
#define VIDEOPLAYER_VIDEOSTATE_H

#include <thread>
#include <vector>
#include <condition_variable>

#include "gameplay.h"

#include "videodefs.hpp"

#define VIDEO_PICTURE_QUEUE_SIZE 5

extern "C"
{
    struct SwsContext;
    struct AVPacketList;
    struct AVPacket;
    struct AVFormatContext;
    struct AVStream;
    struct AVFrame;
}

namespace Video
{

struct VideoState;

class MovieAudioFactory;
class MovieAudioDecoder;

struct ExternalClock
{
    ExternalClock();

    uint64_t mTimeBase;
    uint64_t mPausedAt;
    bool mPaused;

    std::mutex mMutex;

    void setPaused(bool paused);
    uint64_t get();
    void set(uint64_t time);
};

struct PacketQueue {
    PacketQueue()
      : first_pkt(NULL), last_pkt(NULL), flushing(false), nb_packets(0), size(0)
    { }
    ~PacketQueue()
    { clear(); }

    AVPacketList *first_pkt, *last_pkt;
    volatile bool flushing;
    int nb_packets;
    int size;

    std::mutex mutex;
    std::condition_variable cond;

    void put(AVPacket *pkt);
    int get(AVPacket *pkt, VideoState *is);

    void flush();
    void clear();
};

struct VideoPicture {
    VideoPicture() : pts(0.0)
    { }

    std::vector<uint8_t> data;
    double pts;
};

struct VideoState {
    VideoState();
    ~VideoState();

    void setAudioFactory(MovieAudioFactory* factory);

    bool init(const std::string& resourceName);
    void deinit();

    void setPaused(bool isPaused);
    void seekTo(double time);

    double getDuration();

    int stream_open(int stream_index, AVFormatContext *pFormatCtx);

    bool update();

    static void video_thread_loop(VideoState *is);
    static void decode_thread_loop(VideoState *is);

    void video_display(VideoPicture* vp);
    void video_refresh();

    int queue_picture(AVFrame *pFrame, double pts);
    double synchronize_video(AVFrame *src_frame, double pts);

    double get_audio_clock();
    double get_video_clock();
    double get_external_clock();
    double get_master_clock();

//    Ogre::TexturePtr mTexture;
    gameplay::Texture* mTexture;
    
    int mTextureWidth;
    int mTextureHeight;

    MovieAudioFactory* mAudioFactory;
    std::shared_ptr<MovieAudioDecoder> mAudioDecoder;

    ExternalClock mExternalClock;

//    Ogre::DataStreamPtr stream;
    AVFormatContext* format_ctx;    
    int av_sync_type;

    AVStream**  audio_st;
    PacketQueue audioq;

    uint8_t* mFlushPktData;

    AVStream**  video_st;
    double      frame_last_pts;
    double      video_clock; ///<pts of last decoded frame / predicted pts of next decoded frame
    PacketQueue videoq;
    SwsContext*  sws_context;
    VideoPicture pictq[VIDEO_PICTURE_QUEUE_SIZE];
    AVFrame*     rgbaFrame; // used as buffer for the frame converted from its native format to RGBA
    int          pictq_size, pictq_rindex, pictq_windex;
    std::mutex pictq_mutex;
    std::condition_variable pictq_cond;

    std::thread parse_thread;
    std::thread video_thread;

    volatile bool mSeekRequested;
    uint64_t mSeekPos;

    volatile bool mVideoEnded;
    volatile bool mPaused;
    volatile bool mQuit;
    volatile double mStartClock;
};

}

#endif
