package com.example.votingapp;

import android.content.res.AssetManager;
import android.util.Base64;
import android.util.Log;

import java.io.*;
import java.security.*;
import java.security.spec.*;

import javax.crypto.Cipher;

public class Crypto {

    private static final String RSA = "RSA";
    private static final String TAG = "Crypto";

    private static byte[] loadKeyBytes(InputStream is) throws IOException {
        byte[] dataBytes = new byte[is.available()]; is.read(dataBytes); is.close();
        String dataString = new String(dataBytes);

        int firstNewLine = dataString.indexOf('\n') + 1;
        int lastNewLine = dataString.lastIndexOf('\n');
        dataString = dataString.substring(firstNewLine, lastNewLine)
                .replace("\n","");

        dataBytes = Base64.decode(dataString, Base64.DEFAULT);
        return dataBytes;
    }

    public static PrivateKey loadPrivateKey(InputStream is) throws IOException, NoSuchAlgorithmException, InvalidKeySpecException {
        byte[] keyBytes = loadKeyBytes(is);

        EncodedKeySpec spec = new PKCS8EncodedKeySpec(keyBytes);
        KeyFactory kf = KeyFactory.getInstance(RSA);
        return kf.generatePrivate(spec);
    }

    public static PublicKey loadPublicKey(InputStream is) throws IOException, NoSuchAlgorithmException, InvalidKeySpecException {
        byte[] keyBytes = loadKeyBytes(is);

        EncodedKeySpec spec = new X509EncodedKeySpec(keyBytes);
        KeyFactory kf = KeyFactory.getInstance(RSA);
        return kf.generatePublic(spec);
    }

    public static byte[] encryptText(String plain, PublicKey publicKey) {
        byte[] crypt = null;

        try {
            Cipher cipher = Cipher.getInstance(RSA);
            cipher.init(Cipher.ENCRYPT_MODE, publicKey);
            crypt = cipher.doFinal(plain.getBytes());
        } catch (Exception e) {
            e.printStackTrace();
        }

        return crypt;
    }

    public static String decryptText(byte[] crypt, PrivateKey privateKey) {
        String plain = null;

        try {
            Cipher cipher = Cipher.getInstance(RSA);
            cipher.init(Cipher.DECRYPT_MODE, privateKey);
            byte[] result = cipher.doFinal(crypt);
            plain = new String(result);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return plain;
    }
}
