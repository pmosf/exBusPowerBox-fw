#ifndef JETIBOX_MENU
#define JETIBOX_MENU
#else
#error "JETIBOXMENU already included"
#endif

//                                          X  Y
#define JBM_MAIN           __MTYP(D      ,   0, 0)   // Eintritt bzw. Konfig geändert, speichern/verwerfen
#define JBM_SENSCFG        __MTYP(D|U|R  ,   0, 1)   // Sensor Konfig
#define JBM_RESVAL         __MTYP(D|U|R|L, 0xf, 1)   // Reset Summvalues
#define JBM_SETUP          __MTYP(D|U  |L, 0xe, 1)   // allg. Konfig
#define JBM_HM_LAST        JBM_SETUP


// =============
// Sensor Konfig
// =============

// Drehzahl
// --------
#if _SENS_RPM_ == 1
#define X0  XP_RPM
#define JBM_RPM_A          __MTYP(D|U|R  ,   X0, 2)    // Sensor aktivieren
#define JBM_RPM_RELCNT     __MTYP(D|U    ,   X0, 3)    // # Auslösungen je Umdrehung
#define JBM_RPM_ACCURACY   __MTYP(  U    ,   X0, 4)    // Glättung, RPM Genauigkeit
#endif

// Fuel
// ----
#if _SENS_FUEL_ == 1
#define X1  XP_FUEL
#define JBM_FUEL_A         __MTYP(D|U|R|L,   X1, 2)    // Sensor aktivieren
#define JBM_FUEL_VOL       __MTYP(D|U    ,   X1, 3)    // Tankgröße
#define JBM_FUEL_RES       __MTYP(D|U    ,   X1, 4)    // Volume Reset, man/auto
#define JBM_FUEL_CNT       __MTYP(D|U    ,   X1, 5)    // # Impulse je Liter
#define JBM_FUEL_CAL       __MTYP(  U    ,   X1, 6)    // # Impulse je Liter kalibrieren
#endif

// Temperatur
// ----------
#if _SENS_TEJBM_ == 1
#define X2  XP_TEMP
#define JBM_TEMP           __MTYP(D|U|R|L,   X2, 2)    // Sensor Anzeige # aktiver
#define JBM_TEJBM_TYP       __MTYP(  U    ,   X2, 3)    // Temp Typ (IR/NTC)
#endif

// Altimeter
// ---------
#if _SENS_ALT_ == 1
#define X3  XP_ALT
#define JBM_ALT_A          __MTYP(D|U|R|L,   X3, 2)    // Sensor aktivieren
#define JBM_ALT_TOT_A      __MTYP(D|U    ,   X3, 3)    // Gesamt Höhe
#if _SENS_VARIO_ == 1
#define JBM_ALT_VARIO_A    __MTYP(D|U    ,   X3, 4)    // Vario
#define JBM_ALT_VARIO_SET  __MTYP(  U    ,   X3, 5)    // Vario Filter und Totzone
#endif
#endif

// GPS
// ---
#if _SENS_GPS_ == 1
#define X4  XP_GPS
#define JBM_GPS            __MTYP(D|U|R|L,   X4, 2)    // Sensor Anzeige # aktiver
#define JBM_GPS_POS_A      __MTYP(D|U    ,   X4, 3)    // Position, Latitude+Longitude aktivieren
#define JBM_GPS_ALT_A      __MTYP(D|U    ,   X4, 4)    // Höhe aktivieren
#define JBM_GPS_SPEED_A    __MTYP(D|U    ,   X4, 5)    // Speed+Speed max. aktivieren
#define JBM_GPS_DIST_TO_A  __MTYP(D|U    ,   X4, 6)    // Entfernung + Richtung zum Modell aktivieren
#define JBM_GPS_TRIP_A     __MTYP(D|U    ,   X4, 7)    // zurückgelegte Strecke und Gesamstrecke aktivieren
#define JBM_GPS_SATS_A     __MTYP(  U    ,   X4, 8)    // # Sateliten
#endif


// Pitot Rohr, Geschwindigkeit
// ---------------------------
#if _SENS_PITOT_ == 1
#define X5  XP_PITOT
#define JBM_PITOT          __MTYP(D|U|R|L,   X5, 2)    // Pitot Speed
#define JBM_PITOT_PORT     __MTYP(  U    ,   X5, 3)    // Port
#endif

// Spannung
// --------
#if _SENS_VOLT_ == 1
#define X6  XP_VOLT
#define JBM_VOLT           __MTYP(D|U|R|L,   X6, 2)    // Sensor Anzeige # aktiver
#define JBM_VOLT_PORT      __MTYP(D|U    ,   X6, 3)    // Spg. Messung, Port
#define JBM_VOLT_RGND      __MTYP(D|U    ,   X6, 4)    // Spg. Messung, Widerstand gegen GND
#define JBM_VOLT_RVCC      __MTYP(  U    ,   X6, 5)    // Spg. Messung, Widerstand gegen Vcc
#endif


