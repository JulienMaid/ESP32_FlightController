// @file trace_debug.h
/// <b>Date de creation:</b> 16/12/2015                                                      <br>
///                                                                                          <br>
/// @brief Implementation d'un systeme de trace pour aider au debugage                       <br>
///                                                                                          <br>
///==========================================================================================<br>
/// <b>Donnees SVN:</b>                                                                      <br>
/// $Rev:: 587                                                                              $<br>
/// $Author:: julien                                                                        $<br>
/// $Date:: 2017-01-23 15:03:25 +0100 (lun., 23 janv. 2017)                                  $<br>
///                                                                                          <br>
///==========================================================================================<br>
/// <b>Historique:</b>                                                                       <br>
///                                                                                          <br>
///                                                                                          <br>
///==========================================================================================<br>
/// <b>Description detaillee:</b> (si necessaire)                                            <br>
///                                                                                          <br>
///                                                                                          <br>
///                                                                                          <br>
///==========================================================================================<br>

#define TRACE_DEBUG_C

#define ACTIVER_TRACES_UDP

//#define DESACTIVER_TRACE_SERIE

//************************************
//* Includes lies a l'implementation *
//************************************

#include "trace_debug.h"

#include "RTC_soft.h"

#include <stdint.h>
#include "bank_text_debug.h"

#include <string.h>
#include <Arduino.h>

#if defined(ACTIVER_TRACES_UDP)
#include <WiFiUdp.h>
#define IP_DEST_UDP "192.168.79.10"
#define PORT_DEST_UDP 1234
#endif

#if defined(DESACTIVER_TRACE_SERIE)
#define Send_String_UARTX(a,b) {}
#else
#define Send_String_UARTX(a,b) Serial.print(a)
#endif

//*********************
//* Variables privees *
//*********************

/// @brief Buffer d'envoi de la trace
uint8_t Buffer_TX_Trace[30];

/// @brief Niveau maximum de trace a remonter
type_trace_t Max_Debug_Level = INFO;

// Libelles des evenements timer
const char Txt_Timer_Event_Init[] = "Init";
const char Txt_Timer_Event_Start[] = "Start";
const char Txt_Timer_Event_Stop[] = "Stop";
const char Txt_Timer_Event_Halt[] = "Halt";
const char Txt_Timer_Event_Top[] = "Top!";
const char Txt_Timer_Event_FinTop[] = "Fin Top!";
const char Txt_Timer_Event_Delete[] = "Deleted";
const char Txt_Timer_Event_SetValue[] = "SetValue";

QueueHandle_t g_pt_queueTraces;

//********************************
//* Implementation des fonctions *
//********************************

void Init_Trace_Debug(void)
{
  Init_RTC_Soft();

  Serial.begin(115200);

  g_pt_queueTraces = xQueueCreate(10, sizeof(std::string*));
}

const char* Get_Text_Type_Trace(type_trace_t Type_Trace)
{
  Txt_Type_Trace_t *p_Table_Type_Trace = &Table_Type_Trace[0];

  // Recherche du texte correpondant au niveau de trace choisi
  while (p_Table_Type_Trace
      < (&Table_Type_Trace[0] + (sizeof(Table_Type_Trace) / sizeof(Txt_Type_Trace_t))))
  {
    if (p_Table_Type_Trace->Type_Trace == Type_Trace)
      return (const char*) p_Table_Type_Trace->p_Text_Data;

    p_Table_Type_Trace++;
  }

  return NULL;
}

