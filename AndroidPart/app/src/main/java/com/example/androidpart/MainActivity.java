package com.example.androidpart;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.github.angads25.toggle.widget.LabeledSwitch;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;


public class MainActivity extends AppCompatActivity {
    final String DEACTIVATED_STATE_STR = "OFF";
    final String ACTIVATED_STATE_STR = "ON";
    String[] topics = {
            "ntpt7921/f/sensor_temperature",
            "ntpt7921/f/sensor_humidity",
            "ntpt7921/f/light_state",
            "ntpt7921/f/pump_state",
    };
    String aioUsername = "ntpt7921";
    String aioKey = "replace this with real key";

    MQTTClientAIO mqttClient;
    TextView tempText, humidText, currentState;
    LabeledSwitch lightToggle, pumpToggle;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tempText = findViewById(R.id.textCurrentTemperature);
        humidText = findViewById(R.id.textCurrentRelativeHumidity);
        currentState = findViewById(R.id.textCurrentState);

        lightToggle = findViewById(R.id.toggleButtonLight);
        pumpToggle = findViewById(R.id.toggleButtonPump);

        startClientMQTT();

        lightToggle.setOnToggledListener((toggleableView, isOn) -> {
            String command = (isOn) ? ACTIVATED_STATE_STR : DEACTIVATED_STATE_STR;
            outputCurrentState("Setting light state to " + command);
            toggleableView.setEnabled(false);

            if (isOn)
                mqttClient.publishToTopic("ntpt7921/f/light_state", "1");
            else
                mqttClient.publishToTopic("ntpt7921/f/light_state", "0");
        });
        pumpToggle.setOnToggledListener((toggleableView, isOn) -> {
            String command = (isOn) ? ACTIVATED_STATE_STR : DEACTIVATED_STATE_STR;
            outputCurrentState("Setting pump state to " + command);
            toggleableView.setEnabled(false);

            if (isOn)
                mqttClient.publishToTopic("ntpt7921/f/pump_state", "1");
            else
                mqttClient.publishToTopic("ntpt7921/f/pump_state", "0");
        });
    }

    public void startClientMQTT() {
        mqttClient = new MQTTClientAIO(this);
        mqttClient.arrayTopics = topics;
        mqttClient.aioUsername = aioUsername;
        mqttClient.aioKey = aioKey;


        mqttClient.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {
                Log.i("MQTTHelper", serverURI + " connected");
                outputCurrentState("Connection to server is established");
            }

            @Override
            public void connectionLost(Throwable cause) {
                Log.i("MQTTHelper", cause.getMessage());
                outputCurrentState("Connection to server is lost");
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) {
                Log.i("MQTTHelper",
                        "Received message: " + topic + ": " + message.toString());
                String state = message.toString().equals("0") ? DEACTIVATED_STATE_STR : ACTIVATED_STATE_STR;

                if (topic.contains("sensor_temperature")) {
                    outputCurrentState("Receive temperature value " + message);
                    tempText.setText(message + "\n°C");
                } else if (topic.contains("sensor_humidity")) {
                    outputCurrentState("Receive relative humidity value " + message);
                    humidText.setText(message + "\n%");
                } else if (topic.contains("pump_state")) {
                    outputCurrentState("Pump is currently " + state);
                    pumpToggle.setOn(!message.toString().equals("0"));
                    pumpToggle.setEnabled(true);
                } else if (topic.contains("light_state")) {
                    outputCurrentState("Light is currently " + state);
                    lightToggle.setOn(!message.toString().equals("0"));
                    lightToggle.setEnabled(true);
                }
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
            }
        });

        mqttClient.connect();
    }

    public void outputCurrentState(String s) {
        currentState.setText(s);
    }
}