// Stromsensor, Strom/Spannung/Watt
// --------------------------------
#if _SENS_MUI_ == 1
#define X7  XP_MUI
#define JBM_MUI            __MTYP(D|U|R|L,   X7, 2)    // Sensor Anzeige # aktiver
#define JBM_MUI_TYP        __MTYP(D|U    ,   X7, 3)    // Stromsensor, Typ Auswahl
#define JBM_MUI_PORT_A     __MTYP(D|U    ,   X7, 4)    // Stromsensor, Port für Strom
#define JBM_MUI_CAPA_RES   __MTYP(D|U    ,   X7, 5)    // Kapazitäts Reset, man/auto
#define JBM_MUI_PORT_V     __MTYP(D|U    ,   X7, 6)    // Stromsensor, Port für Spannung
#define JBM_MUI_RGND       __MTYP(D|U    ,   X7, 7)    // Spg. Messung, Widerstand gegen GND
#define JBM_MUI_RVCC       __MTYP(  U    ,   X7, 8)   // Spg. Messung, Widerstand gegen Vcc
#endif


// Beschleunigungsmesser
// ---------------------
#if _SENS_ACC_ == 1
#define X8  XP_ACC
#define JBM_ACC            __MTYP(D|U|R|L,   X8, 2)    // Acceleration
#define JBM_ACC_GFORCE     __MTYP(D|U    ,   X8, 3)    // G-Force X/Y/Z Achse
#define JBM_ACC_CAL        __MTYP(  U    ,   X8, 4)    // G-Force X/Y/Z Achse Kalibrieren
#endif



// ===============
// Reset Sumvalues
// ===============
enum
  {
  YRES_START = 1,
  #if _SENS_FUEL_ == 1
  YRES_FVOL,
  #endif
  #if _SENS_MUI_ == 1
  YRES_CAPA,
  #endif
  #if _SENS_GPS_ == 1
  YRES_SPEED,
  YRES_DIST,
  #endif
  #if _SENS_RPM_ == 1
  YRES_RTIM,
  YRES_REVOL,
  #endif
  #if _SENS_FUEL_ == 1
  YRES_FUEL,
  #endif
  };

#if _SENS_FUEL_ == 1
#define JBM_RESVAL_FVOL    __MTYP(D|U    , 0xf, YRES_FVOL)   // verbrauchte Tankmenge zurücksetzen
#endif
#if _SENS_MUI_ == 1
#define JBM_RESVAL_CAPA    __MTYP(D|U    , 0xf, YRES_CAPA)   // MUI1-3, verbrauchte Kapazität zurücksetzen
#endif
#if _SENS_GPS_ == 1
#define JBM_RESVAL_SPEED   __MTYP(D|U    , 0xf, YRES_SPEED)  // max. Speed zurücksetzen
#define JBM_RESVAL_DIST    __MTYP(D|U    , 0xf, YRES_DIST)   // Gesamt Distanz zurücksetzen
#endif
#if _SENS_RPM_ == 1
#define JBM_RESVAL_RTIM    __MTYP(D|U    , 0xf, YRES_RTIM)   // Laufzeit zurücksetzen
#define JBM_RESVAL_REVOL   __MTYP(D|U    , 0xf, YRES_REVOL)  // Gesamt Umdrehungen zurücksetzen
#endif
#if _SENS_FUEL_ == 1
#define JBM_RESVAL_FUEL    __MTYP(D|U    , 0xf, YRES_FUEL)   // Gesamt Verbrauch zurücksetzen
#endif


// =====
// Setup
// =====
#define JBM_SETUP_SENS_NB  __MTYP(D|U    , 0xe, 0x2)   // Sensor Index
#define JBM_SETUP_WDOG     __MTYP(D|U    , 0xe, 0x3)   // Watchdog aktivieren
#define JBM_SETUP_AVCC     __MTYP(D|U    , 0xe, 0x4)   // ADC Referenzspg. in mV
#if _SENS_MUI_ == 1
#define JBM_SETUP_MUI_CAL  __MTYP(D|U    , 0xe, 0x5)   // MUI Strom kalibrieren
#define YSUP  5
#else
#define YSUP  4
#endif
#define JBM_SETUP_FAC      __MTYP(D|U    , 0xe, YSUP+1)   // auf default Werte setzten
#define JBM_SETUP_INFO     __MTYP(D|U    , 0xe, YSUP+2)   // Info
#define JBM_SETUP_DEBUG    __MTYP(  U    , 0xe, YSUP+3)

