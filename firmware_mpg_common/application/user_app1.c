/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/


static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


static AntAssignChannelInfoType sAntSetupData;
static bool bChangeFlag = FALSE;
/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  u8 au8WelcomeMessage1[] = "HIDE AND SEEK";
  u8 au8WelcomeMessage2[] = "HAVE FUN! ";
 
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR+3, au8WelcomeMessage1); 
  LCDMessage(LINE2_START_ADDR+5, au8WelcomeMessage2); 

  

 /* Configure ANT for this application */
  sAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  sAntSetupData.AntChannelType      = ANT_CHANNEL_TYPE_SEEK;
  sAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  sAntSetupData.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sAntSetupData.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;

  sAntSetupData.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sAntSetupData.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
    
  /* If good initialization, set state to Idle */
  if( AntAssignChannel(&sAntSetupData) )
  {
    /* Channel is configured, so change LED to yellow */

    LedOn(RED);   
    UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
  }
  else
  {
    LedBlink(RED, LED_4HZ);
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/


void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
static void UserApp1SM_WaitChannelAssign(void)
{
  /* Check if the channel assignment is complete */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    UserApp1_StateMachine = UserApp1SM_Introduce;
  }
  
  /* Monitor for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, 3000) )
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    UserApp1_StateMachine = UserApp1SM_Error;
  }
      
} /* end UserApp1SM_WaitChannelAssign() */


/*static void UserApp1SM_Delay(void)
{
  static u32 u32UserApp1SM_Counter = 0;
  static u32 au32DelayGroup[]={INITIALIZE_INTRODUCE_DELAY,MASTER_SLAVE_CHANGE_DELAY,INTRODUCE_RULES_DELAY,RULES_INTRODUCE_DELAY};

  u32UserApp1SM_Counter++;
  if(u32UserApp1SM_Counter == au32DelayGroup[u8DelayGroupIndex])
  {
    u32UserApp1SM_Counter = 0;
    if(u8DelayGroupIndex == 0)
    {
      UserApp1_StateMachine = UserApp1SM_Introduce;  
    }
    if(u8DelayGroupIndex == 1)
    {
      UserApp1_StateMachine =  UserApp1SM_WaitChannelOpen;  
    } 
    if(u8DelayGroupIndex == 2)
    {
      UserApp1_StateMachine =  UserApp1SM_GameRules;  
    }  
    if(u8DelayGroupIndex == 3)
    {
      UserApp1_StateMachine =  UserApp1SM_Introduce;  
    }  
  } 
}*/

static void UserApp1SM_Introduce(void)
{
  if(bChangeFlag == FALSE)
  {
    static u8 au8IntroduceMessage1[] = "B0:HIDE,B1:SEEK ";
    static u8 au8IntroduceMessage2[] = "B3:GET DETAILED RULES";
    static u8 au8IntroduceMessage3[] = "Warning to partner ";
    static u8 au8IntroduceMessage4[] = "NOT BUTTON SAME,0/1";
    static u32 u32IntroduceCounter = 0;
    
    u32IntroduceCounter++;
    LedOff(RED);
    LedOff(YELLOW);
    LedOn(ORANGE);
    if(u32IntroduceCounter == WARNING_BLINK_TIME)
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, au8IntroduceMessage1); 
      LCDMessage(LINE2_START_ADDR, au8IntroduceMessage2); 
    }
    if(u32IntroduceCounter == (2*WARNING_BLINK_TIME))
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, au8IntroduceMessage3); 
      LCDMessage(LINE2_START_ADDR, au8IntroduceMessage4);
      u32IntroduceCounter=0;
    }
    
    if(WasButtonPressed(BUTTON0))
    {
      u32IntroduceCounter=0;
      ButtonAcknowledge(BUTTON0);
      sAntSetupData.AntChannelType = ANT_CHANNEL_TYPE_HIDE;
      AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
    }
    if(WasButtonPressed(BUTTON1))
    {
      u32IntroduceCounter=0;
      ButtonAcknowledge(BUTTON1);
      sAntSetupData.AntChannelType = ANT_CHANNEL_TYPE_SEEK;
      AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
    }
    if(WasButtonPressed(BUTTON3))
    {
      u32IntroduceCounter=0;
      ButtonAcknowledge(BUTTON3);
      UserApp1_StateMachine = UserApp1SM_GameRules;
    }
  }
  if(bChangeFlag == TRUE)
  {   
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;    
  }
}

