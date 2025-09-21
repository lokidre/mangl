package mangl

import android.Manifest
import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothGatt
import android.bluetooth.BluetoothGattCharacteristic
import android.bluetooth.BluetoothGattDescriptor
import android.bluetooth.BluetoothGattServer
import android.bluetooth.BluetoothGattServerCallback
import android.bluetooth.BluetoothGattService
import android.bluetooth.BluetoothProfile
import android.bluetooth.le.AdvertiseCallback
import android.bluetooth.le.AdvertiseData
import android.bluetooth.le.AdvertiseSettings
import android.bluetooth.le.BluetoothLeAdvertiser
import android.os.Build
import android.os.ParcelUuid
import androidx.activity.result.ActivityResult
import mangl.ManglDebug.log


abstract class ManglMainActivityBlePeripheral: ManglMainActivityBleBase() {
    //
    // Peripheral
    //
    private var mGattServer: BluetoothGattServer? = null
    private var mBleAdvertiser: BluetoothLeAdvertiser? = null
    private var mPendingBluetoothEnable = false
    private var mPendingBleStartAdvertising = false
    private var mIsAdvertising = false

    private class DeviceContext(dev: BluetoothDevice) {
        val device = dev
        var inBuffer: ByteArray? = null
        var outBuffer: ByteArray? = null
    }

    private val mSubscribedPeers = mutableMapOf<Int, DeviceContext>()
    private val mSubscribedDevices = mutableMapOf<BluetoothDevice, Int>()

    private var mAdvertiserCallback: BleAdvertiserCallback? = null
    private var mGattServerCallback: BleGattServerCallback? = null


    //private var mCharForSend: BluetoothGattCharacteristic? = null
    //private var mCharForIndicate: BluetoothGattCharacteristic? = null


    companion object {
        @JvmStatic
        private external fun jniBlePeripheralStatusCallback(action: Int, code: Int, message: String?)

        @JvmStatic
        private external fun jniBlePeripheralSubscribed(pk: Int, di: String)

        @JvmStatic
        private external fun jniBlePeripheralRecv(pk: Int, data: ByteArray)
    }


    private fun jniBlePeripheralError(message: String) {
        log("Error: %s", message)
        jniBlePeripheralStatusCallback(JniAction.Error.value, JniCode.Error.value, message);
    }

    @SuppressLint("MissingPermission")
    private fun jmBlePeripheralShutdown() {
        log("blePeripheralShutdown")

        runOnUiThread {
            jmBleStopAdvertising()

            for (peer in mSubscribedPeers) {
                mGattServer?.cancelConnection(peer.value.device)
            }

            mSubscribedPeers.clear()
            mSubscribedDevices.clear()

            mGattServer?.clearServices()
            mGattServer?.close()
            mGattServer = null


            bluetoothShutdown()
        }
    }


    private fun bleEnablePeripheral(): Boolean {
        initBluetooth()

        if (!mBluetoothAdapter!!.isEnabled) {
            launchIntent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            return false
        }

        return true
    }


    private fun jmBleStartAdvertising() {
        runOnUiThread {
            bleStartAdvertising()
        }
    }

    private fun bleStartAdvertising() {
        if (!bleEnablePeripheral()) {
            mPendingBleStartAdvertising = true
            return
        }

        if (!mBluetoothAdapter!!.isMultipleAdvertisementSupported()) {
            jniBlePeripheralError("Bluetooth Advertisement not supported");
            return
        }

        when (checkPeripheralPermissions()) {
            PermStatus.Granted -> bleStartAdvertisingImpl()
            PermStatus.Denied -> jniBlePeripheralError("Bluetooth Access Denied")
            PermStatus.Pending -> mPendingBleStartAdvertising = true
        }
    }


