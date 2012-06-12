
void readCurrent() {
  float val = analogRead(CURRENTPIN); 
  //Hole Wert //510=0A 
  val = val -510; //510=0A 0=-75A 1023=75A //springen um 0 reduzieren 
  if(val==1)val=0; 
  if(val==-1)val=0; 
  //Umwandeln um einen Wert zuwischen -75 und +75 zu erhalten 
  val=(float)75/512*val;
  val = val*10;
  Current.smooth( val );
}

void readVoltage() {
  float val  = analogRead(VOLTAGEPIN); //804 == 12.5V
  //Hole Wert //Wert umwandeln für Anzeige 0..12.6 Volt 
  val = (float)12.5/804*val; //über Serielle Schnitstelle ausgeben
  Voltage = val*10;
}

