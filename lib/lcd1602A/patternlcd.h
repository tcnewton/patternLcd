#include <Arduino.h>
#include <LiquidCrystal.h>

const int en = 2, rs = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int btt_fwd = 11, btt_ent = 12, btt_rwd = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


int tokenPos = 0;
int tkPosX = 0;
int Intcumsum = 0;

/* #################################
   #     screen variables          #
   #################################*/
bool flag_title = true;
bool flag_zeroDez = false;

/*
input parameters: <int> x position, <int> y position, <int> max <int> min, <char> nameOfVar, <Template> value variable, <bool> ReadWrite
*/
template <class T>
struct intVarScreen // declare a structure
{
  int x;   // position x of string varName
  int y;   // position y of string varName
  int max; // max number of var1
  int min; // min number of var1
  const char *varName;
  T *value;
  bool readwrite;
};

/*##############################
  # variables defined by user  #
  ##############################*/

// setup variables with initial values
int TestValue = 3;
int TestValue2 = 2;
int bolValue = 4;
int ttyValue = 5;
bool chuva = false;
bool muda = true;
float dayTemp = 22.3;
// defining int variables
const int NumberOfIntFrames = 3;
int Screen_NofIntVar[NumberOfIntFrames] = {2, 2, 1}; // each array element define number of variables on screen
intVarScreen<int> varInt[5] = {
    {0, 1, 5, 3, "temp: ", &TestValue, true},
    {8, 1, 21, 2, "umid: ", &TestValue2, true},
    {0, 1, 5, 4, "bol: ", &bolValue, false},
    {7, 1, 7, 5, "tty: ", &ttyValue, true},
    {0, 1, 5, 4, "bol: ", &bolValue, true}};                                        // declaring array of type structure
const char IntTitle[NumberOfIntFrames][16] = {{"W3E"}, {"Teste"}, {"Bingo arena"}}; // title of the screen

// defining bool variables
const int NumberOfboolFrames = 1;

intVarScreen<bool> varBool[2] = {
    {0, 1, 1, 0, "prev: ", &chuva, true},
    {8, 1, 1, 0, "mudas: ", &muda, false}};
int Screen_NofBoolVar[NumberOfboolFrames] = {2};
const char boolTitle[NumberOfboolFrames][16] = {{"bool screen"}};

// defining float variables
const int NumberOfFloatFrames = 1;
int Screen_NofFloatVar[NumberOfFloatFrames] = {1};

intVarScreen<float> varFloat[1] = {
  {0,1,25,1,"day Temp:",&dayTemp,true}};

const char floatTitle[NumberOfFloatFrames][16] = {{"float screen"}};
// end variables defined by user

const int totalSum = NumberOfIntFrames + NumberOfboolFrames + NumberOfFloatFrames;

int ScreenCurr = 0;
int typeCurrentTela = 0;
int *ptrArrayFrames;
int typeofstructvar; // 0: integer struct 1:bool struct 2:float struct

int CumulativeSum(int telacurrent, int *arr_screen_nofvar) // put array here
{
  int count = 0;
  for (int i = 0; i < telacurrent; i++)
  {
    count = count + arr_screen_nofvar[i]; // change array
  }
  return count;
};

void defTypeScreenCurr(int *curr, int *sumVariables)
{

  if (ScreenCurr < NumberOfIntFrames)
  {
    *curr = ScreenCurr;
    ptrArrayFrames = Screen_NofIntVar;
    *sumVariables = CumulativeSum(*curr, ptrArrayFrames);
    typeofstructvar = 0;
    // Serial.println(*(ptrArrayFrames + *curr));
  }
  else if ((ScreenCurr >= NumberOfIntFrames) and (ScreenCurr < NumberOfIntFrames + NumberOfboolFrames))
  {
    *curr = ScreenCurr - NumberOfIntFrames;
    ptrArrayFrames = Screen_NofBoolVar;
    *sumVariables = CumulativeSum(*curr, ptrArrayFrames);
    typeofstructvar = 1;
  }
  else
  {
    *curr = ScreenCurr - (NumberOfIntFrames + NumberOfboolFrames);
    typeofstructvar = 2;
  }
}

