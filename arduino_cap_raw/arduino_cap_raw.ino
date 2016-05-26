#include <Wire.h>
#include "Adafruit_MPR121.h"
#define NBOARDS 3

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap[ NBOARDS ] = Adafruit_MPR121();

bool debug = 0;

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched[ NBOARDS ];
uint16_t currtouched[ NBOARDS ];

void setup() {
  while (!Serial);        // needed to keep leonardo/micro from starting too fast!

  Serial.begin(9600);
  if ( debug ) {
    Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  }
  
  for ( int j = 0; j < NBOARDS; j++ ) {
    //cap[ j ].setThresholds( 10, 10 );
    lasttouched[ j ] = 0;
    currtouched[ j ] = 0;
  }
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D

 
  if (!cap[ 0 ].begin(0x5A)) {
    if ( debug ) { Serial.println("MPR121 1 not found, check wiring?"); }
    while (1);
  }
  if ( debug ) { Serial.println("MPR121  1 found!"); }
  
  //2nd board cap[ 1 ] is tied to 3.3V
  if (!cap[ 1 ].begin(0x5B)) {
    if ( debug ) { Serial.println("MPR121 2 not found, check wiring?"); }
    while (1);
  }
  Serial.println("MPR121  2 found!");
  
   //3rd board cap[ 2 ] is tied to SDA
  if (!cap[ 2 ].begin(0x5C)) {
    if ( debug ) { Serial.println("MPR121 3 not found, check wiring?"); }
    while (1);
  }
  if ( debug ) { Serial.println("MPR121  3 found!"); }

}

void loop() {
  // Get the currently touched pads

    for ( int j = 0; j < NBOARDS; j ++ ) {
      for ( int i = 0; i < 12; i ++ ) {

        if ( debug ) {
          Serial.print( j * 12 + i + 1 ); Serial.print("\t");
          Serial.print(  (byte) cap[ j ].filteredData(i) );  Serial.print("\t");
          Serial.print( 255 );  Serial.print("\t");
          Serial.println();
        }

         else {
          Serial.write( j * 12 + i);
          Serial.write( (byte) cap[ j ].filteredData(i) );
          //Serial.write( (byte) cap_avg[ i ] );
          //Serial.write( (byte) base_avg[ i ] );
          Serial.write( 255 );
        }
      }
    }
  
  

  // comment out this line for detailed data from the sensor!
  delay(100);
  return;
  
  for ( int j = 0; j < 1; j ++ ) {
  //for ( int j = 0; j < NBOARDS; j ++ ) {

    // debugging info, what
    Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap[ j ].touched(), HEX);
    Serial.print("Filt: ");
    for (uint8_t i=0; i<12; i++) {
      Serial.print(cap[ j ].filteredData(i)); Serial.print("\t");
    }
    Serial.println();
    Serial.print("Base: ");
    for (uint8_t i=0; i<12; i++) {
      Serial.print(cap[ j ].baselineData(i)); Serial.print("\t");
    }
    Serial.println();
    
    // put a delay so it isn't overwhelming
    delay(100);
   }
}
