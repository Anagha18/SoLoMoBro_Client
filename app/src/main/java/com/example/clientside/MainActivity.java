package com.example.clientside;

import android.os.AsyncTask;
import android.os.Environment;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    AsyncTask task = null;
    static TextView consoleText;
    EditText editText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method

        createDownloadsFolder();

        consoleText = findViewById(R.id.consoleView);
        consoleText.setMovementMethod(new ScrollingMovementMethod());
        consoleText.setText("");


        editText = findViewById(R.id.ip_input);


        FloatingActionButton b = findViewById(R.id.floatingActionButton);
        b.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                task = new ExecCpp().execute();
            }
        });


    }

    private void createDownloadsFolder() {
        Log.d("PATHSTUFF", Environment.getExternalStorageDirectory().getPath());
        String dirpath = Environment.getExternalStorageDirectory().getPath() + "/ClusterCreate";

        File directory = new File(dirpath);

        if(!directory.exists())
            directory.mkdir();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void infiniteLoop();

    public native void connectSocket(String ip);

    public native void cppMainFunc();

    public static void log_output(String output) {
//        tv.append("\n");
//        tv.append(output);
        String st = consoleText.getText().toString();

        st += output;
        consoleText.setText(st);
    }

    private class ExecCpp extends AsyncTask<Void, Integer, Void> {

        @Override
        protected Void doInBackground(Void... voids) {
            String ip = editText.getText().toString();
            connectSocket(ip);
//            cppMainFunc();
            return null;
        }
    }
}
