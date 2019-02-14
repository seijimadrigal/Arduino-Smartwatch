 // LIBRARY FOR LCD DISPLAY
#include <U8glib.h>
// BLUETOOTH SERIAL PORT
#include <SoftwareSerial.h>
// BUTTON LIBRARY
#include <OneButton.h>
//DISPLAY//
//U8GLIB_SSD1306_128X64 lcd(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI
U8GLIB_SSD1306_128X64 lcd(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST); // Fast I2C / TWI
//BLUETOOTH LE MODULE
SoftwareSerial btModule( 9,10);
// BUTTONS //
// Setup a new OneButton
OneButton upBtn(A1, true);
OneButton downBtn(A2, true);
OneButton midBtn(A0, true);
// VARIABLE FOR THE CHARACTERS RECEIVED FROM BLUETOOTH
char c;
// VARIABLE FOR SERIAL MONITOR
char f;
bool next;
// DIFFERENT SCREEN PAGES
int page;
int currentPage;
//VARIABLE FOR MOVING OBJECTS ON SCREEN
int xpos;
int xpos2;
int ypos;
bool playPause;
bool weatherBtn;
String temp;
String temp1;
//STRINGS TO PUT CHARACTERS RECEIVED 
String inputString;
String outputString;
//VARIABLES FOR TEXT ON SCREEN 
String Clock;
String Clock2;
String notifString;
String notifString2;
String trackString;
String trackString2;
String artistString;
String artistString2;
// WHEN TRUE ADDS THE CHARACTERS TO SPECIFIC STRINGS
bool writeNotif;
bool writeArtistname;
bool writeTrackname;
bool writeClock;
//Weather Variables
String mainDay;
String today;
String tomorrow;
String twodays;
String notification;
// START SCREEN
void bootScreen()
{
  lcd.setFont(u8g_font_unifont);
  lcd.drawStr( 10, 15, "Glass v1.0");
  lcd.drawStr(10, 40, "Welcome");
}

//STARTING UP CODE//
void setup() {

  //BEGINS BLE COMMUNICATION
  Serial.begin(9600);
  btModule.begin(9600);
//  btTest.begin(9600);
 Serial.print("SERIAL STARTED");
  //INITIALISE BUTTONS//
  upBtn.attachClick(upclick);
  upBtn.attachLongPressStart(uplongPressStart);
  downBtn.attachClick(downclick);
  downBtn.attachLongPressStart(downlongPressStart);
  midBtn.attachClick(midclick);
  midBtn.attachLongPressStart(midlongPressStart);
  //STARTS BOOT SCREEN
  lcd.firstPage();
 do {
    bootScreen();
  } while ( lcd.nextPage() );
  delay(1000);
  //SCREEN POSITION & PAGE //
  ypos = 0;
  xpos, xpos2 = 124;
  page = 0;
// WRITE FALSE
writeTrackname = false;
writeArtistname = false;
writeClock = false;
writeNotif = false;

}
void loop() {
  // READS INPUT TO BUTTONS //
  upBtn.tick();
  midBtn.tick();
  downBtn.tick();
// UPDATES THE SCREEN
  lcd.firstPage();
  do {
    switch (page)
    {
      case 0: homeScreen(); break;
      case 1: Weather (); subWeather(); break;
      case 2: Spotify(); break;
      case 3: notifications(); break;
      default: page = 0;

    }
  }
  while ( lcd.nextPage() );

// THIS IS JUST FOR TESTING TO 
//CHECK IF COMMANDS TO PHONE WORKS
  
   if(Serial.available()>0){
       f =Serial.read();

    if(f == '+'){
      delay(50);
      btModule.print("+++++");
       delay(30);
       btModule.print("+++++");
    }
    if(f == '-'){
    delay(50);
    btModule.print("------");
    delay(30);
    btModule.print("------");
    }

     if(f == '<'){
      delay(50);
    btModule.print("шшш");
    delay(30);
     btModule.print("шшш");
    }
     if(f == '>'){
     btModule.print("&&&&");
     delay(30);
    btModule.print("&&&&&");
    }
   }
   

// BLUETOOTH READING OF INCOMING DATA;

 if (btModule.available() > 0)
  {
   c = btModule.read();
   Serial.print(c);

   if (c == '['){
        trackString = "";
        writeTrackname = true;
   } 

    if (c == '{'){
        Clock = "";
        writeClock = true;
   } 
   
  if (c == '<'){
       artistString = "";
        writeArtistname = true;
   }

  if (c == '%'){
        notifString = "";
        writeNotif = true;
   }
 if(c ==']'){
           Serial.print("\n TRACK:" + trackString + "\n");         
           writeTrackname = false;
           trackString.remove(0,1);          
        if(trackString != trackString2){    
           trackString2 = trackString;
          
         }
    }

 if(c =='>'){
           Serial.print("\n Artist:" + artistString + "\n");
          
           writeArtistname = false;
            artistString.remove(0,1);
         if(artistString != artistString2){
           artistString2 = artistString;
          btModule.print("^");            
         }        
    }
  if(c =='}'){
           Serial.print("\n Clock:" + Clock + "\n");
           writeClock = false;

       Clock.remove(0,1);

         if(Clock != Clock2){
           Clock2 = Clock;
        
         }
    }

     if (c == '@'){
      Serial.print("\n NOTIFICATION: " + notifString + "\n");
      writeNotif = false;
      
      notifString.remove(0,1);

         if(notifString != notifString2){
           notifString2 = notifString;
         
         }
       
   }
if(writeNotif){
  notifString += c;
  
  }
if(writeTrackname){

    trackString += c;
   
  }    
 if(writeArtistname){

    artistString += c;
  }
 if(writeClock){

    Clock += c;
   
  }  
   
   }
  // moving animation
  if (xpos > -1000) {
    xpos--;
  }
  else {
    xpos = 300;
  }
  // moving animation
  if (xpos2 > -1000) {
    xpos2 -= 2;
  }
  else {
    xpos2 = 300;
  }

  if (weatherBtn == true && ypos > -64) {
    ypos -= 11;
  }
  else if (weatherBtn == false && ypos < 0) {
    ypos += 11;
  }

}

