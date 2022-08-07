package com.smoothapplication.smoothcomic;

import android.os.Environment;

import androidx.annotation.NonNull;

public class MyAppUtil {

    @NonNull
    static String getExternalBaseDir() {
        return Environment.getExternalStorageDirectory().getAbsolutePath();
    }

}