uint8_t Send_Trace(type_trace_t Type_Trace, const char Txt_Donnees[], bool Horodatage,
    const char *i_ps8_nomFichier, const char *i_ps8_nomFonction, uint16_t i_u16_numeroLigne)
{
  const char *Txt_Trace = NULL;
  char *P_Buffer = (char*) Buffer_TX_Trace;
  uint8_t Len_Temp;
  uint8_t Returned_Value = 0;
  uint32_t Temps;

#if defined(ACTIVER_TRACES_UDP)
  WiFiUDP udp;
#endif

  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  // Donnees a renvoyer non definie, on sort !
  if (Txt_Donnees == NULL)
    return 1;

  if (Horodatage == true)
    Temps = Get_Time();

  Txt_Trace = Get_Text_Type_Trace(Type_Trace);

  if (Txt_Trace == NULL)
    return 1;

  if (Horodatage == true)
  {
    uint32_t Time_int;
    uint8_t Time_frac;

    Time_int = Temps / 10;
    Time_frac = Temps - (10 * Time_int);

    Len_Temp = Conv_NumToStr(Time_int, P_Buffer);
    P_Buffer += Len_Temp;
    *P_Buffer = '.';
    P_Buffer++;
    Len_Temp = Conv_NumToStr(Time_frac, P_Buffer);
    P_Buffer += Len_Temp;
    *P_Buffer = 's';
    P_Buffer++;
    *P_Buffer = ':';
    P_Buffer++;
    *P_Buffer = ' ';
    P_Buffer++;
  }

  Len_Temp = strlen(Txt_Trace);
  strcpy(P_Buffer, Txt_Trace);
  P_Buffer += Len_Temp;

  *P_Buffer = ' ';
  P_Buffer++;
  *P_Buffer = '-';
  P_Buffer++;
  *P_Buffer = ' ';
  P_Buffer++;

  *P_Buffer = 0;

#if defined(ACTIVER_TRACES_UDP)
  udp.beginPacket(IP_DEST_UDP, PORT_DEST_UDP);
  udp.write(Buffer_TX_Trace, strlen((const char*) Buffer_TX_Trace));
  udp.write((const uint8_t*) Txt_Donnees, strlen((const char*) Txt_Donnees));
//  udp.endPacket();
#endif

  Send_String_UARTX((char* )Buffer_TX_Trace, Numero_UART);
  Send_String_UARTX((char* )Txt_Donnees, Numero_UART);

  if (i_u16_numeroLigne != 0)
  {
    Send_String_UARTX("    @@ (", Numero_UART);

    Send_String_UARTX((char* )i_ps8_nomFonction, Numero_UART);
    Send_String_UARTX(" -> ", Numero_UART);
    Send_String_UARTX((char* )i_ps8_nomFichier, Numero_UART);
    Send_String_UARTX("(l.", Numero_UART);

    {

#if defined(ACTIVER_TRACES_UDP)
      udp.write((const uint8_t*) "    @@ (", strlen("    @@ ("));
      udp.write((const uint8_t*) i_ps8_nomFonction, strlen(i_ps8_nomFonction));
      udp.write((const uint8_t*) " -> ", strlen(" -> "));
      udp.write((const uint8_t*) i_ps8_nomFichier, strlen(i_ps8_nomFichier));
      udp.write((const uint8_t*) "(l.", strlen("(l."));
#endif

      char *TempBuffer = new char[20];
      P_Buffer = TempBuffer;
      Len_Temp = Conv_NumToStr(i_u16_numeroLigne, P_Buffer);
      P_Buffer += Len_Temp;
      *P_Buffer = 0;
      Send_String_UARTX((char* )TempBuffer, Numero_UART);

      Send_String_UARTX("))", Numero_UART);

#if defined(ACTIVER_TRACES_UDP)
      udp.write((const uint8_t*) TempBuffer, strlen(TempBuffer));
      udp.write((const uint8_t*) "))", strlen("))"));
#endif
      delete[] TempBuffer;
    }
  }
  Send_String_UARTX("\n", Numero_UART);

#if defined(ACTIVER_TRACES_UDP)
  udp.endPacket();
#endif

  return Returned_Value;
}

void Set_Max_Debug_Level(type_trace_t Level)
{
  Max_Debug_Level = Level;
}

type_trace_t Get_Max_Debug_Level(void)
{
  return Max_Debug_Level;
}

