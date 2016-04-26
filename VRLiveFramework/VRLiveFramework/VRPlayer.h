#ifndef VRPlayer_H_
#define VRPlayer_H_


class VRGame;

namespace vrlive
{
    const char* version();

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
        virtual ~VRPlayer();
        
        /**
         * Play the given video. If a video is already playing, the old video is closed first.
         * @note The video will be unpaused by default. Use the pause() and play() methods to control pausing.
         */
        bool playVideo (const char* resourceName);
        
        
    protected:
        
        bool initalize();
        void deInitalize();
        VRGame* _playerObj;
    };
}
#endif
