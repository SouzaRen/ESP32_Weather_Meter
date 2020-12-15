import paho.mqtt.client as mqtt

mqttc = mqtt.Client()
mqttc.connect("put your broker server here", 1883)
mqttc.publish("teste/temperatura", "25")
mqttc.loop(2)