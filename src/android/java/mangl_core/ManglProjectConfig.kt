package mangl

abstract class ManglProjectConfig {

    companion object {
        const val LIB_PATH: String = "main"
        const val STL_SHARED: Boolean = false

        @JvmField
        var LOG_TAG: String = "mangl"
    }
}
