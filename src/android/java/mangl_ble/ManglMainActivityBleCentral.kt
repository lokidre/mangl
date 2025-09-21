package mangl

import android.Manifest
import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothGatt
import android.bluetooth.BluetoothGattCallback
import android.bluetooth.BluetoothGattCharacteristic
import android.bluetooth.BluetoothGattDescriptor
import android.bluetooth.BluetoothProfile
import android.bluetooth.le.BluetoothLeScanner
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanFilter
import android.bluetooth.le.ScanResult
import android.bluetooth.le.ScanSettings
import android.os.Build
import android.os.Handler
import android.os.HandlerThread
import android.os.ParcelUuid
import androidx.activity.result.ActivityResult
import mangl.ManglDebug.log
import java.util.UUID

abstract class ManglMainActivityBleCentral: ManglMainActivityBlePeripheral() {
    //
    // Central / Client
    //
    //private var mPendingBluetoothEnable = false;
    private var mBleScanner: BluetoothLeScanner? = null
    private var mIsScanning = false;
    private var mPendingBleScanning = false;
    private var mIsConnected = false;
    private var mIsSubscribed = false;

    private var mConnectedGatt: BluetoothGatt? = null
    //private var mChrForTransfer: BluetoothGattCharacteristic? = null
//    private var mCharacteristicForRead: BluetoothGattCharacteristic? = null
//    private var mCharacteristicForWrite: BluetoothGattCharacteristic? = null
    //private var mCharacteristicForIndicate: BluetoothGattCharacteristic? = null

    private val mScanCallback = BleScanCallback()
    private val mGattCallback = BleGattCallback()

    private val handlerThread = HandlerThread("BLECentralThread")
    private lateinit var handler: Handler


   init {
        handlerThread.start()
        handler = Handler(handlerThread.looper)
    }

    companion object {
        @JvmStatic
        protected external fun jniBleCentralStatusCallback(action: Int, code: Int, message: String?)

        @JvmStatic
        protected external fun jniBleCentralReadDataCallback(data: ByteArray)
    }


    private fun jniBleCentralError(message: String) {
        jniBleCentralStatusCallback(JniAction.Error.value, JniCode.Error.value, message);
    }


    private fun bleCentralInit() {
        initBluetooth()
    }

    @SuppressLint("MissingPermission")
    private fun bleEnableCentral(): Boolean {
        bleCentralInit()

        if (!mBluetoothAdapter!!.isEnabled) {
            try {
                launchIntent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            } catch(e: SecurityException) {
                jm_alert("Please Enable BlueTooth", "Error!")
            }
            return false;
        }

        return true;
    }

    private fun jmBleStartScan() {
        handler.post {
            if (!bleEnableCentral()) {
                mPendingBleScanning = true
                return@post
            }

            when (checkCentralPermissions()) {
                PermStatus.Granted -> bleStartScanImpl()
                PermStatus.Denied -> jniBleCentralError("Bluetooth Access Denied");
                PermStatus.Pending -> mPendingBleScanning = true
            }
        }
    }

    @SuppressLint("MissingPermission")
    private fun bleStartScanImpl() {
        if (mIsScanning) {
            jniBleCentralError("BLE Already Scanning")
            return
        }


        if (mBleScanner == null)
            mBleScanner = mBluetoothAdapter!!.bluetoothLeScanner


        val mScanFilter = ScanFilter.Builder().setServiceUuid(ParcelUuid(mUuidService)).build()

        val mScanSettings = ScanSettings.Builder()
            .setScanMode(ScanSettings.SCAN_MODE_BALANCED)
            .setCallbackType(ScanSettings.CALLBACK_TYPE_FIRST_MATCH)
            //.setMatchMode(ScanSettings.MATCH_MODE_AGGRESSIVE)
            .setNumOfMatches(ScanSettings.MATCH_NUM_ONE_ADVERTISEMENT)
            .setReportDelay(0)
            .build()


        val serviceFilter = mScanFilter.serviceUuid?.uuid
        log("Starting BLE scan, filter: $serviceFilter")

        mIsScanning = true

        mBleScanner!!.startScan(mutableListOf(mScanFilter), mScanSettings, mScanCallback)
        //mBleScanner!!.startScan(mutableListOf(), mScanSettings, mScanCallback)
        //mBleScanner!!.startScan(mScanCallback)
    }

