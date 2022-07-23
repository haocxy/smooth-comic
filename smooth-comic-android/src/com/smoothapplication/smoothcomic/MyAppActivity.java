package com.smoothapplication.smoothcomic;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;

import androidx.core.app.ActivityCompat;

import org.qtproject.qt.android.bindings.QtActivity;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;

public class MyAppActivity extends QtActivity {

    private final static String TAG = "smooth-comic-MyAppActivity";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "MyAppActivity::onCreate before QtActivity::onCreate");
        super.onCreate(savedInstanceState);
        Log.d(TAG, "MyAppActivity::onCreate after QtActivity::onCreate");

        requestMyAppPremissions();

        File f = new File("/storage/emulated/0/BaiduNetdisk/a.zip");
        try {
            FileInputStream fis = new FileInputStream(f);
            Log.d(TAG, "MyAppActivity::onCreate after new FileInputStream" + f);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

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
            } else {
                Log.d(TAG, "permission " + perm + " granted");
            }
        }

        if (notHavePerms.isEmpty()) {
            return;
        }

        String[] notHavePermsArray = new String[notHavePerms.size()];

        ActivityCompat.requestPermissions(this, notHavePerms.toArray(notHavePermsArray), 123);
    }
}
