package com.smoothapplication.smoothcomic;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;

import androidx.core.app.ActivityCompat;

import org.qtproject.qt.android.bindings.QtActivity;

import java.util.ArrayList;

public class MyAppActivity extends QtActivity {

    private final static String TAG = "smooth-comic";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "MyAppActivity::onCreate before QtActivity::onCreate");
        super.onCreate(savedInstanceState);
        Log.d(TAG, "MyAppActivity::onCreate after QtActivity::onCreate");

        requestMyAppPremissions();
    }

    private void requestMyAppPremissions() {
        if (Build.VERSION.SDK_INT < 23) {
            return;
        }

        String[] permissions = new String[]{
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.MANAGE_EXTERNAL_STORAGE
        };

        ArrayList<String> notHavePerms = new ArrayList<>();

        for (String perm : permissions) {
            if (ActivityCompat.checkSelfPermission(this, perm) != PackageManager.PERMISSION_GRANTED) {
                notHavePerms.add(perm);
            }
        }

        if (notHavePerms.isEmpty()) {
            return;
        }

        String[] notHavePermsArray = new String[notHavePerms.size()];

        ActivityCompat.requestPermissions(this, notHavePerms.toArray(notHavePermsArray), 123);
    }
}