// FUNCTIONS FOR DIFFERENT TYPES OF SCREENS THAT CAN BE PRINTED ON DISPLAY 

void homeScreen() {

  lcd.setFont(u8g_font_fur20);
 // lcd.drawStr( 10, 30, );
  lcd.setPrintPos( 25, 40);
  lcd.print(Clock2);
  lcd.setFont(u8g_font_unifont);
  lcd.setPrintPos( xpos2, 61);
  lcd.print(trackString2);
}

void Weather() {

  lcd.setFont(u8g_font_unifont);
  lcd.drawStr( 10, 10 + ypos, "Weather");
  lcd.drawStr( 10, 35 + ypos, "19 Celsius");
  lcd.drawStr( 10, 60 + ypos, "Melbourne, AU");
}
void Spotify() {

  lcd.setFont(u8g_font_unifont);
  lcd.drawStr( 0, 10, "Spotify");
  lcd.setPrintPos(xpos2, 45);
  lcd.print(trackString2);
  lcd.setPrintPos (xpos2, 60);
  lcd.print(artistString2);
}

void notifications() {

  lcd.setFont(u8g_font_unifont);
  lcd.drawStr( 10, 15, "Notifications");
  lcd.setFont(u8g_font_unifont);
   lcd.setPrintPos(xpos2, 45);
  lcd.print(notifString2);
}

void subWeather()

{

  lcd.setFont(u8g_font_unifont);
  lcd.drawStr( 0, 80 + ypos, "today");
  lcd.drawStr( 0, 100 + ypos, "tomorrow");
  lcd.drawStr( 0, 120 + ypos, "saturday" );

}

// FUNCTIONS IF BUTTON IS PRESSED

void upclick() {
  if (page == 2) {
    btModule.print(F("ù"));
    }
}

void downclick() {
 if (page == 2) {
    btModule.print(F("ú"));
    }

}
void midclick() {
   page ++;
  Serial.print("Button Pressed");
}
void midlongPressStart() {
  if (page == 2) {
    playPause ^= true;
  if(playPause == true){  
    btModule.print('*');
  }
 if(playPause == false){   
    btModule.print(F("µ"));
    }
  }
}
void uplongPressStart(){
  if (page == 2) {
    btModule.print(F("¶"));
    }
  }

  void downlongPressStart(){
   if (page == 2) {
    btModule.print(F("ш"));
    }

    
  }
  
  



