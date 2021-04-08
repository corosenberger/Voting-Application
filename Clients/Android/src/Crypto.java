package com.example.votingapp;

import android.content.res.AssetManager;
import android.util.Log;

import java.io.*;
import java.security.*;
import java.security.spec.*;

import javax.crypto.Cipher;

public class Crypto {

    private static final String RSA = "RSA";
    private static final String TAG = "Crypto";

    private static byte[] loadKeyBytes(String file) throws IOException {
        AssetManager am = MainActivity.context.getAssets();
        InputStream is = am.open(file);
        byte[] data = new byte[is.available()]; is.read(data);
        is.close();
        return data;
    }

    private static PrivateKey loadPrivateKey(String file) throws Exception {
        byte[] keyBytes = loadKeyBytes(file);

        PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(keyBytes);
        KeyFactory kf = KeyFactory.getInstance("RSA");
        return kf.generatePrivate(spec);
    }

    private static PublicKey loadPublicKey(String file) throws Exception {
        byte[] keyBytes = loadKeyBytes(file);

        X509EncodedKeySpec spec = new X509EncodedKeySpec(keyBytes);
        KeyFactory kf = KeyFactory.getInstance("RSA");
        return kf.generatePublic(spec);
    }

    public static byte[] encryptText(String plain, String file) {
        byte[] crypt = null;

        try {
            PublicKey publicKey = loadPublicKey(file);
            Cipher cipher = Cipher.getInstance(RSA);
            cipher.init(Cipher.ENCRYPT_MODE, publicKey);
            crypt = cipher.doFinal(plain.getBytes());
        } catch (Exception e) {
            Log.e(TAG,"Failed to encrypt.");
        }

        return crypt;
    }

    public static String decryptText(byte[] crypt, String file) {
        String plain = null;

        try {
            PrivateKey privateKey = loadPrivateKey(file);
            Cipher cipher = Cipher.getInstance(RSA);
            cipher.init(Cipher.DECRYPT_MODE, privateKey);
            byte[] result = cipher.doFinal(crypt);
            plain = new String(result);
        } catch (Exception e) {
            Log.e(TAG,"Failed to decrypt.");
        }

        return plain;
    }
}
