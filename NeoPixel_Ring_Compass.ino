/*    
 *   "Arduino NeoPixel Ring Compass"
 *  Written by: David Ratliff  -  August 2013
 *  Released into the public domain.  The code is free for you to use, change, make your own, etc...
 * However, it is NOT WARRANTEED, GUARANTEED, or even promised to work as may be expected.
 *
 * That said...
 *  The following sketch was written for use with the Adafruit NeoPixel Ring and the 
 * Adafruit LSM303DLHC Compass/Accelerometer to create a simple "Compass" which will point North
 * (**Assuming the ring is oriented correctly!)
 *
 *  This sketch is written to use the SCL & SCK pins on the LSM303 Breakout Board
 *  And, communicating with the NeoPixel Ring on Digital Pin 5 of Arduino UNO R.3
 *      (This can of course be easily changed below.)
 *  
 *  This is an extremely simple and quickly written sketch to prove it could be done and make my video.
 *  --- It does not compensate for 3rd axis rotation. If not held rather horizontally...
 *      compass will typically point "down"  -- PLEASE, feel free to correct this & post updates!  :)
 *
 *  --- The methods used below were also heavily influenced by the
 *      "City Bike NeoPixel Helmet" tuorial & code found on learn.adafruit.com
 *
 *    Thank you!
 */


#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303.h>



// *** Uncomment the following line for debugging - to use serial monitor
// #define debug true

#define PIN 5  // Pin NeoWheel is on


/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Mag mag = Adafruit_LSM303_Mag(12345);

// Initiate NeoPixel Wheel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);


//  Assign pixels to heading directions

const int NNW = 15;
const int NW = 14;
const int WNW = 13;
const int W = 12;
const int WSW = 11;
const int SW = 10;
const int SSW = 9;
const int S = 8;
const int SSE = 7;
const int sE = 6;
const int ESE = 5;
const int E = 4;
const int ENE = 3;
const int NE = 2;
const int NNE = 1;
const int N = 0;
// variable and initial value for "North" indicator
int indicator = 0;

//  brightness level / RGB value used for LEDs
const byte level = 10;    // valid vallues between 0 - 255... but 0 would be a tad hard to see

float heading;


void setup(void) 
{

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // the following serial functions only used if "#define debug true" at beginning of sketch is UNCOMMENTED 
#ifdef debug
  Serial.begin(9600);
  Serial.println("Magnetometer Test"); 
  Serial.println("");

  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
#endif

  if(mag.begin())
  {
    // one cycle of red around the ring to signal we're good to go!
    colorWipe(strip.Color(55, 0, 0), 50); // Wipe RED around LED WHEEL - Verfies magnetometer ready
  }
}

void loop(void) 
{
  /* Get a new sensor event */
  sensors_event_t event; 
  mag.getEvent(&event);

  const float Pi = 3.14159;

  // Calculate the angle of the vector y,x
  heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;

  // Normalize to 0-360
  if (heading < 0)
  {
    heading = 360 + heading;
  }

  // more ebugging serial monitor stuff
#ifdef debug
  Serial.print("Compass Heading: ");
  Serial.println(heading);
#endif

  // use heading to set & show NeoPixels on ring
  setIndicator();
  showIndicator();
  delay(50);    // just to let things settle a bit
}

void setIndicator()
{  // broken up into section to save processor time
  if ((heading >= 11.25)&&(heading < 101.25));  // NNE to E 
  {
    if ((heading >= 11.25) && (heading < 33.75))
    {
      indicator = NNE;
    }  //end if NNE

      if ((heading >= 33.75) && (heading < 56.25))
    {
      indicator = NE;
    }  //end if NE

      if ((heading >= 56.25) && (heading < 78.75))
    {
      indicator = ENE;
    }  //end if ENE

      if ((heading >= 78.75) && (heading < 101.25))
    {
      indicator = E;
    }  //end if E
  }    //end if NNE to E

    if ((heading >= 101.25) && (heading < 191.25))    // ESE to S
  {
    if ((heading >= 101.25) && (heading < 123.75))
    {
      indicator = ESE;
    }  //end if ESE

      if ((heading >= 123.75) && (heading < 146.25))
    {
      indicator = sE;    // Do not change to SE -  "SE" is apparrently a library-used keyword
    }  //end if sE

      if ((heading >= 146.25) && (heading < 168.75))
    {
      indicator = SSE;
    }  //end if SSE

      if ((heading >= 168.75) && (heading < 191.25))
    {
      indicator = S;
    }   //end if S
  }    //end if ESE to S
  if ((heading < 281.25) && (heading > 191.25))    // SSW to W
  {

    if ((heading >= 191.25) && (heading < 213.75))
    {
      indicator = SSW;
    }  //end if SSW

      if ((heading >= 213.75) && (heading < 236.25))
    {
      indicator = SW;
    }   //end if SW

      if ((heading >= 236.25) && (heading < 258.75))
    {
      indicator = WSW;
    }  //end if WSW

      if ((heading >= 258.75) && (heading < 281.25))
    {
      indicator = W;
    }    //end if W
  }    //end if SSW to W

    if ((heading >= 281.25) || (heading < 11.25))    // WNW to N
  {
    if ((heading >= 281.25) && (heading < 303.75))
    {
      indicator = WNW;
    }    //end if WNW

      if ((heading >= 303.75) && (heading < 326.25))
    {
      indicator = NW;
    }  //end if NW

      if ((heading >= 326.25) && (heading < 348.75))
    {
      indicator = NNW;
    }  //end if NNW

      if ((heading >= 348.75) || (heading < 11.25))
    {
      indicator = N;
    }   //end if N

  }  // end if WNW to N

}  // end void setIndicator()

void showIndicator()
{
  // set a little border to highlight the North indicator
  int indicatorLeft = indicator - 1;
  int indicatorRight = indicator + 1;

  // scale / normalize to 0 - 15

  if (indicatorLeft < 0)
  {
    indicatorLeft += 16; 
  }    //end if <0

  if (indicatorRight > 15)
  {
    indicatorRight -= 16; 
  }    //end if <15

  // Assign colors to 'dem thare variables!
  colorWipe(strip.Color(0, 0, level), 0);             //set All Blue (background dial color)
  strip.setPixelColor(indicator, level, 0, 0);        // set indicator RED
  strip.setPixelColor(indicatorLeft, 0, level, 0);    // set indicator border GREEN
  strip.setPixelColor(indicatorRight, 0, level, 0);   // set indicator border GREEN

  strip.show();                                   // Push bits out!

}  // end void showIndicator()

// Fill the pixels one after the other with a color    
// * Borrowed from ADAFRUIT Example Code
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}