void printStructInt()
{
  defTypeScreenCurr(&typeCurrentTela, &Intcumsum);
  if (flag_title)
  {    
    if (typeofstructvar == 0)
    {
      int titlePos = (16 - strlen(IntTitle[typeCurrentTela])) / 2;
      lcd.clear();
      lcd.setCursor(titlePos, 0);
      lcd.print(IntTitle[typeCurrentTela]);
    }
    else if (typeofstructvar == 1)
    {
      int titlePos = (16 - strlen(boolTitle[typeCurrentTela])) / 2;
      lcd.clear();
      lcd.setCursor(titlePos, 0);
      lcd.print(boolTitle[typeCurrentTela]);
    }
    else
    {
      int titlePos = (16 - strlen(floatTitle[typeCurrentTela])) / 2;
      lcd.clear();
      lcd.setCursor(titlePos, 0);
      lcd.print(floatTitle[typeCurrentTela]);
    }
  }
  if (typeofstructvar == 0)
  {
    for (int i = Intcumsum; i < Intcumsum + Screen_NofIntVar[typeCurrentTela]; i++)
    {
      lcd.setCursor(varInt[i].x, varInt[i].y);
      lcd.print(varInt[i].varName);
      lcd.print(*(varInt[i].value));
    }
  }
  else if (typeofstructvar == 1)
  {
    for (int i = Intcumsum; i < Intcumsum + Screen_NofBoolVar[typeCurrentTela]; i++)
    {
      lcd.setCursor(varBool[i].x, varBool[i].y);
      lcd.print(varBool[i].varName);
      lcd.print(*(varBool[i].value));
    }
  }
  else
  {
    for (int i = Intcumsum; i < Intcumsum + Screen_NofFloatVar[typeCurrentTela]; i++)
    {
      lcd.setCursor(varFloat[i].x, varFloat[i].y);
      lcd.print(varFloat[i].varName);
      lcd.print(*(varFloat[i].value));
    }
  }

  flag_title = false;
}




void setTokenPosition()
{
  if (tokenPos == 0)
  {
    lcd.setCursor(0, 0);
    lcd.blink();
  }
  else
  {
    if (typeofstructvar == 0)
    {
      tkPosX = strlen(varInt[Intcumsum + tokenPos - 1].varName) + varInt[Intcumsum + tokenPos - 1].x;
      lcd.setCursor(tkPosX - 1, varInt[Intcumsum + tokenPos - 1].y);
    }
    else if (typeofstructvar == 1)
    {
      tkPosX = strlen(varBool[Intcumsum + tokenPos - 1].varName) + varBool[Intcumsum + tokenPos - 1].x;
      lcd.setCursor(tkPosX - 1, varBool[Intcumsum + tokenPos - 1].y);
    }
    else
    {
     tkPosX = strlen(varFloat[Intcumsum + tokenPos - 1].varName) + varFloat[Intcumsum + tokenPos - 1].x;
      lcd.setCursor(tkPosX - 1, varFloat[Intcumsum + tokenPos - 1].y);
    }
    lcd.blink();
  }
}

void incStructValue()
{
  if (tokenPos == 0)
  {
    ++ScreenCurr;
    if (ScreenCurr == totalSum)
    {
      ScreenCurr = 0;
    }
    defTypeScreenCurr(&typeCurrentTela, &Intcumsum);
    flag_title = true;
    printStructInt();
  }
  else
  {
    if (typeofstructvar == 0)
    {
      ++(*varInt[Intcumsum + tokenPos - 1].value);
      if (*(varInt[Intcumsum + tokenPos - 1].value) > varInt[Intcumsum + tokenPos - 1].max)
      {
        *(varInt[Intcumsum + tokenPos - 1].value) = varInt[Intcumsum + tokenPos - 1].min;
        flag_zeroDez = true;
      }
      lcd.setCursor(tkPosX, varInt[Intcumsum + tokenPos - 1].y);
      lcd.print(*(varInt[Intcumsum + tokenPos - 1].value));
      if (flag_zeroDez)
      {
        lcd.print(" ");
        flag_zeroDez = false;
      }
    }
    else if (typeofstructvar == 1)
    {
      *varBool[Intcumsum + tokenPos - 1].value = !*varBool[Intcumsum + tokenPos - 1].value;
      lcd.setCursor(tkPosX, varBool[Intcumsum + tokenPos - 1].y);
      lcd.print(*(varBool[Intcumsum + tokenPos - 1].value));
    }
    else
    {
      *varFloat[Intcumsum + tokenPos - 1].value+=.2;
      if (*(varFloat[Intcumsum + tokenPos - 1].value) > varFloat[Intcumsum + tokenPos - 1].max)
      {
        *(varFloat[Intcumsum + tokenPos - 1].value) = varFloat[Intcumsum + tokenPos - 1].min;
        flag_zeroDez = true;
      }
      lcd.setCursor(tkPosX, varFloat[Intcumsum + tokenPos - 1].y);
      lcd.print(*(varFloat[Intcumsum + tokenPos - 1].value));
      if (flag_zeroDez)
      {
        lcd.print(" ");
        flag_zeroDez = false;
      }
    }
  }
  setTokenPosition();
}