#if UTILISER_TIMER_SW == 1
uint8_t Send_Timer_Trace(type_trace_t Type_Trace, TimerEvent_t * Timer, type_timer_t Type_Event, bool Horodatage)
{
	char Buffer_Temp[100];
	char * P_Temp = Buffer_Temp;
	const char * P_Txt_Timer_Event = NULL;
	uint8_t Len_Temp;

	// Test pour savoir si la trace a un niveau "remontable" ou non
	if(Test_Trace_Level(Type_Trace) != 0)
		return 2;


	//*****************
	//* Sanity Checks *
	//*****************
	if(Timer == NULL)
		return 1;

	if(strlen(Timer->Label) > 30) // Libelle trop long
		return 1;

	//************************************************
	//* Ecriture de la trame a renvoyer par la trace *
	//************************************************

	switch(Type_Event)
	{
	case TIMER_INIT:
		P_Txt_Timer_Event = Txt_Timer_Event_Init;
		//sprintf(Buffer_Temp,"TMR %s %s", Timer->Label, P_Txt_Timer_Event);
		break;
	case TIMER_START:
		P_Txt_Timer_Event = Txt_Timer_Event_Start;
		//sprintf(Buffer_Temp,"TMR %s %s %lums", Timer->Label, P_Txt_Timer_Event, Timer->Timestamp);
		break;
	case TIMER_STOP:
		P_Txt_Timer_Event = Txt_Timer_Event_Stop;
		//sprintf(Buffer_Temp,"TMR %s %s", Timer->Label, P_Txt_Timer_Event);
		break;
	case TIMER_HALT:
		P_Txt_Timer_Event = Txt_Timer_Event_Halt;
		break;
	case TIMER_TOP:
		P_Txt_Timer_Event = Txt_Timer_Event_Top;
		//sprintf(Buffer_Temp,"TMR %s %s", Timer->Label, P_Txt_Timer_Event);
		break;
	case TIMER_DELETE:
		P_Txt_Timer_Event = Txt_Timer_Event_Delete;
		//sprintf(Buffer_Temp,"TMR %s %s", Timer->Label, P_Txt_Timer_Event);
		break;
	case TIMER_SETVALUE:
		P_Txt_Timer_Event = Txt_Timer_Event_SetValue;
		break;
	case TIMER_FINTOP:
		P_Txt_Timer_Event = Txt_Timer_Event_FinTop;
		break;
	default:
		Send_Trace(DBGX, "Evenement Timer Inconnu", true);
		return 1;
		break;
	}

	strcpy(P_Temp, "TMR ");
	P_Temp += strlen("TMR ");

	Len_Temp = strlen(Timer->Label);
	strcpy(P_Temp, Timer->Label);
	P_Temp += Len_Temp;

	*P_Temp = ' ';
	P_Temp++;

	Len_Temp = strlen(P_Txt_Timer_Event);
	strcpy(P_Temp, P_Txt_Timer_Event);

	if((Type_Event == TIMER_START) || (Type_Event == TIMER_SETVALUE))
	{
		P_Temp += Len_Temp;
		*P_Temp = ' ';
		P_Temp++;

		Len_Temp = Conv_NumToStr(Timer->Timestamp, P_Temp);
		P_Temp += Len_Temp;

		strcpy(P_Temp, "ms");

	}

	return Send_Trace(Type_Trace, Buffer_Temp, true);
}
#endif // UTILISER_TIMER_SW == 1

uint8_t Send_Trace_Num(type_trace_t Type_Trace, const char Txt_Donnees[], uint32_t Num,
    bool Horodatage, const char *i_ps8_nomFichier, const char *i_ps8_nomFonction,
    uint16_t i_u16_numeroLigne)
{
  char Buffer_Temp[80];
  uint8_t Len_Temp;
  char *P_Buffer_Temp = Buffer_Temp;

  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  Len_Temp = strlen(Txt_Donnees);

  //*****************
  //* Sanity Checks *
  //*****************
  if (Len_Temp > 70)
    return 1;

  //sprintf(Buffer_Temp,"%s: %lu", Txt_Donnees, Num);
  strcpy(P_Buffer_Temp, Txt_Donnees);
  P_Buffer_Temp += Len_Temp;

  *P_Buffer_Temp = ':';
  P_Buffer_Temp++;
  *P_Buffer_Temp = ' ';
  P_Buffer_Temp++;

  Len_Temp = Conv_NumToStr(Num, P_Buffer_Temp);

  P_Buffer_Temp += Len_Temp;
  *P_Buffer_Temp = 0;

  return Send_Trace(Type_Trace, Buffer_Temp, Horodatage, i_ps8_nomFichier, i_ps8_nomFonction,
      i_u16_numeroLigne);
}

