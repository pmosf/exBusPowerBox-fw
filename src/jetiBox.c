#include <stdbool.h>
#include <ch.h>
#include "jetibox.h"
#include "crc.h"

static bool init_done = false;

void jetibox_init(void)
{
	chDbgCheck(init_done != true);

	init_done = true;
}

void jetiboxProcessKey(uint8_t key, uint8_t *text)
{
	static uint8_t posX = 0, posY = 0, menuIdx = 0;

	switch (key) {
	case JETIBOX_KEY_UP:
		break;
	case JETIBOX_KEY_DOWN:
		break;
	case JETIBOX_KEY_LEFT:
		break;
	case JETIBOX_KEY_RIGHT:
		break;
	case JETIBOX_KEY_LR:
	case JETIBOX_KEY_UD:
	case JETIBOX_KEY_REFRESH:
		break;
	default:
		return;
	}
}

#if 0

// ===========
// Auswahl +/-
// ===========
uint8_t _JBIdxPlusMinKey( uint8_t Key, uint8_t Index, uint8_t IdxMax, bool Save )
{
	uint8_t LIdx = Index;

	if ( (Key == JBKEY_RIGHT && ++Index >= IdxMax) ||
	     (Key == JBKEY_LEFT  && Index > 0) )
		Index--;
	if ( Save && Index != LIdx )
		EEpromSave = true;
	return ( Index );
}

