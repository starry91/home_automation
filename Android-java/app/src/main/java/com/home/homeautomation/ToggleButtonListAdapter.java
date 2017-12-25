package com.home.homeautomation;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import android.widget.ToggleButton;

import app_cpp_server.*;

import java.util.ArrayList;

public class ToggleButtonListAdapter extends ArrayAdapter<rpcAppliance> {
    private final Context context;
    private final ArrayList<rpcAppliance> apps;
    private String roomName;
    private HouseService service;

    public ToggleButtonListAdapter(Context context, ArrayList<rpcAppliance> app_list, String room, HouseService service) {
        super(context, R.layout.activity_main, app_list);
        this.context = context;
        this.apps = app_list;
        this.roomName = room;
        this.service = service;
        System.out.println(String.format("service class: %s", service));
    }

    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {
        LayoutInflater inflater = (LayoutInflater) context
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View rowView = inflater.inflate(R.layout.activity_appliance_view, parent, false);
        TextView textView = (TextView) rowView.findViewById(R.id.app_type);
        TextView textView1 = (TextView) rowView.findViewById(R.id.settings_text);
        ToggleButton toggleButton = (ToggleButton) rowView.findViewById(R.id.toggle_button);

        Log.e("app_list", String.format("length : %d", apps.size()));
        Log.e("app_list", String.format("contents : %s", apps.toString()));
        textView.setText(apps.get(position).getAppType());
        textView1.setText(apps.get(position).getAppName());
        if (apps.get(position).getAppState() == 1) {
            toggleButton.setChecked(true);
        } else
            toggleButton.setChecked(false);

        toggleButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                synchronized (apps) {
                    rpcAppliance temp_app = apps.get(position);
                    ToggleButton t = v.findViewById(R.id.toggle_button);
                    boolean on = ((ToggleButton) v).isChecked();
                    int changedState;
                    if (on) {
                        changedState = 1;
                        t.setChecked(true);
                    } else {
                        changedState = 0;
                        t.setChecked(false);
                    }
                    rpcAppliance app = rpcAppliance.newBuilder()
                            .setAppName(temp_app.getAppName())
                            .setAppState(changedState)
                            .setAppType(temp_app.getAppType()).build();
                    apps.remove(position);
                    apps.add(position, app);
                    if (service != null) {
                        try {
                            service.setState(roomName, temp_app.getAppType(), temp_app.getAppName(), changedState);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        });

        return rowView;
    }

    void updateService(HouseService s) {
        service = s;
    }
}
