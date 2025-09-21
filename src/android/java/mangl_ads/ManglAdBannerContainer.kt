package mangl

import android.content.Context
import android.util.AttributeSet
import android.view.View
import android.widget.FrameLayout
import android.widget.RelativeLayout
import config.ManglConfig

class ManglAdBannerContainer : FrameLayout {
    //private val mDebugShowBackround: Boolean = ManglConfig.DEBUG && true
    private val mDebugShowBackround = false

    //public final boolean DebugShowBackround = false;
    constructor(context: Context) : super(context)

    @JvmOverloads
    constructor(context: Context, attrs: AttributeSet?, defStyle: Int = 0) : super(
        context,
        attrs,
        defStyle
    )


    fun init() {
        if (mDebugShowBackround) {
            val context = context

            val lp = layoutParams

            val wparm = RelativeLayout.LayoutParams.WRAP_CONTENT
            val hparm = RelativeLayout.LayoutParams.WRAP_CONTENT
            val layoutParams = RelativeLayout.LayoutParams(wparm, hparm)

            layoutParams.topMargin = 0
            layoutParams.leftMargin = 0
            layoutParams.width = lp.width
            layoutParams.height = lp.height

            val backgroundView = View(context)
            //backgroundView.setLayoutParams(layoutParams);
            backgroundView.setBackgroundColor(-0xffff01)
            backgroundView.visibility = VISIBLE

            addView(backgroundView, -2, layoutParams)
        }
    }


}
