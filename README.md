# EnergyMeter
Power and energy consumption monitoring of a household using CT sensor and ESP module.

Setting up the esp module

  -Download the emon library from https://github.com/openenergymonitor/EmonLib
  -Install the arduino gfx library, ssd1306 library (for the oled display module)
  -Upload the sketch to your esp module
  -Note the IP address of the module in serial monitor or your router
  
Running the server
  -Install node.js
  -Run the javscript code through cmd using node app.js
  -Host the server on ngrok (optional)

Creating a google assistant app
  -Go to https://dialogflow.cloud.google.com/
  -Create intents and fulfillments for energy and power
  -Enable webhook on the fulfillment and paste the ngrok link in it
  -Go to integrations and click on test app
  -Huzzah open google assistant on any of your device with the same account and say "Talk to home meter" (or whatever name you've given to the project)
