package mangl

import android.annotation.SuppressLint
import android.app.AlertDialog
import android.content.ActivityNotFoundException
import android.content.DialogInterface
import android.content.Intent
import android.content.res.Configuration
import android.content.res.Resources
import android.graphics.Color
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.util.DisplayMetrics
import android.util.TypedValue
import android.view.Display
import android.view.DisplayCutout
import android.view.Gravity
import android.view.RoundedCorner
import android.view.ViewGroup
import android.view.Window
import android.view.WindowManager
import android.widget.EditText
import android.widget.FrameLayout
import android.widget.RelativeLayout
import android.widget.TextView
import android.widget.Toast
import androidx.activity.result.ActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.WindowInsetsControllerCompat
import mangl.ManglDebug.log
import java.util.concurrent.CountDownLatch
import java.util.concurrent.atomic.AtomicReference
import kotlin.math.max
import kotlin.math.sqrt

class ManglSize ( val w: Float = 0F, val h: Float = 0F)

class ManglSizeI ( val w: Int = 0, val h: Int = 0)


abstract class ManglMainActivityGui : ManglMainActivityJni() {
    private var keepSplashScreen = true
    private val splashScreenDelay = 500L

    private var mPixelDensity = 1F
    private val mDisplayInsets = intArrayOf(0, 0, 0, 0)
    private val mDisplaySize = intArrayOf(0, 0)
    private var mIsTablet = false


    companion object {
        @JvmStatic
        protected fun convertDpToPixel(dp: Float): Int {
            val metrics = Resources.getSystem().displayMetrics
            val px = dp * (metrics.densityDpi.toFloat() / 160f)
            return Math.round(px)
        }

        @JvmStatic
        private external fun nOnAlertConfirmed()

        //
        // Native GUI
        //
        private var currentWidgetId_ = 1
        private val textViews_: MutableMap<Int, TextView> = HashMap()
        private val textEdits_: MutableMap<Int, EditText> = HashMap()

    }

    protected var mIntentAction: String? = null
    protected val mIntentLauncher = registerForActivityResult(
        ActivityResultContracts.StartActivityForResult()) { result: ActivityResult ->
            onIntentResult(result)
    }

    protected fun launchIntent(action: String) {
        runOnUiThread {
            mIntentAction = action
            mIntentLauncher.launch(Intent(action))
        }
    }

    protected open fun onIntentResult(result: ActivityResult) {
        log("onIntentResult: %s", result.data?.action);
    }



    override fun onCreate(savedInstanceState: Bundle?) {

        val splashScreen = installSplashScreen();

        supportRequestWindowFeature(Window.FEATURE_NO_TITLE)

        super.onCreate(savedInstanceState)

        splashScreen.setKeepOnScreenCondition { keepSplashScreen }

        Handler(Looper.getMainLooper()).postDelayed({ keepSplashScreen = false }, splashScreenDelay)

        // No title
        hideSystemUI()

        updateDisplayStats()


//        // Get Memory Information
//        Runtime runtime = Runtime.getRuntime();
//        long usedMem = runtime.totalMemory() - runtime.freeMemory();
//        nInitSetMemoryInfo(runtime.totalMemory(), runtime.freeMemory(), usedMem);


//        if (ManglConfig.DEBUG) {
//
//            if (!isTablet()) {
//                setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
//            }
//
//            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
//        } else {
//            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
//        }


//        orientationListener_ = new OrientationEventListener(this, SensorManager.SENSOR_DELAY_NORMAL) {
//            @Override
//            public void onOrientationChanged(int orientation) {
//                //Log.v("MANGL", "Orientation changed to " + orientation);
//            }
//        };
//
//        if (orientationListener_.canDetectOrientation()) {
//            //Log.v(DEBUG_TAG, "Can detect orientation");
//            orientationListener_.enable();
//        } else {
//            //Log.v(DEBUG_TAG, "Cannot detect orientation");
//            orientationListener_.disable();
//        }
    }

    override fun onStart() {
        super.onStart()
        //log("onStart")
        updateDisplayStats()
    }

