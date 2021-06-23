/*******************************************************************
    A scrolling text sign on 2 RGB LED matrixes where the text
    can be updated via telegram.

    Parts Used:
    2 x 64x32 P3 Matrix display * - https://s.click.aliexpress.com/e/_dYz5DLt
    ESP32 D1 Mini * - https://s.click.aliexpress.com/e/_dSi824B
    ESP32 I2S Matrix Shield (From my Tindie) = https://www.tindie.com/products/brianlough/esp32-i2s-matrix-shield/

 *  * = Affilate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// ----------------------------
// Standard Libraries
// ----------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 MATRIX DMA")
// https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA

#include <UniversalTelegramBot.h>
// This is the library for connecting to Telegram

// Can be installed from the library manager (Search for "Universal Telegram")
// https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/

// ----------------------------
// Dependancy Libraries - each one of these will need to be installed.
// ----------------------------

// Adafruit GFX library is a dependancy for the matrix Library
// Can be installed from the library manager
// https://github.com/adafruit/Adafruit-GFX-Library

// ArduinoJson is used for parsing Json from the API responses.
// It is required by the Telegram Library.
// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson


// -------------------------------------
// ------- Replace the following! ------
// -------------------------------------

// Wifi network station credentials
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

// -------------------------------------
// -------   Display Config   ------
// -------------------------------------

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 64     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 1      // Total number of panels chained one to another

//------- ---------------------- ------

MatrixPanel_I2S_DMA *dma_display = nullptr;

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// -------------------------------------
// -------   Text Configuraiton   ------
// -------------------------------------

#define FONT_SIZE 2 // Text will be FONT_SIZE x 8 pixels tall.

int delayBetweeenAnimations = 35; // How fast it scrolls, Smaller == faster
int scrollXMove = -1; //If positive it would scroll right

int textXPosition = PANEL_RES_X * PANEL_CHAIN;    // Number of pixels wide of each INDIVIDUAL panel module.
// Will start one pixel off screen
int textYPosition = PANEL_RES_Y / 2 - (FONT_SIZE * 8 / 2); // This will center the text

String text = "Hello Smarter Every Day!"; //Starting Text

//------- ---------------------- ------

// For scrolling Text
unsigned long isAnimationDue;

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

void setup() {

  Serial.begin(115200);

  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  );

  mxconfig.double_buff = true;
  mxconfig.gpio.e = 18;

  //mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->fillScreen(myBLACK);
  dma_display->showDMABuffer();
  dma_display->setTextSize(FONT_SIZE);
  dma_display->setTextWrap(false); // N.B!! Don't wrap at end of line
  dma_display->setTextColor(myRED); // Can change the colour here

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());


}

// Will be used in getTextBounds.
int16_t xOne, yOne;
uint16_t w, h;
bool checkTelegram = false;

void loop() {
  unsigned long now = millis();
  if (now > isAnimationDue)
  {
    // This tells the code to update the second buffer
    dma_display->flipDMABuffer();

    // This sets the timer for when we should scroll again.
    isAnimationDue = now + delayBetweeenAnimations;

    textXPosition += scrollXMove;

    // Checking if the very right of the text off screen to the left
    dma_display->getTextBounds(text, textXPosition, textYPosition, &xOne, &yOne, &w, &h);
    if (textXPosition + w <= 0)
    {
      checkTelegram = true;
      textXPosition = dma_display->width();
    }

    dma_display->setCursor(textXPosition, textYPosition);

    // The display has to do less updating if you only black out the area
    // the text is
    //dma_display->fillScreen(myBLACK);
    dma_display->fillRect(0, textYPosition, dma_display->width(), FONT_SIZE * 8, myBLACK);

    dma_display->print(text);

    // This code swaps the second buffer to be visible (puts it on the display)
    dma_display->showDMABuffer();

  }

  // Telegram will only be checked when there is no data on screen
  // as checking interfeers with scrolling.
  if (checkTelegram)
  {
    checkTelegram = false;
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    if (numNewMessages > 0)
    {
      Serial.println("got response");

      // This is where you would check is the message from a valid source
      // You can get your ID from myIdBot in the Telegram client
      //if(bot.messages[0].chat_id == "175753388")
      if (true) {
        // Takes the contents of the message, and sets it to be displayed.
        text = bot.messages[0].text;
      }
    }
  }

}