    @SuppressLint("MissingPermission")
    private fun bleStartAdvertisingImpl() {
        try {
            startGattServer();

            val advertiseSettings = AdvertiseSettings.Builder()
                //.setAdvertiseMode(AdvertiseSettings.ADVERTISE_MODE_BALANCED)
                .setAdvertiseMode(AdvertiseSettings.ADVERTISE_MODE_LOW_LATENCY)
                .setTxPowerLevel(AdvertiseSettings.ADVERTISE_TX_POWER_HIGH)
                .setConnectable(true)
                //.setLega(false)
                .build()


            val advertiseData = AdvertiseData.Builder()
                // Don't include name, if name size > 8 bytes, ADVERTISE_FAILED_DATA_TOO_LARGE
                .setIncludeDeviceName(false)
                .addServiceUuid(ParcelUuid(mUuidService))
                //.addServiceData(ParcelUuid(mUuidService), "sample".toByteArray())
                .build()

            if (mAdvertiserCallback == null)
                mAdvertiserCallback = BleAdvertiserCallback()


            mBleAdvertiser = mBluetoothAdapter!!.bluetoothLeAdvertiser
            mBleAdvertiser!!.startAdvertising(advertiseSettings, advertiseData, mAdvertiserCallback)
            //mBleAdvertiser?.startAdvertisingSet()
        }
        catch(e: Exception) {
            val text = if (e.message != null) e.message!! else "Error"
            jniBlePeripheralError(text);
        }
    }

    @SuppressLint("MissingPermission")
    private fun jmBleStopAdvertising() {
        runOnUiThread {
            if (mIsAdvertising) {
                mIsAdvertising = false
                stopGattServer()
            }

            mBleAdvertiser?.stopAdvertising(mAdvertiserCallback)
            mBleAdvertiser = null


            mAdvertiserCallback = null
        }
    }

    @SuppressLint("MissingPermission")
    private fun jmBlePeripheralDisconnect(pk: Int) {
        runOnUiThread {
            val ctx = mSubscribedPeers[pk] ?: return@runOnUiThread
            mGattServer?.cancelConnection(ctx.device)

            mSubscribedDevices.remove(ctx.device)
            mSubscribedPeers.remove(pk)
        }
    }


    @SuppressLint("MissingPermission")
    @Suppress("deprecation")
    private fun jmBlePeripheralSendData(pk: Int, data: ByteArray) {
        //log("jmBlePeripheralSendData")

        runOnUiThread {

            //log("  data: $data")
            // TODO: handle pk == 0 (broadcast)
            // TODO: bring back
            val ctx = mSubscribedPeers[pk] ?: return@runOnUiThread
            if (mGattServer == null) return@runOnUiThread

            if (mChrForP2L == null) return@runOnUiThread
            mChrForP2L?.value = data
            mGattServer?.notifyCharacteristicChanged(ctx.device, mChrForP2L, true)


            //if (mCharForIndicate == null) return@runOnUiThread
//        mCharForIndicate!!.value = data
//        mGattServer!!.notifyCharacteristicChanged(ctx.device, mCharForIndicate!!, true)
        }
    }

