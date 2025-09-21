```java
    public void onAdsBannerShow(int x, int y, int w, int h) {
    }

    public void onAdsBannerSetVisible(boolean visible) {
    }


    public void onAdsInterstitialLoad() {

    }
    public void onAdsInterstitialShow() {
    }

    public void onAdsInterstitialShowDelayed(float delay) {
    }


```


```
    //
    // Native callbacks
    //
    @SuppressWarnings("unused")
    private static void jniAdsBannerShow(int x, int y, int w, int h) {
        if (created_)
            getInstance().onAdsBannerShow(x, y, w, h);
    }

    @SuppressWarnings("unused")
    private static void jniAdsBannerHide() {
        if (created_)
            getInstance().onAdsBannerSetVisible(false);
    }

    @SuppressWarnings("unused")
    private static void jm_adsBannerUnhide() {
        if (created_)
            getInstance().onAdsBannerSetVisible(true);
    }

    @SuppressWarnings("unused")
    private static void jniAdsInterstitialLoad() {
        if (created_)
            getInstance().onAdsInterstitialLoad();
    }

    @SuppressWarnings("unused")
    private static void jniAdsInterstitialShow() {
        if (created_)
            getInstance().onAdsInterstitialShow();
    }

    @SuppressWarnings("unused")
    private static void jm_adsInterstitialShowDelayed(float delay) {
        if (created_)
            getInstance().onAdsInterstitialShowDelayed(delay);
    }

```


```java

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // Ignore MENU
        if (keyCode == KeyEvent.KEYCODE_MENU)
            return false;

        nOnKeyDown(keyCode);
        return false;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        // Ignore MENU
        if (keyCode == KeyEvent.KEYCODE_MENU)
            return false;

        nOnKeyUp(keyCode);
        return false;
    }

    @Override
    public synchronized boolean onTouchEvent(MotionEvent event) {

        int action = event.getAction();
        int count = event.getPointerCount();

//        if (action == MotionEvent.ACTION_CANCEL) {
//            int a = 0;
//        }

        if (count > 1) {

            float[] xValues = new float[count];
            float[] yValues = new float[count];

            for (int index = 0; index < count; ++index) {
                xValues[index] = event.getX(index);
                yValues[index] = event.getY(index);
            }

            nOnTouchEvents(action, count, xValues, yValues);
        } else {
            nOnTouchEvent(action, event.getX(), event.getY());
        }
        return true;
    }


```

