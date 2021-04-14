package com.example.votingapp;

import android.util.Log;
import android.widget.EditText;
import android.widget.RadioButton;

import java.security.PrivateKey;
import java.security.PublicKey;

import javax.xml.parsers.FactoryConfigurationError;

public class ServerThreadject implements Runnable{

    private Thread t;
    
    private MainActivity main;

    private PublicKey publicKey;
    private PrivateKey privateKey;

    private static final String SUCCESS_RESPONSE = "Success";
    private static final String FAILURE_RESPONSE = "Failure";
    private static final String TAG = "ServerThreadject";

    public static final int SIGNUP = 0;
    public static final int LOGIN = 1;
    public static final int CREATE = 2;
    public static final int VOTE = 3;
    private int target;
    private String plain;

    public ServerThreadject(MainActivity main, PublicKey publicKey, PrivateKey privateKey) {
        this.t = null;

        this.main = main;
        
        this.publicKey = publicKey;
        this.privateKey = privateKey;

        this.target = SIGNUP;
        this.plain = "";
    }

    public void setTarget(int newTarget) {
        if(newTarget >= SIGNUP && newTarget <= VOTE) {
            this.target = newTarget;
        }
    }

    public void setPlain(String plain) {
        this.plain = plain;
    }

    private String sendToServer(String plain) {
        byte[] crypt = Crypto.encryptText(plain, publicKey);
        byte[] response = Client.serverConnect(crypt);
        String reply = Crypto.decryptText(response, privateKey);
        Log.e(TAG, reply + " <- recv");
        return reply;
    }

    private void signup() {
        main.swapFromThread(R.layout.server_wait);

        String reply = sendToServer(plain);

        if(SUCCESS_RESPONSE.contains(reply)) {
            main.swapFromThread(R.layout.create_account);
        } else if(FAILURE_RESPONSE.contains(reply)) {
            main.swapFromThread(R.layout.activity_main);
        } else {
            main.swapFromThread(R.layout.server_offline);
        }
    }

    private void login() {
        main.swapFromThread(R.layout.server_wait);

        String reply = sendToServer(plain);

        if(SUCCESS_RESPONSE.contains(reply)) {
            main.swapFromThread(R.layout.vote_page);
        } else if(FAILURE_RESPONSE.contains(reply)) {
            main.swapFromThread(R.layout.activity_main);
        } else {
            main.swapFromThread(R.layout.server_offline);
        }
    }

    private void create() {
        main.swapFromThread(R.layout.server_wait);

        String reply = sendToServer(plain);

        if(SUCCESS_RESPONSE.contains(reply)) {
            main.swapFromThread(R.layout.vote_page);
        } else if(FAILURE_RESPONSE.contains(reply)) {
            main.swapFromThread(R.layout.activity_main);
        } else {
            main.swapFromThread(R.layout.server_offline);
        }
    }

    private void vote() {
        main.swapFromThread(R.layout.server_wait);

        String reply = sendToServer(plain);

        if(SUCCESS_RESPONSE.contains(reply)) {
            main.swapFromThread(R.layout.finish_page);
        } else if(FAILURE_RESPONSE.contains(reply)) {
            main.swapFromThread(R.layout.vote_page);
        } else {
            main.swapFromThread(R.layout.server_offline);
        }
    }

    private String getName() {
        switch (target) {
            case SIGNUP: return "signup";
            case LOGIN: return "login";
            case CREATE: return "create";
            case VOTE: return "vote";
            default: return "thread";
        }
    }

    @Override
    public void run() {
        Log.e(TAG, plain + " <- sending to server");
        switch (target) {
            case SIGNUP: signup(); break;
            case LOGIN: login(); break;
            case CREATE: create(); break;
            case VOTE: vote(); break;
        }
    }

    public void start() {
         try {
             if(t != null) t.join();
             t = new Thread (this, getName());
             t.start();
         } catch (InterruptedException e) {
             e.printStackTrace();
         }
    }
}