    @SuppressLint("MissingPermission")
    private fun startGattServer() {

        if (mGattServerCallback == null)
            mGattServerCallback = BleGattServerCallback();

        mGattServer = mBluetoothManager!!.openGattServer(this, mGattServerCallback)

        val service = BluetoothGattService(mUuidService, BluetoothGattService.SERVICE_TYPE_PRIMARY)



        mChrForL2P = BluetoothGattCharacteristic(mUuidChrForL2P,
            BluetoothGattCharacteristic.PROPERTY_READ
                    or BluetoothGattCharacteristic.PROPERTY_WRITE
                    or BluetoothGattCharacteristic.PROPERTY_NOTIFY
                    //or BluetoothGattCharacteristic.PROPERTY_INDICATE
                    ,
            BluetoothGattCharacteristic.PERMISSION_READ
                    or BluetoothGattCharacteristic.PERMISSION_WRITE)

        val descriptorL2P = BluetoothGattDescriptor(mUuidDescriptorCcc,
            BluetoothGattDescriptor.PERMISSION_READ
                    or BluetoothGattDescriptor.PERMISSION_WRITE)
        mChrForL2P?.addDescriptor(descriptorL2P)

        service.addCharacteristic(mChrForL2P)


        mChrForP2L = BluetoothGattCharacteristic(mUuidChrForP2L,
            BluetoothGattCharacteristic.PROPERTY_READ
                    or BluetoothGattCharacteristic.PROPERTY_WRITE
                    or BluetoothGattCharacteristic.PROPERTY_NOTIFY
                    //or BluetoothGattCharacteristic.PROPERTY_INDICATE
                    ,
            BluetoothGattCharacteristic.PERMISSION_READ
                    or BluetoothGattCharacteristic.PERMISSION_WRITE)

        val descriptorP2L = BluetoothGattDescriptor(mUuidDescriptorCcc,
            BluetoothGattDescriptor.PERMISSION_READ
                    or BluetoothGattDescriptor.PERMISSION_WRITE)
        mChrForP2L?.addDescriptor(descriptorP2L)


        service.addCharacteristic(mChrForP2L)




//        val charForWrite = BluetoothGattCharacteristic(mUuidCharForWrite,
//            BluetoothGattCharacteristic.PROPERTY_WRITE,
//            BluetoothGattCharacteristic.PERMISSION_WRITE)
//
//        service.addCharacteristic(charForWrite)

//        mCharForIndicate = BluetoothGattCharacteristic(mUuidCharForIndicate,
//            BluetoothGattCharacteristic.PROPERTY_INDICATE,
//            BluetoothGattCharacteristic.PERMISSION_READ)
//
//        val charConfigDescriptor = BluetoothGattDescriptor(mUuidDescriptorCcc,
//            BluetoothGattDescriptor.PERMISSION_READ or BluetoothGattDescriptor.PERMISSION_WRITE)
//
        //mCharForIndicate!!.addDescriptor(charConfigDescriptor)
        //service.addCharacteristic(mCharForIndicate)

        val result = mGattServer!!.addService(service)
    }

    @SuppressLint("MissingPermission")
    private fun stopGattServer() {
        //runOnUiThread {
        mGattServer?.close()
        mGattServer = null

        mGattServerCallback = null
        //}
    }

