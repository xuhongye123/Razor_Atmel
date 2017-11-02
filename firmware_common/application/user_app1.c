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
void UserApp1Initialize(void)
{
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOn(RED);
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
  static u8 au8Password[10]={0,0,0,0,0,0,0,0,0,0};//Initial password
  
  static u8 au8PressKey[10];
 
  static bool bSetOrInputFlag=FALSE;//TRUE is set passward,FALSE is input passward

  static u8 u8KeyNumber=0;
  
  static u8 u8PasswordLong=10;
  
  if( IsButtonHeld(BUTTON3, 2000) )// change work mode to set password  and when reset please must held on at least 2S,otherwise the LED's blink has some BUGs
  {
    ButtonAcknowledge(BUTTON3);
    LedOff(RED);
    LedOff(GREEN);
    LedBlink(RED,LED_2HZ);
    LedBlink(GREEN,LED_2HZ);
    bSetOrInputFlag=TRUE;
    u8PasswordLong=0;
  }
  if (bSetOrInputFlag==TRUE)//set password
  {
    if( WasButtonPressed(BUTTON0) )
    {
      ButtonAcknowledge(BUTTON0);
      au8Password[u8PasswordLong]=0;
      u8PasswordLong++;
    }
    if( WasButtonPressed(BUTTON1) )
    {
      ButtonAcknowledge(BUTTON1);
      au8Password[u8PasswordLong]=1;
      u8PasswordLong++;
    }
    if( WasButtonPressed(BUTTON2) )
    {
      ButtonAcknowledge(BUTTON2);
      au8Password[u8PasswordLong]=2;
      u8PasswordLong++;
    }
    if( WasButtonPressed(BUTTON3) )//use BUTTON3 to finish setting
    {
      ButtonAcknowledge(BUTTON3);
      LedOn(RED);
      LedOff(GREEN);
      bSetOrInputFlag=FALSE;
    }
  }
  
  
  
  
  if(bSetOrInputFlag==FALSE)//Input password
  {
    if( WasButtonPressed(BUTTON0) )
    {
      ButtonAcknowledge(BUTTON0);
      au8PressKey[u8KeyNumber]=0;
      u8KeyNumber++;
    }
    if( WasButtonPressed(BUTTON1) )
    {
      ButtonAcknowledge(BUTTON1);
      au8PressKey[u8KeyNumber]=1;
      u8KeyNumber++;
    }
    if( WasButtonPressed(BUTTON2) )
    {
      ButtonAcknowledge(BUTTON2);
      au8PressKey[u8KeyNumber]=2;
      u8KeyNumber++;
    }
    if( WasButtonPressed(BUTTON3) )// Use BUTTON3 to confirm password
    {
      ButtonAcknowledge(BUTTON3);
      if(u8KeyNumber>=10&&u8PasswordLong>=10)//If the set and input go beyond the limit(10),take the front 10 number,
      {
        for(u8 i=0;i<10;i++)//Determine password content
        if(au8Password[i]!=au8PressKey[i])
        {
          LedBlink(RED,LED_2HZ);
          u8KeyNumber=0;
        }
        else
        {
          LedBlink(GREEN,LED_2HZ);//Password correct   
          LedOff(RED);
          u8KeyNumber=0;
        }   
      }
      else 
      { 
        if(u8KeyNumber!=u8PasswordLong)
        {
          LedBlink(RED,LED_2HZ);//If Input wrong,only can reset password
          u8KeyNumber=0;
        }
        else 
        {
          for(u8 i=0;i<(u8KeyNumber-1);i++)//Determine password content
          if(au8Password[i]!=au8PressKey[i])
          {
            LedBlink(RED,LED_2HZ);
            u8KeyNumber=0;
          }
          else
          {
            LedBlink(GREEN,LED_2HZ);//Password correct   
            LedOff(RED);
            u8KeyNumber=0;
          }  
        }
      }
    }
  } 
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
