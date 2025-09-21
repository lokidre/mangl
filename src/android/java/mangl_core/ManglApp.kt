package mangl

import java.util.Timer
import java.util.TimerTask
import kotlin.system.exitProcess

internal object ManglApp {
    @JvmStatic
    fun exit() {
        Timer().schedule(object : TimerTask() {
            override fun run() {
                exitProcess(0)
            }
        }, 500)
    }
}
