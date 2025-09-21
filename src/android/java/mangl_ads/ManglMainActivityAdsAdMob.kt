package mangl

import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.View
import android.widget.RelativeLayout
import com.google.android.gms.ads.AdError
import com.google.android.gms.ads.AdListener
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.AdSize
import com.google.android.gms.ads.AdView
import com.google.android.gms.ads.FullScreenContentCallback
import com.google.android.gms.ads.LoadAdError
import com.google.android.gms.ads.MobileAds
import com.google.android.gms.ads.initialization.InitializationStatus
import com.google.android.gms.ads.interstitial.InterstitialAd
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback
import com.google.android.gms.ads.appopen.AppOpenAd
import com.google.android.gms.ads.rewarded.RewardedAd
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback
import com.google.firebase.analytics.FirebaseAnalytics
import com.google.firebase.analytics.analytics
import com.google.firebase.Firebase
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import mangl.ManglDebug.log


abstract class ManglMainActivityAdsAdMob : ManglMainActivityAds() {
    private var mBannerView: AdView? = null
    private var mInterstitialAd: InterstitialAd? = null
    private var mAppOpenAd: AppOpenAd? = null
    private var mRewardedAd: RewardedAd? = null
    private var mRewardedScheduled = false


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val consentSettings = mutableMapOf<FirebaseAnalytics.ConsentType, FirebaseAnalytics.ConsentStatus>()

        consentSettings[FirebaseAnalytics.ConsentType.AD_STORAGE] = FirebaseAnalytics.ConsentStatus.GRANTED
        consentSettings[FirebaseAnalytics.ConsentType.ANALYTICS_STORAGE] = FirebaseAnalytics.ConsentStatus.GRANTED
        consentSettings[FirebaseAnalytics.ConsentType.AD_USER_DATA] = FirebaseAnalytics.ConsentStatus.GRANTED
        consentSettings[FirebaseAnalytics.ConsentType.AD_PERSONALIZATION] = FirebaseAnalytics.ConsentStatus.GRANTED

        Firebase.analytics.setConsent(consentSettings)


        val backgroundScope = CoroutineScope(Dispatchers.IO)

        backgroundScope.launch {

            MobileAds.initialize(this@ManglMainActivityAdsAdMob) { initializationStatus: InitializationStatus ->

//                initializationStatus.adapterStatusMap.forEach { (adapterClass, status) ->
//                    log("Ads: MobileAds", "$adapterClass: ${status.initializationState}")
//                }

                val statusMap = initializationStatus.adapterStatusMap
                for (adapterClass in statusMap.keys) {
                    val status = statusMap[adapterClass]

                    log("Ads: init adaptor: $adapterClass")

                    if (status != null) {
//                        log("Ads: init status, adapter: $adapterClass, latency: ${status.latency}")
                        log("Ads: init status, adapter: $adapterClass, status: ${status.initializationState}")
                    }
                }

                jniOnAdsInitialized(AdNetwork.AdMob.value)
            }
        }
    }

