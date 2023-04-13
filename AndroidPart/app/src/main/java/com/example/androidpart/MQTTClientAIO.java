package com.example.androidpart;

import android.content.Context;
import android.util.Log;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.DisconnectedBufferOptions;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.nio.charset.StandardCharsets;

public class MQTTClientAIO {
    final String serverUri = "tcp://io.adafruit.com:1883";
    final String clientId = "some_random_text_served_as_ID";
    public String[] arrayTopics = {};
    public String aioUsername = "";
    public String aioKey = "";
    public MqttAndroidClient mqttAndroidClient;

    public MQTTClientAIO(Context context) {
        mqttAndroidClient = new MqttAndroidClient(context, serverUri, clientId);
        mqttAndroidClient.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean b, String s) {
            }

            @Override
            public void connectionLost(Throwable throwable) {
            }

            @Override
            public void messageArrived(String topic, MqttMessage mqttMessage) {
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
            }
        });
    }

    public void setCallback(MqttCallbackExtended callback) {
        mqttAndroidClient.setCallback(callback);
    }

    public void connect() {
        MqttConnectOptions mqttConnectOptions = new MqttConnectOptions();
        mqttConnectOptions.setAutomaticReconnect(true);
        mqttConnectOptions.setCleanSession(false);
        mqttConnectOptions.setUserName(aioUsername);
        mqttConnectOptions.setPassword(aioKey.toCharArray());

        try {
            mqttAndroidClient.connect(mqttConnectOptions, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    DisconnectedBufferOptions disconnectedBufferOptions = new DisconnectedBufferOptions();
                    disconnectedBufferOptions.setBufferEnabled(true);
                    disconnectedBufferOptions.setBufferSize(100);
                    disconnectedBufferOptions.setPersistBuffer(false);
                    disconnectedBufferOptions.setDeleteOldestMessages(false);
                    mqttAndroidClient.setBufferOpts(disconnectedBufferOptions);

                    for (String topic : arrayTopics)
                        subscribeToTopic(topic);
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.w("Mqtt", "Failed to connect to: " + serverUri + exception.toString());
                }
            });


        } catch (MqttException ex) {
            ex.printStackTrace();
        }
    }

    public void subscribeToTopic(String topic) {
        try {
            mqttAndroidClient.subscribe(topic, 0, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.i("MQTTHelper", "Subscribe to topic: " + topic + " succeeded");
                    getLastValueOfTopic(topic);
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.i("MQTTHelper", "Subscribe to topic: " + topic + " failed");
                }
            });

        } catch (MqttException ex) {
            System.err.println("Exception subscribing");
            ex.printStackTrace();
        }
    }

    public void publishToTopic(String topic, String payload) {
        try {
            MqttMessage message = new MqttMessage();
            message.setQos(0);
            message.setRetained(false);
            message.setPayload(payload.getBytes(StandardCharsets.UTF_8));

            mqttAndroidClient.publish(topic, message, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.i("MQTTHelper", "Sent to " + topic + " " + payload);
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.i("MQTTHelper", "Fail to sent to " + topic + " " + payload);
                }
            });
        } catch (MqttException ex) {
            System.err.println("Exception publishing");
            ex.printStackTrace();
        }
    }

    public void getLastValueOfTopic(String topic) {
        String topicRetain = topic + "/get";
        publishToTopic(topicRetain, "_");
    }
}