void HandleJB( bool Refresh )
{
	static bool FuelCalActiv;
	static uint8_t PosX, PosY, MenuIdx;        // Index in Menu Tabelle, aktueller Eintrag
	static uint8_t LastPosX, LastPosY;
	union U_MVAL MVal;                         // Pos und zugelassene Richtungen, Kopie aus Flash
	uint8_t Key, Idx;

	static uint8_t Index;
	static bool Dir;

	char    JB_Buff[17];                       // JB Ausgabe, Zeile 1/2
#define  JBPOS_TXT  8                     // kleiner Text (5Ch 8-12)
#define  JBPOS_CHK  15                    // Pos Sensortestausgabe in Zeile 2
#define  JBPOS_INIT 14                    // Pos Init Fehler in Zeile 2
#define  JBPOS_FAIL 15                    // Pos Ausfall in Zeile 2

	SensorChk.Id = -1;                         // Sensor Funktionstest ggf. ausschalten

	if ( Refresh )
		Key = JBKEY_REFRESH;                     // Aufruf um Anzeige zu aktualisieren
	else
#if HW_TERM == 1 || SOFT_TERM == 1
		Key = ConGetKey();                       // liefert 0 wenn nichts gedrückt
#else
		// JB Empfang, Sender liefert zu schnell Tastenwiederholungen wenn schon etwas zu lange gedrückt
		if ( (Key = JetiEx.GetJetiboxKey()) != 0 ) { // liefert 0 wenn nichts gedrückt
			static uint32_t LastKeyMS;
			if ( millis() < LastKeyMS )
				return;
			LastKeyMS = millis() + 300;
		}
#endif
	if ( Key == 0 )
		return;



	// akt. Eintrag für Zugriff aus Flash ins RAM kopieren
	memcpy_P( &MVal, &Menu[MenuIdx].u, sizeof(MVal) );

	switch ( Key ) {
	case JBKEY_LEFT:                // links
		if ( MVal.s1.KeyLeft ) {
			if ( PosY == 1 )            // Hauptmenu, Richtung tauschen
				PosX++;
			else
				PosX--;
		}
		break;
	case JBKEY_RIGHT:               // rechts
		if ( MVal.s1.KeyRight ) {
			if ( PosY == 1 )            // Hauptmenu, Richtung tauschen
				PosX--;
			else if ( PosX < XP_LAST - 1 ) // Ende noch nicht erreicht ?
				PosX++;
		}
		break;
	case JBKEY_UP:                  // hoch
		if ( MVal.s1.KeyUp ) {
			PosY--;
			// bei Übergang in MAIN oder Übergang aus irgendeinem SETUP Submenu
			if ( __MPOSY(JBM_MAIN) == PosY || (__MPOSY(JBM_SENSCFG) == PosY && PosX < __MPOSX(JBM_HM_LAST)) )
				PosX = 0;
		}
		break;
	case JBKEY_DOWN:                // runter
		if ( MVal.s1.KeyDown )
			PosY++;
		break;

	case JBKEY_LR:                  // links+rechts
	case JBKEY_UD:                  // Hoch+Runter
	case JBKEY_REFRESH:
		break;
	default:
		return;
	}
	PosX &= 0xf;  // bei Wechsel von Setup auf Service

	Idx = ARRAYCNT(Menu);
	uint8_t MPos = (PosX << 4) | PosY;
	while ( MPos != (MVal.Val & 0xff) ) {             // Positionsänderung, neuen Menueintrag suchen
		FuelCalActiv =
		    false;                           // FuelFlow Kalibrierung ohne speichern beenden, Cal-Menue wurde verlassen

		MenuIdx++;
		if ( MenuIdx >= ARRAYCNT(Menu) )
			MenuIdx = 0;
		memcpy_P( &MVal, &Menu[MenuIdx].u, sizeof(MVal) );  // aus Flash ins RAM kopieren
		if ( Idx-- == 0 ) {
			TermPut("MPosXY:");
			TermPut(MPos, HEX);
			TermPut(" nicht gefunden\n");
			PosX = LastPosX;
			PosY = LastPosY;
			return;
		}
	}
	LastPosX = PosX;
	LastPosY = PosY;

#if HW_TERM == 1 || SOFT_TERM == 1
	JB_Buff[sizeof(JB_Buff) - 1] = 0;                 // Strg Ende nur für TermPut() notwendig
#endif
	// Menutext Zeile 1
	if ( MVal.Val == JBM_MAIN ) {                      // Eintritt bzw. Konfig geändert, speichern/verwerfen
		if ( EEpromSave ) {                             // bei Austritt fragen ob Änderungen gespeichert werden sollen
			const static char Strg[] PROGMEM = "config changed";
			strcpy_P( JB_Buff, Strg );
		} else
			strcpy_P( JB_Buff, JB_TEXT_Z1 );
	} else {
		sprintf(JB_Buff, "%02X", MPos);
		memcpy_P( &JB_Buff[2], &Menu[MenuIdx].Zeile1, sizeof(Menu[MenuIdx].Zeile1) );
	}
	JetiEx.SetJetiboxText( JetiExProtocol::LINE1, JB_Buff );
	memset( JB_Buff, ' ', sizeof(JB_Buff) - 1 );


	// ================
	// Auswahl bedienen
	// ================
	int8_t  IdActiv = -1;
	bool    ErrFail = false;
	const char *Txt = "";
	char  TxtBuff[10];
	uint8_t PosJB = 0;
	uint32_t Val = 0;

	const static char StrgExitSave[] PROGMEM = "\x12 exit / save <>";
	switch ( MVal.Val ) {
	case JBM_MAIN:                                   // Eintritt bzw. Konfig geändert, speichern/verwerfen
		strcpy_P( JB_Buff, JB_TEXT_Z2 );
		if ( EEpromSave ) {                           // bei Austritt fragen ob Änderungen gespeichert werden sollen
			strcpy_P( JB_Buff, StrgExitSave );
			if ( Key == JBKEY_LR )                      // Rechts+Links > Änderungen speichern
				EEpromRW( true, false );
			else if ( Key != JBKEY_UD )                 // Hoch+Runter > Änderungen verwerfen / Reset
				break;
			SoftReset();                                // Neustart mit geänderter oder alter Konfig
		}
		break;



	// =====
	// Setup
	// =====
	case JBM_SETUP:
		Index = Dir = 0;
		break;
	case JBM_SETUP_DEBUG: {
		const static char Strg[] PROGMEM = "free RAM:%u";
		sprintf_P(JB_Buff, Strg, freeRam());
		break;
	}

	case JBM_SETUP_INFO: {
		// 1234567890123456
		const static char Strg[] PROGMEM = " thomas@thls.de";
		strcpy_P(JB_Buff, Strg);
		break;
	}

	case JBM_SETUP_SENS_NB:        // Sensor Nummer festlegen, 0 = ohne, 1-9
		if ( Key == JBKEY_RIGHT ) { // Änderung ?
			GData.EEprom.Data.SensNb++;
			EEpromSave = true;
		}
		// 1234567890123456
		sprintf(JB_Buff, "#%d", GData.EEprom.Data.SensNb);
		break;

	case JBM_SETUP_WDOG:
		if ( Key == JBKEY_LR ) {   // Änderung ?
			GData.EEprom.Data.WDogActiv ^= true;
			EEpromSave = true;
		}
		sprintf( JB_Buff, "%sactiv", GData.EEprom.Data.WDogActiv ? "" : "de");
		break;

	case JBM_SETUP_AVCC:           // ADC Referenzspg. in mV
		_JBCfgValue( Key, &GData.EEprom.Data.AVcc, 5 );
		// 1234567890123456
		sprintf(JB_Buff, "%umV", GData.EEprom.Data.AVcc);
		break;

#if _SENS_MUI_ == 1
	case JBM_SETUP_MUI_CAL:        // MUI Strom kalibrieren
#define CFG_MUI   GData.EEprom.Data.CfgMuiCalib[Index]
	{
		// Dir == 0 > MUI 1-n einstellen
		// Dir == 1 > Kalibrierwert einstellen
		// Key == LR> zwischen 0 und 1 für Dir wechseln
		int Pos = _JBDirIdx(&Dir, Key, JB_Buff, "MUI", &Index, CNT_SENS_MUI, false, &CFG_MUI, 100, true );
		if ( SensPwrMui[Index] )
			sprintf( &JB_Buff[Pos], "%umA", SensPwrMui[Index]->GetCurrent());
		break;
	}
#undef CFG_MUI
#endif


	case JBM_SETUP_FAC:
		if ( Key == JBKEY_LR ) {
			strcpy_P( JB_Buff, StrgExitSave );
			GData.EEprom.Data.Magic = 0xffff;
			EEpromRW( true, false );
			SoftReset();
		}
		break;


	// ===============
	// Reset Sumvalues
	// ===============
	case JBM_RESVAL:
		Index = 0;
		break;

		uint32_t *NV_Val;
#if _SENS_FUEL_ == 1
	case JBM_RESVAL_FVOL:                 // verbrauchte Tankmenge zurücksetzen
		if ( SensFuel ) {
			if ( Key == JBKEY_LR )
				SensFuel->ResetFlowCntTank();
			NV_Val = &GData.EEprom.NV.FuelFlowCntTank;
			Val = SensFuel->TVal.VolumeRemain;
			Txt = "ml";
			goto RESET_NV_VAL;
		}
		break;
#endif
#if _SENS_MUI_ == 1
	case JBM_RESVAL_CAPA:               // MUI1-3 verbrauchte Kapazität zurücksetzen
		// MUI 1-n einstellen
		Index = _JBIdxPlusMinKey( Key, Index, CNT_SENS_MUI, false );
		sprintf( JB_Buff, "MUI%d ", Index + 1);
		PosJB = 5;
		NV_Val = (uint32_t *)&GData.EEprom.NV.MuiCapaConsumed[Index];
		Val = GData.EEprom.NV.MuiCapaConsumed[Index];
		Txt = "mAh";
		goto RESET_NV_VAL;
#endif
#if _SENS_GPS_ == 1
	case JBM_RESVAL_SPEED:              // max. Speed zurücksetzen
		NV_Val = &GData.EEprom.NV.GpsSpeedMax;
		goto RESET_NV_VAL;
	case JBM_RESVAL_DIST:               // Gesamt Distanz zurücksetzen
		NV_Val = &GData.EEprom.NV.GpsDistTot;
		goto RESET_NV_VAL;
#endif
#if _SENS_RPM_ == 1
	case JBM_RESVAL_RTIM:               // Laufzeit zurücksetzen
		NV_Val = &GData.EEprom.NV.RpmRunTimeS;
		goto RESET_NV_VAL;
	case JBM_RESVAL_REVOL:              // Gesamt Umdrehungen zurücksetzen
		NV_Val = &GData.EEprom.NV.RpmRevolTot;
		goto RESET_NV_VAL;
#endif
#if _SENS_FUEL_ == 1
	case JBM_RESVAL_FUEL:               // Gesamt Verbrauch zurücksetzen
		NV_Val = &GData.EEprom.NV.FuelFlowCntTot;
		goto RESET_NV_VAL;
#endif
RESET_NV_VAL:
		if ( Key == JBKEY_LR ) {
			*NV_Val = 0L;
			EEpromRW( true, false );
			GData.Signal |= SIG_JB_REFRESH;
			break;
		}
		if ( Val == 0 )
			Val = *NV_Val;
		// 1234567890123456
		sprintf( &JB_Buff[PosJB], "%lu%s", Val, Txt);
		break;



	// =============
	// Sensor Konfig
	// =============
	case JBM_SENSCFG:
		break;

		// Drehzahl
		// --------
#if _SENS_RPM_ == 1
	case JBM_RPM_A:                                  // Sensor aktivieren
		IdActiv = ID_RPM;
		if ( Key == JBKEY_LR ) {                      // Änderung ?
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_RPM_REVOL );
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_RPM_RUNTIME );
		}
		SensorChk.Id = ID_RPM;                        // Sensorcheck aktiv, Änderungen anzeigen
		break;
	case JBM_RPM_RELCNT: {                           // # Auslösungen je Umdrehung
		if ( Key == JBKEY_RIGHT ) {
			GData.EEprom.Data.CfgRpmCntMag ^= 1;
			EEpromSave = true;
		}
		// 1234567890123456
		const static char Strg[] PROGMEM = "%u / revolution";
		sprintf_P( JB_Buff, Strg, GData.EEprom.Data.CfgRpmCntMag + 1);
		break;
	}

	case JBM_RPM_ACCURACY:                           // RPM Anzeige, Genauigkeit n UPM
		_JBCfgValue( Key, (uint16_t *)&GData.EEprom.Data.CfgRpmAccuracy, 10 );
		if ( GData.EEprom.Data.CfgRpmAccuracy > RPM_ACCURACY_MAX )
			GData.EEprom.Data.CfgRpmAccuracy = RPM_ACCURACY_MAX;
		// 1234567890123456
		sprintf( JB_Buff, "%u RPM/min", GData.EEprom.Data.CfgRpmAccuracy ? GData.EEprom.Data.CfgRpmAccuracy : 1);
		break;