uint8_t Send_Trace_GroupValues(type_trace_t Type_Trace, Bank_Grouped_Values_t *Grouped_Values,
    uint8_t Raw_Size, bool Horodatage)
{
  uint8_t Taille_Reelle;
  uint8_t Buffer_Temp[128];
  uint8_t Buffer_Temp2[15];
  uint8_t i;

  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  if (Grouped_Values == NULL)
    return 1;

  Taille_Reelle = Raw_Size / sizeof(Bank_Grouped_Values_t);

  if ((Raw_Size % sizeof(Bank_Grouped_Values_t)) != 0)
    return 1;

  memset(Buffer_Temp, 0, sizeof(Buffer_Temp));

  for (i = 0; i < Taille_Reelle; i++)
  {
    uint8_t Var_Temp;

    strcat((char*) Buffer_Temp, Grouped_Values[i].p_Text_Data);
    strcat((char*) Buffer_Temp, ": ");

    Var_Temp = Conv_NumToStr(Grouped_Values[i].Value, (char*) Buffer_Temp2);

    if (Var_Temp >= sizeof(Buffer_Temp2))
      return 1;

    Buffer_Temp2[Var_Temp] = 0;

    strcat((char*) Buffer_Temp, (char*) Buffer_Temp2);

    if (i < (Taille_Reelle - 1))
      strcat((char*) Buffer_Temp, ", ");
  }

  return Send_Trace(Type_Trace, (char*) Buffer_Temp, Horodatage);
}

uint8_t Send_Trace_SignNum(type_trace_t Type_Trace, const char Txt_Donnees[], int32_t Num,
    bool Horodatage, const char *i_ps8_nomFichier, const char *i_ps8_nomFonction,
    uint16_t i_u16_numeroLigne)
{
  char Buffer_Temp[80];
  uint8_t Len_Temp;
  char *P_Buffer_Temp = Buffer_Temp;

  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  Len_Temp = strlen(Txt_Donnees);

  //*****************
  //* Sanity Checks *
  //*****************
  if (Len_Temp > 70)
    return 1;

  //sprintf(Buffer_Temp,"%s: %lu", Txt_Donnees, Num);
  strcpy(P_Buffer_Temp, Txt_Donnees);
  P_Buffer_Temp += Len_Temp;

  *P_Buffer_Temp = ':';
  P_Buffer_Temp++;
  *P_Buffer_Temp = ' ';
  P_Buffer_Temp++;

  if (Num < 0)
  {
    //Num = -Num;
    Num = (~Num) + 1;
    *P_Buffer_Temp = '-';
    P_Buffer_Temp++;
  }
  else if (Num > 0)
  {
    *P_Buffer_Temp = '+';
    P_Buffer_Temp++;
  }

  Len_Temp = Conv_NumToStr(Num, P_Buffer_Temp);

  P_Buffer_Temp += Len_Temp;
  *P_Buffer_Temp = 0;

  return Send_Trace(Type_Trace, Buffer_Temp, Horodatage, i_ps8_nomFichier, i_ps8_nomFonction,
      i_u16_numeroLigne);
}

uint8_t Send_Trace_FPoint(type_trace_t Type_Trace, const char Txt_Donnees[], double Num,
    uint8_t Precision, bool Horodatage, const char *i_ps8_nomFichier, const char *i_ps8_nomFonction,
    uint16_t i_u16_numeroLigne)
{
  char Buffer_Temp[80];
  uint8_t Len_Temp;
  char i;
  char *P_Buffer_Temp = Buffer_Temp;
  uint32_t Integer_Part, Fraction_Part;

  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  Len_Temp = strlen(Txt_Donnees);

  //*****************
  //* Sanity Checks *
  //*****************
  if (Len_Temp > 70)
    return 1;

  //sprintf(Buffer_Temp,"%s: %lu", Txt_Donnees, Num);
  strcpy(P_Buffer_Temp, Txt_Donnees);
  P_Buffer_Temp += Len_Temp;

  *P_Buffer_Temp = ':';
  P_Buffer_Temp++;
  *P_Buffer_Temp = ' ';
  P_Buffer_Temp++;

  Integer_Part = Num;
  Len_Temp = Conv_NumToStr(Integer_Part, P_Buffer_Temp);

  P_Buffer_Temp += Len_Temp;

  Num = Num - Integer_Part;

  if (Precision != 0)
  {
    for (i = 0; i < Precision; i++)
    {
      Num = Num * 10.0;
    }
    Fraction_Part = Num;

    *P_Buffer_Temp = '.';
    P_Buffer_Temp++;

    Len_Temp = Conv_NumToStr(Fraction_Part, P_Buffer_Temp);

    P_Buffer_Temp += Len_Temp;
  }

  *P_Buffer_Temp = 0;

  return Send_Trace(Type_Trace, Buffer_Temp, Horodatage, i_ps8_nomFichier, i_ps8_nomFonction,
      i_u16_numeroLigne);
}

