package mangl

import android.accounts.AccountManager
import android.app.AlertDialog
import android.content.DialogInterface
import android.content.SharedPreferences
import android.content.pm.PackageManager
import android.media.AudioManager
import android.os.Bundle
import android.widget.FrameLayout
import androidx.core.app.ActivityCompat
import com.google.androidgamesdk.GameActivity
import config.ManglConfig
import mangl.ManglDebug.log

abstract class ManglMainActivityJni : GameActivity() {

    protected var mDied: Boolean = false

    companion object {
        init {
            ManglConfig.onInit()

            if (ManglProjectConfig.STL_SHARED) {
                System.loadLibrary("c++_shared")
            }

            log("Loading JNI: %s", ManglProjectConfig.LIB_PATH)
            System.loadLibrary(ManglProjectConfig.LIB_PATH)
        }

        @JvmStatic
        fun jmFinishAppStatic() {
            ManglApp.exit()
        }
    }


    public override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        mPreferences = getSharedPreferences("jni", MODE_PRIVATE)
        volumeControlStream = AudioManager.STREAM_MUSIC

        //onCreateInstance();
        //created_ = true
    }

    protected fun getRootFramelayout(): FrameLayout {
        return findViewById(contentViewId)
    }


    //    static protected View getRootView() {
    //        MMainActivityJNI activity = getInstance();
    //        return activity.findViewById(activity.contentViewId);
    //    }
    //    @SuppressWarnings("unused")
    //    private static String jm_getUuidString() {
    //        return UUID.randomUUID().toString();
    //    }
    //
    // Exiting the application
    //

    private fun jmFinishApp() {
        //Log.d(getClass().getName(), "jmFinishApp");
        //finishAffinity();
        //finishAndRemoveTask();
        //System.exit(0);
        ManglApp.exit()
    }

    fun getDeviceUserName(): String {
        val am = AccountManager.get(this)
        val accounts = am.getAccountsByType("com.google")
        if (accounts.isEmpty()) return "Player"
        return accounts[0].name
    }


    //
    // Exceptions and error handling
    //
    private fun jmDie(message: String?) {
        mDied = true
        runOnUiThread {
            val alert = AlertDialog.Builder(this)
            alert.setCancelable(false).setTitle("Critical Error!").setMessage(message)
            alert.setPositiveButton("Close") { _: DialogInterface?, _: Int -> finishAffinity() }
            alert.create().show()
        }
    }


    //
    // Settings
    //
    private var mPreferences: SharedPreferences? = null

    fun settingsSetString(name: String, value: String) {
        if (mPreferences != null) mPreferences!!.edit().putString(name, value).apply()
    }

    fun settingsCommit() {
        if (mPreferences != null) mPreferences!!.edit().apply()
    }

    fun settingsGetString(name: String, defaultValue: String): String? {
        if (mPreferences != null) return mPreferences!!.getString(name, defaultValue)
        return ""
    }


    //
    // Permissions
    //

    enum class PermissionRequest {
        Unknown,
        BluetoothEnable,
        BluetoothConnect,
        BluetoothAdvertise,
        BluetoothAdmin,
        CoarseLocation,
        FineLocation,
    }

    enum class PermStatus {
        Pending,
        Granted,
        Denied,
    }

    class PermRequestCtx(reqCode: Int, perm: String) {
        val code = reqCode;
        val permission = perm;
        var status = PermStatus.Pending;
    }

    private val mPermissionsRequested = mutableMapOf<Int, PermRequestCtx>();

    protected fun getPermissionsStatus(): PermStatus {
        for ((key, value) in mPermissionsRequested) {
            if (value.status == PermStatus.Denied)
                return PermStatus.Denied;
            if (value.status == PermStatus.Pending)
                return PermStatus.Pending;
        }
        return PermStatus.Granted;
    }

    protected fun requestPermissions(permissions: Map<Int, String>): PermStatus {
        mPermissionsRequested.clear()

        for ((key, value) in permissions) {
            if (ActivityCompat.checkSelfPermission(this, value) == PackageManager.PERMISSION_GRANTED)
                continue;

            val context = mPermissionsRequested[key];
            if (context == null)
                mPermissionsRequested[key] = PermRequestCtx(key, value)
            else
                if (context.status != PermStatus.Granted)
                    context.status = PermStatus.Pending;
        }

        if (mPermissionsRequested.isEmpty())
            return PermStatus.Granted;

        //
        val pending = mPermissionsRequested.iterator().next().value

        runOnUiThread {
            ActivityCompat.requestPermissions(this, arrayOf(pending.permission), pending.code)
        }

        return PermStatus.Pending;
    }

    protected fun checkPermission(permission: String, requestCode: PermissionRequest): Boolean {
        //ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, REQUEST_ENABLE_BT);
        if (ActivityCompat.checkSelfPermission(this,permission) == PackageManager.PERMISSION_GRANTED) {
            log("Permission granted: %s", permission)
            return true
        }
        ActivityCompat.requestPermissions(this, arrayOf(permission), requestCode.ordinal)
        return false
    }



    override fun onRequestPermissionsResult(code: Int, perms: Array<String>, results: IntArray) {
        super.onRequestPermissionsResult(code, perms, results)

        for (result in results) {

            val context = mPermissionsRequested[code];
            if (context != null) {
                log("onRequestPermissionsResult: %s, %s", context.permission, if (result == PackageManager.PERMISSION_GRANTED) "Granted" else "Denied")

                when (result) {
                    PackageManager.PERMISSION_GRANTED -> context.status = PermStatus.Granted
                    PackageManager.PERMISSION_DENIED -> context.status = PermStatus.Denied
                    else -> context.status = PermStatus.Denied
                }
            } else {
                log("onRequestPermissionsResult: %d, %s", code, if (result == PackageManager.PERMISSION_GRANTED) "Granted" else "Denied")
            }
        }

        // Search for pending permissions
        for ((key, value) in mPermissionsRequested) {
            if (value.status == PermStatus.Pending) {
                ActivityCompat.requestPermissions(this, arrayOf(value.permission), key)
                break;
            }
        }
    }

}


