package com.home.homeautomation;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import com.google.protobuf.InvalidProtocolBufferException;

import app_cpp_server.*;

import java.util.ArrayList;

public class RoomActivity extends AppCompatActivity {

    private rpcRoom.Builder room;
    private rpcData.Builder sig_data;
    private BroadcastReceiver broadcastReceiver;
    ArrayList<rpcAppliance> appliances;
    ToggleButtonListAdapter adapter;
    HouseService mBoundService;
    boolean mServiceBound = false;
    ListView lv;
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
            adapter.updateService(mBoundService);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_room);

        Intent intent = new Intent(this, HouseService.class);
        startService(intent);
        bindService(intent, mServiceConnection, Context.BIND_AUTO_CREATE);

        Bundle b = getIntent().getExtras();
        byte[] room_bytes = b.getByteArray("room");
        try {
            room = rpcRoom.parseFrom(room_bytes).toBuilder();
        } catch (InvalidProtocolBufferException e) {
            e.printStackTrace();
        }
        appliances = new ArrayList<>(room.getApplianceList());

        adapter = new ToggleButtonListAdapter(this, appliances, room.getRoomName(), mBoundService);
        lv = (ListView) findViewById(R.id.tog_list);
        lv.setAdapter(adapter);

        lv.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

            }
        });
    }
    @Override
    protected void onResume() {
        super.onResume();
        Log.e("MainAct", "onResume");
        if (broadcastReceiver == null) {
            broadcastReceiver = new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    Bundle b = intent.getExtras();
                    byte[] sig_bytes = b.getByteArray("notification");
                    try {
                        sig_data = rpcData.parseFrom(sig_bytes).toBuilder();
                    } catch (InvalidProtocolBufferException e) {
                        e.printStackTrace();
                    }
                    synchronized (appliances) {
                        for (int i = 0; i < appliances.size(); i++) {
                            if (appliances.get(i).getAppName().equals(sig_data.getApplianceName())) {
                                rpcAppliance app = rpcAppliance.newBuilder()
                                        .setAppName(sig_data.getApplianceName())
                                        .setAppState(sig_data.getState())
                                        .setAppType(sig_data.getApplianceType()).build();
                                appliances.remove(i);
                                appliances.add(i, app);
                                adapter.notifyDataSetChanged();
                            }
                        }
                    }
                }
            };
            registerReceiver(broadcastReceiver, new IntentFilter("notifier"));
        }
    }
}
