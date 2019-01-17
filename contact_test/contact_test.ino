#include <Adafruit_GFX.h> 
#include <gfxfont.h>
#include <Adafruit_ILI9341.h>
#include <adafruit_1947_Obj.h>  // ADAFRUIT_1947
#include <SD.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <mechButton.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <screen.h>

#include "contactList.h"

#define TFT_CS  10
#define TFT_RST -1    // I think I remember this is not used.
#define SD_CS   4

#define CONTACT_FILE_PATH "/SYSTEM/CONTACTS"

contactList*  ourContacts;
blockFile*    mFile;

void setup() {

    // First bring the screen online.
  if (!initScreen(ADAFRUIT_1947,TFT_CS,TFT_RST,PORTRAIT)) {
    while(true);
  }
  
  if (!SD.begin(SD_CS)) {               // Bring the diskdrive online.
    while(true);
  }
  //mFile = new blockFile(CONTACT_FILE_PATH);
  //mFile->deleteBlockfile();
  //delete (mFile);
  mFile = new blockFile(CONTACT_FILE_PATH);
  Serial.println("New mFile, Not crashed yet.");Serial.flush();
  ourContacts = new contactList(mFile);
  contact* aContact;
  aContact = NULL;
  Serial.println("Not crashed yet.");Serial.flush();
  //aContact = ourContacts->findOrAddContact("14083400352");
  //aContact = ourContacts->findOrAddContact("13603335200");
  //aContact = ourContacts->findOrAddContact("13609298457");
  //aContact = ourContacts->findByPN("14083400352");
  //aContact = ourContacts->findByPN("13603335200");
  //aContact = ourContacts->findByPN("13609298457");
  Serial.print(" aContact : ");Serial.println((int)aContact,HEX);Serial.flush();
  if (aContact) {
    /*
    aContact->setNickName("Alliebob");
    aContact->setFirstName("Alex");
    aContact->setLastName("Lee");
    aContact->setCompanyName("Money people");
   */
    Serial.println("This is what we found.");
    Serial.print(aContact->mNickName);
     Serial.print("   Phone : ");Serial.print(aContact->mPN);
    Serial.print("   First : ");Serial.print(aContact->mFirstName);
    Serial.print("   Last : ");Serial.print(aContact->mLastName);
    Serial.print("   Company : ");Serial.println(aContact->mCompanyName);
    Serial.println("Saving to file..");Serial.flush();Serial.flush();
    ourContacts->saveToFile();
    Serial.println("Ok scary, saved to file. Not crashed yet.");Serial.flush();
  }
  int count = ourContacts->getCount();
  Serial.print("We have : ");Serial.print(count);Serial.println(" contacts in the list.");
  for (int i=0;i<count;i++) {
    aContact = (contact*)ourContacts->getByIndex(i);
    Serial.print(aContact->mNickName);
     Serial.print("   Phone : ");Serial.print(aContact->mPN);
    Serial.print("   First : ");Serial.print(aContact->mFirstName);
    Serial.print("   Last : ");Serial.print(aContact->mLastName);
    Serial.print("   Company : ");Serial.println(aContact->mCompanyName);
  }
}


void loop() {
  idle();
  delay(4000);
  Serial.print(".");
  
}
