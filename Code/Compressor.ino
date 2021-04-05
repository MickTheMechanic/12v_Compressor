//Another ridiculously good mod by Mick ;)

#include "GUIslice.h"
#include "GUIslice_drv.h"

// Include any extended elements
#include "elem/XRingGauge.h"

// Defines for resources
#define relayPin 13        //Will turn relay on and off
#define pressurePin A5      //Connected to pressure transducer
#define latchPin  11       //Connected to latch circuit for auto shutdown if display is powered from LiPo and not from 12v
#define buckPin 10


//counting cycles as a makeshift timer. millis and unsigned longs use too much memory.
uint8_t loopcount = 0;


// Enumerations for pages, elements, fonts, images
enum { E_PG_MAIN };
enum { E_ELEM_XRING, E_ELEM_TEXT3, 
E_ELEM_TEXT2,
     };
enum { E_xGaugeFont, E_TxtFont, MAX_FONT };

// Instantiate the GUI
#define MAX_PAGE            1
#define MAX_ELEM_PG_MAIN    3

gslc_tsGui                  m_gui;
gslc_tsDriver               m_drv;
gslc_tsFont                 m_asFont[MAX_FONT];
gslc_tsPage                 m_asPage[MAX_PAGE];
gslc_tsElem                 m_asPageElem[MAX_ELEM_PG_MAIN];
gslc_tsElemRef              m_asPageElemRef[MAX_ELEM_PG_MAIN];

gslc_tsXRingGauge           m_sXRingGauge;

// Save some element references for quick access
gslc_tsElemRef*  m_pElemXRingGauge = NULL;
gslc_tsElemRef*  m_pE_ELEM_TEXT2 = NULL;

void setup()
{
  //Set pin defaults
  pinMode(pressurePin, INPUT);
  pinMode(buckPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  digitalWrite (relayPin, LOW);
  pinMode(latchPin, OUTPUT);
  digitalWrite (latchPin, HIGH); //set latchpin high to trigger latch transistor so device remains powered.

  
  gslc_tsElemRef* pElemRef = NULL;
  // Initialize
  if (!gslc_Init(&m_gui, &m_drv, m_asPage, MAX_PAGE, m_asFont, MAX_FONT)) {
    return;
  }

  // Load Fonts
  if (!gslc_FontSet(&m_gui, E_xGaugeFont, GSLC_FONTREF_PTR, NULL, 5)) {
    return;
  }
  if (!gslc_FontSet(&m_gui, E_TxtFont, GSLC_FONTREF_PTR, NULL, 3)) {
    return;
  }

  // -----------------------------------

  // Create page elements
  gslc_PageAdd(&m_gui, E_PG_MAIN, m_asPageElem, MAX_ELEM_PG_MAIN, m_asPageElemRef, MAX_ELEM_PG_MAIN);

  // Background flat color
  gslc_SetBkgndColor(&m_gui, GSLC_COL_BLACK);


  // Create a RingGauge
  static char m_str3[3] = "";
  pElemRef = gslc_ElemXRingGaugeCreate(&m_gui, E_ELEM_XRING, E_PG_MAIN, &m_sXRingGauge,
  (gslc_tsRect) {
    15, 50, 210, 210
  }, m_str3, 3, E_xGaugeFont);
  gslc_ElemXRingGaugeSetValRange(&m_gui, pElemRef, 0, 15);
  gslc_ElemXRingGaugeSetVal(&m_gui, pElemRef, 45); // Set initial value
  gslc_ElemXRingGaugeSetAngleRange(&m_gui, pElemRef, -135, 270, true); // Three-Quarter circle
  gslc_ElemXRingGaugeSetThickness(&m_gui, pElemRef, 35);
  gslc_ElemXRingGaugeSetColorInactive(&m_gui, pElemRef, GSLC_COL_GRAY_DK2);
  gslc_ElemXRingGaugeSetColorActiveGradient(&m_gui, pElemRef, GSLC_COL_ORANGE, GSLC_COL_GREEN);
  gslc_ElemSetTxtCol  ( &m_gui, pElemRef, GSLC_COL_WHITE ) ;
  m_pElemXRingGauge = pElemRef; // Save for quick access


  //Create text "BAR"
  pElemRef = gslc_ElemCreateTxt(&m_gui, E_ELEM_TEXT3, E_PG_MAIN, (gslc_tsRect) {
    81, 250, 73, 34
  },
  (char*)"BAR", 0, E_xGaugeFont);
  gslc_ElemSetTxtCol(&m_gui, pElemRef, ((gslc_tsColor) {
    245, 246, 250
  }));


  //Create text "REFILL!"
  pElemRef = gslc_ElemCreateTxt(&m_gui, E_ELEM_TEXT2, E_PG_MAIN, (gslc_tsRect) {
    65, 10, 145, 34
  }, (char*)"REFILL!", 0, E_TxtFont);
  gslc_ElemSetTxtCol(&m_gui, pElemRef, GSLC_COL_ORANGE);
  gslc_ElemSetVisible  ( &m_gui, pElemRef, false);
  m_pE_ELEM_TEXT2 = pElemRef; // Save for quick access



  // -----------------------------------
  // Start up display on main page
  gslc_SetPageCur(&m_gui, E_PG_MAIN);

}





void loop()
{
  char acStr[3];
  delay (200);
  

  //Map transducer to bar: 0.5v - 4.5v = 0 - 20 BAR
  //I made up some simple integer math as map() function uses too much memory
  int8_t currentPressure = ((analogRead(pressurePin)-102)/41);

  //set min and max pressures
  //I include these in the loop so that they are placed in dynamic memory. there is too little space in progmem.
 uint8_t offPressure = 16;
 uint8_t onPressure = 12;

  //turn on relay if pressure drops below 12 bar
  if (currentPressure <= onPressure) {
    digitalWrite(relayPin, HIGH);  
    //Show "REFILL!" text
    gslc_ElemSetVisible  ( &m_gui, m_pE_ELEM_TEXT2, true);
  }
  //turn off relay when pressure reaches 15 bar
  else if (currentPressure >= offPressure) {
    digitalWrite(relayPin, LOW);
    //Hide "REFILL!" text
    gslc_ElemSetVisible  ( &m_gui, m_pE_ELEM_TEXT2, false);
  }

  //auto shutdown timer. Only runs if buckPin is high (ie. only runs if there is no 12v supply).
  //Counting loop cycles as a makeshift timer. millis and unsigned longs use too much memory.
  if (digitalRead (buckPin) == HIGH) {
    loopcount ++;
    if (loopcount == 80) {
      digitalWrite (latchPin, LOW);
    }
  }

  //Set pressure value to ring gauge
  gslc_ElemXRingGaugeSetVal(&m_gui, m_pElemXRingGauge, (currentPressure));

  //Print pressure on screen
  sprintf(acStr, "%d", currentPressure);
  gslc_ElemSetTxtStr(&m_gui, m_pElemXRingGauge, acStr);

// Periodically call GUIslice update function
  gslc_Update(&m_gui);
  
}
