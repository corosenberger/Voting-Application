package com.example.votingapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.res.AssetManager;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Button;

import android.os.Bundle;

import java.io.IOException;
import java.io.InputStream;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.spec.InvalidKeySpecException;

public class MainActivity extends AppCompatActivity  {

    private static final String PUBLIC_KEY_FILENAME = "key_send.pem";
    private static final String PRIVATE_KEY_FILENAME = "key_recv.pem";
    private static final String CONFIG_FILE = "cli.configure";
    private static final String TAG = "Login Page";

    private PublicKey publicKey;
    private PrivateKey privateKey;

    private EditText voteridET;
    private EditText emailET;
    private EditText passwordET;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        try {
            AssetManager assets = getAssets();

            InputStream publicKeyStream = assets.open(PUBLIC_KEY_FILENAME);
            publicKey = Crypto.loadPublicKey(publicKeyStream);

            InputStream privateKeyStream = assets.open(PRIVATE_KEY_FILENAME);
            privateKey = Crypto.loadPrivateKey(privateKeyStream);

            InputStream configStream = assets.open(CONFIG_FILE);
            Client.initServer(configStream);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (InvalidKeySpecException e) {
            e.printStackTrace();
        }

        voteridET = findViewById(R.id.editTextVoterID);
        emailET = findViewById(R.id.editTextEmail);
        passwordET = findViewById(R.id.editTextPassword);
    }

    private String sendToServer(String plain) {
        byte[] crypt = Crypto.encryptText(plain, publicKey);
        byte[] response = Client.serverConnect(crypt);
        return Crypto.decryptText(response, privateKey);
    }

    public void signup(View view) {
        String voterid = voteridET.getText().toString();
        String plain = voterid + "\nsignup";
        String reply = sendToServer(plain);
        Log.e(TAG,reply);
    }

    public void login(View view) {
        String email = emailET.getText().toString();
        String pass = passwordET.getText().toString();
        String plain = email + "\n" + pass + "\nlogin";
        String reply = sendToServer(plain);
        Log.e(TAG,reply);
    }
}