#endif



		// Fuel
		// ----
#if _SENS_FUEL_ == 1
	case JBM_FUEL_A:                                 // Sensor aktivieren
		IdActiv = ID_FUEL_REMAIN;
		if ( Key == JBKEY_LR ) {                      // Änderung ?
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_FUEL_CONSUMED  );
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_FUEL_FLOW  );
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_FUEL_TOT );
		}
		SensorChk.Id = ID_FUEL_FLOW;                  // Sensorcheck aktiv, Änderungen anzeigen
		break;
	case JBM_FUEL_VOL:                               // Tankgröße
		_JBCfgValue( Key, &GData.EEprom.Data.CfgFuelVolume, 50 );
		// 1234567890123456
		sprintf( JB_Buff, "%umL", GData.EEprom.Data.CfgFuelVolume);
		break;
	case JBM_FUEL_RES:                               // Volume Reset, man/auto
#define CFG_FUEL GData.EEprom.Data.CfgFuelVolRes
		CFG_FUEL = _JBGetResFct( Key, JB_Buff, 1, CFG_FUEL );
		break;
#undef CFG_FUEL


	case JBM_FUEL_CNT:                               // # Impulse je Liter
		_JBCfgValue( Key, &GData.EEprom.Data.CfgFuelFlowCnt1L, 10 );

		if ( Key == JBKEY_LR ) {                      // Reset auf default # Pulse
			GData.EEprom.Data.CfgFuelFlowCnt1L = FUEL_FLOWCNT_1L;
			EEpromSave = true;
		}
		// 1234567890123456
		sprintf( JB_Buff, "%5u/L    def<>", GData.EEprom.Data.CfgFuelFlowCnt1L);
		break;

	case JBM_FUEL_CAL:                               // # Impulse je Liter kalibrieren
		if ( Key == JBKEY_LR )                        // Start/Stop
			FuelCalActiv ^= true;
		// zyklischer Aufruf von SensorChk bei Änderungen
		if ( FuelCalActiv ) {
			SensorChk.Id = ID_FUEL_FLOW;  // Sensorcheck aktiv, Änderungen summieren
			if ( SensorChk.MessureVal ) {
				// 1234567890123456
				const static char Strg[] PROGMEM = "pulse cnt %5u";
				sprintf_P( JB_Buff, Strg, SensorChk.MessureVal);
			} else {
				// 1234567890123456
				const static char Strg[] PROGMEM = "start flow 1L";
				sprintf_P( JB_Buff, Strg);
			}
		} else if ( SensorChk.MessureVal > 1000L ) { // Ende der Messung, speichern
			GData.EEprom.Data.CfgFuelFlowCnt1L = SensorChk.MessureVal;
			EEpromSave = true;
			// 1234567890123456
			const static char Strg[] PROGMEM = "pulse cnt saved";
			sprintf_P( JB_Buff, Strg);
		}
		break;