    private fun checkPeripheralPermissions(): PermStatus {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            val permissions = mapOf(
//                PermissionRequest.CoarseLocation.ordinal to Manifest.permission.ACCESS_COARSE_LOCATION,
                PermissionRequest.FineLocation.ordinal to Manifest.permission.ACCESS_FINE_LOCATION,
                PermissionRequest.BluetoothConnect.ordinal to Manifest.permission.BLUETOOTH_CONNECT,
                PermissionRequest.BluetoothAdvertise.ordinal to Manifest.permission.BLUETOOTH_ADVERTISE,
                //PermissionRequest.BluetoothEnable.ordinal to Manifest.permission.BLUETOOTH,
            )
            return requestPermissions(permissions)
        }
        return PermStatus.Granted
    }


    private fun handleIncomingData(device: BluetoothDevice, fragment: Boolean, value: ByteArray?) {
        if (value == null) return
        val pk = mSubscribedDevices[device] ?: return
        val ctx = mSubscribedPeers[pk] ?: return


        if (fragment) {
            if (ctx.inBuffer != null) {
                ctx.inBuffer = ctx.inBuffer!! + value
            } else {
                ctx.inBuffer = value
            }
        } else {
            jniBlePeripheralRecv(pk, value)
        }
    }

    private fun onExecuteWrite(device: BluetoothDevice) {
        val pk = mSubscribedDevices[device] ?: return
        val ctx = mSubscribedPeers[pk] ?: return
        if (ctx.inBuffer == null) return
        jniBlePeripheralRecv(pk, ctx.inBuffer!!)
        ctx.inBuffer = null
    }



    private inner class BleGattServerCallback : BluetoothGattServerCallback() {
        override fun onServiceAdded(status: Int, service: BluetoothGattService?) {
            super.onServiceAdded(status, service)

        }

        override fun onConnectionStateChange(device: BluetoothDevice, status: Int, newState: Int) {
            log("onConnectionStateChange status=$status, newState=$newState")
            when (newState) {
                BluetoothProfile.STATE_CONNECTED -> {

                }
                BluetoothProfile.STATE_DISCONNECTED -> {
                    mSubscribedDevices.remove(device)
                }
                else -> {}
            }


        }
        override fun onNotificationSent(device: BluetoothDevice, status: Int) {
            log("onNotificationSent status=$status")
        }

        @SuppressLint("MissingPermission")
        override fun onCharacteristicReadRequest(device: BluetoothDevice,
                                                 requestId: Int,
                                                 offset: Int,
                                                 characteristic: BluetoothGattCharacteristic
        ) {
            log("onCharacteristicReadRequest requestId=$requestId offset=$offset characteristic=${characteristic.uuid}")

            var respStat = BluetoothGatt.GATT_FAILURE
            var respVal: ByteArray? = null

            if (characteristic.uuid == mUuidChrForP2L) {
                val pk = mSubscribedDevices[device]
                if (pk != null) {
                    val ctx = mSubscribedPeers[pk]
                    if (ctx != null) {
                        respStat = BluetoothGatt.GATT_SUCCESS
                        respVal = ctx.outBuffer

                        ctx.outBuffer = null

                        log("Characteristic sending data")
                    }
                }
            }

            mGattServer?.sendResponse(device, requestId, respStat, 0, respVal)

        }

        override fun onExecuteWrite(device: BluetoothDevice?, requestId: Int, execute: Boolean) {
            super.onExecuteWrite(device, requestId, execute)
            if (device != null && execute)  onExecuteWrite(device)
        }

        @SuppressLint("MissingPermission")
        override fun onCharacteristicWriteRequest(device: BluetoothDevice,
                                                  requestId: Int,
                                                  characteristic: BluetoothGattCharacteristic,
                                                  preparedWrite: Boolean,
                                                  responseNeeded: Boolean,
                                                  offset: Int,
                                                  value: ByteArray?) {
            //super.onCharacteristicWriteRequest(device, requestId, characteristic, preparedWrite, responseNeeded, offset, value)
            log("onCharacteristicWriteRequest requestId=${requestId} offset=$offset characteristic=${characteristic.uuid}")

            var respVal: ByteArray? = null
            var respStat = BluetoothGatt.GATT_FAILURE
            //var respStat = BluetoothGatt.GATT_SUCCESS

            if (characteristic.uuid == mUuidChrForL2P) {
                handleIncomingData(device, preparedWrite, value)
                respStat = BluetoothGatt.GATT_SUCCESS
                if (responseNeeded) respVal = byteArrayOf(1)
            }

            if (responseNeeded) {
                mGattServer?.sendResponse(device, requestId, respStat, 0, respVal)
            }
        }

        @SuppressLint("MissingPermission")
        override fun onDescriptorReadRequest(device: BluetoothDevice,
                                             requestId: Int,
                                             offset: Int,
                                             descriptor: BluetoothGattDescriptor
        ) {
            super.onDescriptorReadRequest(device, requestId, offset, descriptor)

            log("onDescriptorReadRequest requestId=$requestId offset=$offset descriptor=${descriptor.uuid}")
            var respStatus = BluetoothGatt.GATT_FAILURE
            var respValue: ByteArray? = null

            if (descriptor.uuid == mUuidDescriptorCcc) {
                respStatus = BluetoothGatt.GATT_SUCCESS
                respValue = if (mSubscribedDevices.contains(device))
                    BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE
                else
                    BluetoothGattDescriptor.DISABLE_NOTIFICATION_VALUE
            }

            mGattServer?.sendResponse(device, requestId, respStatus, 0, respValue)
        }

        @SuppressLint("MissingPermission")
        override fun onDescriptorWriteRequest(device: BluetoothDevice,
                                              requestId: Int,
                                              descriptor: BluetoothGattDescriptor,
                                              preparedWrite: Boolean,
                                              responseNeeded: Boolean,
                                              offset: Int,
                                              value: ByteArray) {
            log("onDescriptorWriteRequest requestId=$requestId offset=$offset descriptor=${descriptor.characteristic.uuid}")

            var respStat = BluetoothGatt.GATT_FAILURE

            //if (descriptor.characteristic.uuid == UUID.fromString(cUuidCharForIndicate)) {
            if (descriptor.characteristic.uuid == mUuidChrForL2P) {
                respStat = BluetoothGatt.GATT_SUCCESS
                if (value.contentEquals(BluetoothGattDescriptor.ENABLE_INDICATION_VALUE) or value.contentEquals(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE)) {

                    val pk = genPeerKey()

                    mSubscribedDevices[device] = pk
                    mSubscribedPeers[pk] = DeviceContext(device)

                    log("   Subscribed: ${device.address}, name: ${device.name}, pk: $pk")

                    val di = DiscoveryInfo()
                    di.alias = device.name ?: device.address ?: "NAME"
                    di.uuid = device.address

                    jniBlePeripheralSubscribed(pk, di.toString())

                } else if (value.contentEquals(BluetoothGattDescriptor.DISABLE_NOTIFICATION_VALUE)) {
                    //mSubscribedDevices.remove(device)
                    log("   Unsubscribed");
                }

            } else {
                log("   Unknown UUID=${descriptor.uuid}")
                respStat = BluetoothGatt.GATT_REQUEST_NOT_SUPPORTED
            }

            if (responseNeeded) {
                mGattServer?.sendResponse(device, requestId, respStat, 0, null)
            }

        }
    }


    private inner class BleAdvertiserCallback : AdvertiseCallback() {
        override fun onStartSuccess(settingsInEffect: AdvertiseSettings) {
            log("Advertise Started: $cUuidService")
            mIsAdvertising = true;
        }

        override fun onStartFailure(errorCode: Int) {
            val desc = when (errorCode) {
                ADVERTISE_FAILED_DATA_TOO_LARGE -> "Advertise Failed: Data Too Large"
                ADVERTISE_FAILED_TOO_MANY_ADVERTISERS -> "Advertise Failed: Too Many Advertisers"
                ADVERTISE_FAILED_ALREADY_STARTED -> "Advertise Failed: Already Started"
                ADVERTISE_FAILED_INTERNAL_ERROR -> "Advertise Failed: Internal Error"
                ADVERTISE_FAILED_FEATURE_UNSUPPORTED -> "Advertise Failed: Feature Unsupported"
                else -> "Advertise Unknown Error"
            }
            log("Advertise start failed: errorCode=$errorCode $desc")
            mIsAdvertising = false;

            jniBlePeripheralError(desc);
            //isAdvertising = false
        }
    }


    override fun onIntentResult(result: ActivityResult) {
        super.onIntentResult(result)

        //log("onIntentResult: BLE: %s", result);

        if (mIntentAction == BluetoothAdapter.ACTION_REQUEST_ENABLE) {

            if (mPendingBleStartAdvertising) {
                mPendingBleStartAdvertising = false
                when (checkPeripheralPermissions()) {
                    PermStatus.Granted -> bleStartAdvertising()
                    PermStatus.Denied -> jniBlePeripheralError("Bluetooth Access Denied");
                    PermStatus.Pending -> mPendingBleStartAdvertising = true
                }
            }
        }

    }

    override fun onRequestPermissionsResult(code: Int, perms: Array<String>, results: IntArray) {
        super.onRequestPermissionsResult(code, perms, results)

        when (getPermissionsStatus()) {
            PermStatus.Denied ->  jniBlePeripheralError("Bluetooth Permission Denied")
            PermStatus.Granted -> {
                if (mPendingBleStartAdvertising) {
                    mPendingBleStartAdvertising = false;
                    bleStartAdvertisingImpl();
                }
            }
            PermStatus.Pending -> {}
        }
    }


}
