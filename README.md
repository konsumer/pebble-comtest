# pebble-comtest

Test cordova communication with Pebble watch. The basic idea is that the watch app & cordova app are both sending the alphabet & waiting for ACK messages on every message transaction. The mobile app will log to console (use Chrome's "inspect device" developer tools) and the Pebble will log it's messages (use `pebble logs`, if you have [pebble CLI](https://developer.getpebble.com/2/additional/pebble-tool/) installed locally.)

## usage

You will need [cordova installed](https://cordova.apache.org/docs/en/3.0.0/guide_cli_index.md.html).

In the "app" directory, run these to install the app on your phone:

    cordova plugin add https://github.com/konsumer/phonegap-pebble.git
    cordova platform add android
    cordova run

Install the pebble-app in the "watch" directory on your watch. I used [cloudpebble](https://cloudpebble.net/) to make it super-easy. If you have [pebble CLI](https://developer.getpebble.com/2/additional/pebble-tool/) installed locally, run `pebble build`.