#endif

		// Temperatur
		// ----------
#if _SENS_TEJBM_ == 1
	case JBM_TEMP:                                 // Sensor Anzeige # aktiver
		_JBPutSensActivCnt( &JB_Buff[3], ID_TEMP1, ID_TEMP1 + CNT_SENS_TEMP - 1, false );
		Index = Dir = 0;
		break;
	case JBM_TEJBM_TYP:                             // Temp Typ (IR/NTC)
#define CFG_TEMP GData.EEprom.Data.CfgSensTypTemp[Index]
	{
		const char *TypTxt[] = STYP_TEJBM_TXT;
		uint8_t CntSTyp = CNT_STYP_TEMP;
		if ( Index >= CNT_SENS_TEJBM_NTC )
			CntSTyp--;                                // nur 4 NTC aber 7 IR
		int Pos = _JBDirIdx(&Dir, Key, JB_Buff, "TEMP", &Index, CNT_SENS_TEMP, false, &CFG_TEMP, CntSTyp, true );
		sprintf(&JB_Buff[Pos], "%s", TypTxt[CFG_TEMP]);

		IdActiv = (ID_TEMP1 + Index);
		if ( CFG_TEMP == STYP_TEJBM_NONE )
			PBIT_CLR( GData.EEprom.Data.SensActivMsk, IdActiv );
		else
			PBIT_SET( GData.EEprom.Data.SensActivMsk, IdActiv );
		IdActiv |= 0x80;                             // ohne "activ/deactiv" Anzeige

		if ( SensTemp[Index] )
			ErrFail = SensTemp[Index]->Failure;
		break;
	}
