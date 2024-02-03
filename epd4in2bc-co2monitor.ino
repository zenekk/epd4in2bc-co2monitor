/* Includes ------------------------------------------------------------------*/
#include "DEV_Config.h"
#include "EPD.h"
#include "GUI_Paint.h"
#include "ImageData.h"
#include "pusheen.h"
#include <stdlib.h>

#include <Wire.h>
#include "SparkFun_SCD4x_Arduino_Library.h"

#include <ArduinoJson.h>
#include "SecretWifiPassword.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HttpClient.h>

SCD4x SCD41;


int lastCO2;
int loopIterator;


WiFiMulti WiFiMulti;

const char kHostname[] = "worldtimeapi.org";
const char kPath[] = "/api/timezone/Europe/Prague";


// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;



void PaintText(char *row1, char *row2, char *row3, char *row4, char *row5, PAINT_TIME *pTime)
{

printf("EPD_4IN2BC_test Demo\r\n");
  DEV_Module_Init();

  printf("e-Paper Init and Clear...\r\n");
  EPD_4IN2BC_Init();
  //EPD_4IN2BC_Clear();
  DEV_Delay_ms(500);

  //Create a new image cache named IMAGE_BW and fill it with white
  UBYTE *BlackImage, *RYImage; // Red or Yellow
  UWORD Imagesize = ((EPD_4IN2BC_WIDTH % 8 == 0) ? (EPD_4IN2BC_WIDTH / 8 ) : (EPD_4IN2BC_WIDTH / 8 + 1)) * EPD_4IN2BC_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    while(1);
  }
  if ((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for red memory...\r\n");
    while(1);
  }
  printf("NewImage:BlackImage and RYImage\r\n");
  Paint_NewImage(BlackImage, EPD_4IN2BC_WIDTH, EPD_4IN2BC_HEIGHT, 180, WHITE);
  Paint_NewImage(RYImage, EPD_4IN2BC_WIDTH, EPD_4IN2BC_HEIGHT, 180, WHITE);

  //Select Image
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  Paint_SelectImage(RYImage);
  Paint_Clear(WHITE);

//#if 1   // show image for array    
  printf("show image for array\r\n");
  //EPD_4IN2BC_Display(gImage_4in2bc_b, gImage_4in2bc_ry);
//  EPD_4IN2BC_Display(PICTURE, gImage_4in2bc_ry);
//  DEV_Delay_ms(2000);
//#endif

//#if 1   // Drawing on the image
  /*Horizontal screen*/
  //1.Draw black image
  printf("Draw black image\r\n");
  Paint_SelectImage(BlackImage);
  //Paint_Clear(WHITE);
  /*Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 110, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  */

Paint_DrawBitMap(PICTURE);

  
  //Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
  
  
  Paint_DrawString_EN(10, 0, row1, &Font16, WHITE, BLACK);  
  Paint_DrawString_EN(10, 20, row2, &Font16, WHITE, BLACK);  
  Paint_DrawString_EN(10, 40, row3, &Font16, WHITE, BLACK);  
  Paint_DrawString_EN(10, 60, row4, &Font16, WHITE, BLACK);  
  Paint_DrawString_EN(10, 80, row5, &Font16, WHITE, BLACK);  
  Paint_DrawTime(200, 80, pTime, &Font16, WHITE, BLACK);
  
  
  //Paint_DrawString_CN(130, 20, "微雪电子", &Font24CN, WHITE, BLACK);
  //Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
/*
  //2.Draw red image
  printf("Draw red image\r\n");
  Paint_SelectImage(RYImage);
  Paint_Clear(WHITE);
  Paint_DrawCircle(160, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawCircle(210, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawString_CN(130, 0, "你好abc", &Font12CN, BLACK, WHITE);
  Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
  Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
*/
  printf("EPD_Display\r\n");
  EPD_4IN2BC_Display(BlackImage, RYImage);
  DEV_Delay_ms(2000);
//#endif

  printf("Clear...\r\n");
  //EPD_4IN2BC_Clear();

  printf("Goto Sleep...\r\n");
  EPD_4IN2BC_Sleep();
  free(BlackImage);
  free(RYImage);
  BlackImage = NULL;
  RYImage = NULL;

}