#if UTILISER_NUMERO_VERSION_H == 1
uint8_t Send_Trace_Version(type_trace_t Type_Trace, const struct Struct_Version * Data, bool Horodatage)
{
	char Buffer_Temp[80];
	uint8_t Len_Temp;
	char * P_Buffer_Temp = Buffer_Temp;

	// Test pour savoir si la trace a un niveau "remontable" ou non
	if(Test_Trace_Level(Type_Trace) != 0)
		return 2;


	//*****************
	//* Sanity Checks *
	//*****************
	if(Data == NULL)
		return 1;

	if(sizeof(*Data) >= (sizeof(Buffer_Temp) + 4))
		return 1;

	//sprintf(Buffer_Temp, "%s %s - %s/%s/%s", Data->Type_Produit, Data->Version, Data->Jour, Data->Mois, Data->Annee);
	Len_Temp = strlen((char*)Data->Type_Produit);
	strcpy(P_Buffer_Temp, (char*)Data->Type_Produit);
	P_Buffer_Temp += Len_Temp;

	*P_Buffer_Temp = ' ';
	P_Buffer_Temp++;

	Len_Temp = strlen((char*)Data->Version);
	strcpy(P_Buffer_Temp, (char*)Data->Version);
	P_Buffer_Temp += Len_Temp;

	Len_Temp = strlen(" - ");
	strcpy(P_Buffer_Temp, " - ");
	P_Buffer_Temp += Len_Temp;


	Len_Temp = strlen((char*)Data->Jour);
	strcpy(P_Buffer_Temp, (char*)Data->Jour);
	P_Buffer_Temp += Len_Temp;

	*P_Buffer_Temp = '/';
	P_Buffer_Temp++;

	Len_Temp = strlen((char*)Data->Mois);
	strcpy(P_Buffer_Temp, (char*)Data->Mois);
	P_Buffer_Temp += Len_Temp;

	*P_Buffer_Temp = '/';
	P_Buffer_Temp++;

	Len_Temp = strlen((char*)Data->Annee);
	strcpy(P_Buffer_Temp, (char*)Data->Annee);
	P_Buffer_Temp += Len_Temp;

	*P_Buffer_Temp = 0;

	return Send_Trace(Type_Trace, Buffer_Temp, Horodatage);
}
#endif // UTILISER_NUMERO_VERSION_H == 1

uint8_t Send_Trace_Buffer(type_trace_t Type_Trace, const char Txt_Donnees[], uint8_t *Buffer,
    uint8_t Size, bool Horodatage)
{
  char Buffer_Temp[128];
  char *P_Buffer_Temp;
  uint8_t lenText_Donnnes;
  uint8_t Partie_Haute, Partie_Basse;

  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  //*****************
  //* Sanity Checks *
  //*****************
  if (Txt_Donnees == NULL)
    return 1;

  if (Buffer == NULL)
    return 1;

  lenText_Donnnes = strlen(Txt_Donnees);

  if ((lenText_Donnnes + Size + 3) >= 128)
    return 1;

  strcpy(Buffer_Temp, Txt_Donnees);
  P_Buffer_Temp = &Buffer_Temp[0] + lenText_Donnnes;

  *P_Buffer_Temp = ' ';
  P_Buffer_Temp++;
  *P_Buffer_Temp = ':';
  P_Buffer_Temp++;
  *P_Buffer_Temp = ' ';
  P_Buffer_Temp++;

  while (Size != 0)
  {
    Partie_Haute = *Buffer >> 4;

    if (Partie_Haute >= 0x0a)
      Partie_Haute += 0x57;
    else
      Partie_Haute += 0x30;

    *P_Buffer_Temp = Partie_Haute;

    P_Buffer_Temp++;

    Partie_Basse = *Buffer & 0xf;

    if (Partie_Basse >= 0x0a)
      Partie_Basse += 0x57;
    else
      Partie_Basse += 0x30;

    *P_Buffer_Temp = Partie_Basse;

    P_Buffer_Temp++;

    if (Size != 1)
    {
      *P_Buffer_Temp = ' ';
      P_Buffer_Temp++;
    }

    Size--;
    Buffer++;
  }

  *P_Buffer_Temp = 0;

  return Send_Trace(Type_Trace, Buffer_Temp, Horodatage);
}

