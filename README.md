# ScrollingLEDMatrixTelegram
Send messages to a LED Matrix display through Telegram. Inspired by a tweet from Smarter Every Day

## Parts

I used the following parts to build the project:

- [ESP32 i2s Matrix Shield available on Tindie](https://www.tindie.com/products/brianlough/esp32-i2s-matrix-shield/) - This is not essential, you can use the wiring diagram [described here](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA#wiring-esp32-with-the-led-matrix-panel)
- ESP32 Mini32 Development board (ESP32 D1 Mini)
    - [Aliexpress\*]( https://s.click.aliexpress.com/e/_dSi824B)
    - [Amazon.com\*](https://amzn.to/3gArkAY)
    - Also available as an add-on on the Tindie listing
- 2 x RGB LED Matrix. 64x32 P3 Matrix display - [Aliexpress\*](https://s.click.aliexpress.com/e/_dYz5DLt)

If you are not using my shield, you can use the method described in [step 5 of this post](https://www.instructables.com/RGB-LED-Matrix-With-an-ESP8266/) to power the panel.

I also mention in the video about a bigger power supply, it's not important for this project as the scrolling text doesn't use much power (how many leds are on dictates how much power is used) but for other projects you would be safer to use one. This is the one I use: [5V 8A laptop style power supply\*]( https://s.click.aliexpress.com/e/_d7uVLXt)

## Instructions

### Arduino Software Setup

These are the steps for setting up the Arduino IDE to work with the ESP32.

- Download and install the [Arduino IDE](https://www.arduino.cc/en/software). (Note: you can not use the web version)
- Follow [these instructions](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md) to install the ESP32 Software for the Arduino IDE. (Use the "stable release" URL)
    
### Programming the board

These steps assume you are using the same board I did in the video, and may need to be adapted for different boards. When finished these steps you should be able to install the Blink sketch

- In the Arduino IDE, got to "Tools->Board:" and under the "ESP32 Arduino" section select "WEMOS D1 Mini ESP32"
- Expand "Tools-> Port:" and take note of the options, then plug the USB cable into your board and repeat the same steps. The new option that shows up is your board, select it. (If no new option shows up, you may need to [install a driver]).

### Installing Arduino Libraries

Some of features I used in the video came from libraries. This section will allow you to install these libraies.

- In the Arduino IDE, go to "Sketch -> Include Library -> Manage Libraries"
- When the Library Manager Opens, search for and then install the following libraries:
    - **Universal Telegram Library** by Brian Lough
    - **Arduino Json** by Benoit Blanchon
    - **ESP32 HUB75 LED MATRIX PANEL DMA Display** by Faptastic
    - **Adafruit GFX** by Adafruit
    
### Enabling Two Matrix Displays

This section is what configures the LED Matrix to work with two panels instead of one.

- Open the "Arduino" folder, it is under the "Documents" folder on both Windows and Mac.
- In that folder open "libraries" and then "ESP32_HUB75_LED_MATRIX_PANEL_DMA_Display" folder
- Open the "ESP32-HUB75-MatrixPanel-I2S-DMA.h" file with a text editor
- Change ` #define MATRIX_WIDTH                64` to  `#define MATRIX_WIDTH                128`. You will need to change this value back if you ever go back to using a single panel.

### Setting Up Telegram

These are the steps you need to get a bot token to use in your porject.

- Install Telegram from your phones app store (I believe you need to set the account up on your phone to begin with, I'm not 100% sure though as I set it up years ago). Once logged in you can install the desktop versions, or even use the Telegram web interface version.
- Inside Telegram, search for a user called "botfather" and open a chat
- Click the "/newbot" option or you can type that message in either (exactly as shown)
- Follow the on screen instructions to create your bot
- You will get a message back with the token, you can copy this into Arduino code.
- You should click on the link up the top of the message to open the chat with the new bot. There will be a "/start" button, you should click that to activate the bot.