    private fun jmBleStopScan() {
        handler.post {
            bleStopScan()
        }
    }

    @SuppressLint("MissingPermission")
    private fun bleStopScan() {
        if (!mIsScanning) return
        mBleScanner!!.stopScan(mScanCallback)
        mIsScanning = false;
    }


    @SuppressLint("MissingPermission")
    @Suppress("deprecation")
    private fun jmBleCentralSendData(data: ByteArray) {
        handler.post {
            val gatt = mConnectedGatt ?: return@post
            val characteristic = mChrForL2P ?: return@post
            //characteristic.writeType = BluetoothGattCharacteristic.WRITE_TYPE_DEFAULT
            characteristic.writeType = BluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE
            characteristic.value = data

            log("jmBleCentralSendData");
            gatt.writeCharacteristic(characteristic)
        }
    }

    @SuppressLint("MissingPermission")
    @Suppress("deprecation")
    private fun setCharacteristicNotification(characteristic: BluetoothGattCharacteristic?, gatt: BluetoothGatt, enabled: Boolean) {
        if (characteristic == null) {
            log("ERROR: characteristic is null")
            return
        }

        val descriptor = characteristic.getDescriptor(mUuidDescriptorCcc) ?: return

        val result = gatt.setCharacteristicNotification(characteristic, enabled)

        if (!result) {
            log("ERROR: setCharacteristicNotification failed for ${characteristic.uuid}")
            return
        }

        //descriptor.value = if (enabled) BluetoothGattDescriptor.ENABLE_INDICATION_VALUE else BluetoothGattDescriptor.DISABLE_NOTIFICATION_VALUE
        descriptor.value = if (enabled) BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE else BluetoothGattDescriptor.DISABLE_NOTIFICATION_VALUE
        //descriptor.value = BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE
        gatt.writeDescriptor(descriptor)



//
//        characteristic.getDescriptor(mUuidDescriptorCcc)?.let { descriptor ->
//            if (!gatt.setCharacteristicNotification(characteristic, true)) {
//                log("ERROR: setNotification(true) failed for ${characteristic.uuid}")
//                return
//            }
//
//            //gatt.writeDescriptor(cccDescriptor, BluetoothGattDescriptor.ENABLE_INDICATION_VALUE)

//            descriptor.value = BluetoothGattDescriptor.ENABLE_INDICATION_VALUE
//            gatt.writeDescriptor(descriptor)
//        }
    }


