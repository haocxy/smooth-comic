package com.smoothapplication.smoothcomic;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings;
import android.util.Log;

import androidx.core.app.ActivityCompat;

import org.qtproject.qt.android.bindings.QtActivity;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;

public class MyAppActivity extends QtActivity {

    private final static String TAG = "smooth-comic-java";

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
        requestMyAppNormalPermissions();
        requestMyAppManageExternalStoragePermission();
    }

    private void requestMyAppNormalPermissions() {
        if (Build.VERSION.SDK_INT < 23) {
            return;
        }

        String[] permissions = new String[]{
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
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

    private void requestMyAppManageExternalStoragePermission() {
        if (Build.VERSION.SDK_INT < 30) {
            return;
        }

        if (Environment.isExternalStorageManager()) {
            return;
        }

        final Uri packageUri = Uri.fromParts("package", BuildConfig.APPLICATION_ID, null);

        final Intent intent = new Intent(
                Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION,
                packageUri);

        startActivity(intent);
    }
}
