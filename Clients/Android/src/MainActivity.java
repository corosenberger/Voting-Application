package com.example.votingapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Button;

import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    private static final String PUBLIC_KEY_FILENAME = "assets/key_send.pem";
    private static final String PRIVATE_KEY_FILENAME = "assets/key_recv.pem";
    private static final String TAG = "Login Page";

    public static Context context;

    private EditText voteridET;
    private EditText emailET;
    private EditText passwordET;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        context = getApplicationContext();

        voteridET = findViewById(R.id.editTextVoterID);
        emailET = findViewById(R.id.editTextEmail);
        passwordET = findViewById(R.id.editTextPassword);
    }

    private String sendToServer(String plain) {
        byte[] crypt = Crypto.encryptText(plain, PUBLIC_KEY_FILENAME);
        byte[] response = Client.serverConnect(crypt);
        return Crypto.decryptText(response, PRIVATE_KEY_FILENAME);
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