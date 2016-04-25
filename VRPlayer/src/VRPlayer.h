#ifndef VRPlayer_H_
#define VRPlayer_H_

#include <string>

/**
 * VRPlayer.
 */
class VRPlayer
{
public:

    /**
     * Constructor.
     */
    VRPlayer();
    
    bool initalize();
    
    void deInitalize();

    virtual ~VRPlayer();
    
    
    bool isVREnabled() const { return _isVREnabled; }
    
    void setVREnabled(bool enable) { _isVREnabled = enable; }

    /// Play the given video. If a video is already playing, the old video is closed first.
    /// @note The video will be unpaused by default. Use the pause() and play() methods to control pausing.
    bool playVideo (const std::string& resourceName);
    
    /// Get the current playback time position in the video, in seconds
    double getCurrentTime();
    
    /// Get the duration of the video in seconds
    double getDuration();
    
    /// Seek to the specified time position in the video
    void seek(double time);
    
    void play();
    void pause();
    bool isPaused();
    
    /// This should be called every frame by the user to update the video texture.
    /// @return Returns true if the video is still playing, false if we have reached the end of the video stream.
    bool update();
    
    /// Stop the currently playing video, if a video is playing.
    void close();

    /// Return the width of the currently playing video, or 0 if no video is playing.
    int getVideoWidth();
    /// Return the height of the currently playing video, or 0 if no video is playing.
    int getVideoHeight();
    
protected:
    bool _isVREnabled;
    
};

#endif