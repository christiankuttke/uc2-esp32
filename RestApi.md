Rest Api Endpoint Description
==============================

| Endpoint                                  | Type | Socket|
| ------------------------------------------| ---- | ----  |
| [/modules_get](#modules_get)              | Get  | false |
|   |  |
| [/ledarr_act](#ledarr_act)                | Post | true  |
| [/ledarr_get](#ledarr_get)                | Get  | false |
|  |  |
| [/motor_act](#motor_act)                  | Post | true  |
| [/motor_get](#motor_get)                  | Get  | false |
|   |  |
| [/wifi/scan](#wifiscan)                   | Get  | false |
| [/wifi/connect](#wificonnect)             | Post | false |
|   |  |
| [/bt_scan](#bt_scan)                      | Get  | false |
| [/bt_connect](#bt_connect)                | Post | false |
| [/bt_remove](#bt_remove)                  | Post | false |
| [/bt_paireddevices](#bt_paireddevices)    | Post | false |
|   |  |
| [/analog_joystick_get](#analog_joystick_get)    | Post | false |


/modules_get
============
GET
```
{
    "modules" : 
    {
        "led" : 1,
        "motor": 1, 
        "slm" : 0, 
        "sensor" : 0, 
        "pid" : 0, 
        "laser" : 0, 
        "dac" : 0, 
        "analog" : 0, 
        "digitalout" : 0, 
        "scanner" : 0
    }

}
```

/ledarr_act
===========
| Item      | Type              | Description |
| ----------|----------------- | ---- |
| LEDArrMode | int | //enum "array", "full",  "single", "off", "left", "right", "top", "bottom" |
| led_array | Object[] | The rgb data foreach led, depending on the mode only one color item get set |

POST
```
{
    "led": {
        "LEDArrMode": 1,
        "led_array": [
            {
                "blue": 0,
                "green": 0,
                "id": 0,
                "red": 0
            }
        ]
    }
}
```

/ledarr_get
===========
GET
```
{
    ledArrNum : 64,
    ledArrPin : 27
}
```

/motor_act
===========
POST
```
{
    motor:
    {
        steppers: [
            { stepperid: 0, speed: 0, isforever: 0 }, 
            { stepperid: 1, speed: 0, isforever: 0 },
            { stepperid: 2, speed: 0, isforever: 0 },
            { stepperid: 3, speed: 0, isforever: 0 } 
        ]
    }
}
```
/motor_get
===========
GET
```
{
  "steppers": [
    {
      "stepperid": 0,
      "dir": 0,
      "step": 0,
      "enable": 0,
      "dir_inverted": false,
      "step_inverted": false,
      "enable_inverted": false,
      "position": 0,
      "speed": 0,
      "speedmax": 20000
    },
    {
      "stepperid": 1,
      "dir": 21,
      "step": 19,
      "enable": 18,
      "dir_inverted": false,
      "step_inverted": false,
      "enable_inverted": true,
      "position": 0,
      "speed": 0,
      "speedmax": 20000
    },
    {
      "stepperid": 2,
      "dir": 0,
      "step": 0,
      "enable": 0,
      "dir_inverted": false,
      "step_inverted": false,
      "enable_inverted": false,
      "position": 0,
      "speed": 0,
      "speedmax": 20000
    },
    {
      "stepperid": 3,
      "dir": 0,
      "step": 0,
      "enable": 0,
      "dir_inverted": false,
      "step_inverted": false,
      "enable_inverted": false,
      "position": 0,
      "speed": 0,
      "speedmax": 20000
    }
  ]
}
```

/wifi/scan
===========
GET
```
{
    [
        "ssid1",
        "ssid2"
    ]
}
```
/wifi/connect
===========
POST
```
{
    "ssid": "networkid"
    "PW" : "password"
    "AP" : 0
}
```
/bt_scan
===========
GET
```
{
    [
        {
            "name" :"HyperX",
            "mac": "01:02:03:04:05:06"
        }
        ,
        {
            "name": "",
            "mac": "01:02:03:04:05:06"
        },
    ]
}
```
/bt_paireddevices
===========
GET
```
{
    [
        "01:02:03:04:05:06",
        "01:02:03:04:05:06"
    ]
}
```
/bt_connect
psx = 1 Ps3Controller, psx = 2 Ps4Controller
===========
POST
```
{ 
    "mac": "01:02:03:04:05:06", 
    "psx": 0 
}
```
/bt_remove
===========
POST
```
{ 
    "mac": "01:02:03:04:05:06",
}
```

/analog_joystick_get
============
GET
```
{
    "joy" : 
    {
        "joyX" : 35,
        "joyY": 34
    }

}
```