#undef CFG_TEMP
#endif



		// Altimeter
		// ---------
#if _SENS_ALT_ == 1
	case JBM_ALT_A:                                  // Sensor aktivieren
		IdActiv = ID_ALT_REL;
		if ( SensAlt )
			ErrFail = SensAlt->Failure;
		break;
	case JBM_ALT_TOT_A:                              // Gesamt Höhe
		IdActiv = ID_ALT_TOT;
		break;
#if _SENS_VARIO_ == 1
	case JBM_ALT_VARIO_A:                            // Vario
		IdActiv = ID_ALT_VARIO;
		Dir = 0;
		break;
	case JBM_ALT_VARIO_SET:                          // Vario Filter und Totzone
#define CFG_FILT   GData.EEprom.Data.CfgVarioSmoothing
#define CFG_DZ     GData.EEprom.Data.CfgVarioDeadzone
	{
		// Dir == 0 > Smoothing Filter 0-100% einstellen
		// Dir == 1 > Totzone 0- einstellen
		// Key == LR> zwischen 0 und 1 für Dir wechseln
		CFG_FILT--;                                   // Anzeige erfolgt mit +1
		int Pos = 3;
		Pos += _JBDirIdx(&Dir, Key, &JB_Buff[Pos], "% ", &CFG_FILT, 99, true, &CFG_DZ, 100, true );
		sprintf( &JB_Buff[Pos], "0.%02u m/s", CFG_DZ);
		CFG_FILT++;
		break;
	}