void decStructValue()
{
  if (tokenPos == 0)
  {
    --ScreenCurr;
    if (ScreenCurr < 0)
    {
      ScreenCurr = totalSum - 1;
    }
    defTypeScreenCurr(&typeCurrentTela, &Intcumsum);
    flag_title = true;
    printStructInt();
  }
  else
  {
    if (typeofstructvar == 0)
    {
      --(*varInt[Intcumsum + tokenPos - 1].value);
      if (*(varInt[Intcumsum + tokenPos - 1].value) < varInt[Intcumsum + tokenPos - 1].min)
      {
        *(varInt[Intcumsum + tokenPos - 1].value) = varInt[Intcumsum + tokenPos - 1].max;
      }
      if ((varInt[Intcumsum + tokenPos - 1].max >= 10) and (*(varInt[Intcumsum + tokenPos - 1].value) == 9))
      {
        flag_zeroDez = true;
      }
      lcd.setCursor(tkPosX, varInt[Intcumsum + tokenPos - 1].y);
      lcd.print(*(varInt[Intcumsum + tokenPos - 1].value));
      if (flag_zeroDez)
      {
        lcd.print(" ");
        flag_zeroDez = false;
      }
    }
    else if (typeofstructvar == 1)
    {
      *varBool[Intcumsum + tokenPos - 1].value = !*varBool[Intcumsum + tokenPos - 1].value;
      lcd.setCursor(tkPosX, varBool[Intcumsum + tokenPos - 1].y);
      lcd.print(*(varBool[Intcumsum + tokenPos - 1].value));
    }
    else
    {
      *varFloat[Intcumsum + tokenPos - 1].value-=.2;
      if (*(varFloat[Intcumsum + tokenPos - 1].value) < varFloat[Intcumsum + tokenPos - 1].min)
      {
        *(varFloat[Intcumsum + tokenPos - 1].value) = varFloat[Intcumsum + tokenPos - 1].max;
      }
      if ((varFloat[Intcumsum + tokenPos - 1].max >= 10) and (*(varFloat[Intcumsum + tokenPos - 1].value) == 9))
      {
        flag_zeroDez = true;
      }
      lcd.setCursor(tkPosX, varFloat[Intcumsum + tokenPos - 1].y);
      lcd.print(*(varFloat[Intcumsum + tokenPos - 1].value));
      if (flag_zeroDez)
      {
        lcd.print(" ");
        flag_zeroDez = false;
      }
    }
  }
  setTokenPosition();
}

void changeTokenPos()
{
  defTypeScreenCurr(&typeCurrentTela, &Intcumsum);
  if (typeofstructvar == 0)
  {
    do
    {
      ++tokenPos;
      if (tokenPos > *(ptrArrayFrames + typeCurrentTela))
        tokenPos = 0;
    } while (!(varInt[Intcumsum + tokenPos - 1].readwrite) and (tokenPos != 0));
  }
  else if (typeofstructvar == 1)
  {
    do
    {
      ++tokenPos;
      if (tokenPos > *(ptrArrayFrames + typeCurrentTela))
        tokenPos = 0;
    } while (!(varBool[Intcumsum + tokenPos - 1].readwrite) and (tokenPos != 0));
  }
  else
  {
    
    do
    {
      ++tokenPos;
      if (tokenPos > *(ptrArrayFrames + typeCurrentTela))
        tokenPos = 0;
    } while (!(varFloat[Intcumsum + tokenPos - 1].readwrite) and (tokenPos != 0));
  }
  setTokenPosition();
}

