package mangl

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.widget.FrameLayout
import android.widget.RelativeLayout
import com.google.android.gms.ads.identifier.AdvertisingIdClient
import com.google.android.ump.ConsentInformation
import com.google.android.ump.ConsentRequestParameters
import com.google.android.ump.FormError
import com.google.android.ump.UserMessagingPlatform
import config.ManglConfig
import mangl.ManglDebug.log
import java.util.concurrent.atomic.AtomicBoolean

abstract class ManglMainActivityAds : ManglMainActivityBle() {
    protected var mBannerParentLayout: FrameLayout? = null

    @JvmField
    var mBannerViewContainer: ManglAdBannerContainer? = null

    private var mConsentInformation: ConsentInformation? = null
    private val mIsMobileAdsInitializeCalled = AtomicBoolean(false)

    enum class AdNetwork (val value: Int) {
        AdMob(1),
    }

    fun isPrivacyOptionsRequired(): Boolean {
        return mConsentInformation?.privacyOptionsRequirementStatus == ConsentInformation.PrivacyOptionsRequirementStatus.REQUIRED
    }

    companion object {
        @JvmStatic
        protected external fun jniOnAdsInitialized(network: Int)

        @JvmStatic
        protected external fun jniOnAdsBannerWillPresentFullScreen()

        @JvmStatic
        protected external fun jniOnAdsBannerDidDismissedFullScreen()

        @JvmStatic
        protected external fun jniOnAdsInterstitialWillPresentFullScreen(unitId: String)

        @JvmStatic
        protected external fun jniOnAdsInterstitialDidDismissFullScreen(unitId: String)

        @JvmStatic
        protected external fun jniOnAdsAppOpenWillPresentFullScreen(unitId: String)

        @JvmStatic
        protected external fun jniOnAdsAppOpenDidDismissFullScreen(unitId: String)

        @JvmStatic
        protected external fun jniOnAdsReward(reward: String, amount: Int)

        @JvmStatic
        protected external fun jniOnAdsRewardError(message: String)
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Create a ConsentRequestParameters object.
        val params = ConsentRequestParameters.Builder().build()

        mConsentInformation = UserMessagingPlatform.getConsentInformation(this)



        mConsentInformation!!.requestConsentInfoUpdate(
            this,
            params,
            {
                UserMessagingPlatform.loadAndShowConsentFormIfRequired(this) { err: FormError? ->
                    if (err != null) {
                        // Consent gathering failed.
                        log("%s: %s",err.errorCode,err.message)
                    }

                    // Consent has been gathered.
                    if (mConsentInformation!!.canRequestAds()) {
                        initilializeAdsSdk()
                    }
                }
            },
            { reqErr: FormError ->
                // Consent gathering failed.
                log("%s: %s",reqErr.errorCode,reqErr.message)
            }
        )

        if (mConsentInformation!!.canRequestAds()) {
            initilializeAdsSdk()
        }
    }


