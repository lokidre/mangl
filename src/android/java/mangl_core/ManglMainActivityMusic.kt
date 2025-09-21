package mangl

import android.media.AudioAttributes
import android.media.AudioFocusRequest
import android.media.AudioManager
import android.media.MediaPlayer
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.KeyEvent
import mangl.ManglDebug.log


abstract class ManglMainActivityMusic: ManglMainActivityGui() {
    enum class MusicState (val value: Int) {
        Started(1),
        Stopped(2),
        Finished(3),
        Paused(4),
        Resumed(5),
    }


    companion object {
        @JvmStatic
        private external fun nOnMusicState(trackId: Int, state: Int)

        private fun musicPlayerOnResume() {
        }
    }

    //
    // Music
    //
    private var mAudioManager: AudioManager? = null
    private var mMediaPlayer: MediaPlayer? = null
    private var mMusicTrackId = 0
    private var mMusicVolume = 1f
    private var musicPlaying_ = false


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        volumeControlStream = AudioManager.STREAM_MUSIC
    }

    override fun onResume() {
        super.onResume()
        musicPlayerOnResume()
    }

    override fun onStop() {
        super.onStop()

        musicStop(0.0F, 0.0F);
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent?): Boolean {
        return when (keyCode) {
            KeyEvent.KEYCODE_VOLUME_UP -> {
//                log("KeyEvent.KEYCODE_VOLUME_UP")
                if (mAudioManager != null)
                    mAudioManager!!.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI)
                // Handle volume up button
                true // Indicate the event is handled
            }
            KeyEvent.KEYCODE_VOLUME_DOWN -> {
//                log("KeyEvent.KEYCODE_VOLUME_DOWN")
                if (mAudioManager != null)
                    mAudioManager!!.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI)
                // Handle volume down button
                true // Indicate the event is handled
            }
            else -> super.onKeyDown(keyCode, event)
        }
    }

    private fun initMusicPlayer() {
        if (mMediaPlayer != null) return

//        volumeControlStream = AudioManager.STREAM_MUSIC

        mAudioManager = getSystemService(AUDIO_SERVICE) as AudioManager

        mMediaPlayer = MediaPlayer()

//        val mAudioAttributes = AudioAttributes.Builder()
//            .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
//            .setUsage(AudioAttributes.USAGE_MEDIA)
//            .build()
//
//        mMediaPlayer!!.setAudioAttributes(mAudioAttributes)

        val focusRequestListener = AudioManager.OnAudioFocusChangeListener { focusChange ->
            when (focusChange) {
                AudioManager.AUDIOFOCUS_GAIN -> {
//                    log("AudioManager.AUDIOFOCUS_GAIN")
                }
                AudioManager.AUDIOFOCUS_LOSS -> {
//                    log("AudioManager.AUDIOFOCUS_LOSS")
                }
                AudioManager.AUDIOFOCUS_LOSS_TRANSIENT -> {
                    log("AudioManager.AUDIOFOCUS_LOSS")
                }
            }
        }

        val focusRequest = AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN).run {
            setAudioAttributes(AudioAttributes.Builder().run {
                setUsage(AudioAttributes.USAGE_GAME)
//                setUsage(AudioAttributes.USAGE_MEDIA)
                setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                build()
            })
            setAcceptsDelayedFocusGain(true)
            setOnAudioFocusChangeListener(focusRequestListener)
            build()
        }


//        val focusRequest = AudioFocusRequest.Builder(AudioManager.AUDIOFOCUS_GAIN)
//            .setAudioAttributes(mAudioAttributes!!)
//            .setWillPauseWhenDucked(true)
//            .setOnAudioFocusChangeListener { focusChange ->
//                when (focusChange) {
//                    AudioManager.AUDIOFOCUS_GAIN -> {
//                        // Handle gaining audio focus
//                    }
//                    AudioManager.AUDIOFOCUS_LOSS -> {
//                        // Handle losing audio focus
//                    }
//                    AudioManager.AUDIOFOCUS_LOSS_TRANSIENT -> {
//                        // Handle temporary loss
//                    }
//                }
//            }
//            .build()

        val audioFocusRequestResult = mAudioManager!!.requestAudioFocus(focusRequest)