    override fun onResume() {
        super.onResume()

//        val insets = getDisplayInsets()
//
//        log("onResume: insets={%g, %g, %g, %g}",
//            insets[0], insets[1], insets[2], insets[3]
//        )

        // https://d.android.com/training/system-ui/immersive
        hideSystemUI()
        updateDisplayStats()
    }


    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        updateDisplayStats()
    }

    private fun updateDisplayStats() {
        updateDisplaySize()
        mPixelDensity = getPixelDensity()
        updateDisplayInsets()
        mIsTablet = isTablet()
    }



    @SuppressLint("InternalInsetResource")
    private fun updateDisplayInsets() {
        for (i in 0..3) {
            mDisplayInsets[i] = 0
        }

        val decorView = window?.decorView
        val insets = decorView?.rootWindowInsets

        val display = getCurrentDisplay()

        var displayCutout: DisplayCutout? = null
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            displayCutout = display?.cutout
        }

        if (displayCutout == null && insets != null) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                displayCutout = insets.displayCutout
            }
        }

        if (displayCutout != null) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                mDisplayInsets[0] = max(displayCutout.safeInsetTop, mDisplayInsets[0])
                mDisplayInsets[1] = max(displayCutout.safeInsetRight, mDisplayInsets[1])
                mDisplayInsets[2] = max(displayCutout.safeInsetBottom, mDisplayInsets[2])
                mDisplayInsets[3] = max(displayCutout.safeInsetLeft, mDisplayInsets[3])
            }

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                val waterfallInsets = displayCutout.waterfallInsets

                mDisplayInsets[0] = max(waterfallInsets.top, mDisplayInsets[0])
                mDisplayInsets[1] = max(waterfallInsets.right,mDisplayInsets[1])
                mDisplayInsets[2] = max(waterfallInsets.bottom, mDisplayInsets[2])
                mDisplayInsets[3] = max(waterfallInsets.left, mDisplayInsets[3])
            }
        }


        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            val topLeftCorner = display!!.getRoundedCorner(RoundedCorner.POSITION_TOP_LEFT)
            val topLeftRadius = (topLeftCorner?.radius ?: 0)
            mDisplayInsets[0] = max(topLeftRadius, mDisplayInsets[0])

            val bottomRightCorner = display.getRoundedCorner(RoundedCorner.POSITION_BOTTOM_RIGHT)
            val bottomRightRadius = (bottomRightCorner?.radius ?: 0)
            mDisplayInsets[2] = max(bottomRightRadius, mDisplayInsets[2])
        }


        // Check if there were no insets whatsoever
        if (mDisplayInsets[0] == 0) {
            // Assume rounded corners anyway
            // Uset the

            var statusBarHeight = 0
            @SuppressLint("DiscouragedApi")
            val resourceId = resources.getIdentifier("status_bar_height", "dimen", "android")
            if (resourceId > 0) {
                statusBarHeight = resources.getDimensionPixelSize(resourceId)
            }

            if (statusBarHeight > convertDpToPixel(24f)) {
                val topbarLp = RelativeLayout.LayoutParams(
                    RelativeLayout.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT
                )

                mDisplayInsets[0] = statusBarHeight
                mDisplayInsets[2] = statusBarHeight
            }
        }

        mDisplayInsets[0] = max(mDisplayInsets[0], 44)
        mDisplayInsets[2] = max(mDisplayInsets[2], 44)

        // Work on orientation
        mDisplayInsets[1] = max(mDisplayInsets[1], mDisplayInsets[3])
        mDisplayInsets[3] = mDisplayInsets[1]
    }

    private fun jmGetDisplayInsets(): IntArray {
        return mDisplayInsets
    }


    @SuppressLint("deprecation")
    @Suppress("deprecation")
    fun getCurrentDisplay(): Display? {
        var display: Display? = null

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            display = getDisplay()
        }

        if (display == null) {
            display = windowManager.defaultDisplay
        }

        if (display == null) {
            display = (getSystemService(WINDOW_SERVICE) as WindowManager).defaultDisplay
        }
        return display
    }

    @SuppressLint("deprecation")
    @Suppress("deprecation")
    private fun updateDisplaySize() {

        //
        // Only getRealMetrics gives display physical size
        //
        val display = getCurrentDisplay();
        val metrics = DisplayMetrics()
        display!!.getRealMetrics(metrics)

        mDisplaySize[0] = metrics.widthPixels
        mDisplaySize[1] = metrics.heightPixels


//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
//            val windowMetrics = windowManager.currentWindowMetrics
//            mDisplaySize[0] = windowMetrics.bounds.width()
//            mDisplaySize[1] = windowMetrics.bounds.height()
//        } else {
//        }
    }

    private fun jniGetDisplaySize(): IntArray { return mDisplaySize }

    protected fun getDisplaySize(): ManglSizeI {
        return ManglSizeI(mDisplaySize[0], mDisplaySize[1])
    }

