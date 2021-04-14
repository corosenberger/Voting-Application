package com.example.votingapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

import android.os.Bundle;
import android.widget.RadioButton;

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
    private static final String TAG = "Activity Main";

    private ServerThreadject threadRunner;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        PublicKey publicKey = null;
        PrivateKey privateKey = null;

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

        threadRunner = new ServerThreadject(this, publicKey, privateKey);

        setContentView(R.layout.activity_main);
    }

    public RadioButton[][] getGroups() {
        RadioButton[][] groups = new RadioButton[3][3];

        groups[0][0] = findViewById(R.id.radioButtonBlueParty1);
        groups[0][1] = findViewById(R.id.radioButtonRedParty1);
        groups[0][2] = findViewById(R.id.radioButtonYellowParty1);

        groups[1][0] = findViewById(R.id.radioButtonBlueParty2);
        groups[1][1] = findViewById(R.id.radioButtonRedParty2);
        groups[1][2] = findViewById(R.id.radioButtonYellowParty2);

        groups[2][0] = findViewById(R.id.radioButtonBlueParty3);
        groups[2][1] = findViewById(R.id.radioButtonRedParty3);
        groups[2][2] = findViewById(R.id.radioButtonYellowParty3);

        return groups;
    }

    public void signup(View view) {
        String voterid = ((EditText)findViewById(R.id.editTextVoterID)).getText().toString();
        String plain = voterid + "\nsignup";
        threadRunner.setPlain(plain);
        threadRunner.setTarget(ServerThreadject.SIGNUP);
        threadRunner.start();
    }

    public void login(View view) {
        String email = ((EditText)findViewById(R.id.editTextEmail)).getText().toString();
        String pass = ((EditText)findViewById(R.id.editTextPassword)).getText().toString();
        String plain = email + "\n" + pass + "\nlogin";
        threadRunner.setPlain(plain);
        threadRunner.setTarget(ServerThreadject.LOGIN);
        threadRunner.start();
    }

    public void create(View view) {
        String email = ((EditText)findViewById(R.id.editTextEmailCreate)).getText().toString();
        String pass1 = ((EditText)findViewById(R.id.editTextPasswordCreate1)).getText().toString();
        String pass2 = ((EditText)findViewById(R.id.editTextPasswordCreate2)).getText().toString();

        String plain = (pass1.equals(pass2)) ? (email + '\n' + pass1 + "\ncreate"):("\ncreate");
        threadRunner.setPlain(plain);
        threadRunner.setTarget(ServerThreadject.CREATE);
        threadRunner.start();
    }

    public void vote(View view) {
        RadioButton[][] groups = getGroups();
        String vote = Build.buildVote(groups[0], groups[1], groups[2]);
        threadRunner.setPlain(vote);
        threadRunner.setTarget(ServerThreadject.VOTE);
        threadRunner.start();
    }

    public void swapFromThread(int target) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                setContentView(target);
            }
        });
        Log.e(TAG, "Performed swap");
    }

    public void reset(View view) {
        RadioButton[][] groups = getGroups();
        for(RadioButton[] g: groups) for(RadioButton rb: g) rb.setChecked(false);
    }
}