    private fun initilializeAdsSdk() {
        if (mIsMobileAdsInitializeCalled.getAndSet(true)) {
            return
        }

        if (ManglConfig.DEBUG) {
            Thread {
                try {
                    val ai = AdvertisingIdClient.getAdvertisingIdInfo(applicationContext)
                    val idfa = ai.id
                    log("IDFA: $idfa")
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }.start()
        }


        onInitializeAdsSdk()
    }


    private fun createBannerParent(x: Int, y: Int, w: Int, h: Int) {
        if (mBannerViewContainer != null) return

        val wparm = RelativeLayout.LayoutParams.MATCH_PARENT
        val hparm = RelativeLayout.LayoutParams.MATCH_PARENT
        val layoutParams = RelativeLayout.LayoutParams(wparm, hparm)

        layoutParams.leftMargin = x
        layoutParams.topMargin = y
        layoutParams.width = w
        layoutParams.height = h

        mBannerViewContainer = ManglAdBannerContainer(this)
        mBannerViewContainer!!.layoutParams = layoutParams

        getRootFramelayout().addView(mBannerViewContainer)

        mBannerViewContainer!!.init()
    }

    protected open fun onInitializeAdsSdk() {
        //initializeMobileAdsSdk();
    }

    protected open fun onBannerShow(network: Int, unitId: String, x: Int, y: Int, w: Int, h: Int) {
        log("onBannerShow: %s; rect: {%d,%d,%d,%d}", unitId, x, y, w, h);
    }

    protected open fun onBannerHide(network: Int) {}


    protected open fun onInterstitialLoad(network: Int, unitId: String) {
        log("onInterstitialLoad: %s", unitId);
    }
    protected open fun onInterstitialShow(network: Int, unitId: String) {
        log("onInterstitialShow: %s", unitId)
    }

    protected open fun onInterstitialIsReady(network: Int, unitId: String): Boolean {
        return false
    }

    protected open fun onAppOpenLoad(network: Int, unitId: String) {
        log("onAppOpenLoad: %s", unitId);
    }
    protected open fun onAppOpenShow(network: Int, unitId: String) {
        log("onAppOpenShow: %s", unitId)
    }

    protected open fun onAppOpenIsReady(network: Int, unitId: String): Boolean {
        return false
    }


    protected open fun onRewardedLoad(network: Int, unitId: String) {
        log("onRewardedLoad: %s", unitId)
    }

    protected open fun onRewardedShow(network: Int, unitId: String) {
        log("onRewardedShow: %s", unitId)
    }

    protected open fun onRewardedIsReady(network: Int, unitId: String): Boolean {
        return false
    }


    private fun jniAdsBannerShow(network: Int, unitId: String, x: Float, y: Float, w: Float, h: Float) {
        runOnUiThread {
            val scale = getPixelDensity()
            val bannerX = (x * scale).toInt()
            val bannerY = (y * scale).toInt()
            val bannerW = (w * scale).toInt()
            val bannerH = (h * scale).toInt()
            createBannerParent(bannerX, bannerY, bannerW, bannerH)
            onBannerShow(network, unitId, bannerX, bannerY, bannerW, bannerH)
        }
    }

    private fun jniAdsBannerHide(network: Int) {
        runOnUiThread { onBannerHide(network) }
    }

    private fun jniAdsInterstitialLoad(network: Int, unitId: String) {
        runOnUiThread { onInterstitialLoad(network, unitId) }
    }

    private fun jniAdsInterstitialShow(network: Int, unitId: String, delay: Float) {
        runOnUiThread {
            if (delay == 0f) {
                onInterstitialShow(network, unitId)
                return@runOnUiThread
            }

            Handler(Looper.getMainLooper()).postDelayed({
                onInterstitialShow(network, unitId)
            }, (delay * 1000).toLong())
        }
    }

    private fun jniAdsInterstitialIsReady(network: Int, unitId: String): Boolean {
        return onInterstitialIsReady(network, unitId)
    }


    private fun jniAdsAppOpenLoad(network: Int, unitId: String) {
        runOnUiThread { onAppOpenLoad(network, unitId) }
    }

    private fun jniAdsAppOpenShow(network: Int, unitId: String) {
        runOnUiThread {
                onAppOpenShow(network, unitId)
        }
    }

    private fun jniAdsAppOpenIsReady(network: Int, unitId: String): Boolean {
        return onAppOpenIsReady(network, unitId)
    }


    private fun jniAdsRewardedLoad(network: Int, unitId: String) {
        runOnUiThread { onRewardedLoad(network, unitId) }
    }

    private fun jniAdsRewardedShow(network: Int, unitId: String) {
        runOnUiThread { onRewardedShow(network, unitId) }
    }

    private fun jniAdsRewardedIsReady(network: Int, unitId: String): Boolean {
        return onRewardedIsReady(network, unitId)
    }

}

