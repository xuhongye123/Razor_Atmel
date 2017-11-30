/**********************************************************************************************************************
File: user_app1.c                                                                

Description:
Provides a Tera-Term driven system to display, read and write an LED command list.

Test1.
Test2 from Engenuics.
Test3.

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:
None.

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
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u8 G_au8DebugScanfBuffer[DEBUG_SCANF_BUFFER_SIZE]; /* From debug.c */
extern u8 G_u8DebugScanfCharCount;                        /* From debug.c */


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
  
  
  
  
  
          
  u8 au8UserApp1Start1[] = "LED program task started\n\r";
  
  /* Turn off the Debug task command processor and announce the task is ready */
  DebugSetPassthrough();
  DebugPrintf(au8UserApp1Start1);
  
    /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
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
/* Wait for input */
static void UserApp1SM_Idle(void)
{
  static u8 au8MenuMessage1[]="****************************************************";
  static u8 au8MenuMessage2[]="LED Programming Interface";
  static u8 au8MenuMessage3[]="Press 1 to program(or add) LED command sequence";
  static u8 au8MenuMessage4[]="Press 2 to show current USER program";
  static u8 au8MenuMessage5[]="Enter commands as LED-ONTIME-OFFTIME and press Enter";
  static u8 au8MenuMessage6[]="Time is in millisseconds, max 100 commands";
  static u8 au8MenuMessage18[]="ONTIME must be smaller than OFFTIME\n\r";
  static u8 au8MenuMessage7[]="LED colors: R, O, Y, G, C, B, P, W";
  static u8 au8MenuMessage8[]="Example: R-100-1000 (Red on at 100ms and off at 200ms)";
  static u8 au8MenuMessage23[]="Can use backspace,but do not BS left blank\n\r";
  static u8 au8MenuMessage9[]="Press Enter on blank to end";
  static u8 au8MenuMessage10[]="Invalid command:incorrect format. Please use L-ONTIME-OFFTIME";
  static u8 au8MenuMessage11[]="Current USER program:";
  static u8 au8MenuMessage12[]=": ";
  static u8 au8MenuMessage13[]="The number of total LED command: ";
  static u8 au8MenuMessage16[]="The line of command is:  ";
  static u8 au8MenuMessage14[]="LED  ON TIME   OFF TIME\n\r";
  static u8 au8MenuMessage15[]="---------------------------\n\r";
  static u8 au8MenuMessage17[]="NO current USER program and please press 1 to continue\n\r";
  static u8 au8MenuMessage19[]="Use BOTTON1,2,3 to let LED ON/OFF\n\r";
  static u8 au8MenuMessage20[]="If you want to create a new command list,press Y\n\r";
  static u8 au8MenuMessage21[]="And press N to go back to the menu\n\r";
  static u8 au8MenuMessage22[]="The new command list:  \n\r";
  static u8 au8ErrorMessage1[50]="Not the specified color\n\r";
  static u8 au8ErrorMessage2[50]="Correct format like -  -  (enter)\n\r\n\r";
  static u8 au8ErrorMessage3[50]="Time must be Number 0~9\n\r";
  static u8 au8ErrorMessage4[50]="Extra:Ontime must be smaller than Offtime\n\r";
  static u8 au8ErrorOutput1[50]={0};
  static u8 au8ErrorOutput2[50]={0};
  static u8 au8ErrorOutput3[50]={0};
  static u8 au8ErrorOutput4[50]={0};
    /*Interface  information*/
  /*
  static u8 au8MucisRight[]={523,587,659};
  static u8 au8MucisWrongt[]={659,587,532};
  */
  
  
  
  static u8 au8Input[1]; //Temporary storage
  static u8 u8State=0;   //State 1,2,3,4,5
  static u8 au8InputString[100]={0};
  static u8 u8Index=0;
  static bool bCheckFlag=FALSE;
  static bool bCommandCorrectFlag=TRUE;
  static u8 u8LineNumber=1;
  LedCommandType eUserCommand;
  static u32 u32UserOnTime=0;
  static u32 u32UserOffTime=0;
  static u8 u8CommandNumber=0;
  /*
  static u8 u8Delay=0;   //the list can not list completely,there should be a delay
  */
  
  static u8 u8i=0;
  /*
  static u16 u16MucisCounter=0;
  static u8 u8ToneIndex=0;
  */


  
  
  
 /*Menu  tell USER to choose 1 or 2*/ 
  if(u8State==0)
  {
    DebugPrintf(au8MenuMessage1);
    DebugLineFeed();
    DebugPrintf(au8MenuMessage2);
    DebugLineFeed();
    DebugPrintf(au8MenuMessage3);
    DebugLineFeed();
    DebugPrintf(au8MenuMessage4);
    DebugLineFeed();
    DebugPrintf(au8MenuMessage1);
    DebugLineFeed();
    u8State=1;
  }
  
  
  /*Menu if USER choose, give more information and  change state */
  if(u8State==1)
  {
   if(DebugScanf(au8Input)>0)
   {
     if(au8Input[0]=='1')
     {
      DebugLineFeed();
      DebugPrintf(au8MenuMessage5);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage6);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage18);
      DebugPrintf(au8MenuMessage7);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage8);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage23);
      DebugPrintf(au8MenuMessage9);
      DebugLineFeed();
      DebugPrintNumber(u8LineNumber);
      DebugPrintf(au8MenuMessage12);
      u8State=2;  
     }
     if(au8Input[0]=='2')
     {
      DebugLineFeed();
      DebugPrintf(au8MenuMessage11);
      DebugLineFeed();
      u8State=5;   
     }   
   }  
  }
  
  
 /*Input state and record what you input*/
  if(u8State==2)
  {
    if(DebugScanf(au8Input)>0)
    {
      if(au8Input[0]!= 13)
      {
        au8InputString[u8Index]=au8Input[0];
        u8Index++;
        bCheckFlag=TRUE;
      }
      if(au8Input[0]== 13)
      {
        if(bCheckFlag==TRUE)
        {
          u8State=3;          
        }
        if(bCheckFlag==FALSE)
        {
          u8State=5;     
        }        
      } 
    }
  }
  
  
 /* Check    if correct add to list,else tell user where wrong and rewrite(back to state2)*/ 
  if(u8State==3)
  {
    bCommandCorrectFlag=TRUE;//let cottect,and following are various errors 
    for(u8i=0;u8i<strlen((char*)au8InputString);u8i++)
    {
      if(au8InputString[u8i]==8)
      {
        u8Index=u8i;
        for(u8 i=0;i<(strlen((char*)au8InputString)-u8Index+1);i++)//+1 to prevent the last is backspace
        {
          au8InputString[u8Index-1+i]=au8InputString[u8Index+1+i];   
        } 
        u8i=0;
      }  
    }
    /* let backspace useful,but can not delete more than already input*/
    
    if((au8InputString[0]!='W')&&(au8InputString[0]!='P')&&(au8InputString[0]!='B')&&(au8InputString[0]!='C')&&(au8InputString[0]!='G')&&(au8InputString[0]!='Y')&&(au8InputString[0]!='O')&&(au8InputString[0]!='R'))
    {
      bCommandCorrectFlag=FALSE;// the first must be 'W','P','B','C','G','Y','O','R'
      strcpy((char*)au8ErrorOutput1,(char*)au8ErrorMessage1);
    }
    if(au8InputString[1]!='-')
    {
      bCommandCorrectFlag=FALSE;// the second must be '-'
      strcpy((char*)au8ErrorOutput2,(char*)au8ErrorMessage2);
    }
    for(u8 i=2;i<strlen((char*)au8InputString);i++)
    {
      if((au8InputString[i]!='-')&&((au8InputString[i]<48)||(au8InputString[i]>57)))
      {
        bCommandCorrectFlag=FALSE;// not number+'-'
        strcpy((char*)au8ErrorOutput2,(char*)au8ErrorMessage2);
        strcpy((char*)au8ErrorOutput3,(char*)au8ErrorMessage3);
      }    
    }
    for(u8 i=2;i<strlen((char*)au8InputString);i++)
    {
      if(au8InputString[i]=='-')
      {
        u8Index=i;
      }
    }
    if(u8Index==strlen((char*)au8InputString))
    {
      bCommandCorrectFlag=FALSE;// No offtime  like x-x-
      strcpy((char*)au8ErrorOutput2,(char*)au8ErrorMessage2);
    }
    for(u8 i=2;i<u8Index;i++)
    {
      if(au8InputString[i]=='-')
      {
        bCommandCorrectFlag=FALSE;//Prevent third or more '-'
        strcpy((char*)au8ErrorOutput3,(char*)au8ErrorMessage3);
      }
      else
      {
        u32UserOnTime=u32UserOnTime*10+au8InputString[i]-'0';          
      }   
    }
    for(u8 i=(u8Index+1);i<strlen((char*)au8InputString);i++)
    {        
      if(au8InputString[i]=='-')
      {
        bCommandCorrectFlag=FALSE;//Prevent third or more '-'
        strcpy((char*)au8ErrorOutput3,(char*)au8ErrorMessage3);
      }
      else
      {
        u32UserOffTime=u32UserOffTime*10+au8InputString[i]-'0';
      }
    }
    if((u32UserOffTime<u32UserOnTime)||(u32UserOffTime==u32UserOnTime))
    {
      bCommandCorrectFlag=FALSE;// On smaller than off
      strcpy((char*)au8ErrorOutput4,(char*)au8ErrorMessage4);
    }
    /* Check and find where wrong*/
     
    if(bCommandCorrectFlag==TRUE) 
    {
      u8LineNumber++;
      DebugLineFeed();    
      DebugPrintNumber(u8LineNumber);
      DebugPrintf(au8MenuMessage12);
      if(au8InputString[0]=='R')
      {
        eUserCommand.eLED=RED;
      }
      if(au8InputString[0]=='W')
      {
        eUserCommand.eLED=WHITE;
      }
      if(au8InputString[0]=='B')
      {
        eUserCommand.eLED=BLUE;
      }
      if(au8InputString[0]=='C')
      {
        eUserCommand.eLED=CYAN;
      }
      if(au8InputString[0]=='G')
      {
        eUserCommand.eLED=GREEN;
      }
      if(au8InputString[0]=='Y')
      {
        eUserCommand.eLED=YELLOW;
      }
      if(au8InputString[0]=='O')
      {
        eUserCommand.eLED=ORANGE;
      }
      if(au8InputString[0]=='P')
      {
        eUserCommand.eLED=PURPLE;
      }     
      eUserCommand.bOn=TRUE;
      eUserCommand.u32Time=u32UserOnTime;
      LedDisplayAddCommand(USER_LIST,&eUserCommand);
      u8CommandNumber++;
      eUserCommand.bOn=FALSE;
      eUserCommand.u32Time=u32UserOffTime;
      LedDisplayAddCommand(USER_LIST,&eUserCommand);
      u8CommandNumber++;
      u8Index=0;
      memset(au8InputString,'\0',100);
      u8State=2;
      bCheckFlag=FALSE;
      u32UserOffTime=0;
      u32UserOnTime=0;
    }
    /*write correct command in list*/
    
    if(bCommandCorrectFlag==FALSE)
    {
      DebugLineFeed();
      DebugPrintf(au8MenuMessage10);
      DebugLineFeed();
      if(au8ErrorOutput1[0]!=NULL)
      {
        DebugPrintf(au8ErrorOutput1);
        memset(au8ErrorOutput1,'\0',50);
      }
      if(au8ErrorOutput2[0]!=NULL)
      {
        DebugPrintf(au8ErrorOutput2); 
        memset(au8ErrorOutput2,'\0',50);
      }
      if(au8ErrorOutput3[0]!=NULL)
      {
        DebugPrintf(au8ErrorOutput3);
        memset(au8ErrorOutput3,'\0',50);
      }
      if(au8ErrorOutput4[0]!=NULL)
      {
        DebugPrintf(au8ErrorOutput4);
        memset(au8ErrorOutput4,'\0',50);
      }
      DebugPrintNumber(u8LineNumber);
      DebugPrintf(au8MenuMessage12);
      memset(au8InputString,'\0',100);
      u8State=2;
      u8Index=0;
      bCheckFlag=FALSE;
      u32UserOffTime=0;
      u32UserOnTime=0;
    }
    
    /*command wrong and return to write   */
  }
  
  
  /*display end, new or back to menu*/
  if(u8State==4)
  {
   if(DebugScanf(au8Input)>0)
    {
      if(au8Input[0]=='N')
      {
        u8State=0; 
        DebugLineFeed();
      }
      if(au8Input[0]=='Y')
      {
        u8LineNumber=1;
        u8CommandNumber=0;
        LedDisplayStartList();
        u8State=2;
        DebugLineFeed();
        DebugPrintf(au8MenuMessage22);
        DebugPrintNumber(u8LineNumber);
        DebugPrintf(au8MenuMessage12);
      }
    }     
  }
  
  /* Display the list,and change state*/
  if(u8State==5)
  {
    if(u8CommandNumber==0)
    {
      DebugPrintf(au8MenuMessage17);
      u8State=1;  
    }
    else
    {
      DebugPrintf(au8MenuMessage13);
      DebugPrintNumber(u8CommandNumber);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage16);
      DebugPrintNumber(u8CommandNumber/2);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage14);
      DebugPrintf(au8MenuMessage15);
      u8 i=0;
      while( LedDisplayPrintListLine(i++));// here should be delay,when the list is too long,can not list complitely
      DebugPrintf(au8MenuMessage15);       // recently 
      DebugPrintf(au8MenuMessage19);
      DebugPrintf(au8MenuMessage20);
      DebugPrintf(au8MenuMessage21);         
      u8State=4;     
    }
  }
  
 
  
  
  
 
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
/*if(bMenuFlag)//Menu
  {
    if(WasButtonPressed(BUTTON1))//Menu 1
    {
      ButtonAcknowledge(BUTTON1);
      bMenuFlag=FALSE;
      DebugPrintf(au8MenuMessage5);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage6);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage7);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage8);
      DebugLineFeed();
      DebugPrintf(au8MenuMessage9);
      DebugLineFeed();
      DebugPrintNumber(u8LineNumber);
      DebugPrintf(au8MenuMessage12);  
    }
    if(WasButtonPressed(BUTTON2))//Menu 2
    {
      ButtonAcknowledge(BUTTON2);
      bMenuFlag=FALSE;
      DebugPrintf(au8MenuMessage11);
      DebugLineFeed();
      bDisplayFlag=TRUE;  
    }
  }

  
  
  
  if(DebugScanf(au8Input)>0)//Input to sting
  {
    if(au8Input[0]!= 13)
    {       
      au8InputString[u8Index]=au8Input[0];
      u8Index++;
      bProgramFlag=TRUE;
      
    }
 
    if((au8Input[0]==13))
    {
      if(bProgramFlag)
      {
        bCommandCheckFlag=TRUE;
      }
      if(bProgramFlag==FALSE&&bCommandCorrectFlag==TRUE)
      {
        bEndProgramFlag=TRUE;
      }
    }
  }
  
  
  
  
  
  if(bCommandCheckFlag)//Check command
  {    
    if((au8InputString[0]=='R')||(au8InputString[0]=='O')||(au8InputString[0]=='Y')||(au8InputString[0]=='G')||(au8InputString[0]=='Y')||(au8InputString[0]=='o')||(au8InputString[0]=='C')||(au8InputString[0]!='B')||(au8InputString[0]=='P')||(au8InputString[0]=='W'))
    {
      if((au8InputString[1]=='-')&&(au8InputString[2]!='-'))
      {
        for(u8 i=2;i<strlen((char*)au8InputString);i++)
        {
          if(au8InputString[i]!='-')
          {
           bCommandCorrectFlag=FALSE;         
          }   
          if(au8InputString[i]=='-')
          {
            u8Index=i;
          }   
        }
        for(u8 i=2;i<u8Index;i++)
        {
          if(au8InputString[i]=='-')
          {
            bCommandCorrectFlag=FALSE;
          }
          else
          {
           u32UserOnTime=u32UserOnTime*10+au8InputString[i]-'0';          
          }
        }
        for(u8 i=u8Index;i<strlen((char*)au8InputString);i++)
        {        
          if(au8InputString[i]=='-')
          {
            bCommandCorrectFlag=FALSE;
          }
          else
          {
            u32UserOffTime=u32UserOffTime*10+au8InputString[i]-'0';
          }
        }
        
        
        for(u8 i=2;i<strlen((char*)au8InputString);i++)
        {
          if((au8InputString[i]=='-')||(au8InputString[i]>47)||(au8InputString[i]<58))
          {
            if(u32UserOffTime>u32UserOnTime)
            {
              bCommandCorrectFlag=TRUE;
            }
            else 
            {
              bCommandCorrectFlag=FALSE;
            }    
          }
        }
      }
      else
      {
        bCommandCorrectFlag=FALSE;
      }
    }
    else
    {
      bCommandCorrectFlag=FALSE;
    }
    if(bCommandCorrectFlag)//Command right and store
    {
     
      u8LineNumber++;
      DebugLineFeed();
      bCommandCheckFlag=FALSE;
      bProgramFlag=FALSE;    
      u8Index=0;
      DebugPrintNumber(u8LineNumber);
      DebugPrintf(au8MenuMessage12);
      if(au8InputString[0]=='R')
      {
        eUserCommand.eLED=RED;
      }
      if(au8InputString[0]=='W')
      {
        eUserCommand.eLED=WHITE;
      }
      if(au8InputString[0]=='B')
      {
        eUserCommand.eLED=BLUE;
      }
      if(au8InputString[0]=='C')
      {
        eUserCommand.eLED=CYAN;
      }
      if(au8InputString[0]=='G')
      {
        eUserCommand.eLED=GREEN;
      }
      if(au8InputString[0]=='Y')
      {
        eUserCommand.eLED=YELLOW;
      }
      if(au8InputString[0]=='O')
      {
        eUserCommand.eLED=ORANGE;
      }
      
      eUserCommand.bOn=TRUE;
      eUserCommand.u32Time=u32UserOnTime;
      LedDisplayAddCommand(USER_LIST,&eUserCommand);
      u8CommandNumber++;
      eUserCommand.bOn=FALSE;
      eUserCommand.u32Time=u32UserOffTime;
      LedDisplayAddCommand(USER_LIST,&eUserCommand);
      u8CommandNumber++;
      memset(au8InputString,'\0',100);
    }
    if(bCommandCorrectFlag==FALSE)//Command wrong and go to rewrite
    {
      u8Index=0;
      bProgramFlag=FALSE; 
      bCommandCheckFlag=FALSE;
      DebugLineFeed();
      DebugPrintf(au8MenuMessage10);
      DebugLineFeed();
      DebugPrintNumber(u8LineNumber);
      DebugPrintf(au8MenuMessage12);
      memset(au8InputString,'\0',100);
    }
  }
  
  
  
  if(bEndProgramFlag)//?
  {
    LedDisplayStartList();
  }
  if(bDisplayFlag)
  {
    DebugPrintf(au8MenuMessage13);
    DebugLineFeed();
   
  }
          */                              
                                        
                                       
        
        
       
        
        
        
      
      
     
       
  
  
  
  
 
    
    
    
    
    
    
    
    /* 
    if(DebugScanf(au8Input)>0)
    { 
      if(bInput0Flag)
      {
        if(au8Input[0]==13)
        {
          bEndProgramFlag=TRUE;
          bProgramFlag=FALSE;
          bInput1Flag=FALSE;
        }            
      }      
      if(bInput1Flag)
      {        
        if(au8Input[0]=='R'||'O'||'Y'||'G'||'C'||'B'||'P'||'W')
        {
          au8InputStingPart1[u8Index]=au8Input[0];
          u8Index++;
          bInput0Flag=FALSE;
        }
        if(au8Input[0]=='-')
        {
          bInput1Flag=FALSE;
          bInput2Flag=TRUE;
          u8Index=0;        
        }
        else
        {
          au8InputStingOther[u8Index2]=au8Input[0];
          u8Index2++;      
        }
      }
      if(bInput2Flag)
      {
        if(au8Input[0]<=57&au8Input[0]>=48)
        {
          au8InputStingPart2[u8Index]=au8Input[0];
          u8Index++;
        }
        if(au8Input[0]=='-')
        {
          bInput2Flag=FALSE;
          bInput3Flag=TRUE;
          u8Index=0;         
        }
        else
        {
          au8InputStingOther[u8Index2]=au8Input[0];
          u8Index2++;      
        }
      }
      if(bInput3Flag)
      {
        if(au8Input[0]<=57&&au8Input[0]>=48)
        {
          au8InputStingPart3[u8Index]=au8Input[0];
          u8Index++;      
        }
        else
        {
          au8InputStingOther[u8Index2]=au8Input[0];
          u8Index2++;      
        }
      }
      if(bInput3Flag||bInput2Flag||bInput1Flag)
      {
        if(au8Input[0]==13)
        {
          bInput0Flag=FALSE;
          bInput1Flag=FALSE;
          bInput2Flag=FALSE;
          bInput3Flag=FALSE;
          bCommandCheckFlag=TRUE;
        }     
      }     
    }
    
    if(bCommandCheckFlag)
    {
      for(u8 i=0;i<sizeof(au8InputStingPart2);i++)
        {
          u32UserOnTime=u32UserOnTime*10+au8InputStingPart2[i]-'0';  
        }
        for(u8 i=0;i<sizeof(au8InputStingPart3);i++)
        {
          u32UserOffTime=u32UserOffTime*10+au8InputStingPart3[i]-'0';  
        }        
            
      if(bCommandCorrectFlag==FALSE)
      {
        DebugLineFeed();
        DebugPrintf(au8MenuMessage10);
        DebugLineFeed();
        DebugPrintNumber(u8LineNumber);
        DebugPrintf(au8MenuMessage12);
        bInput1Flag=TRUE;
        bCommandCheckFlag=FALSE;
      }
      if(bCommandCorrectFlag)  //д������
      {
        u8LineNumber++;
        DebugLineFeed();
        DebugPrintNumber(u8LineNumber);
        DebugPrintf(au8MenuMessage12);
        if(au8InputStingPart1[0]=='R')
        {
          u8UserLedName=RED;
        }
        if(au8InputStingPart1[0]=='W')
        {
          u8UserLedName=WHITE;
        }
        if(au8InputStingPart1[0]=='B')
        {
          u8UserLedName=BLUE;
        }
        if(au8InputStingPart1[0]=='C')
        {
          u8UserLedName=CYAN;
        }
        if(au8InputStingPart1[0]=='G')
        {
          u8UserLedName=GREEN;
        }
        if(au8InputStingPart1[0]=='Y')
        {
          u8UserLedName=YELLOW;
        }
        if(au8InputStingPart1[0]=='O')
        {
          u8UserLedName=ORANGE;
        }
        
        eUserCommand.eLED=u8UserLedName;
        eUserCommand.bOn=TRUE;
        eUserCommand.u32Time=u32UserOnTime;
        eUserCommand.eCurrentRate=LED_PWM_100;
        LedDisplayAddCommand(USER_LIST,&eUserCommand);
        u8CommandNumber++;
        eUserCommand.eLED=u8UserLedName;
        eUserCommand.bOn=FALSE;
        eUserCommand.u32Time=u32UserOffTime;
        eUserCommand.eCurrentRate=LED_PWM_0;
        LedDisplayAddCommand(USER_LIST,&eUserCommand);
        u8CommandNumber++;       
        bInput1Flag=TRUE;
        bInput0Flag=TRUE;
        bCommandCheckFlag=FALSE;
      }
    }  
  }
  if(bEndProgramFlag)
  {
    LedDisplayStartList();
  }
  if(bDisplayFlag)
  {
    DebugPrintf(au8MenuMessage12);
    DebugLineFeed();
    while( LedDisplayPrintListLine(u8ListEntry++) );
  }
*/
  
  
  
 
    
    
    
   
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
  
  
}/* end UserApp1SM_Idle() */
                      
            
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
