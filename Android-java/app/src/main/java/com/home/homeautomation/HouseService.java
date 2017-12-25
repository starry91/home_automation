package com.home.homeautomation;

import android.app.IntentService;
import android.app.Service;
import android.content.Intent;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;

import java.util.Iterator;
import java.util.Locale;

import app_cpp_server.AppCppServerGrpc;
import app_cpp_server.*;

import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.stub.StreamObserver;

/**
 * Created by praveen on 12/10/17.
 */

public class HouseService extends Service {
    private static String LOG_TAG = "BoundService";
    public String server_addr = null;
    public Integer server_port;
    public rpcHouse data = null;
    private IBinder mBinder = new MyBinder();
    private NetworkDiscovery disc;
    private String app_addr = null;
    private DiscoveryHandler discoveryHandler = new DiscoveryHandler(this);

    private String id = null;

    @Override
    public void onCreate() {
        super.onCreate();
        app_addr = getIP();
        disc = new NetworkDiscovery(app_addr, discoveryHandler);
        disc.start();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Thread t = new Thread("house_data") {
            @Override
            public void run() {
                while (true) {
                    if (server_addr != null) {
                        rpcHouse temp_data = null;
                        try {
                            temp_data = getHouseData();
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                        if (!temp_data.equals(data)) {
                            data = temp_data;
                            Intent i = new Intent("FromHouseService");
                            i.putExtra("Message", "DataLoaded");
                            sendBroadcast(i);
                        }
                    }

                    if (data != null)
                        System.out.println("entered Thread and data pull succeess");
                    else
                        System.out.println("entered Thread and data pull failed");
                    try {
                        Thread.sleep(2000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        t.start();
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.v(LOG_TAG, "in onBind");
        return mBinder;
    }

    public rpcHouse getHouseData(Void... nothing) throws Exception {
        ManagedChannel mChannel = ManagedChannelBuilder.forAddress(server_addr, 50051)
                .usePlaintext(true)
                .build();
        AppCppServerGrpc.AppCppServerBlockingStub stub = AppCppServerGrpc.newBlockingStub(mChannel);
        rpcId message = rpcId.newBuilder().setId(app_addr).build();
        rpcHouse reply = stub.getAll(message);
        mChannel.shutdown();
        return reply;
    }

    public void startNotification(String id) throws Exception {
        ManagedChannel mChannel = ManagedChannelBuilder.forAddress(server_addr, 50051)
                .usePlaintext(true)
                .build();
        AppCppServerGrpc.AppCppServerStub stub = AppCppServerGrpc.newStub(mChannel);
        rpcId message = rpcId.newBuilder().setId(id).build();
        StreamObserver<rpcData> respStub = new StreamObserver<rpcData>() {
            @Override
            public void onNext(rpcData value) {
                Log.e("grpc_notification", value.toString());
                if (value.getId() != app_addr) {
                    try {
                        data = getHouseData();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    Intent i = new Intent("FromHouseService");
                    i.putExtra("Message", "DataLoaded");
                    sendBroadcast(i);

                    Bundle b = new Bundle();
                    b.putByteArray("notification", value.toByteArray());
                    Intent j = new Intent("notifier");
                    j.putExtras(b);
                    sendBroadcast(j);
                }
            }

            @Override
            public void onError(Throwable t) {

            }

            @Override
            public void onCompleted() {
                mChannel.shutdown();

            }
        };
        stub.notification(message, respStub);
    }

    public void setState(String room_name, String appliance_type, String appliance_name, int state) throws Exception {
        Thread t1 = new Thread("setState") {
            @Override
            public void run() {
                ManagedChannel mChannel = ManagedChannelBuilder.forAddress(server_addr, 50051)
                        .usePlaintext(true)
                        .build();
                AppCppServerGrpc.AppCppServerBlockingStub stub = AppCppServerGrpc.newBlockingStub(mChannel);
                rpcData message = rpcData.newBuilder().setId(app_addr)
                        .setRoomName(room_name)
                        .setApplianceType(appliance_type)
                        .setApplianceName(appliance_name)
                        .setState(state)
                        .build();
                rpcResponse reply = stub.setState(message);
            }
        };
        t1.start();
        t1.join();
    }


    public rpcHouse getData() {
        return data;
    }

    public class MyBinder extends Binder {
        HouseService getService() {
            return HouseService.this;
        }
    }

    private String getIP() {
        try {
            WifiManager wifiManager = (WifiManager) getApplicationContext().getSystemService(WIFI_SERVICE);
            WifiInfo wifiInfo = wifiManager.getConnectionInfo();
            int ipAddress = wifiInfo.getIpAddress();
            return String.format(Locale.getDefault(), "%d.%d.%d.%d",
                    (ipAddress & 0xff), (ipAddress >> 8 & 0xff),
                    (ipAddress >> 16 & 0xff), (ipAddress >> 24 & 0xff));
        } catch (Exception ex) {
            Log.e("GETiP", ex.getMessage());
            return null;
        }
    }

    private void updateServerAddr(String addr) {
        //synchronized (server_addr) {
        server_addr = addr;
        //}
    }

    private void updateServerPort(Integer p) {
        //synchronized (server_port) {
        server_port = p;
        //}
    }

    public static class DiscoveryHandler extends Handler {
        public static final int UPDATE_SERVER_ADDR = 0;
        public static final int UPDATE_SERVER_PORT = 1;
        private HouseService parent;

        public DiscoveryHandler(HouseService parent) {
            super();
            this.parent = parent;
        }

        @Override
        public void handleMessage(Message msg) {

            switch (msg.what) {
                case UPDATE_SERVER_ADDR:
                    parent.updateServerAddr((String) msg.obj);
                    Thread t = new Thread("notification") {
                        @Override
                        public void run() {
                            try {
                                parent.startNotification(parent.app_addr);
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    };
                    t.start();

                    break;
                case UPDATE_SERVER_PORT:
                    parent.updateServerPort((Integer) msg.obj);
                    break;
                default:
                    super.handleMessage(msg);
            }

        }
    }
}
