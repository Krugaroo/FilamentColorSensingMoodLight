/*
* Name: ColorSensor
* Author: Michael Kruger
* Brief:
* 
* This program takes color values from the color sensor and draws it in a screen.
* 
* See the Arduino sketch in this project for code to read the colors from the color sensor.
*
* NOTE: THIS CODE ASSUMES YOUR ARDUINO/MICROCONTROLLER IS THE FIRST LISTED COM PORT!
* IF THIS IS NOT THE CASE YOU NEED TO CHANGE THE CHOSEN COM PORT:
* myPort = new Serial(this, Serial.list()[0], 9600);
* Serial.list()[0] CHANGE 0 TO THE INDEX OF YOUR COM PORT
* (OR INCREMENT IT UNTIL IT WORKS)
* 
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

import processing.serial.*;

Serial myPort;        // The serial port

void setup () {
   // set the window size:
   size(1200, 600);
  
   // List all the available serial ports
   // if using Processing 2.1 or later, use Serial.printArray()
   println(Serial.list());
  
   // The Arduino/Microcontroller will have an COM port.
   // Open the correct COM port in this case the first one. Serial.list()[0].
   myPort = new Serial(this, Serial.list()[0], 115200);
  
   // don't generate a serialEvent() unless you get a newline character:
   myPort.bufferUntil('\n');
  
   // set inital background:
   background(0);

}
 
 
 void draw () {
 // everything happens in the serialEvent()
}

//this function is called when data has arrived on the COM port with a newline
 void serialEvent (Serial myPort) {
   // get the ASCII string:
   String inString = myPort.readStringUntil('\n');
  
   if (inString != null) 
   {
     // trim off any whitespace:
     inString = trim(inString);
     String[] list = split(inString, ' ');
     
     /*
     "Red: xxx xxx Green: xxx xxx Blue: xxx xxx"
      [0]  [1] [2]  [3]   [4] [5]  [6]  [7] [8]
     */
     

     float inR = float(list[2]);
     float inG = float(list[5]);
     float inB = float(list[8]);
     
        
     println(inR);
     println(inG);
     println(inB);
     background(inR,inG,inB);
    
     
   }
 }
