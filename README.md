# pebble-comtest

tests cordova communication with Pebble watch

## usage

You will need [cordova installed](https://cordova.apache.org/docs/en/3.0.0/guide_cli_index.md.html).

In the "app" directory, run these to install the app on your phone:

    cordova plugin add https://github.com/konsumer/phonegap-pebble.git
    cordova platform add android
    cordova run

Install the pebble-app in the "watch" directory on your watch. I used [cloudpebble](https://cloudpebble.net/) to make it super-easy.