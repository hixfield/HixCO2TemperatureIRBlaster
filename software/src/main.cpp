#include <Arduino.h>
#include <HixPinDigitalOutput.h>
#include <IRac.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRtext.h>
#include <IRutils.h>
#include <MHZ19.h>
#include <ir_Samsung.h>


//connected devices and software modules
HixPinDigitalOutput g_beeper(2);
//HixPinDigitalOutput g_ir(15);
IRSamsungAc         g_IRTransmitter(15);



void AC_On(int nTemperature) {
    Serial.print("AC ON to ");
    Serial.print(nTemperature);
    Serial.println(" C");
    //transmit our commands
    g_IRTransmitter.on();
    g_IRTransmitter.setFan(kSamsungAcFanAuto);
    g_IRTransmitter.setMode(kSamsungAcAuto);
    g_IRTransmitter.setTemp(nTemperature);
    g_IRTransmitter.setSwing(true);
    g_IRTransmitter.send();
}


//////////////////////////////////////////////////////////////////////////////////
// Setup
//////////////////////////////////////////////////////////////////////////////////

void setup() {
    Serial.begin(115200);
    Serial.print(F("Startup "));
   //setup beeper
    Serial.println(F("Setup beeper"));
    g_beeper.begin();    
    //configure ir transmitter
    Serial.println("Setting up IR Transmitter");
    //g_ir.begin();
    g_IRTransmitter.begin();
    //all done
    g_beeper.blink(true, 5, 100);
    Serial.println(F("Setup complete"));
}

//////////////////////////////////////////////////////////////////////////////////
// Loop
//////////////////////////////////////////////////////////////////////////////////

void loop() {
    //g_ir.blink(true,4,1);
    AC_On(23);
    g_beeper.blink(true,4,1);    
}