#undef CFG_FILT
#undef CFG_DZ
	break;
#endif
#endif



		// Pitot Rohr, Speedmessung
		// ------------------------
#if _SENS_PITOT_ == 1
	case JBM_PITOT:                                  // Speed Messung
		IdActiv = ID_PITOT_SPEED;
		Key = 0;
		break;
	case JBM_PITOT_PORT:                             // Port Auswahl
#define PA  GData.EEprom.Data.CfgAnaFct[ANAFCT_PITOT]
		_JBPortAnaChoice( JB_Buff, ID_PITOT_SPEED, &PA, Key );
		EEpromSave = true;
		break;
#undef PA
#endif


		// Spannung
		// --------
#if _SENS_VOLT_ == 1
	case JBM_VOLT:                                 // Sensor Anzeige # aktiver
		_JBPutSensActivCnt( &JB_Buff[3], ID_PWR_VOLT1, ID_PWR_VOLT1 + CNT_SENS_VOLT - 1, false );
		Index = Dir = 0;
		break;

	case JBM_VOLT_PORT:                            // Spg. Messung, Port
#define PA  GData.EEprom.Data.CfgAnaFct[ANAFCT_VOLT1 + Index]
	{
		int Pos = _JBDirIdx(&Dir, Key, JB_Buff, "VOLT", &Index, CNT_SENS_VOLT, false, 0, 0, -1 );
		if ( Dir == 0 )                             // Port Auswahl selektiert
			Key = 0;                                  // nur Refresh Analog Port Anzeige
		_JBPortAnaChoice( &JB_Buff[Pos], ID_PWR_VOLT1 + Index, &PA, Key );
		break;
	}
#undef PA

	case JBM_VOLT_RGND:                            // Spg. Messung, Widerstand gegen GND
	case JBM_VOLT_RVCC:                            // Spg. Messung, Widerstand gegen Vcc
#define RGND  GData.EEprom.Data.CfgVoltRGnd[Index]
#define RVCC  GData.EEprom.Data.CfgVoltRVcc[Index]
		Idx = MVal.Val - JBM_VOLT_RGND;
		_JBCfgResistor( &JB_Buff[0], Idx, &RGND, &RVCC, Key );
		break;
#undef RGND
#undef RVCC
#endif



		// Stromsensor ACSxx
		// -----------------
#if _SENS_MUI_ == 1
	case JBM_MUI: {                                // Sensor Anzeige # aktiver
		int Pos = 3;
		Pos += _JBPutSensActivCnt( &JB_Buff[Pos], ID_PWR_MUI1_A, ID_PWR_MUI1_A + CNT_SENS_MUI - 1, false );
		JB_Buff[Pos] = '/';
		_JBPutSensActivCnt( &JB_Buff[Pos + 1], _ID_MUI_FIRST, _ID_MUI_LAST, true );
		Index = 0;
		break;
	}

	case JBM_MUI_TYP:                              // Stromsensor Typ
