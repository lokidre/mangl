package mangl

import android.util.Log

internal object ManglDebug {
    @JvmStatic
    fun log(text: String) {
        Log.d(ManglProjectConfig.LOG_TAG, text)
    }

    @JvmStatic
    fun log(fmt: String, vararg args: Any?) {
        log(String.format(fmt, *args))
    }
}