//    override fun onInitializeAdsSdk() {
//        super.onInitializeAdsSdk()
//    }

    private fun getAdSize(): AdSize {
        // Determine the screen width (less decorations) to use for the ad width.
//            if (Build.VERSION.SDK_INT > Build.VERSION_CODES.S) {
//            } else {
//            val display = getDisplay()
//        val display = (getSystemService(WINDOW_SERVICE) as WindowManager).defaultDisplay
//
//        //Display display = getWindowManager().getDefaultDisplay();
//        val outMetrics = DisplayMetrics()
//        //display.getMetrics(outMetrics);
//        display.getRealMetrics(outMetrics)
//
//        val density = outMetrics.density

        //float adWidthPixels = 0; //adContainerView.getWidth();

        // If the ad hasn't been laid out, default to the full screen width.
        val adWidthPixels = getDisplaySize().w
        val adWidth = (adWidthPixels.toFloat() / getPixelDensity()).toInt()
        return AdSize.getCurrentOrientationAnchoredAdaptiveBannerAdSize(this, adWidth)
    }


    override fun onBannerShow(network: Int, unitId: String, x: Int, y: Int, w: Int, h: Int) {
        super.onBannerShow(network, unitId, x, y, w, h)

        //log("onBannerShow: %s; rect: {%d,%d,%d,%d}", unitId, x.toInt(), y.toInt(), w.toInt(), h.toInt())

        if (AdNetwork.AdMob.value != network) return

        if (mBannerView == null) {
            mBannerView = AdView(this)
            //bannerView_.setAdSize(AdSize.BANNER);
            mBannerView!!.adUnitId = unitId

            mBannerView!!.adListener = object : AdListener() {
                override fun onAdFailedToLoad(adError: LoadAdError) {
                    super.onAdFailedToLoad(adError)
                    log("Ad failed to load: %s", adError.toString())
                }

                override fun onAdLoaded() {
                    super.onAdLoaded()
                    // Code to be executed when an ad finishes loading.
                    log("Ads:banner:onAdLoaded")
                    mBannerView!!.visibility = View.VISIBLE
                }

                override fun onAdImpression() {
                    super.onAdImpression()
                    log("Ads:banner:onAdImpression")
                }

//                override fun onAdClicked() {
//                    super.onAdClicked()
//                }

                override fun onAdOpened() {
                    super.onAdOpened()
                    // Code to be executed when an ad opens an overlay that
                    // covers the screen.
                    log("Ads:appopen:onAdOpened")
                    jniOnAdsBannerWillPresentFullScreen()
                }

                override fun onAdClosed() {
                    super.onAdClosed()
                    jniOnAdsBannerDidDismissedFullScreen()
                }
            }

            val wparm = RelativeLayout.LayoutParams.WRAP_CONTENT
            val hparm = RelativeLayout.LayoutParams.WRAP_CONTENT
            val layoutParams = RelativeLayout.LayoutParams(wparm, hparm)

            val adSize = getAdSize()

            log("Banner Size: %d, %d", adSize.width, adSize.height)
            log("Banner Rect: %d, %d, %d, %d", x, y, w, h)

            layoutParams.leftMargin = 0 //(int)x;
            layoutParams.topMargin = 0 //(int)y;
            layoutParams.rightMargin = 0 //width = (int)w;
            layoutParams.bottomMargin = 0 //height = (int)h;

            mBannerView!!.setAdSize(adSize)
            mBannerView!!.layoutParams = layoutParams
        }


        mBannerViewContainer!!.removeView(mBannerView)
        mBannerViewContainer!!.addView(mBannerView)


        // Request the ad
        val adRequest = AdRequest.Builder().build()
        mBannerView!!.loadAd(adRequest)
    }

    override fun onBannerHide(network: Int) {
        super.onBannerHide(network)
        if (AdNetwork.AdMob.value != network) return
        if (mBannerView == null) return
        mBannerView!!.visibility = View.INVISIBLE

        val rootFrameLayout = getRootFramelayout()
        rootFrameLayout.removeView(mBannerView)
    }

    //
    // Interstitials
    //

    override fun onInterstitialLoad(network: Int, unitId: String) {
        super.onInterstitialLoad(network, unitId)
        if (AdNetwork.AdMob.value != network) return

        //interstitialUnidId_ = unitId;
        loadInterstitial(unitId)
    }

    private fun loadInterstitial(unitId: String) {
        val adRequest = AdRequest.Builder().build()

        InterstitialAd.load(this, unitId, adRequest, object : InterstitialAdLoadCallback() {
            override fun onAdFailedToLoad(err: LoadAdError) {
                log("Ads:Interstitial failed to load: %s", err.toString())

                super.onAdFailedToLoad(err)

                runOnUiThread {
                    Handler(Looper.getMainLooper())
                        .postDelayed({ loadInterstitial(unitId) }, 10000)
                }
            }

            override fun onAdLoaded(interstitialAd: InterstitialAd) {
                super.onAdLoaded(interstitialAd)
                mInterstitialAd = interstitialAd

                mInterstitialAd!!.fullScreenContentCallback = object : FullScreenContentCallback() {
                    override fun onAdClicked() {
                        // Called when a click is recorded for an ad.
                        log("Ads:inter:Clicked.")
                    }

                    override fun onAdDismissedFullScreenContent() {
                        // Called when ad is dismissed.
                        // Set the ad reference to null so you don't show the ad a second time.
                        log("Ads:inter:Dismissed fullscreen content.")
                        mInterstitialAd = null
                        loadInterstitial(unitId)
                        jniOnAdsInterstitialDidDismissFullScreen(interstitialAd.adUnitId)
                    }

                    override fun onAdFailedToShowFullScreenContent(adError: AdError) {
                        // Called when ad fails to show.
                        log("Ads:inter:Failed to show fullscreen content.")
                        mInterstitialAd = null
                        runOnUiThread {
                            Handler(Looper.getMainLooper())
                                .postDelayed({ loadInterstitial(unitId) }, 10000)
                        }
                    }

                    override fun onAdImpression() {
                        // Called when an impression is recorded for an ad.
                        log("Ads:inter:Recorded an impression.")
                    }

                    override fun onAdShowedFullScreenContent() {
                        // Called when ad is shown.
                        log("Ads:inter:Showed fullscreen content.")
                        jniOnAdsInterstitialWillPresentFullScreen(interstitialAd.adUnitId)
                    }
                }
            }
        })
    }


    override fun onInterstitialShow(network: Int, unitId: String) {
        super.onInterstitialShow(network, unitId)

        if (AdNetwork.AdMob.value != network) return
        if (mInterstitialAd == null) return
        mInterstitialAd!!.show(this)
    }

    override fun onInterstitialIsReady(network: Int, unitId: String): Boolean {
        if (mInterstitialAd == null)
            return false
//        if (!mInterstitialAd!!..isAdAvailable(this, unitId))
//            return false
        return true
    }


    //
    // AppOpen
    //
    override fun onAppOpenLoad(network: Int, unitId: String) {
        super.onAppOpenLoad(network, unitId)
        if (AdNetwork.AdMob.value != network) return

        loadAppOpen(unitId)
    }

    private fun loadAppOpen(unitId: String) {
        val adRequest = AdRequest.Builder().build()

        AppOpenAd.load(this, unitId, adRequest, object : AppOpenAd.AppOpenAdLoadCallback() {
            override fun onAdFailedToLoad(err: LoadAdError) {
                log("Ads:AppOpen failed to load: %s", err.toString())

                super.onAdFailedToLoad(err)

                runOnUiThread {
                    Handler(Looper.getMainLooper())
                        .postDelayed({ loadAppOpen(unitId) }, 10000)
                }
            }

            override fun onAdLoaded(appOpenAd: AppOpenAd) {
                super.onAdLoaded(appOpenAd)
                mAppOpenAd = appOpenAd

                mAppOpenAd!!.fullScreenContentCallback = object : FullScreenContentCallback() {
                    override fun onAdClicked() {
                        // Called when a click is recorded for an ad.
                        log("Ads:inter:Clicked.")
                    }

                    override fun onAdDismissedFullScreenContent() {
                        // Called when ad is dismissed.
                        // Set the ad reference to null so you don't show the ad a second time.
                        log("Ads:inter:Dismissed fullscreen content.")
                        mAppOpenAd = null
                        loadAppOpen(unitId)
                        jniOnAdsAppOpenDidDismissFullScreen(appOpenAd.adUnitId)
                    }

                    override fun onAdFailedToShowFullScreenContent(adError: AdError) {
                        // Called when ad fails to show.
                        log("Ads:inter:Failed to show fullscreen content.")
                        mAppOpenAd = null
                        runOnUiThread {
                            Handler(Looper.getMainLooper())
                                .postDelayed({ loadAppOpen(unitId) }, 10000)
                        }
                    }

                    override fun onAdImpression() {
                        // Called when an impression is recorded for an ad.
                        log("Ads:inter:Recorded an impression.")
                    }

                    override fun onAdShowedFullScreenContent() {
                        // Called when ad is shown.
                        log("Ads:inter:Showed fullscreen content.")
                        jniOnAdsAppOpenWillPresentFullScreen(appOpenAd.adUnitId)
                    }
                }
            }
        })
    }


    override fun onAppOpenShow(network: Int, unitId: String) {
        super.onAppOpenShow(network, unitId)

        if (AdNetwork.AdMob.value != network) return
        if (mAppOpenAd == null) return
        mAppOpenAd!!.show(this)
    }

    override fun onAppOpenIsReady(network: Int, unitId: String): Boolean {
        if (mAppOpenAd == null)
            return false
//        if (!mInterstitialAd!!..isAdAvailable(this, unitId))
//            return false
        return true
    }



    override fun onRewardedLoad(network: Int, unitId: String) {
        super.onRewardedLoad(network, unitId)

        val adRequest = AdRequest.Builder().build()

        RewardedAd.load(this, unitId, adRequest, object : RewardedAdLoadCallback() {
            override fun onAdFailedToLoad(adError: LoadAdError) {
                log("Ads:rewarded: %s", adError.toString())
                mRewardedAd = null
                mRewardedScheduled = false
                jniOnAdsRewardError(adError.toString())
            }

            override fun onAdLoaded(ad: RewardedAd) {
                log("Ads:rewarded:Loaded.")
                mRewardedAd = ad

                mRewardedAd?.fullScreenContentCallback = object: FullScreenContentCallback() {
                  override fun onAdClicked() {
                    // Called when a click is recorded for an ad.
                    //Log.d(TAG, "Ad was clicked.")
                  }

                  override fun onAdDismissedFullScreenContent() {
                    // Called when ad is dismissed.
                    // Set the ad reference to null so you don't show the ad a second time.
                    //Log.d(TAG, "Ad dismissed fullscreen content.")
                    mRewardedAd = null
                  }

                  override fun onAdFailedToShowFullScreenContent(addError: AdError) {
                    // Called when ad fails to show.
                    //Log.e(TAG, "Ad failed to show fullscreen content.")
                    mRewardedAd = null

                  }

                  override fun onAdImpression() {
                    // Called when an impression is recorded for an ad.
                    //Log.d(TAG, "Ad recorded an impression.")
                  }

                  override fun onAdShowedFullScreenContent() {
                    // Called when ad is shown.
                    //Log.d(TAG, "Ad showed fullscreen content.")
                  }
                }

                if (mRewardedScheduled) {
                    rewardedShowLoaded()
                }
            }
        })
    }

    override fun onRewardedShow(network: Int, unitId: String) {
        super.onRewardedShow(network, unitId)

        if (mRewardedAd == null) {
            mRewardedScheduled = true
            onRewardedLoad(network, unitId)
            return
        }

        rewardedShowLoaded()
    }

    private fun rewardedShowLoaded() {
        mRewardedScheduled = false

        if (mRewardedAd == null) {
            log("Ads:rewarded:Not loaded")
            return
        }

        mRewardedAd!!.show(this) { rewardItem ->
            //Log.d(TAG, "User earned the reward.")
            // Handle the reward.
            val rewardType = rewardItem.type
            val rewardAmount = rewardItem.amount
            jniOnAdsReward(rewardType, rewardAmount)
        }
    }

    override fun onRewardedIsReady(network: Int, unitId: String): Boolean {
        return mRewardedAd != null
    }
}