#define CFG_MUI GData.EEprom.Data.CfgMuiSensTyp[Index]
	{
		int Pos = _JBDirIdx(&Dir, Key, JB_Buff, "MUI", &Index, CNT_SENS_MUI, false, 0, 0, -1 );
		if ( Dir == 0 )                             // MUI Index Auswahl selektiert
			Key = 0;                                  // nur Refresh Typ Anzeige
		CFG_MUI = _JBIdxPlusMinKey( Key, CFG_MUI, CNT_STYP_MUI, true );
		if ( CFG_MUI )
			Pos += sprintf_P( &JB_Buff[Pos], AcsComTxt);
		sprintf_P( &JB_Buff[Pos], Acs[CFG_MUI].Text);
		if ( CFG_MUI == 0 )                         // Sensor Text ist zu lang, 'I' stört dann
			IdActiv = ID_PWR_MUI1_A + Index | 0x80;   // Init Fehler anzeigen aber ohne "activ/deactiv" Anzeige
		break;
	}
	case JBM_MUI_PORT_A:                           // vewendeter AnalogPort für Ampere
#define PA  GData.EEprom.Data.CfgAnaFct[ANAFCT_MUI1_A + Index]
	{
		if ( CFG_MUI == STYP_MUI_ACS_NONE ) {       // kein Sensor Typ gewählt ?
			PosY--;
			GData.Signal |= SIG_JB_REFRESH;
			break;
		}
		// wenn Ampere aktiv können wir auch Kapazität berechnen
		if ( _JBPortAnaChoice(JB_Buff, (ID_PWR_MUI1_A + Index), &PA, Key) )
			PBIT_SET( GData.EEprom.Data.SensActivMsk, ID_PWR_MUI1_C + Index );
		else
			PBIT_CLR( GData.EEprom.Data.SensActivMsk, ID_PWR_MUI1_C + Index );
		break;
	}
#undef PA
#undef CFG_MUI

	case JBM_MUI_CAPA_RES:                         // Stromsensor, Kapazitäts Reset
#define CFG_MUI GData.EEprom.Data.CfgMuiCapaRes[Index]
		CFG_MUI = _JBGetResFct( Key, JB_Buff, 5, CFG_MUI ); // auto + 1-4%
		if ( CFG_MUI > RESFCT_AUTO )                // Auto = immer, oder Auto mit 1-4% höherer Spg.
			sprintf(&JB_Buff[RESFCT_POSAUTO], "%d%%", CFG_MUI - RESFCT_AUTO);
		break;
#undef CFG_MUI

	case JBM_MUI_PORT_V:                           // vewendeter AnalogPort für Spannung
#define PA  GData.EEprom.Data.CfgAnaFct[ANAFCT_MUI1_V + Index]
		// Ampere und Spannung > dann auch Watt
		if ( _JBPortAnaChoice(JB_Buff, (ID_PWR_MUI1_V + Index), &PA, Key) )
			PBIT_SET( GData.EEprom.Data.SensActivMsk, ID_PWR_MUI1_W + Index );
		else
			PBIT_CLR( GData.EEprom.Data.SensActivMsk, ID_PWR_MUI1_W + Index );
		break;
#undef PA
	case JBM_MUI_RGND:                            // Spg. Messung, Widerstand gegen GND
	case JBM_MUI_RVCC:                            // Spg. Messung, Widerstand gegen Vcc
#define RGND  GData.EEprom.Data.CfgMuiRGnd[Index]
#define RVCC  GData.EEprom.Data.CfgMuiRVcc[Index]
		Idx = MVal.Val - JBM_MUI_RGND;
		_JBCfgResistor( &JB_Buff[0], Idx, &RGND, &RVCC, Key );
		break;
#undef RGND
#undef RVCC
#endif




