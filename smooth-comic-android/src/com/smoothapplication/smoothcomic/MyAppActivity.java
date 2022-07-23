package com.smoothapplication.smoothcomic;

import android.os.Bundle;
import android.util.Log;

import org.qtproject.qt.android.bindings.QtActivity;

public class MyAppActivity extends QtActivity {

    private final static String TAG = "smooth-comic";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "MyAppActivity::onCreate before QtActivity::onCreate");
        super.onCreate(savedInstanceState);
        Log.d(TAG, "MyAppActivity::onCreate after QtActivity::onCreate");
    }

}
