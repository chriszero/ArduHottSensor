void readBmp085() {
  AbsoluteAltitude.smooth( bmp.readAltitude() );
  Temperature = bmp.readTemperature();
    /*
    Serial.print("Temperatur = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Druck = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Hoehe = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" Meter"); 
    */
}
