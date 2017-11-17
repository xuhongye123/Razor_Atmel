/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

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
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
           
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
 


void UserApp1Initialize(void) //Enter initial interface and  go to guide interface
{ 
  static u8 au8InitialMessage1[]="Welcome to use!";
  static u8 au8InitialMessage2[]="Please press any button to continue";
  LCDCommand(LCD_CLEAR_CMD);     
  LCDMessage(LINE1_START_ADDR+3,au8InitialMessage1);
  LCDMessage(LINE2_START_ADDR,au8InitialMessage2);
  LedPWM(RED,LED_PWM_10);
 
 
  
  
  
 
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
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
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static bool bUseFlag=FALSE;//TRUE is on use,FLASE is in initial interface
  static u16 u16Counter=0;//Count scroll
  static u8 u8Index=0;
  static u8 au8InitialMessage2[]="Please press any button to continue.";
  static u8 au8DisplayString1[20];
  static u8 au8DisplayString2[20];
  static bool bRollForwardFlag=TRUE;//TRUE is forward,FLASE is backward
  static u8 au8GuideMessage1[]="Here is guidance:   B0:back to guidance";
  static u8 au8GuideMessage2[]="B1:show my name. B2:use debug interface";
  static u8 i=0;
  static u8 temp=0;



    
  static u8 au8AuthorName[]=NAME;
  static u8 au8Input[2]={0};
  static u8 au8InputString[128]={0};
  static u8 au8InputString1[128]={0};
  static u8 au8InputString2[128]={0};
  static bool bDymaic=FALSE;//TRUE is dymaic,FALSE is static
  static bool bButton0=FALSE;//TRUE to use this button's function
  static bool bButton1=FALSE;
  static bool bButton2=FALSE;
 
  
  if(bUseFlag==FALSE)//Initial interface and tell how to continue
  {
    u16Counter++;
    if(u16Counter==SCROLLTIME)//Scroll character
    {
      u16Counter=0;
      if(bRollForwardFlag==TRUE)
      {
        u8Index++;
      }
      if(bRollForwardFlag==FALSE)
      {
        u8Index--;    
      }
      for(u8 i=0;i<20;i++)
      {
        au8DisplayString2[i]=au8InitialMessage2[i+u8Index];     
      }
      LCDMessage(LINE2_START_ADDR,au8DisplayString2);     
      if(u8Index==(sizeof(au8InitialMessage2)-21))
      {
       bRollForwardFlag=FALSE;
      }
      if(u8Index==0)
      {
       bRollForwardFlag=TRUE;
      }  
    }
    if(WasButtonPressed(BUTTON0)||WasButtonPressed(BUTTON1)||WasButtonPressed(BUTTON2)||WasButtonPressed(BUTTON3))//press any button to use
    {
      ButtonAcknowledge(BUTTON0);
      ButtonAcknowledge(BUTTON1);
      ButtonAcknowledge(BUTTON2);
      ButtonAcknowledge(BUTTON3);         
      bUseFlag=TRUE;
      u16Counter=0;
      u8Index=0;
      bButton0=TRUE;
      bRollForwardFlag=TRUE;
      LedOff(RED);
      LedBlink(ORANGE,LED_2HZ);
    }
  }
  
  
  if(bUseFlag)//use the function 
  {
    if(WasButtonPressed(BUTTON0))//Back to guide interface
    {
      ButtonAcknowledge(BUTTON0);
      u16Counter=0;
      u8Index=0;     
      bButton0=TRUE;
      bButton1=FALSE;
      bButton2=FALSE;
      bRollForwardFlag=TRUE;
      LedOff(RED);
      LedBlink(ORANGE,LED_2HZ);
      PWMAudioOff(BUZZER1);
    }
    if(WasButtonPressed(BUTTON1))//Show author's name and special effects
    {
      ButtonAcknowledge(BUTTON1);
      u16Counter=0;
      u8Index=0;
      bButton0=FALSE;
      bButton1=TRUE;
      bButton2=FALSE;
      LedBlink(ORANGE,LED_2HZ);
      LedBlink(RED,LED_2HZ);    
       
    }
     if(WasButtonPressed(BUTTON2))//Enter debug interface
    {
      ButtonAcknowledge(BUTTON2);
      u16Counter=0;
      u8Index=0;
      bButton0=FALSE;
      bButton1=FALSE;
      bButton2=TRUE;
      LedOff(ORANGE);
      LedBlink(RED,LED_2HZ);
      PWMAudioOff(BUZZER1);
      LCDCommand(LCD_CLEAR_CMD);
    }
   
    if(bButton0)//Back to guide interface
    {
      u16Counter++;     
      if(u16Counter==SCROLLTIME)
      {
        u16Counter=0;
        LCDCommand(LCD_CLEAR_CMD);
        for(u8 i=0;i<20;i++)
        {
          au8DisplayString1[i]=au8GuideMessage1[i+u8Index];
          au8DisplayString2[i]=au8GuideMessage2[i+u8Index];     
        }
        LCDMessage(LINE1_START_ADDR,au8DisplayString1); 
        LCDMessage(LINE2_START_ADDR,au8DisplayString2);
        if(bRollForwardFlag==TRUE)
        {
          u8Index++;
        }
        if(bRollForwardFlag==FALSE)
        {
          u8Index--;    
        }      
        if(u8Index==(sizeof(au8GuideMessage2)-21))
        {
         bRollForwardFlag=FALSE;
        }
        if(u8Index==0)
        {
         bRollForwardFlag=TRUE;
        }  
      }
    }
    
    if(bButton1)//Show author's name and special effects
    {
      u16Counter++;
      if(u16Counter==SCROLLTIME)
      {
        u16Counter=0;
        LCDCommand(LCD_CLEAR_CMD);              
        if(u8Index==(22-sizeof(au8AuthorName)))
        {
          u8Index=0;
        }
        LCDMessage(LINE1_START_ADDR+u8Index,au8AuthorName);
        PWMAudioSetFrequency(BUZZER1, 100+30*u8Index);
        PWMAudioOn(BUZZER1);
        u8Index++;          
      }
      
    
    }
      
    if(bButton2)//Enter debug Interface
    {
      if(DebugScanf(au8Input)>0)
      {
        au8InputString[u8Index]=au8Input[0];
        if(u8Index<20)
        {
          au8InputString1[u8Index]=au8Input[0];
          u8Index++;     
        }
        else
        {
          au8InputString2[u8Index-20]=au8Input[0];
          u8Index++;       
        }
        if(u8Index==40)
        {
          bDymaic=TRUE;       
        }
        if(bDymaic==FALSE)
        {
          LCDCommand(LCD_CLEAR_CMD);
          LCDMessage(LINE1_START_ADDR,au8InputString1);                  
          LCDMessage(LINE2_START_ADDR,au8InputString2);        
        }
      }
      if(bDymaic)
      {
        u16Counter++;
        if(u16Counter==SCROLLTIME)
        {
          u16Counter=0;
          LCDCommand(LCD_CLEAR_CMD);
          au8InputString[39]=au8InputString[0];
          for(i=0;i<39;i++)
          {
            temp=au8InputString[i+1];
            au8InputString[i]=temp;             
          }
          for(i=0;i<20;i++)
          {
            au8InputString1[i]=au8InputString[i];
          }
          for (i=20;i<40;i++)
          {
            au8InputString2[i-20]=au8InputString[i];
          }
          LCDMessage(LINE1_START_ADDR,au8InputString1);                  
          LCDMessage(LINE2_START_ADDR,au8InputString2);         
        }       
      } 
    }   
  } 
  

    
    
    
    
    
    
    
  
 
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