static void UserApp1SM_GameRules(void)
{ 
  static u8 u8GameRulesCounter=0;                
  static u8 au8RulesMessage1[]="LAZY,HHH~_~";
  static u8 au8RulesMessage2[]="B3:GO OUT ";
  
  u8GameRulesCounter++;
  LedOff(ORANGE);
  LedOn(YELLOW);
  if(u8GameRulesCounter == LCDMESSAGE_DELAY )
  {
    u8GameRulesCounter=0;
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, au8RulesMessage1); 
    LCDMessage(LINE2_START_ADDR, au8RulesMessage2); 
  }
  if(WasButtonPressed(BUTTON3))
  {
    ButtonAcknowledge(BUTTON3);
    UserApp1_StateMachine = UserApp1SM_Introduce;
  }
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to open */
static void UserApp1SM_WaitChannelOpen(void)
{
  /* Monitor the channel status to check if channel is opened */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {
    LedOn(GREEN);
    LedOff(ORANGE);
    UserApp1_StateMachine = UserApp1SM_ChannelOpen;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE) )
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(GREEN);
    LedOn(YELLOW);   
    UserApp1_StateMachine = UserApp1SM_Error;
  }  
} /* end UserApp1SM_WaitChannelOpen() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Channel is open, so monitor data */
static void UserApp1SM_ChannelOpen(void)
{
  static s8 s8UserApp1_RSSI = 0;
  static u8 au8RSSI_String[]="-   ";
  static u16 au16BuzzerFrequency[] = {262,294,330,349,392};
  static u8 au8SeekerMessage1[]="Faint signal";
  static u8 au8SeekerMessage2[]="Near!";
  static u8 au8SeekerMessage3[]="Find,congratulations!";
  static bool bMessage1=FALSE;
  static bool bMessage2=FALSE;
  static bool bMessage3=FALSE;
  static bool bGameOverFlag=FALSE;
  static u32 u32ChannelOpenCounter1=0;
  static u32 u32ChannelOpenCounter2=0;
    
  if(sAntSetupData.AntChannelType == ANT_CHANNEL_TYPE_HIDE)
  {
    u32ChannelOpenCounter1++;
    if(u32ChannelOpenCounter1 == LCDMESSAGE_DELAY)
    {
      u32ChannelOpenCounter1=0;
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage( LINE1_START_ADDR , "Wait for seeking..." );
    }  
  }
  
  
  
  if(sAntSetupData.AntChannelType == ANT_CHANNEL_TYPE_SEEK)
  {
    u32ChannelOpenCounter1++;
    if(u32ChannelOpenCounter1 == LCDMESSAGE_DELAY)
    {
      u32ChannelOpenCounter1=0;
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage( LINE1_START_ADDR , "Seeking..." );
      LCDMessage( LINE2_START_ADDR , "RSSI: " );
      LCDMessage( LINE2_START_ADDR+5,au8RSSI_String);
      LCDMessage( LINE2_START_ADDR+9,"dBm");  
    }
    if( AntReadAppMessageBuffer())
    {
      if(G_eAntApiCurrentMessageClass == ANT_DATA)
      {
        if(s8UserApp1_RSSI != G_sAntApiCurrentMessageExtData.s8RSSI)
        {
          s8UserApp1_RSSI = G_sAntApiCurrentMessageExtData.s8RSSI;
          if(s8UserApp1_RSSI>-100)
          {
            au8RSSI_String[1] = (s8UserApp1_RSSI/-1)/10+'0';
            au8RSSI_String[2] = (s8UserApp1_RSSI/-1)%10+'0';
          }
          else
          {
            au8RSSI_String[1] = (s8UserApp1_RSSI/-1)/100+'0';
            au8RSSI_String[2] = (s8UserApp1_RSSI/-1)%100/10+'0';
            au8RSSI_String[3] = (s8UserApp1_RSSI/-1)%10+'0';         
          }                
        }
        if(s8UserApp1_RSSI<  -40 && s8UserApp1_RSSI>= -50)
        {
          PWMAudioSetFrequency(BUZZER1, au16BuzzerFrequency[4]);
          PWMAudioOn(BUZZER1);
          LedBlink(GREEN,LED_0_5HZ);
          LedOn(CYAN);
          LedOn(BLUE);
          LedOn(PURPLE);
          LedOn(WHITE);
          bMessage1=FALSE;
          bMessage2=FALSE;
          bMessage3=TRUE;
          u32ChannelOpenCounter1=0;
          AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
          UserApp1_u32Timeout = G_u32SystemTime1ms;
          UserApp1_StateMachine = UserApp1SM_WaitChannelClose;
        } 
        if(s8UserApp1_RSSI<  -50 && s8UserApp1_RSSI>= -60)
        {
          PWMAudioSetFrequency(BUZZER1, au16BuzzerFrequency[3]);
          PWMAudioOn(BUZZER1);
          LedBlink(GREEN,LED_1HZ);
          LedOn(CYAN);
          LedOn(BLUE);
          LedOn(PURPLE);
          LedOff(WHITE);
        }          
        if(s8UserApp1_RSSI<  -60 && s8UserApp1_RSSI>= -70)
        {
          PWMAudioSetFrequency(BUZZER1, au16BuzzerFrequency[2]);
          PWMAudioOn(BUZZER1);
          LedBlink(GREEN,LED_2HZ);
          LedOn(CYAN);
          LedOn(BLUE);
          LedOff(PURPLE);
          LedOff(WHITE);
        }         
        if(s8UserApp1_RSSI<  -70 && s8UserApp1_RSSI>= -80)
        {
          PWMAudioSetFrequency(BUZZER1, au16BuzzerFrequency[1]);
          PWMAudioOn(BUZZER1);
          LedBlink(GREEN,LED_4HZ);
          LedOn(CYAN);
          LedOff(BLUE);
          LedOff(PURPLE);
          LedOff(WHITE);
        }          
        if(s8UserApp1_RSSI<  -80 && s8UserApp1_RSSI>= -90)
        {
          PWMAudioSetFrequency(BUZZER1, au16BuzzerFrequency[0]);
          PWMAudioOn(BUZZER1);
          LedBlink(GREEN,LED_8HZ);
          LedOff(CYAN);
          LedOff(BLUE);
          LedOff(PURPLE);
          LedOff(WHITE);        
        }
      }   
    }
  }
}


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to close */
static void UserApp1SM_WaitChannelClose(void)
{
  /* Monitor the channel status to check if channel is closed */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CLOSED)
  {
    LedOff(GREEN);
    LedOff(CYAN);
    LedOff(BLUE);
    LedOff(PURPLE);
    LedOff(WHITE);        
    LedOn(YELLOW);
    PWMAudioOff(BUZZER1);
    UserApp1_StateMachine = UserApp1SM_Change; 
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE) )
  {
    LedOff(GREEN);
    LedOff(YELLOW);
    LedBlink(RED, LED_4HZ); 
    UserApp1_StateMachine = UserApp1SM_Error;
  }   
} /* end UserApp1SM_WaitChannelClose() */

