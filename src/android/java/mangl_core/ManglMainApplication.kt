package mangl

import android.app.Application
//import com.google.android.gms.games.PlayGamesSdk

open class ManglMainApplication : Application() {
    override fun onCreate() {
        config.ManglConfig.onInit()

        super.onCreate()


//        if (ManglProjectConfig.MANGL_PLAY_GAMES) {
//            PlayGamesSdk.initialize(this)
//        }
    }
}