#if _SENS_GPS_ == 1
	// GPS
	// ---
	case JBM_GPS:                                    // Sensor Anzeige # aktiver
		_JBPutSensActivCnt( &JB_Buff[3], _ID_GPS_FIRST, _ID_GPS_LAST, false );
		if ( SensGps )
			ErrFail = SensGps->Failure;
		break;

	case JBM_GPS_POS_A:           // Position Latitude+Longitude aktivieren
		IdActiv = ID_GPS_LAT;
		if ( Key == JBKEY_LR ) {   // Änderung ?
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_GPS_LON );
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_GPS_HEAD );
		}
		break;
	case JBM_GPS_ALT_A:           // Höhe aktivieren
		IdActiv = ID_GPS_ALT;
		break;
	case JBM_GPS_SPEED_A:         // Speed+max. aktivieren
		IdActiv = ID_GPS_SPEED;
		if ( Key == JBKEY_LR )     // Änderung ?
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_GPS_SPEED_MAX );
		break;
	case JBM_GPS_DIST_TO_A:       // Entfernung aktivieren
		IdActiv = ID_GPS_DIST_TO;
		if ( Key == JBKEY_LR )     // Änderung ?
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_GPS_COURSE_TO );
		break;
	case JBM_GPS_TRIP_A:           // zurückgelegte Strecke aktivieren
		IdActiv = ID_GPS_DIST_TRIP;
		if ( Key == JBKEY_LR )     // Änderung ?
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_GPS_DIST_TOT );
		break;
	case JBM_GPS_SATS_A:          // # Sateliten
		IdActiv = ID_GPS_SATS;
		break;
#endif


#if _SENS_ACC_ == 1
	// Beschleunigungsmesser
	// ---------------------
	case JBM_ACC:                  // Acceleration
		//_JBPutSensActivCnt( JB_Buff, _ID_ACC_FIRST, _ID_ACC_LAST );
		//SensAcc->Failure
		break;
	case JBM_ACC_GFORCE:           // G-Force X/Y/Z Achse
		IdActiv = ID_ACC_GFORCE_X;
		if ( Key == JBKEY_LR ) {    // Änderung ? G-Force immer für alle Achsen
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_ACC_GFORCE_Y );
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, ID_ACC_GFORCE_Z );
		}
		break;
	case JBM_ACC_CAL:             // G-Force X/Y/Z Achse Kalibrieren
		// G-Force nicht aktiv, Calib überspringen
		if ( !PBIT_TST(GData.EEprom.Data.SensActivMsk, ID_ACC_GFORCE_X) ) {
			// 1234567890123456
			const static char Strg[] PROGMEM = "not available";
			sprintf_P( JB_Buff, Strg );
		}
		break;
#endif


	default:
		sprintf(JB_Buff, "?? MVal:%x ??", MVal.Val);
		break;
	}



	// Sensor Akt/Deaktivierung
	// ========================
	if ( IdActiv != -1 ) {
		uint8_t IdA80 = IdActiv & 0x80;   // 0x80 = keine "activ" Ausgabe
		IdActiv &= 0x7f;
		if ( IdA80 )
			Key = 0;

		if ( Key == JBKEY_LR ) {      // Änderung ?
			PBIT_TGL( GData.EEprom.Data.SensActivMsk, IdActiv );
			EEpromSave = true;
		}
		if ( IdA80 == 0 )
			sprintf( JB_Buff, "%sactiv%s", PBIT_TST(GData.EEprom.Data.SensActivMsk, IdActiv) ? "" : "de", Txt);
		if ( SensorChk.Id != -1 )                     // Sensor Funktionstest, Anzeige 0-9 bei jeder Änderung
			JB_Buff[JBPOS_CHK] = (SensorChk.MessureVal % 10) | '0';
		// Sensor bereits aktiviert konnte aber nicht initialisiert werden ?
		if ( PBIT_TST(Tele.ErrInit, IdActiv) )
			JB_Buff[JBPOS_INIT] = 'I';                  // Sensor init Fehler
	}

	if ( ErrFail )                                  // Ausfall zur Laufzeit
		JB_Buff[JBPOS_FAIL] = 'F';

	// alle \0 durch Space ersetzen
	for ( Idx = 0; Idx < sizeof(JB_Buff) - 1; Idx++ )
		if ( JB_Buff[Idx] == 0 )
			JB_Buff[Idx] = ' ';
	JetiEx.SetJetiboxText( JetiExProtocol::LINE2, JB_Buff );


#if HW_TERM == 1
	// Cursor 2 Zeilen hoch
	Serial.write(27);
	Serial.write('M');
	Serial.write(27);
	Serial.write('M');
#endif

//  if( SensTstCh )                                 // Sensor Funktionstest
//    JB_Buff[JBPOS_TEST] = SensTstCh;
}
#endif