static void UserApp1SM_Change(void)
{
  static u8 au8ChangeMessage1[]="Change or restart?";
  static u8 au8ChangeMessage0[]="BUTTON0 or BUTTON1";
  static u8 au8ChangeMessage2[]="Changing...";
  static u8 au8ChangeMessage3[]="Run away,Wait 10s";
  static u8 au8ChangeMessage4[]="Backing...";
  
  static u32 u32ChangeCounter1=0;
  static u32 u32ChangeCounter2=0;
  
  u32ChangeCounter1++;
  if(u32ChangeCounter1 == LCDMESSAGE_DELAY)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, au8ChangeMessage1);
    LCDMessage(LINE2_START_ADDR, au8ChangeMessage0);
  }
  if(WasButtonPressed(BUTTON0))
  {
    u32ChangeCounter2++;
    ButtonAcknowledge(BUTTON0);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, au8ChangeMessage2);
    LCDMessage(LINE2_START_ADDR, au8ChangeMessage3);
    if(sAntSetupData.AntChannelType == ANT_CHANNEL_TYPE_SEEK)
    {
      sAntSetupData.AntChannelType = ANT_CHANNEL_TYPE_HIDE;
    }
    if(sAntSetupData.AntChannelType == ANT_CHANNEL_TYPE_HIDE)
    {
      sAntSetupData.AntChannelType = ANT_CHANNEL_TYPE_SEEK;
    }
    bChangeFlag = TRUE;
    if(u32ChangeCounter2 == MASTER_SLAVE_CHANGE_DELAY)
    {
      u32ChangeCounter2 = 0;
      AntAssignChannel(&sAntSetupData);
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;  
    } 
  }
  if(WasButtonPressed(BUTTON1))
  {
    u32ChangeCounter2++;
    ButtonAcknowledge(BUTTON1);
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, au8ChangeMessage4);
    bChangeFlag = FALSE;
    if(u32ChangeCounter2 == 1000);
    {
      u32ChangeCounter2 = 0;
      AntAssignChannel(&sAntSetupData);
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_WaitChannelAssign;
    }
  }
}



/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */

static void UserApp1SM_Idle(void)
{
  
}



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
