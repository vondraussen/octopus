# Octopus [![Build Status](https://travis-ci.org/vondraussen/octopus.svg?branch=master)](https://travis-ci.org/vondraussen/octopus)
This is a simple example project for the Octopus PCB from [fab-lab.eu](http://fab-lab.eu/octopus/). It uses the ESP8266 Wifi module.
The Octopus is equipped with two RGB LED's wich are controlled via MQTT messages from a smartphone or an other computer. There is also a [Bosch BME280](https://www.bosch-sensortec.com/bst/products/all_products/bme280) on board which is used to publish the environment values trough MQTT.

## IDE
I've used platformio to code and build. See [platformio.org](http://platformio.org/)

You could use arduino ide instead.

# Howto simple setup with Raspberry Pi
## Mosquitto MQTT Broker (server)
```wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key && sudo apt-key add mosquitto-repo.gpg.key && cd /etc/apt/sources.list.d/ && sudo wget http://repo.mosquitto.org/debian/mosquitto-jessie.list && sudo apt-get update && sudo apt-get install -y mosquitto mosquitto-clients```

## Receive BME280 Values
```mosquitto_sub -t octo/#```

## Set LED1 Color (0-255)
```mosquitto_pub -t octo/led1/red -m 255```

```mosquitto_pub -t octo/led1/green -m 255```

```mosquitto_pub -t octo/led1/blue -m 255```