uint8_t Send_Trace_Text(type_trace_t Type_Trace, const char Txt_Donnees[],
    const char Txt_Donnees2[], bool Horodatage, const char *i_ps8_nomFichier,
    const char *i_ps8_nomFonction, uint16_t i_u16_numeroLigne)
{
  char Buffer_Temp[128];

  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  //*****************
  //* Sanity Checks *
  //*****************
  if (Txt_Donnees == NULL)
    return 1;

  if (Txt_Donnees2 == NULL)
    return 1;

  strcpy(Buffer_Temp, Txt_Donnees);
  strcpy(Buffer_Temp + strlen(Txt_Donnees), Txt_Donnees2);

  return Send_Trace(Type_Trace, Buffer_Temp, Horodatage, i_ps8_nomFichier, i_ps8_nomFonction,
      i_u16_numeroLigne);
}

uint8_t Send_Trace_IfNotZero(type_trace_t Type_Trace, const char Txt_Donnees[], uint8_t Num,
    bool Horodatage)
{
  // Si Num est nul, pas la peine de perdre du temps en allant plus loin
  if (Num == 0)
    return 0;

  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  //*****************
  //* Sanity Checks *
  //*****************
  if (Txt_Donnees == NULL)
    return 1;

  return Send_Trace_Num(Type_Trace, Txt_Donnees, Num, Horodatage);
}

uint8_t Send_Trace_Test_Result(type_trace_t Type_Trace, const char Txt_Donnees[], uint8_t Num,
    bool Horodatage)
{
  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  //*****************
  //* Sanity Checks *
  //*****************
  if (Txt_Donnees == NULL)
    return 1;

  switch (Num)
  {
  case 0:
    return Send_Trace_Text(Type_Trace, Txt_Donnees, " : PASS", Horodatage);
    break;
  case 255:
    return Send_Trace_Text(Type_Trace, Txt_Donnees, "...", Horodatage);
    break;
  default:
    return Send_Trace_Text(Type_Trace, Txt_Donnees, " : KO", Horodatage);
    break;
  }

  return 1;
}

uint8_t Send_Trace_Bool(type_trace_t Type_Trace, const char Txt_Donnees[], bool Num_Bool,
    bool Horodatage)
{
  // Test pour savoir si la trace a un niveau "remontable" ou non
  if (Test_Trace_Level(Type_Trace) != 0)
    return 2;

  //*****************
  //* Sanity Checks *
  //*****************
  if (Txt_Donnees == NULL)
    return 1;

  if (Num_Bool == true)
  {
    return Send_Trace_Text(Type_Trace, Txt_Donnees, ": true", Horodatage);
  }
  else
  {
    return Send_Trace_Text(Type_Trace, Txt_Donnees, ": false", Horodatage);
  }

  return 1;
}

uint8_t Test_Trace_Level(type_trace_t Level)
{
  // Pas de trace voulue, on sort !
  if (Max_Debug_Level == NONE)
    return 1;

  if (Level == NONE)
    return 1;

  // Niveau de trace superieur au niveau max defini, on sort !
  if (Level > Max_Debug_Level)
  {
    // Sauf s'il s'agit du niveau de "debug specifique" permettant d'extraire
    // une trace particuliere
    if (Level != DBGX)
      return 1;
  }

  return 0;
}

