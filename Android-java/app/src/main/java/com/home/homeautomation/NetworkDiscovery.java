package com.home.homeautomation;

import android.os.Message;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

/**
 * Created by praveen on 11/28/17.
 */

public class NetworkDiscovery extends Thread {

    public String server_addr = new String();
    public int server_port;
    //private Activity activity;
    private String ip_v4;
    HouseService.DiscoveryHandler handler;

    public NetworkDiscovery(String ip, HouseService.DiscoveryHandler handler) {
        //this.activity = activity;
        this.ip_v4 = ip;
        this.handler = handler;
    }

    //Open a random port to send the package
    @Override
    public void run() {
        String ip[] = ip_v4.split("\\.");
        String broadcast_ip = ip[0] + "." + ip[1] + "." + ip[2] + ".255";

        DatagramSocket c = null;
        try {
            c = new DatagramSocket();
        } catch (SocketException e) {
            e.printStackTrace();
        }
        try {
            c.setBroadcast(true);
        } catch (SocketException e) {
            e.printStackTrace();
        }

        byte[] sendData = "DISCOVER_SERVER_REQUEST".getBytes();
        DatagramPacket sendPacket = null;
        while (true) {
            try {
                sendPacket = new DatagramPacket(sendData, sendData.length, InetAddress.getByName(broadcast_ip), 8080);
            } catch (UnknownHostException e) {
                e.printStackTrace();
            }
            try {
                c.send(sendPacket);
            } catch (IOException e) {
                e.printStackTrace();
            }
            System.out.println(getClass().getName() + ">>> Request packet sent to: 192.168.0.255 (DEFAULT)");

            byte[] buf = new byte[1024];
            DatagramPacket dp = new DatagramPacket(buf, 1024);
            try

            {
                c.receive(dp);
            } catch (
                    IOException e)

            {
                e.printStackTrace();
            }

            String str = new String(dp.getData(), 0, dp.getLength());
            System.out.println(str);
            System.out.println(str.length());
            System.out.println((str.equals("HI")));
            if (str.equals("HI")) {
                System.out.println("entered");
                String temp_addr;
                temp_addr = (String) dp.getAddress().getHostAddress();
                if(!server_addr.equals(temp_addr)) {
                    server_addr = temp_addr;
                    handler.sendMessage(
                            Message.obtain(handler, HouseService.DiscoveryHandler.UPDATE_SERVER_ADDR, server_addr));
                }
            }
            try {
                Thread.sleep(10000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}






