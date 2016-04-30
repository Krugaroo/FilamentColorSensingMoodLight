/*
* Name: FilamentColorSensingMoodLight
* Author: Michael Kruger
* Brief:
* 
* The GitHub for this project can be found here:
* https://github.com/Krugaroo/FilamentColorSensingMoodLight 
* 
* License: MIT License
*
* Copyright (c) 2016 Michael Kruger, Krugaroo
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#include <Adafruit_NeoPixel.h>

/* RGB board configuration */
#define RGB_DATAPIN     5
#define RGB_NUM_LEDS    8

Adafruit_NeoPixel RGB_strip = Adafruit_NeoPixel(RGB_NUM_LEDS, RGB_DATAPIN, NEO_GRB + NEO_KHZ800);

/* Pin configuration */
#define OUTPUT_PIN       2
#define S2_COLOR_SEL_PIN 3
#define S3_COLOR_SEL_PIN 4

/* Calibration data */
#define RED_MIN   748
#define GREEN_MIN 648
#define BLUE_MIN  791

#define RED_MAX   3117
#define GREEN_MAX 2948
#define BLUE_MAX  3102

enum Color{
  COLOR_RED,  COLOR_GREEN,  COLOR_BLUE
};

typedef struct{
  unsigned int red;  unsigned int green;  unsigned int blue;
  unsigned int rawRed; unsigned int rawGreen; unsigned int rawBlue;
} ColorData;

void ColorSensor_Initialize();
void ColorSensor_MeasureColor(ColorData* outColors);
void ColorSensor_SelectColor(Color inColor);
void ColorSensor_ConvertInput(ColorData* outColors);
void ColorSensor_AmplifyColor(ColorData* inColors, ColorData* outColors);

/*
 * Initialize the pins of the color sensor
 */
void ColorSensor_Initialize()
{
  pinMode(OUTPUT_PIN, INPUT);
  pinMode(S2_COLOR_SEL_PIN, OUTPUT);
  pinMode(S3_COLOR_SEL_PIN, OUTPUT);

  RGB_strip.begin();
  RGB_strip.show();
}

/*
 * Set the pins to the colour
 */
void ColorSensor_SelectColor(Color inColor)
{
  switch(inColor)
  {
    case COLOR_RED:
    digitalWrite(S2_COLOR_SEL_PIN, LOW);
    digitalWrite(S3_COLOR_SEL_PIN, LOW);
    break;
    case COLOR_GREEN:
    digitalWrite(S2_COLOR_SEL_PIN, HIGH);
    digitalWrite(S3_COLOR_SEL_PIN, HIGH);
    break;
    case COLOR_BLUE:
    digitalWrite(S2_COLOR_SEL_PIN, LOW);
    digitalWrite(S3_COLOR_SEL_PIN, HIGH);
    break;
  }
}

/*
 * Measures the colours seen by the sensor
 * The results are put in the ColorData
 * Takes 75 ms
 */
void ColorSensor_MeasureColor(ColorData* outColors)
{
  unsigned long startTime = 0;
  unsigned long countPulses = 0;
  char previousState = 0;

  ColorSensor_SelectColor(COLOR_RED);
  
  for(int i=0;i<3;i++)
  {
    startTime = micros();
    countPulses = 0;
    
    /* read for 25ms */
    while((startTime+25000L) > micros() )
    {
      char currState = digitalRead(OUTPUT_PIN);

      /*if the level on the pin changed increase the count */
      if(currState!=previousState)
      {
        countPulses++;
        previousState = currState;
      }
    }

    /* record the result and select the next color */
    switch(i)
    {
      case 0:
      outColors->rawRed = countPulses;
      ColorSensor_SelectColor(COLOR_GREEN);
      break;
      case 1:
      outColors->rawGreen = countPulses;
      ColorSensor_SelectColor(COLOR_BLUE);
      break;
      case 2:
      outColors->rawBlue = countPulses;
      ColorSensor_SelectColor(COLOR_RED);
      break;
    }
  }

  /* Convert values to RGB 0-255*/
  ColorSensor_ConvertInput(outColors);
}

/*
 * Converts the raw values to RGB values with 0-255
 */
void ColorSensor_ConvertInput(ColorData* outColors)
{
  if(outColors->rawRed < RED_MIN)
  {
    /* limit to a minimum of 0 */
    outColors->red = 0;
  }
  else if(outColors->rawRed > RED_MAX)
  {
    /* limit to a maximum of 255 */
    outColors->red = 255;
  }
  else
  {
    /* linearly convert the output to 0-255*/
    outColors->red = 255 * (outColors->rawRed*1.0 - RED_MIN) / (RED_MAX - RED_MIN);
  }

  if(outColors->rawGreen < GREEN_MIN)
  {
    /* limit to a minimum of 0 */
    outColors->green = 0;
  }
  else if(outColors->rawGreen > GREEN_MAX)
  {
    /* limit to a maximum of 255 */
    outColors->green = 255;
  }
  else
  {
    /* linearly convert the output to 0-255*/
    outColors->green = 255 * (outColors->rawGreen*1.0 - GREEN_MIN) / (GREEN_MAX - GREEN_MIN);
  }

  if(outColors->rawBlue < BLUE_MIN)
  {
    /* limit to a minimum of 0 */
    outColors->blue = 0;
  }
  else if(outColors->rawBlue > BLUE_MAX)
  {
    /* limit to a maximum of 255 */
    outColors->blue = 255;
  }
  else
  {
    /* linearly convert the output to 0-255*/
    outColors->blue = 255 * (outColors->rawBlue*1.0 - BLUE_MIN) / (BLUE_MAX - BLUE_MIN);
  }
}

/*
 * 
 */
void ColorSensor_AmplifyColor(ColorData* inColors, ColorData* outColors)
{
  if(inColors->blue > inColors->red && inColors->blue > inColors->green)
  {
    float ratio = inColors->blue/255.0;
    outColors->blue = 255;
    outColors->red = inColors->red/ratio;
    outColors->green = inColors->green/ratio;
  }
  else if(inColors->green > inColors->red && inColors->green > inColors->blue)
  {
    float ratio = inColors->green/2.55;
    outColors->green = 255;
    outColors->red = inColors->red/ratio;
    outColors->blue = inColors->blue/ratio;
  }
  else if(inColors->red > inColors->green && inColors->red > inColors->blue)
  {
    float ratio = inColors->red/255.0;
    outColors->red = 255;
    outColors->blue = inColors->blue/ratio;
    outColors->green = inColors->green/ratio;
  }
}

void setup() {
  // put your setup code here, to run once:
  ColorSensor_Initialize();

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  ColorData colours,coloursAmplified = {0};

  ColorSensor_MeasureColor(&colours);

  ColorSensor_AmplifyColor(&colours, &coloursAmplified);

  Serial.print("Red: ");
  Serial.print(colours.rawRed);
  Serial.print(" ");
  Serial.print(coloursAmplified.red);
  Serial.print(" Green: ");
  Serial.print(colours.rawGreen);
  Serial.print(" ");
  Serial.print(coloursAmplified.green);
  Serial.print(" Blue: ");
  Serial.print(colours.rawBlue);
  Serial.print(" ");
  Serial.println(coloursAmplified.blue);

  /* Set the LED strip */
  for(int i=0;i<RGB_NUM_LEDS;i++)
  {
    RGB_strip.setPixelColor(i, coloursAmplified.red, coloursAmplified.green, coloursAmplified.blue);
  }
  RGB_strip.show();

  delay(100);
}