#if 0
#define JETIBOX_MENU_LIST {\
  //                     12345678901234
  { JBM_MAIN,			"              " },\
  { JBM_SENSCFG,       	":Sensor config" },\
  { JBM_RESVAL,         ":Reset Values " },\
  { JBM_SETUP,          ":Setup/Info   " }\
  }


  #if _SENS_RPM_ == 1
  { JBM_RPM_A,           ":Sensor RPM <>" },
  { JBM_RPM_RELCNT,      ".Release cnt >" },
  { JBM_RPM_ACCURACY,    ".Accuracy    >" },
  #endif

  #if _SENS_FUEL_ == 1
  { JBM_FUEL_A,          ":Sensor Fuel<>" },
  { JBM_FUEL_VOL,        ".Fuel vol. <->" },
  { JBM_FUEL_RES,        ".Volume reset>" },
  { JBM_FUEL_CNT,        ".pulse/L   <->" },
  { JBM_FUEL_CAL,        ".cal pulse/L<>" },
  #endif

  #if _SENS_TEJBM_ == 1
  { JBM_TEMP,            ":Sensor Temp  " },
  { JBM_TEJBM_TYP,        ".Temp Typ  <->" },
  #endif

  #if _SENS_ALT_ == 1
  { JBM_ALT_A,           ":Sensor Alt <>" },
  { JBM_ALT_TOT_A,       ".Sens AltTot<>" },
  #if _SENS_VARIO_ == 1
  { JBM_ALT_VARIO_A,     ".Sens Vario <>" },
  { JBM_ALT_VARIO_SET,   ".Filt   Deadz." },
  #endif
  #endif

  #if _SENS_GPS_ == 1
  { JBM_GPS,             ":Sensor GPS   " },
  { JBM_GPS_POS_A,       ".Sens Pos   <>" },
  { JBM_GPS_ALT_A,       ".Sens Alt   <>" },
  { JBM_GPS_SPEED_A,     ".Sens Speed <>" },
  { JBM_GPS_DIST_TO_A,   ".Sens DistTo<>" },
  { JBM_GPS_TRIP_A,      ".Sens Trip  <>" },
  { JBM_GPS_SATS_A,      ".Sens Sats  <>" },
  #endif

  #if _SENS_PITOT_ == 1
  { JBM_PITOT,           ":Sens Pitot   " },
  { JBM_PITOT_PORT,      ".Port      <->" },
  #endif

  #if _SENS_VOLT_ == 1
  { JBM_VOLT,            ":Sens Voltage " },
  { JBM_VOLT_PORT,       ".Port      <->" },
  { JBM_VOLT_RGND,       ".Resistor  <->" },
  { JBM_VOLT_RVCC,       ".Resistor  <->" },
  #endif

  #if _SENS_MUI_ == 1
  { JBM_MUI,             ":Sens MUI tot " },
  { JBM_MUI_TYP,         ".Typ       <->" },
  { JBM_MUI_PORT_A,      ".Port curr <->" },
  { JBM_MUI_CAPA_RES,    ".capa reset<->" },
  { JBM_MUI_PORT_V,      ".Port volt <->" },
  { JBM_MUI_RGND,        ".Resistor  <->" },
  { JBM_MUI_RVCC,        ".Resistor  <->" },
  #endif

  #if _SENS_ACC_ == 1
  { JBM_ACC,             ":Sensor Accel " },
  { JBM_ACC_GFORCE,      ".Sens GForce<>" },
  { JBM_ACC_CAL,         ".calib GForc<>" },
  #endif



  // Reset SumValues
  #if _SENS_FUEL_ == 1
  { JBM_RESVAL_FVOL,     ".Fuel volume<>" },
  #endif
  #if _SENS_MUI_ == 1
  { JBM_RESVAL_CAPA,     ".Capacity   <>" },
  #endif
  #if _SENS_GPS_ == 1
  { JBM_RESVAL_SPEED,    ".Speed max  <>" },
  { JBM_RESVAL_DIST,     ".Dist total <>" },
  #endif
  #if _SENS_RPM_ == 1
  { JBM_RESVAL_RTIM,     ".RunTime    <>" },
  { JBM_RESVAL_REVOL,    ".Revolution <>" },
  #endif
  #if _SENS_FUEL_ == 1
  { JBM_RESVAL_FUEL,     ".Fuel total <>" },
  #endif


  // Setup
  { JBM_SETUP_SENS_NB,   ".Sensor Nb   >" },
  { JBM_SETUP_WDOG,      ".Watchdog   <>" },
  { JBM_SETUP_AVCC,      ".ADC RefVcc<->" },
  #if _SENS_MUI_ == 1
  { JBM_SETUP_MUI_CAL,   ".MUI calib <->" },
  #endif
  { JBM_SETUP_FAC,       ".factory def<>" },
  { JBM_SETUP_INFO,      ".(c)Th.Lehmann" },
  { JBM_SETUP_DEBUG,     ".Debug        " },

  };
#endif
