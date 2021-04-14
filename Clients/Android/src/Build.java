package com.example.votingapp;

import android.widget.RadioButton;

public class Build {

    private static String subBuild(RadioButton[] group, String header) {
        String vote = "";
        boolean groupChecked = false;

        for(RadioButton voteButton: group) {
            if(groupChecked=voteButton.isChecked()) {
                String candidate = voteButton.getText().toString();
                candidate = candidate.substring(0, candidate.indexOf('(')-1);
                vote += header + ":" + candidate + '\n';
                break;
            }
        }
        if(!groupChecked) vote += header + ": None Selected\n";
        return vote;
    }

    public static String buildVote(RadioButton[] presRG, RadioButton[] govRG, RadioButton[] mayorRG) {
        String vote = "";

        vote += subBuild(presRG, "Presidential");
        vote += subBuild(govRG, "Gubernatorial");
        vote += subBuild(mayorRG, "Mayoral");
        vote += "vote";

        return vote;
    }
}
