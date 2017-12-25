package com.home.homeautomation;


import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import app_cpp_server.*;

import java.util.ArrayList;


public class MainActivity extends AppCompatActivity {

    public String server_addr;
    public int server_port;
    private ListView listview;
    private ArrayList<String> room_names = new ArrayList<String>();
    private rpcHouse house_data;
    public UdpClientHandler udpClientHandler = new UdpClientHandler(this);
    private BroadcastReceiver broadcastReceiver;
    ArrayAdapter adapter;
    HouseService mBoundService;
    boolean mServiceBound = false;
    boolean houseDataLoadStatus = false;
    private ServiceConnection mServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceDisconnected(ComponentName name) {

            mServiceBound = false;
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            HouseService.MyBinder myBinder = (HouseService.MyBinder) service;
            mBoundService = myBinder.getService();
            mServiceBound = true;
            Intent i = new Intent("Message");
            i.putExtra("ServiceStatus", "connected");
            sendBroadcast(i);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        listview = (ListView) findViewById(R.id.room_list);
        adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_list_item_1, room_names);
        listview.setAdapter(adapter);

    }

    @Override
    protected void onStart() {
        super.onStart();

        Intent intent = new Intent(this, HouseService.class);
        startService(intent);
        bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.e("MainAct", "onResume");
        if (broadcastReceiver == null) {
            broadcastReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    if (intent.getStringExtra("Message").equals("DataLoaded"))
                        if (mServiceBound) {
                            if ((house_data = mBoundService.getData()) != null) {
                                populate_room_names(house_data);
                                enableClick();
                                adapter.notifyDataSetChanged();
                            }
                        }
                }
            };
            registerReceiver(broadcastReceiver, new IntentFilter("FromHouseService"));
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (broadcastReceiver != null)
            unregisterReceiver(broadcastReceiver);
    }

    public void enableClick() {
        listview.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Intent i = new Intent(getBaseContext(), RoomActivity.class);
                Bundle b = new Bundle();
                b.putByteArray("room", house_data.getRoom(position).toByteArray());
                i.putExtras(b);
                startActivity(i);
            }
        });
    }


    protected void populate_room_names(rpcHouse house_data) {
        room_names.clear();
        for (int i = 0; i < house_data.getRoomCount(); i++) {
            rpcRoom room = house_data.getRoom(i);
            room_names.add(room.getRoomName());
        }
    }

    private void updateServerAddr(String addr) {
        server_addr = addr;
    }

    private void updateServerPort(int port) {
        server_port = port;
    }

    public static class UdpClientHandler extends Handler {
        public static final int UPDATE_SERVER_ADDR = 0;
        public static final int UPDATE_SERVER_PORT = 1;
        public static final int UPDATE_MSG = 2;
        public static final int UPDATE_END = 3;
        private MainActivity parent;

        public UdpClientHandler(MainActivity parent) {
            super();
            this.parent = parent;
        }

        @Override
        public void handleMessage(Message msg) {

            switch (msg.what) {
                case UPDATE_SERVER_ADDR:
                    //parent.updateServerAddr((String) msg.obj);
                    break;
                case UPDATE_SERVER_PORT:
                    //parent.updateServerPort((int) msg.obj);
                    break;
                default:
                    super.handleMessage(msg);
            }

        }
    }

//"android.intent.action.MAIN"
}