void PaintTextSetup(char *text)
{

printf("EPD_4IN2BC_test Demo\r\n");
  DEV_Module_Init();

  printf("e-Paper Init and Clear...\r\n");
  EPD_4IN2BC_Init();
  EPD_4IN2BC_Clear();
  DEV_Delay_ms(500);

  //Create a new image cache named IMAGE_BW and fill it with white
  UBYTE *BlackImage, *RYImage; // Red or Yellow
  UWORD Imagesize = ((EPD_4IN2BC_WIDTH % 8 == 0) ? (EPD_4IN2BC_WIDTH / 8 ) : (EPD_4IN2BC_WIDTH / 8 + 1)) * EPD_4IN2BC_HEIGHT;
  if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for black memory...\r\n");
    while(1);
  }
  if ((RYImage = (UBYTE *)malloc(Imagesize)) == NULL) {
    printf("Failed to apply for red memory...\r\n");
    while(1);
  }
  printf("NewImage:BlackImage and RYImage\r\n");
  Paint_NewImage(BlackImage, EPD_4IN2BC_WIDTH, EPD_4IN2BC_HEIGHT, 180, WHITE);
  Paint_NewImage(RYImage, EPD_4IN2BC_WIDTH, EPD_4IN2BC_HEIGHT, 180, WHITE);

  //Select Image
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  Paint_SelectImage(RYImage);
  Paint_Clear(WHITE);

#if 0   // show image for array    
  printf("show image for array\r\n");
  EPD_4IN2BC_Display(gImage_4in2bc_b, gImage_4in2bc_ry);
  DEV_Delay_ms(2000);
#endif

#if 1   // Drawing on the image
  /*Horizontal screen*/
  //1.Draw black image
  printf("Draw black image\r\n");
  Paint_SelectImage(BlackImage);
  Paint_Clear(WHITE);
  Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Paint_DrawPoint(10, 110, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
  Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
  Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  //Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
  Paint_DrawString_EN(10, 0, text, &Font16, BLACK, WHITE);
  Paint_DrawString_CN(130, 20, "微雪电子", &Font24CN, WHITE, BLACK);
  Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);

  //2.Draw red image
  printf("Draw red image\r\n");
  Paint_SelectImage(RYImage);
  Paint_Clear(WHITE);
  Paint_DrawCircle(160, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawCircle(210, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawString_CN(130, 0, "你好abc", &Font12CN, BLACK, WHITE);
  Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
  Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);

  printf("EPD_Display\r\n");
  EPD_4IN2BC_Display(BlackImage, RYImage);
  DEV_Delay_ms(2000);
#endif

  printf("Clear...\r\n");
  //EPD_4IN2BC_Clear();

  printf("Goto Sleep...\r\n");
  EPD_4IN2BC_Sleep();
  free(BlackImage);
  free(RYImage);
  BlackImage = NULL;
  RYImage = NULL;

}









/* Entry point ----------------------------------------------------------------*/
void setup()
{
    Serial.begin(115200);
    Wire.begin();

    WiFiMulti.addAP(WIFIAP, WIFIPASS);
 Serial.print("Waiting for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    delay(100);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
    Serial.println("Setup done");

if (SCD41.begin(false, true) == false)
  {
    Serial.println("SCD41 was not set correctly. Check the connection.");
    while (1)
      ;
  }

  if (SCD41.startLowPowerPeriodicMeasurement() == true)  {
    Serial.println("Low power mode enabled.");
  }


    Serial.println("Starting to do the Setup painting.");


    delay(1000);


  //PaintText("Setup");
 
}

/* The main loop -------------------------------------------------------------*/
void loop()
{
loopIterator++;
int co2;
float temp;
float humidity;

  co2= 42;
  char textBuffers [40][100];
   char datetime[100];

PAINT_TIME pTime;

   
  int numTexts =0;
  //
Serial.print("xxxxxxxxxxxxxxxxxx\n\n\n\n\n\n\n\nLoop start: ");
Serial.print(loopIterator);
Serial.print("\nlast co2: ");
Serial.print(lastCO2);
  delay(10000);




 Serial.print("Connecting to ");
    Serial.println(kHostname);

    // Use WiFiClient class to create TCP connections
    WiFiClient mClient;
    HttpClient http(mClient);
  int err =0;



err = http.get(kHostname, kPath);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);
if(err == 200)
{
      // Usually you'd check that the response code is 200 or a
      // similar "success" code (200-299) before carrying on,
      // but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;


 // Allocate the JSON document
  JsonDocument doc;

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, mClient);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    mClient.stop();
    return;
  }

  // Extract values
  Serial.println(F("Response:"));
 // Serial.println(doc["sensor"].as<const char*>());
 // Serial.println(doc["time"].as<long>());
 // Serial.println(doc["data"][0].as<float>(), 6);
  Serial.println(doc["datetime"].as<const char*>());
 String datetimeStr;
 datetimeStr = doc["datetime"].as<String>();

 
  //strcpy(datetime, doc["datetime"].as<const char*>());

 String tempStr;
 tempStr = datetimeStr.substring(datetimeStr.indexOf("T")+1); //time (from hours till the end of string)
