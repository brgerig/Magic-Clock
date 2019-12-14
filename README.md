# Magic-Clock

<p align="center"><img src=https://i.imgur.com/KgbZ5a8.gif</img></p>

In the Harry Potter books and movies, Harry's friends, the Weasleys, have a magical clock that tracks where all of the family members are. This project creates a functional version of that clock, using data from phone GPS by way of an MQTT message broker. There are two versions of the code: one uses OwnTracks data directly from MQTT, and the other uses custom MQTT topics that can be sent from any system. I use Home Assistant and its Zones.

# The Parts List

* 1 NodeMCU (https://www.amazon.com/dp/B071WRD25D)
* 1 Arduino Pro Mini 3.3v (https://www.amazon.com/dp/B07RS911JD)
* Programming board for Pro Mini, if necessary (https://www.amazon.com/dp/B00N4MCS1A)
* 4 28BYJ-48 5v stepper motors with ULN2003 driver boards(https://www.amazon.com/dp/B015RQ97W8)
* Jumper wires (https://www.amazon.com/dp/B01L5ULRUA)
* 6v, 2A power adapter (https://www.amazon.com/dp/B01GCI99OG)
* Brass tubes: 1/4", 7/32", 3/16", 5/32" (https://www.amazon.com/dp/B07VLNMCRC)
* #10 threaded rod
* #10 nuts
* #10 tee nuts
* .5x3mm machine screws
* .5x3mm nuts
* Printable transparency sheets
* A bunch of 3d printed stuff
* Misc paint, stain, and finishing supplies

# The Code

First, you'll need to install some libraries into your Arduino IDE. If you don't already have them, you need:
* SoftwareSerial
* AccelStepper
* ESP8266WiFi
* PubSubClient
* ArduinoJson (if using the OwnTracks version)

ClockPrimary gets loaded on the NodeMCU. The NodeMCU is responsible for connecing to WiFi and to the MQTT server, then listening for and processing messages, and sending those messages to the Arduino Pro Mini. Pick your flavor:

* The OwnTracks version can process location and region updates directly from OwnTracks clients, which means you just need an MQTT instance. CloudMQTT has a free version which allows up to 5 connections, which is perfect for this project. You'll need to edit `config.h` with your WiFi and MQTT information. **Important:** You need to find where PubSubClient is installed, and change `MQTT_MAX_PACKET_SIZE` in `PubSubClient.h` to 256.
* The HA (Home Assistant) version allows another source, like Home Assistant automations, to publish messages to the clock with location updates that can come from anywhere you want. By default, they should take the form of topic: `home/clock/*person*`, message: `*location*`

For both, the names of the people/hands in the code should be changed to match the usernames/topic names.

ClockSecondary gets loaded on the Arduino Pro Mini. There's not much to change here. The ClockHand class extends AccelStepper to provide some additional functionality to make the code cleaner. The instantiations of the ClockHands have a boolean as the first parameter: this changes the direction of rotation for that stepper motor, since I mounted mine in opposite directions to use space efficiently. So, if a clock hand is rotating the wrong direction, change that boolean. You can also change the names of the ClockHands to match the Primary code, but if you do, make sure to change the pointer assignments in `processCommand()`, too.

# The Clients

If using OwnTracks directly, make sure the usernames of the clients match the strings in `ClockPrimary-OwnTracks.ino`. Then, on each client, define regions that are named the same as those in the code. There are three exceptions to this: Travel, Lost, and Mortal Peril: besides the location information, the code also extracts the velocity and battery level. If the velocity is over 10kph, the location used is Travel, regardless of other location information. If the client isn't in a region but isn't moving, Lost is used. And, if the client reports a battery level of <10%, we're obviously in Mortal Peril.

If using Home Assistant, feel free to use any location logic that you want. I use zones and Person entities, and send MQTT updates any time the Person state changes. I also use the same speed/battery level logic described previously, but that logic is all handled by my Home Assistant instance. Basically, do whatever you want, just make sure the *person* part of the topic matches the strings in `ClockPrimary-HA.ino`, and the message matches one of the locations listed in the code. For your convenience, not_home is treated identically to Lost.

# The Wiring

`Wiring diagram.png` contains all of the hookup information. This entire project is designed to be created with almost no wiring, and no annoying extra resistors or other little compnents. Just hook up the jumper wires and you're good to go. You can of course opt to solder everything if desired. The only non point-to-point wiring is the power, which needs to be distributed from the power jack to the NodeMCU and each of the ULN2003 boards. The Arduino is powered from the NodeMCU.

# The Build

I've documented this [here](https://imgur.com/a/wX98fOc). The files for 3d printing are all in the `3d print files` folder, with the exception of the clock hands, since those were created by someone else. Additionally, the clock face was purchased from a small laser-cut items vendor at a convention, so I'm unfortunately not able to provide a link to it.