//    fun getScreenSizeArray(): IntArray {
//        val size = getScreenSize()
//        log("Screen Size: %d, %d", size.w, size.h);
//        return intArrayOf(size.w, size.h)
//    }


    @SuppressLint("deprecation")
    @Suppress("deprecation")
    fun getPixelDensity(): Float {
        var density = 1f

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            val windowMetrics = windowManager.currentWindowMetrics

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.UPSIDE_DOWN_CAKE) {
                density = windowMetrics.density
            } else {
                density = resources.displayMetrics.density
            }
        } else {
            val display = getCurrentDisplay()

            if (display != null) {
                val metrics = DisplayMetrics()
                display.getRealMetrics(metrics)
                density = metrics.density
            }
        }

        return density
    }

    private fun jniGetPixelDensity(): Float { return mPixelDensity }


    fun isDevice5Inch(): Boolean {
        try {
            val displayMetrics = resources.displayMetrics
            val yInch = displayMetrics.heightPixels / displayMetrics.ydpi
            val xInch = displayMetrics.widthPixels / displayMetrics.xdpi

            val diagonalInch = sqrt((xInch * xInch + yInch * yInch).toDouble())
            return diagonalInch >= 7
        } catch (e: Exception) {
            return false
        }
    }

    fun isLargeScreen(): Boolean {
        val screenLayout = resources.configuration.screenLayout
        return (screenLayout and Configuration.SCREENLAYOUT_SIZE_MASK) >= Configuration.SCREENLAYOUT_SIZE_LARGE
    }

    fun isTablet(): Boolean { return isLargeScreen() && isDevice5Inch(); }

    private fun jniIsTablet(): Boolean { return mIsTablet }


    private fun hideSystemUI() {
        val decorView = window.decorView

        WindowCompat.setDecorFitsSystemWindows(window, false)

        val insetsController = WindowCompat.getInsetsController(window, decorView)

        insetsController.hide(WindowInsetsCompat.Type.systemBars())
        insetsController.hide(WindowInsetsCompat.Type.displayCutout())
        insetsController.systemBarsBehavior =
            WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE


        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            window.attributes.layoutInDisplayCutoutMode =
                WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES
        }


        // This will put the game behind any cutouts and waterfalls on devices which have
        // them, so the corresponding insets will be non-zero.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            window.attributes.layoutInDisplayCutoutMode =
                WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_ALWAYS

            // From API 30 onwards, this is the recommended way to hide the system UI, rather than
            // using View.setSystemUiVisibility.
            //WindowInsetsControllerCompat controller = new WindowInsetsControllerCompat(window, decorView);
        }


        /*
        else {
            window.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

            int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
            uiOptions |= View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_IMMERSIVE;
            uiOptions |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            uiOptions |= View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_LAYOUT_STABLE;
            decorView.setSystemUiVisibility(uiOptions);
        }
        */
    }

    private fun jmAlert(message: String, title: String) {
        runOnUiThread {
            val alert = AlertDialog.Builder(this)
            alert.setCancelable(false).setTitle(title).setMessage(message)
            alert.setPositiveButton("OK") { _: DialogInterface?, _: Int -> nOnAlertConfirmed() }
            alert.create().show()
        }
    }


    //
    // Invites
    //
    private fun jmInviteSMS(message: String) {
        runOnUiThread {
            val intent = Intent(Intent.ACTION_VIEW, Uri.parse("sms:"))
            intent.putExtra("sms_body", message)
            try {
                startActivity(intent)
            } catch (ex: ActivityNotFoundException) {
                Toast.makeText(this, "No SMS applications installed.", Toast.LENGTH_SHORT).show()
            }
        }
    }

    @Suppress("unused")
    private fun jmInviteEmail(message: String, title: String) {
        val i = Intent(Intent.ACTION_SEND)
        i.setType("plain/text")
        i.putExtra(Intent.EXTRA_SUBJECT, title)
        i.putExtra(Intent.EXTRA_TEXT, message) //Html.fromHtml(body.toString()));
        try {
            startActivity(i)
        } catch (ex: ActivityNotFoundException) {
            Toast.makeText(this, "There are no email applications installed.", Toast.LENGTH_SHORT)
                .show()
        }
    }


    private fun jmGuiClearWidgets() {
        runOnUiThread {
            val rootLayout = getRootFramelayout()
            if (textViews_.isNotEmpty()) {
                for (textView in textViews_.values) {
                    rootLayout.removeView(textView)
                }
                textViews_.clear()
            }
            if (textEdits_.isNotEmpty()) {
                for (textEdit in textEdits_.values) {
                    rootLayout.removeView(textEdit)
                }
                textEdits_.clear()
            }
        }
    }

    private fun jm_guiCreateLabel(text: String, x: Int, y: Int, w: Int, h: Int): Int {
        runOnUiThread {
            val label = TextView(this)
            label.text = text

            val width = FrameLayout.LayoutParams.WRAP_CONTENT
            val height = FrameLayout.LayoutParams.WRAP_CONTENT
            val layoutParams =
                FrameLayout.LayoutParams(width, height, Gravity.START)

            layoutParams.topMargin = y //y ; // margin in pixels, not dps
            layoutParams.leftMargin = x // margin in pixels, not dps
            layoutParams.width = w

            //layoutParams.height = h ;
            label.layoutParams = layoutParams

            currentWidgetId_ += 1
            textViews_[currentWidgetId_] = label
            getRootFramelayout().addView(label)
        }

        return currentWidgetId_
    }

    private fun jm_guiCreateTextInput(text: String, x: Int, y: Int, w: Int, h: Int): Int {
        currentWidgetId_ += 1

        runOnUiThread {
            //ManglMainActivityJNI activity = getInstance();
            val edit = EditText(this)
            edit.setText(text)

            val width = FrameLayout.LayoutParams.WRAP_CONTENT
            val height = FrameLayout.LayoutParams.WRAP_CONTENT
            val layoutParams = FrameLayout.LayoutParams(width, height, Gravity.START)

            layoutParams.topMargin = y //y ; // margin in pixels, not dps
            layoutParams.leftMargin = x // margin in pixels, not dps
            layoutParams.width = w

            //layoutParams.height = h ;
            edit.layoutParams = layoutParams

            textEdits_[currentWidgetId_] = edit
            getRootFramelayout().addView(edit)
        }

        return currentWidgetId_
    }


    private fun jm_guiSetTextColor(widgetId: Int, r: Float, g: Float, b: Float, a: Float) {
        runOnUiThread {
            val colorCode = makeColorCode(r, g, b, a)
            textViews_[widgetId]?.setTextColor(colorCode)
            textEdits_[widgetId]?.setTextColor(colorCode)
        }
    }

    private fun jm_guiSetBackgroundColor(widgetId: Int, r: Float, g: Float, b: Float, a: Float) {
        runOnUiThread {
            val colorCode = makeColorCode(r, g, b, a)
            val textView: TextView? = textEdits_[widgetId]
            textView?.setBackgroundColor(colorCode)
            val textEdit = textEdits_[widgetId]
            textEdit?.setBackgroundColor(colorCode)
        }
    }

    private fun jm_guiSetTextSize(widgetId: Int, height: Float) {
        runOnUiThread {
            val textView: TextView? = textViews_[widgetId]
            textView?.setTextSize(TypedValue.COMPLEX_UNIT_PX, height)
            val textEdit = textEdits_[widgetId]
            textEdit?.setTextSize(TypedValue.COMPLEX_UNIT_PX, height)
        }
    }

    private fun jm_guiSetText(widgetId: Int, text: String) {
        runOnUiThread {
            val textView: TextView? = textEdits_[widgetId]
            if (textView != null) textView.text = text
            val textEdit = textEdits_[widgetId]
            textEdit?.setText(text)
        }
    }

    fun jm_guiGetText(widgetId: Int): String {
        val latch = CountDownLatch(1)
        val value = AtomicReference<String>()

        runOnUiThread {
            val textView = textEdits_[widgetId]
            if (textView != null) value.set(textView.text.toString())
            val textEdit = textEdits_[widgetId]
            if (textEdit != null) value.set(textEdit.text.toString())
            latch.countDown()
        }

        try {
            latch.await()
        } catch (e: InterruptedException) {
            e.printStackTrace()
        }

        return value.get()
    }

    private fun makeColorCode(r: Float, g: Float, b: Float, a: Float): Int {
        val colorCode = Color.argb(a, r, g, b)

        return colorCode
    }

}