pTime.Hour = tempStr.substring(0, tempStr.indexOf(":")).toInt();
tempStr = tempStr.substring(tempStr.indexOf(":")+1); //time string from minutes (first colon) until the end
pTime.Min = tempStr.substring(0, datetimeStr.indexOf(":")).toInt(); //minutes, i.e. until the next colon
tempStr = tempStr.substring(tempStr.indexOf(":")+1, tempStr.indexOf(".")); //time string from seconds(seconds colon) until period (dot), i.e. this is seconds
pTime.Sec = tempStr.toInt();

 Serial.println(datetimeStr.indexOf("T"));
 Serial.println(datetimeStr.substring(0, datetimeStr.indexOf("T")));
datetimeStr = datetimeStr.substring(0, datetimeStr.indexOf("T"));
 Serial.println(datetimeStr);
  
  strcpy(datetime, datetimeStr.c_str());
  Serial.println(datetime);
      }
        
      
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
      
      
      }
      else
      {
        Serial.print("Response not 200: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();


  

    Serial.println("Closing connection.");
    mClient.stop();

    Serial.println("Waiting 5 seconds before restarting...");
    delay(5000);




    


// https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/


// https://github.com/LaskaKit/SCD41-CO2-Sensor/blob/main/FW/SCD41_MeteoMini/SCD41_MeteoMini.ino

if (SCD41.readMeasurement()) // wait for a new data (approx 30s)
  {
    Serial.println();

co2 = SCD41.getCO2();

    Serial.print("CO2(ppm):");
    Serial.print(co2, 1);
    Serial.println("---");

    Serial.print("\tTemperature(C):");
    Serial.print(SCD41.getTemperature(), 1);
temp = SCD41.getTemperature();
    Serial.print("\tHumidity(%RH):");
    Serial.print(SCD41.getHumidity(), 1);
humidity = SCD41.getHumidity();
    Serial.println();
  }

Serial.println("Paiting text");
//sprintf(textBuffer, "Temperature(C): %.1f. Humidity(%%): %.1f. CO2(ppm): %d. Last co2: %d. Iteration: %d", temp, humidity ,co2, lastCO2, loopIterator);


sprintf(textBuffers[numTexts], "T:   %.1f C", temp); numTexts++;
sprintf(textBuffers[numTexts], "H:   %.1f %%", humidity); numTexts++;
sprintf(textBuffers[numTexts], "CO2: %d ppm, last CO2: %d ppm", co2, lastCO2); numTexts++;
sprintf(textBuffers[numTexts], "Iteration: %d", loopIterator); numTexts++;
sprintf(textBuffers[numTexts], "Time: %s", datetime); numTexts++;



for(int i=0;i<numTexts;i++)
{
Serial.println(textBuffers[i]);  
}

  delay(1000);

if(co2 !=lastCO2)
{
  Serial.println("CO2 different. Painting.");
    delay(1000);

  PaintText(textBuffers[0], textBuffers[1], textBuffers[2], textBuffers[3], textBuffers[4], &pTime);
Serial.println("Paiting done. Waiting 5 min");
  delay(5*60*1000);
}
  

  
  delay(1000);

lastCO2 = co2;
  
}
