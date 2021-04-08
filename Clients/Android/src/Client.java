package com.example.votingapp;

import android.content.res.AssetManager;
import android.util.Log;

import java.io.*;
import java.util.Scanner;
import java.net.Socket;
import java.util.ArrayList;

public class Client {

    private static final String CONFIG_FILE = "assets/cli.configure";
    private static final String TAG = "Client";

    private static Socket getServer() throws IOException {
        AssetManager am = MainActivity.context.getAssets();
        InputStream is = am.open(CONFIG_FILE);
        Scanner scanner = new Scanner(is);
        String host = scanner.nextLine();
        int port = Integer.parseInt(scanner.nextLine());
        return new Socket(host, port);
    }

    public static byte[] serverConnect(byte[] text) {
        ArrayList<Byte> responseList = new ArrayList<Byte>();

        try {
            Socket socket = getServer();

            InputStream in = socket.getInputStream();
            OutputStream out = socket.getOutputStream();

            out.write(text);

            int bytesRead;
            byte[] buffer = new byte[2048];
            while((bytesRead = in.read(buffer)) > 0) {
                for(int i = 0; i < bytesRead; i++) responseList.add(buffer[i]);
            }

            in.close(); out.close(); socket.close();
        } catch (Exception e) {
            Log.e(TAG, "Unable to connect to server.");
        }

        byte[] response = new byte[responseList.size()];
        for(int i = 0; i < responseList.size(); i++) response[i] = responseList.get(i);
        return response;
    }
}
