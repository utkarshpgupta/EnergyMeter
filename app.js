//Use app in strict mode
'use strict';

//Using actions on google library
const {
  dialogflow,
} = require('actions-on-google');

//Using expressjs
var express = require('express');
var bodyParser = require('body-parser');
var cors = require('cors');
var request = require("request");

const app = dialogflow({debug: true});
var expressApp = express();

var ip_add = "http://192.168.1.12/";
var url;



expressApp.use(bodyParser());
expressApp.use(bodyParser.json()); 
expressApp.use(cors());

expressApp.post('/fulfillment', app);

app.intent('Meter', (conv,{metrics}) => {
    // speech = "I got "+lednumber+" and "+status;
    var device_name = metrics;
    return new Promise((resolve, reject) => {
        var options = { 
            method: 'GET',
            url: ip_add+'Power',
            headers: 
            { 
                'cache-control': 'no-cache'
            } 
        };
    
        request(options, function (error, response, body) {
            if (error) {
                reject(conv.ask("I am not able to get the readings."));
            }
            else{
                var sensor_data = (JSON.parse(body).Power);
                console.log(sensor_data);
                resolve(conv.ask(device_name +" is " +sensor_data+ " Watts"));
            }
           
        });
  });
});

app.intent('Energy', (conv,{metrics}) => {
    // speech = "I got "+lednumber+" and "+status;
    var device_name = metrics;
    return new Promise((resolve, reject) => {
        var options = { 
            method: 'GET',
            url: ip_add+'Energy',
            headers: 
            { 
                'cache-control': 'no-cache'
            } 
        };
    
        request(options, function (error, response, body) {
            if (error) {
                reject(conv.ask("I am not able to get the readings."));
            }
            else{
                var sensor_data = (JSON.parse(body).Energy);
                console.log(sensor_data);
                resolve(conv.ask(device_name +" is " +sensor_data+ " Watt Hours"));
            }
           
        });
  });
});

expressApp.listen(8050);
console.log('Listening on port 8050...');

