package mangl

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothGattCharacteristic
import android.bluetooth.BluetoothManager
import android.content.Context
import android.content.pm.PackageManager
import java.util.UUID
import java.util.concurrent.atomic.AtomicInteger


abstract class ManglMainActivityBleBase: ManglMainActivityMusic() {
    protected enum class JniAction(val value: Int) {
        Invalid(0),
        Error(1),
        Status(2),
    }

    protected enum class JniCode(val value: Int) {
        Invalid(0),
        Error(1),
        Connected(2),
        Disconnected(3),
        Connecting(4),
    }

    protected class DiscoveryInfo {
        var alias: String? = null
        var proto: String? = null
        var protv: String? = null
        var appn: String? = null
        var appv: String? = null
        var uuid: String? = null
        var params: HashMap<String, String>? = null

        val ksep = ':'
        val rsep = ";"

        override fun toString(): String {
            var vals = arrayOf<String>()
            if (alias != null) vals += "alin$ksep$alias"
            if (proto != null) vals += "prtn$ksep$proto"
            if (protv != null) vals += "prtv$ksep$protv"
            if (appn != null) vals += "appn$ksep$appn"
            if (appv != null) vals += "appv$ksep$appv"
            if (uuid != null) vals += "uuid$ksep$uuid"
            return vals.joinToString(rsep)
        }
    }

    protected val cUuidService = "25AE1441-05D3-4C5B-8281-93D4E07420CF"
    protected val cUuidChrForL2P = "25AE1442-05D3-4C5B-8281-93D4E07420CF"
    protected val cUuidChrForP2L = "25AE1443-05D3-4C5B-8281-93D4E07420CF"
    //protected val cUuidCharForRead = "25AE1442-05D3-4C5B-8281-93D4E07420CF"
    //protected val cUuidCharForWrite = "25AE1443-05D3-4C5B-8281-93D4E07420CF"
    //protected val cUuidCharForIndicate = "25AE1444-05D3-4C5B-8281-93D4E07420CF"

    private val cUuidDescriptorCcc = "00002902-0000-1000-8000-00805f9b34fb"
    protected val mUuidDescriptorCcc: UUID = UUID.fromString(cUuidDescriptorCcc)

    protected val mUuidService: UUID = UUID.fromString(cUuidService)
    protected val mUuidChrForL2P: UUID = UUID.fromString(cUuidChrForL2P)
    protected val mUuidChrForP2L: UUID = UUID.fromString(cUuidChrForP2L)
    //protected val mUuidCharForRead: UUID = UUID.fromString(cUuidCharForRead)
    //protected val mUuidCharForWrite: UUID = UUID.fromString(cUuidCharForWrite)
    //protected val mUuidCharForIndicate: UUID = UUID.fromString(cUuidCharForIndicate)

    protected var mChrForL2P: BluetoothGattCharacteristic? = null
    protected var mChrForP2L: BluetoothGattCharacteristic? = null




    protected var mBluetoothManager: BluetoothManager? = null
    protected var mBluetoothAdapter: BluetoothAdapter? = null

    protected fun initBluetooth() {
        if (mBluetoothManager == null)
            mBluetoothManager = getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        if (mBluetoothAdapter == null)
            mBluetoothAdapter = mBluetoothManager!!.adapter
    }

    protected fun bluetoothShutdown() {
        mBluetoothAdapter = null
        mBluetoothManager = null
    }

    private fun jmBluetoothIsAvailable(): Boolean { return bluetoothIsAvailable(); }
    //private fun jmBluetoothIsEnabled(): Boolean { return bluetoothIsEnabled(); }

    fun bluetoothIsAvailable(): Boolean {
        val bluetoothAvailable = packageManager.hasSystemFeature(PackageManager.FEATURE_BLUETOOTH)
        if (!bluetoothAvailable) return false
        return true

//        initBluetooth()
//        return mBluetoothAdapter != null
    }

//    fun bluetoothIsEnabled(): Boolean {
//        initBluetooth()
//        if (mBluetoothAdapter == null) return false
//        return mBluetoothAdapter!!.isEnabled
//    }


    private val mPeerKeyCounter = AtomicInteger(0)

    protected fun genPeerKey(): Int {
        return mPeerKeyCounter.addAndGet(1)
    }

}
