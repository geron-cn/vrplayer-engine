#ifndef VRPlayer_H_
#define VRPlayer_H_

#include <string>
/**
 * Stereo Render.
 */
class VRPlayer
{
    friend class SphereVideoSurround;
public:

    static VRPlayer* getInstance();
    
    void* initialize(int x = 0, int y = 0, int widith = 0, int height = 0);
    
    void finalize();
    
    bool play(const char* url);
    
    double getDuration();
    
    void stop();
    
    bool isPause() const;
    
    bool pause(bool p);
    
    double getCurrentPos() const;
    
    bool seek(double pos);
    
    bool isVREnabled() const { return _vrEnabled; }
    
    void setVREnabled(bool enable) { _vrEnabled = enable; }

protected:
    /**
     * Constructor.
     */
    VRPlayer();
    
    ~VRPlayer();
    
    bool _vrEnabled;
    
    std::string _url;
    
//    std::

private:
    
};

#endif