uint8_t Conv_NumToStr(uint32_t Num, char *String)
{
  uint8_t Nbre = 0;
  int32_t Temp;

  if (Num > 999999999)
    Num = 999999999;

  Temp = (signed long) Num;

  if (Temp > 99999999)
  {
    if (Nbre == 0)
      Nbre = 9 + 1 + 1;

    *String = 0x30;

    while (Temp >= 0)
    {
      Temp -= 100000000;
      (*String)++;
    }

    Temp += 100000000;
    (*String)--;

    String++;
  }

  if ((Temp > 9999999) || (Nbre > 8))
  {
    if (Nbre == 0)
      Nbre = 8 + 1 + 1;

    *String = 0x30;

    while (Temp >= 0)
    {
      Temp -= 10000000;
      (*String)++;
    }

    Temp += 10000000;
    (*String)--;

    String++;

  }

  if ((Temp > 999999) || (Nbre > 7))
  {
    if (Nbre == 0)
      Nbre = 7 + 1 + 1;

    *String = 0x30;

    while (Temp >= 0)
    {
      Temp -= 1000000;
      (*String)++;
    }

    Temp += 1000000;
    (*String)--;

    String++;
    *String = '\'';
    String++;
  }

  if ((Temp > 99999) || (Nbre > 6))
  {
    if (Nbre == 0)
      Nbre = 6 + 1;

    *String = 0x30;

    while (Temp >= 0)
    {
      Temp -= 100000;
      (*String)++;
    }

    Temp += 100000;
    (*String)--;

    String++;
  }

  if ((Temp > 9999) || (Nbre > 5))
  {
    if (Nbre == 0)
      Nbre = 5 + 1;

    *String = 0x30;

    while (Temp >= 0)
    {
      Temp -= 10000;
      (*String)++;
    }

    Temp += 10000;
    (*String)--;

    String++;
  }

  if ((Temp > 999) || (Nbre > 4))
  {
    if (Nbre == 0)
      Nbre = 4 + 1;

    *String = 0x30;

    while (Temp >= 0)
    {
      Temp -= 1000;
      (*String)++;
    }

    Temp += 1000;
    (*String)--;

    String++;

    *String = '\'';
    String++;
  }

  if ((Temp > 99) || (Nbre > 3))
  {
    if (Nbre == 0)
      Nbre = 3;

    *String = 0x30;

    while (Temp >= 0)
    {
      Temp -= 100;
      (*String)++;
    }

    Temp += 100;
    (*String)--;

    String++;
  }

  if ((Temp > 9) || (Nbre > 2))
  {
    if (Nbre == 0)
      Nbre = 2;

    *String = 0x30;

    while (Temp >= 0)
    {
      Temp -= 10;
      (*String)++;
    }

    Temp += 10;
    (*String)--;

    String++;
  }

  // reste les unites !
  if (Nbre == 0)
    Nbre = 1;

  *String = 0x30 + Temp;

  String++;
  *String = 0;

  return Nbre;
}

uint8_t Send_VTrace(type_trace_t Type_Trace, bool Horodatage, const char *i_ps8_nomFichier,
    const char *i_ps8_nomFonction, uint16_t i_u16_numeroLigne, const char *Txt_Donnees, ...)
{
  char ts8_BufferTx[200];
  va_list argp;

  va_start(argp, Txt_Donnees);
  vsprintf(ts8_BufferTx, Txt_Donnees, argp);
  va_end(argp);

  return Send_Trace(Type_Trace, ts8_BufferTx, Horodatage, i_ps8_nomFichier, i_ps8_nomFonction,
      i_u16_numeroLigne);
}

uint8_t Send_VTrace2(type_trace_t Type_Trace, bool Horodatage, const char *i_ps8_nomFichier,
    const char *i_ps8_nomFonction, uint16_t i_u16_numeroLigne, const char *Txt_Donnees, ...)
{
  char ts8_BufferTx[200];
  char ts8_BufferTxString[300];
  va_list argp;
  uint32_t u32_Temps;
  uint32_t u32_TimeInt;
  uint8_t u8_TimeFrac;
  std::string *l_pt_StringTxt;

  if (Horodatage == true)
  {
    u32_Temps = Get_Time();

    u32_TimeInt = u32_Temps / 10;
    u8_TimeFrac = u32_Temps - (10 * u32_TimeInt);
  }

  va_start(argp, Txt_Donnees);
  vsprintf(ts8_BufferTx, Txt_Donnees, argp);
  va_end(argp);

  sprintf(ts8_BufferTxString, "%u.%us: %s - %s    @@ (%s -> %s(l.%u))", u32_TimeInt, u8_TimeFrac,
      Get_Text_Type_Trace(Type_Trace), ts8_BufferTx, i_ps8_nomFonction, i_ps8_nomFichier,
      i_u16_numeroLigne);

  l_pt_StringTxt = new std::string(ts8_BufferTxString);

  xQueueSend(g_pt_queueTraces, &l_pt_StringTxt, 0);

//  return Send_Trace(Type_Trace, ts8_BufferTx, Horodatage, i_ps8_nomFichier, i_ps8_nomFonction,
//      i_u16_numeroLigne);

  return 0;
}

void ThreadTxTrace(void)
{
  std::string *l_pt_stringRxUdp = NULL;

  while (1)
  {
    if (xQueueReceive(g_pt_queueTraces, &l_pt_stringRxUdp, portMAX_DELAY) == pdTRUE)
    {
      std::string l_t_localMsg = *l_pt_stringRxUdp;
      delete l_pt_stringRxUdp;

      Serial.println(l_t_localMsg.c_str());
    }
  }
}