//        log("AudioFocusRequestResult: %d", audioFocusRequestResult)

        mMediaPlayer!!.setOnPreparedListener {
            mMediaPlayer!!.setVolume(mMusicVolume, mMusicVolume)
            mMediaPlayer!!.start()
            nOnMusicState(mMusicTrackId, MusicState.Started.value)
        }

        mMediaPlayer!!.setOnCompletionListener {
            nOnMusicState(mMusicTrackId, MusicState.Finished.value)
            musicPlaying_ = false
        }

        mMediaPlayer!!.setOnErrorListener { mp, what, extra ->
            log("MediaPlayer::Error: %d, %d", what, extra)
            false
        }
    }


    fun musicIsOtherAudioPlaying(): Boolean {
        return false

//        if (mAudioManager == null) musicPlayerOnCreate()
//
//        val focusResult = mAudioManager!!.requestAudioFocus(musicFocusRequest_!!)
//
//        return (focusResult == AudioManager.AUDIOFOCUS_REQUEST_FAILED)
    }


    private fun musicPlayImpl(filename: String, trackId: Int, ramp: Float, volume: Float) {
        log("MediaPlayer::play: %s", filename)

        try {
            initMusicPlayer()

            mMediaPlayer!!.reset()

            if (filename[0] == ':') {
                val assetFilename = filename.substring(filename.indexOf('/') + 1)

                val afd = assets.openFd(assetFilename)
                mMediaPlayer!!.setDataSource(afd.fileDescriptor, afd.startOffset, afd.length)
                afd.close()
            } else {
                mMediaPlayer!!.setDataSource(filename)
            }

            mMusicTrackId = trackId
            mMusicVolume = volume
            mMediaPlayer!!.prepareAsync()

            //player_.prepare();
        } catch (e: Exception) {
            println(e.message)
        }
    }

    private fun musicPlay(
        filename: String,
        trackId: Int,
        delay: Float,
        ramp: Float,
        volume: Float
    ) {
        if (delay == 0f) {
            musicPlayImpl(filename, trackId, ramp, volume)
        } else {
            Handler(Looper.getMainLooper()).postDelayed({
                musicPlayImpl(filename, trackId, ramp, volume)
            }, (delay * 1000).toLong())
        }
    }

    fun musicPlayCrossfade(
        filename: String,
        trackId: Int,
        delay: Float,
        ramp: Float,
        gap: Float,
        volume: Float
    ) {
        if (delay == 0f) {
            musicPlayImpl(filename, trackId, ramp, volume)
        } else {
            Handler(Looper.getMainLooper()).postDelayed({
                musicPlayImpl(filename, trackId, ramp, volume)
            }, (delay * 1000).toLong())
        }
    }


    fun musicIsPlaying(): Boolean {
        if (mMediaPlayer == null) return false
        return mMediaPlayer!!.isPlaying
    }

    fun musicSetVolume(volume: Float, ramp: Float) {
        mMusicVolume = volume
        if (mMediaPlayer == null) return
        mMediaPlayer!!.setVolume(volume, volume)
    }

    fun musicStop(delay: Float, ramp: Float) {
//        ManglDebug.log("musicStop");

        if (mMediaPlayer == null) return
        if (mMediaPlayer!!.isPlaying) mMediaPlayer!!.stop()
    }

    fun musicPause() {
        if (mMediaPlayer == null) return
        if (mMediaPlayer!!.isPlaying) {
            mMediaPlayer!!.pause()
        }
    }

    fun musicResume() {
        if (mMediaPlayer == null) return
        if (!mMediaPlayer!!.isPlaying) {
            mMediaPlayer!!.start()
        }
    }

}