    private fun checkCentralPermissions(): PermStatus {
        //log("checking central permissions")
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            val permissions = mapOf(
                PermissionRequest.FineLocation.ordinal to Manifest.permission.ACCESS_FINE_LOCATION,
                PermissionRequest.BluetoothConnect.ordinal to Manifest.permission.BLUETOOTH_CONNECT,
                PermissionRequest.BluetoothAdvertise.ordinal to Manifest.permission.BLUETOOTH_SCAN,
                PermissionRequest.BluetoothAdmin.ordinal to Manifest.permission.BLUETOOTH_ADMIN,
            )
            return requestPermissions(permissions);
        }
        return PermStatus.Granted
    }


    override fun onRequestPermissionsResult(code: Int, perms: Array<String>, results: IntArray) {
        super.onRequestPermissionsResult(code, perms, results)

        when (getPermissionsStatus()) {
            PermStatus.Denied -> {
                //if (mPendingBluetoothEnable) {
                    jniBleCentralError("Bluetooth Permission Denied")
                //}
            }
            PermStatus.Granted -> {
//                if (mPendingBluetoothEnable) {
//                    mPendingBluetoothEnable = false;
//                    launchBluetoothEnableIntent();
//                }
                if (mPendingBleScanning) {
                    mPendingBleScanning = false;
                    bleStartScanImpl();
                }
            }
            PermStatus.Pending -> {}
        }
    }

    override fun onIntentResult(result: ActivityResult) {
        super.onIntentResult(result)

        //log("onIntentResult: BLE: %s", result);

        if (mIntentAction == BluetoothAdapter.ACTION_REQUEST_ENABLE) {

            if (mPendingBleScanning) {
                mPendingBleScanning = false
                when (checkCentralPermissions()) {
                    PermStatus.Granted -> bleStartScanImpl()
                    PermStatus.Denied -> jniBleCentralError("Bluetooth Access Denied");
                    PermStatus.Pending -> mPendingBleScanning = true
                }
            }
        }

    }


    @SuppressLint("MissingPermission")
    private fun connectToDevice(device: BluetoothDevice) {
        //device.connectGatt(this, false, mGattCallback)
        device.connectGatt(
            this,
            false,
            mGattCallback,
            BluetoothDevice.TRANSPORT_LE,
            BluetoothDevice.PHY_LE_1M_MASK or BluetoothDevice.PHY_LE_2M_MASK,
            handler)
    }

    private inner class BleScanCallback: ScanCallback() {
        @SuppressLint("MissingPermission")
        override fun onScanResult(callbackType: Int, result: ScanResult) {
            val name: String? = result.scanRecord?.deviceName ?: result.device.name
            log("onScanResult name=${name} address=${result.device?.address}")
            bleStopScan()
            connectToDevice(result.device)
        }

        override fun onBatchScanResults(results: MutableList<ScanResult>?) {
            log("onBatchScanResults")
        }

        override fun onScanFailed(errorCode: Int) {
            log("onScanFailed errorCode=$errorCode")
            bleStopScan()
            jniBleCentralError("BLE Scan Failed")
        }
    }

    private inner class BleGattCallback : BluetoothGattCallback() {
        @SuppressLint("MissingPermission")
        override fun onConnectionStateChange(gatt: BluetoothGatt, status: Int, newState: Int) {
            super.onConnectionStateChange(gatt, status, newState)

            log("onConnectionStateChange: status=${status} newState=${newState}")

            //if (status != BluetoothGatt.GATT_SUCCESS) return

            //val deviceAddress = gatt.device.address
//            if (status == 255) {
//                return
//            }

            if (status != BluetoothGatt.GATT_SUCCESS) {
                jniBleCentralStatusCallback(JniAction.Status.value, JniCode.Disconnected.value, null)
                gatt.close()
                return;
            }

            when (newState) {
                BluetoothProfile.STATE_CONNECTED -> {
                    gatt.discoverServices()
                }
                BluetoothProfile.STATE_DISCONNECTED -> {
                    gatt.close()
                    jniBleCentralStatusCallback(JniAction.Status.value, JniCode.Disconnected.value, null)
                }
                BluetoothProfile.STATE_CONNECTING -> {
                    jniBleCentralStatusCallback(JniAction.Status.value, JniCode.Connecting.value, null)
                }
                else -> {}
            }

//            if (newState == BluetoothProfile.STATE_CONNECTED) {
//            }


        }

        @SuppressLint("MissingPermission")
        override fun onServicesDiscovered(gatt: BluetoothGatt, status: Int) {
            super.onServicesDiscovered(gatt, status)

            log("onServicesDiscovered: status=${status}");

            // Internal error
            if (status == 129) {
                gatt.disconnect()
                return
            }

            val service = gatt.getService(mUuidService) ?: run {
                gatt.disconnect()
                return
            }

            if (status == BluetoothGatt.GATT_SUCCESS) {


                mConnectedGatt = gatt

                //mConnectedGatt?.requestConnectionPriority(BluetoothGatt.CONNECTION_PRIORITY_BALANCED)
                mConnectedGatt?.requestConnectionPriority(BluetoothGatt.CONNECTION_PRIORITY_HIGH)



                mChrForL2P = service.getCharacteristic(mUuidChrForL2P)
                setCharacteristicNotification(mChrForL2P, gatt, true)

                mChrForP2L = service.getCharacteristic(mUuidChrForP2L)
                setCharacteristicNotification(mChrForP2L, gatt, true)

//                mCharacteristicForRead?.let {
//                    subscribeToIndications(it, gatt)
//                } ?: run {
//                    log("Characteristic not found: $cUuidCharForRead")
//                }

//                if (mCharacteristicForRead != null) {
//                    gatt.setCharacteristicNotification(mCharacteristicForRead, true)
//                    val descriptor = mCharacteristicForRead!!.getDescriptor(UUID.fromString("00002902-0000-1000-8000-00805f9b34fb"))
//                    gatt.writeDescriptor(descriptor)
//                }

//                mCharacteristicForWrite = service.getCharacteristic(mUuidCharForWrite)
//                setCharacteristicNotification(mCharacteristicForWrite, gatt, true)

                //mCharacteristicForIndicate = service.getCharacteristic(mUuidCharForIndicate)
                //setCharacteristicNotification(mCharacteristicForIndicate, gatt, true)

//                mCharacteristicForIndicate?.let {
//                    subscribeToIndications(it, gatt)
//                } ?: run {
//                    log("Characteristic not found: $cUuidCharForIndicate")
//                }

                jniBleCentralStatusCallback(JniAction.Status.value, JniCode.Connected.value, null)
            }

        }

        @SuppressLint("MissingPermission")
        @Deprecated("Deprecated in Java")
        @Suppress("deprecation")
        override fun onCharacteristicChanged(gatt: BluetoothGatt, characteristic: BluetoothGattCharacteristic) {
            super.onCharacteristicChanged(gatt, characteristic)
            log("onCharacteristicChanged: ${characteristic.uuid} value=${characteristic.value.toString(Charsets.UTF_8)}")

            if (characteristic.uuid == mUuidChrForP2L) {
                gatt.readCharacteristic(characteristic)
                jniBleCentralReadDataCallback(characteristic.value)
            }
        }

        @Deprecated("Deprecated in Java", ReplaceWith("log(\"onCharacteristicRead: status=\${status}\")","log"))
        @Suppress("deprecation")
        override fun onCharacteristicRead(gatt: BluetoothGatt,
                                          characteristic: BluetoothGattCharacteristic,
                                          status: Int) {
            super.onCharacteristicRead(gatt, characteristic, status)
            log("onCharacteristicRead: status=${status} chr=${characteristic.uuid} value=${characteristic.value.toString(Charsets.UTF_8)}");

//            if (characteristic.uuid == mUuidCharForTransfer) {
//                jniBleCentralReadDataCallback(characteristic.value)
//            }
        }



        override fun onCharacteristicWrite(gatt: BluetoothGatt,
                                           characteristic: BluetoothGattCharacteristic,
                                           status: Int) {
            super.onCharacteristicWrite(gatt, characteristic, status)
            log("onCharacteristicWrite: status=${status} chr=${characteristic.uuid}");

//            if (characteristic.uuid == mUuidCharForWrite) {
//                if (status != BluetoothGatt.GATT_SUCCESS) {
//                    //log
//                }
//            }
        }




        @Suppress("deprecation")
        override fun onDescriptorWrite(gatt: BluetoothGatt, descriptor: BluetoothGattDescriptor, status: Int) {
            super.onDescriptorWrite(gatt, descriptor, status)

            log("onDescriptorWrite status=${status} descriptor=${descriptor.uuid} chr=${descriptor.characteristic.uuid}")

            //if (descriptor.characteristic.uuid == mUuidCharForIndicate) {
            if (descriptor.characteristic.uuid == mUuidChrForL2P) {

                if (status == BluetoothGatt.GATT_SUCCESS) {
                    val value = descriptor.value
                    val isSubscribed = value.isNotEmpty() && value[0].toInt() != 0
                    log("  subscribed=$isSubscribed");
                }

                mIsConnected = true
            }
        }

    }


}