```java


    //
    // Native GUI
    //
    private static int currentWidgetId_ = 1;
    private static final Map<Integer, TextView> textViews_ = new HashMap<>();
    private static final Map<Integer, EditText> textEdits_ = new HashMap<>();

    @SuppressWarnings("unused")
    private static void jmGuiClearWidgets() {
        if (!created_)
            return;
        /*
        MMainActivity activity_ = getInstance();
        activity_.runOnUiThread(() -> {
            if (!textViews_.isEmpty()) {
                for (TextView textView : textViews_.values()) {
                    activity_.surfaceLayout_.removeView(textView);
                }
                textViews_.clear();
            }

            if (!textEdits_.isEmpty()) {
                for (EditText textEdit : textEdits_.values()) {
                    activity_.surfaceLayout_.removeView(textEdit);
                }
                textEdits_.clear();
            }
        });

         */
    }

    @SuppressWarnings("unused")
    private static int jm_guiCreateLabel(String text, int x, int y, int w, int h) {
        if (!created_)
            return 0;
        /*
        MMainActivity activity_ = getInstance();
        TextView label = new TextView(activity_);
        label.setText(text);

        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, Gravity.START);

        layoutParams.topMargin = y; //y ; // margin in pixels, not dps
        layoutParams.leftMargin = x; // margin in pixels, not dps
        layoutParams.width = w;
        //layoutParams.height = h ;

        label.setLayoutParams(layoutParams);

        currentWidgetId_ += 1;
        textViews_.put(currentWidgetId_, label);

        activity_.runOnUiThread(() -> {
            activity_.surfaceLayout_.addView(label);
        });

        return currentWidgetId_;

         */
        return 0;
    }

    @SuppressWarnings("unused")
    private static int jm_guiCreateTextInput(String text, int x, int y, int w, int h) {
        if (!created_)
            return 0;
        /*
        MMainActivity activity_ = getInstance();
        EditText edit = new EditText(activity_);
        edit.setText(text);

        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, Gravity.LEFT);

        layoutParams.topMargin = y; //y ; // margin in pixels, not dps
        layoutParams.leftMargin = x; // margin in pixels, not dps
        layoutParams.width = w;
        //layoutParams.height = h ;

        edit.setLayoutParams(layoutParams);

        currentWidgetId_ += 1;
        textEdits_.put(currentWidgetId_, edit);


        activity_.runOnUiThread(() -> {
            activity_.surfaceLayout_.addView(edit);
        });

        return currentWidgetId_;

         */
        return 0;
    }


    @SuppressWarnings("unused")
    private static void jm_guiSetTextColor(int widgetId, float r, float g, float b, float a) {
        if (!created_)
            return;
        getInstance().runOnUiThread(() -> {
            int colorCode = Color.argb(a, r, g, b);
            if (textViews_.containsKey(widgetId))
                textViews_.get(widgetId).setTextColor(colorCode);
            if (textEdits_.containsKey(widgetId))
                textEdits_.get(widgetId).setTextColor(colorCode);
        });
    }

    @SuppressWarnings("unused")
    private static void jm_guiSetBackgroundColor(int widgetId, float r, float g, float b, float a) {
        getInstance().runOnUiThread(() -> {
            int colorCode = Color.argb(a, r, g, b);
            if (textViews_.containsKey(widgetId)) {
                textViews_.get(widgetId).setBackgroundColor(colorCode);
            }
            if (textEdits_.containsKey(widgetId)) {
                textEdits_.get(widgetId).setBackgroundColor(colorCode);
            }
        });
    }

    @SuppressWarnings("unused")
    private static void jm_guiSetTextSize(int widgetId, float height) {
        if (!created_)
            return;
        getInstance().runOnUiThread(() -> {
            if (textViews_.containsKey(widgetId))
                textViews_.get(widgetId).setTextSize(TypedValue.COMPLEX_UNIT_PX, height);
            if (textEdits_.containsKey(widgetId))
                textEdits_.get(widgetId).setTextSize(TypedValue.COMPLEX_UNIT_PX, height);
        });
    }

    @SuppressWarnings("unused")
    private static void jm_guiSetText(int widgetId, String text) {
        if (!created_)
            return;
        getInstance().runOnUiThread(() -> {
            if (textViews_.containsKey(widgetId))
                textViews_.get(widgetId).setText(text);
            if (textEdits_.containsKey(widgetId))
                textEdits_.get(widgetId).setText(text);
        });
    }

    @SuppressWarnings("unused")
    private static String jm_guiGetText(int widgetId) {
        if (!created_)
            return "";

        final CountDownLatch latch = new CountDownLatch(1);
        AtomicReference<String> value = new AtomicReference<>();

        getInstance().runOnUiThread(() -> {
            if (textViews_.containsKey(widgetId))
                value.set(textViews_.get(widgetId).getText().toString());
            if (textEdits_.containsKey(widgetId))
                value.set(textEdits_.get(widgetId).getText().toString());
            latch.countDown();
        });

        try {
            latch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        return value.get();
    }



```


```java

    //
    // Nearby Connections
    //
    @SuppressWarnings("unused")
    private static String jm_getNearbyConnectionServiceId() {
        if (!created_)
            return "";
        MMainActivity activity_ = getInstance();
        String serviceId = "UNKNOWN";
        try {
            ApplicationInfo ai = activity_.getPackageManager().getApplicationInfo(activity_.getPackageName(), PackageManager.GET_META_DATA);
            Bundle bundle = ai.metaData;
            serviceId = bundle.getString("com.google.android.gms.nearby.connection.SERVICE_ID");
        } catch (PackageManager.NameNotFoundException e) {
            Log.e("NDKHelper", "Failed to load meta-data, NameNotFound: " + e.getMessage());
        } catch (NullPointerException e) {
            Log.e("NDKHelper", "Failed to load meta-data, NullPointer: " + e.getMessage());
        }
        return serviceId;
    }


    @SuppressWarnings("unused")
    private static boolean jm_requestMultiplayerPermissions() {
        if (!created_)
            return false;
        MMainActivity activity_ = getInstance();
        int accessCoarseLocation = activity_.checkSelfPermission(android.Manifest.permission.ACCESS_COARSE_LOCATION);
        int accessFineLocation = activity_.checkSelfPermission(android.Manifest.permission.ACCESS_FINE_LOCATION);

        List<String> listRequestPermission = new ArrayList<String>();

        if (accessCoarseLocation != PackageManager.PERMISSION_GRANTED) {
            listRequestPermission.add(android.Manifest.permission.ACCESS_COARSE_LOCATION);
        }
        if (accessFineLocation != PackageManager.PERMISSION_GRANTED) {
            listRequestPermission.add(android.Manifest.permission.ACCESS_FINE_LOCATION);
        }

        if (listRequestPermission.isEmpty())
            return true;

        String[] strRequestPermission = listRequestPermission.toArray(new String[listRequestPermission.size()]);
        activity_.requestPermissions(strRequestPermission, MMainActivity.REQUEST_CODE_REQUIRED_PERMISSIONS);
        return false;
    }


```
