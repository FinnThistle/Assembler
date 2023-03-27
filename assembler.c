/*


*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#define NULL ((void *)0)

map compMap;
map jmpMap;
map dstMap;
map lblvrbMap;
int RAM_COUNT = 16;
int OUTPUT_LINE_COUNT = 0;
int INPUT_LINE_COUNT = 0;
int INPUT_OUTPUT_DIF = 0;
void setMaps()
{
  // create a map covering this part of binary
  // string for c instruction
  // [111][ccccccc]000000
  compMap = createMap(28);
  // a=0
  insertKey(compMap, "0", "1110101010");
  insertKey(compMap, "1", "1110111111");
  insertKey(compMap, "-1", "1110111010");
  insertKey(compMap, "D", "1110001100");
  insertKey(compMap, "A", "1110110000");
  insertKey(compMap, "!D", "1110001101");
  insertKey(compMap, "!A", "1110110001");
  insertKey(compMap, "-D", "1110001111");
  insertKey(compMap, "-A", "1110110011");
  insertKey(compMap, "D+1", "1110011111");
  insertKey(compMap, "A+1", "1110110111");
  insertKey(compMap, "D-1", "1110001110");
  insertKey(compMap, "A-1", "1110110010");
  insertKey(compMap, "D+A", "1110000010");
  insertKey(compMap, "D-A", "1110010011");
  insertKey(compMap, "A-D", "1110000111");
  insertKey(compMap, "D&A", "1110000000");
  insertKey(compMap, "D|A", "1110010101");
  // a=1
  insertKey(compMap, "M", "1111110000");
  insertKey(compMap, "!M", "1111110001");
  insertKey(compMap, "-M", "1111110011");
  insertKey(compMap, "M+1", "1111110111");
  insertKey(compMap, "M-1", "1111110010");
  insertKey(compMap, "D+M", "1111000010");
  insertKey(compMap, "D-M", "1111010011");
  insertKey(compMap, "M-D", "1111000111");
  insertKey(compMap, "D&M", "1111000000");
  insertKey(compMap, "D|M", "1111010101");

  jmpMap = createMap(8);
  insertKey(jmpMap, "null", "000");
  insertKey(jmpMap, "JGT", "001");
  insertKey(jmpMap, "JEQ", "010");
  insertKey(jmpMap, "JGE", "011");
  insertKey(jmpMap, "JLT", "100");
  insertKey(jmpMap, "JNE", "101");
  insertKey(jmpMap, "JLE", "110");
  insertKey(jmpMap, "JMP", "111");

  dstMap = createMap(8);
  insertKey(dstMap, "null", "000");
  insertKey(dstMap, "M", "001");
  insertKey(dstMap, "D", "010");
  insertKey(dstMap, "MD", "011");
  insertKey(dstMap, "A", "100");
  insertKey(dstMap, "AM", "101");
  insertKey(dstMap, "AD", "110");
  insertKey(dstMap, "AMD", "111");
  lblvrbMap = createMap(1500);
  insertKey(lblvrbMap, "SCREEN",    "0100000000000000");
  insertKey(lblvrbMap, "KBD",       "0110000000000000");
  insertKey(lblvrbMap, "R0",        "0000000000000000");
  insertKey(lblvrbMap, "SP",        "0000000000000000");
  insertKey(lblvrbMap, "R1",        "0000000000000001");
  insertKey(lblvrbMap, "LCL",       "0000000000000001");
  insertKey(lblvrbMap, "R2",        "0000000000000010");
  insertKey(lblvrbMap, "ARG",       "0000000000000010");
  insertKey(lblvrbMap, "R3",        "0000000000000011");
  insertKey(lblvrbMap, "THIS",      "0000000000000011");
  insertKey(lblvrbMap, "R4",        "0000000000000100");
  insertKey(lblvrbMap, "THAT",      "0000000000000100");
  insertKey(lblvrbMap, "R5",        "0000000000000101");
  insertKey(lblvrbMap, "R6",        "0000000000000110");
  insertKey(lblvrbMap, "R7",        "0000000000000111");
  insertKey(lblvrbMap, "R8",        "0000000000001000");
  insertKey(lblvrbMap, "R9",        "0000000000001001");
  insertKey(lblvrbMap, "R10",       "0000000000001010");
  insertKey(lblvrbMap, "R11",       "0000000000001011");
  insertKey(lblvrbMap, "R12",       "0000000000001100");
  insertKey(lblvrbMap, "R13",       "0000000000001101");
  insertKey(lblvrbMap, "R14",       "0000000000001110");
  insertKey(lblvrbMap, "R15",       "0000000000001111");
}
// function will take an empty string and a num and change the string
// to a binary representation of the num value
void intStringToBinary(char *str, int n)
{
  // 0  0  0  0  0  0  0  0
  // 128 64 32 16 8  4  2  1
  int binaryNum[16];
  char binaryStr[16];
  // counter for binaryNum
  int i = 0;
  while (n > 0)
  {
    // storing remainder in array
    binaryNum[i] = n % 2;
    n = n / 2;
    i++;
  }
  // reverse order and store in string form
  int f = 0;
  int length = 0;
  for (int j = i - 1; j >= 0; j--)
  {
    char currentChar;
    if (binaryNum[j] == 0)
    {
      currentChar = '0';
    }
    else if (binaryNum[j] == 1)
    {
      currentChar = '1';
    }
    binaryStr[f] = currentChar;
    length++;
    f++;
  }
  // end line and null terminate

  binaryStr[f] = 0;

  //"fill" digits with zero in front of first 1 in string
  char fillZeroes[16 - length];
  int x = 0;
  while (x < 16 - length)
  {
    fillZeroes[x] = '0';
    x++;
  }
  fillZeroes[x] = 0; // null term

  // concat strings together
  strcat(fillZeroes, binaryStr);
  // copy to output string
  strcpy(str, fillZeroes);
}

// returns true if char is a number, false otherwise
int isNum(char c)
{
  if (c == '0' ||
      c == '1' ||
      c == '2' ||
      c == '3' ||
      c == '4' ||
      c == '5' ||
      c == '6' ||
      c == '7' ||
      c == '8' ||
      c == '9')
  {
    return 1;
  }
  return 0;
}
void commandA(char *line, FILE *file_copy)
{
  // gets the length of the line passed in
  int combed_buffer = 70;
  // set the length of the line passed in to the combed line
  char combed[combed_buffer];
  int combed_i = 0;
  for (int i = 1; i < combed_buffer; i++)
  {
    if (isspace(line[i]))
    {
      break;
    }
    combed[combed_i] = line[i];
    combed_i++;
  }
  combed[combed_i] = 0;
  // now "combed" is the string without the "@" character

  // conditional to check if the first char is a number

  if (isNum(combed[0]))
  {
    char binaryString[17]; // 16 elements and room for '\0' '\n' characters
    intStringToBinary(binaryString, atoi(combed));
    fputs(binaryString, file_copy);
    

  }
  else
  {
    // look it up in the map
    // if it exist in the map
    // fput that map value
    if (containsKey(lblvrbMap, combed) != -1)
    {
      char *value = lookupKey(lblvrbMap, combed);
      fputs(value, file_copy);
    }
    // else its a variable that hasn't been created yet
    // so find the value of the RAM adress it needs to go to
    // translate that to binary
    // fput that binary string AND store it to the map
    else
    {
      char binaryString[17]; // 16 elements and room for '\0' '\n' characters
      intStringToBinary(binaryString, RAM_COUNT);
    //   if (strcmp("0000000000011011", binaryString) == 0) //<--Hard code (temp solution)
    // {
    //     fputs("0100100110101011",file_copy);
    //     return;
    // }
      RAM_COUNT++;
      insertKey(lblvrbMap, combed, binaryString);
      fputs(binaryString, file_copy);
    }
  }
  OUTPUT_LINE_COUNT++;
}

void formatDestPart(char *line, char *out)
{
  const char equalChar = 61;
  const char semiColonChar = 59;
  if (strchr(line, semiColonChar) != NULL)
  {
    strcpy(out, "null");
    return;
  }
  int i = 0;
  while (line[i] != equalChar && line[i] != 0)
  {
    out[i] = line[i];
    i++;
  }
  // end string
  out[i] = 0;
}

// changes line to format Comp part strings and outputs 1 if line contains an equal sign
int formatCompPart(char *line, char *out)
{
  const char equalChar = 61;
  const char semiColonChar = 59;
  // check if line has an equal sign
  int hasEqual = 0;
  if (strchr(line, equalChar) != NULL)
  {
    hasEqual = 1;
  }

  // comppart if line has equal
  if (hasEqual)
  {
    // find index of '=', once found use next index of line to store to start of outAE
    int i = 0;
    int found = 0;
    char combed[70];
    int combed_i = 0;
    while (line[i] != 0)
    {
      if (line[i] == equalChar)
      {
        found = 1;
      }

      if (found && line[i] != equalChar)
      {
        combed[combed_i] = line[i];
        combed_i++;
      }
      i++;
    }
    // out after equal is part after
    combed[combed_i] = 0;
    strcpy(out, combed);
    return 1;
  }
  // comppart if line has semi colon
  else
  {
    int j = 0;
    while (line[j] != semiColonChar && line[j] != 0)
    {
      out[j] = line[j];
      j++;
    }
    // end string
    out[j] = 0;
    return 0;
  }
}

int formatJumpPart(char *line, char *out) // returns 1  and sets out to value after ';' if has ';'
// otherwise returns 0 and sets out to null
{
  const char semiColonChar = 59;
  // check if line has an semi colon sign

  // comppart if line has sc
  if (strchr(line, semiColonChar) != NULL)
  {
    // find index of ';', once found use next index of line to store to start of out
    int i = 0;
    int found = 0;
    char combed[12];
    int combed_i = 0;
    while (line[i] != 0)
    {
      if (line[i] == semiColonChar)
      {
        found = 1;
      }

      if (found && line[i] != semiColonChar)
      {
        combed[combed_i] = line[i];
        combed_i++;
      }
      i++;
    }
    // out after equal is part after
    combed[combed_i] = 0;
    strcpy(out, combed);
    return 1;
  }
  else
  {
    strcpy(out, "null");
    return 0;
  }
}
void commandC(char *line, FILE *file_copy)
{
  // destpart +++++++++++++++++++++++
  // string before the equal sign
  char dest[50];
  formatDestPart(line, dest);

  // jmppart ++++++++++++++++++++++++
  // string after the semi colon
  char jump[50];
  int hasSemiColon = formatJumpPart(line, jump);
  // comppart ++++++++++++++++++++++
  // string after the equal sign
  char comp[50];
  // sets comp to either string part after equal sign or before semi colon
  // depending on the existance of a semi colon
  int hasEqual = formatCompPart(line, comp);

  char *compBinaryString = lookupKey(compMap, comp);
  char *jumpBinaryString = lookupKey(jmpMap, jump);
  char *destBinaryString = lookupKey(dstMap, dest);

  // printf("comp|%s|=|%s|\n", comp, compBinaryString);
  // printf("dest|%s|=|%s|\n", dest, destBinaryString);
  // printf("jump|%s|=|%s|\n", jump, jumpBinaryString);
  // printf("=======\n");

  fputs(compBinaryString, file_copy);
  fputs(destBinaryString, file_copy);
  fputs(jumpBinaryString, file_copy);
  OUTPUT_LINE_COUNT++;
}

void finalPrint(char *line, FILE *file_copy)
{
  if (line[0] != '\0')
  {
    if (line[0] == '@')
    {
      commandA(line, file_copy);
      fputs("\n", file_copy);
    }
    else
    {
      // handle this
      commandC(line, file_copy);
      fputs("\n", file_copy);
    }
  }
}
// takes an input and outputs a char without parenthesis on the end (for labels)
void withoutParens(char *in, char *out)
{
  int j = 1;
  while (in[j] != ')')
  {
    out[j - 1] = in[j];
    j++;
  }
  // end string
  out[j - 1] = 0;
}

void combline(FILE *file, char *out, char *buffer, int buffer_size)
{
  char combed[buffer_size];
  int combed_i = 0;
  for (int i = 0; i < buffer_size; i++)
  {
    // if the line is a space or '/' do not add it
    // if line is a space don't add it
    // else its a '/n' so send line to print and go to next line
    //  if the line is a '/' add a '/0' to the end of the line and send it to the print
    //  otherwise

    if (isspace(buffer[i]))
    {
      if (buffer[i] != ' ') // end of the line
      {
        combed[combed_i] = 0; // null terminate the line and go to next line
        break;
      }
    }
    else if (buffer[i] == '/')
    {
      combed[combed_i] = 0; // null terminate the line and go to next line
      break;
    }
    else
    {
      combed[combed_i] = buffer[i]; // its a valid character! add it it to combed and keep going
      combed_i++;
    }
  }
  strcpy(out, combed);
}
void secondPass(FILE *file, FILE *file_copy)
{
  rewind(file); // set file pointer back to the top
  int buffer_size = 100;
  char buffer[buffer_size];
  while (1)
  {
    INPUT_LINE_COUNT++;
    fgets(buffer, buffer_size, file);
    if (feof(file))
    {
      break;
    }
    char combed[buffer_size];
    combline(file, combed, buffer, buffer_size);
    // initiate printing logic
    if(OUTPUT_LINE_COUNT == 18523)
    {
        // printf("Promblem line of input = %d\n",INPUT_LINE_COUNT );
        // printf("Promblem line of output = %d\n",OUTPUT_LINE_COUNT);
        // printf(combed);
        // printf("\n");
    }
    
    if (combed[0] != '(')
    {
      finalPrint(combed, file_copy);
    }
    // break if at the end of the file
  }
}

void firstPass(FILE *file)
{
  int buffer_size = 100;
  char buffer[buffer_size];
  int linecount = 0;
  while (1)
  {
    fgets(buffer, buffer_size, file);
    
    if (feof(file))
    {
      break;
    }
    
    char combed[buffer_size];
    combline(file, combed, buffer, buffer_size);
    // figure out what line to store the labels to:
    if ((!isspace(combed[0])) && (combed[0] != 0 && combed[0] != '('))
    {
      linecount++;
    }
    else if (combed[0] == '(')
    {
      // get label without parenthesis
      char key[30];
      withoutParens(combed, key);
      char binString[17];
      intStringToBinary(binString, linecount);
      // store binary string of the next line in map with the key as the label
      insertKey(lblvrbMap, key, binString);
    }

    // break if at the end of the file
  }
}
int main(int argc, char **file_name)
{
  FILE *file = fopen(file_name[1], "r");
  FILE *file_copy = fopen(file_name[2], "w");
  setMaps();

  if (file == NULL || file_copy == NULL)
  {
    perror("Error: a file is NULL.");
    return (-1);
  }
  firstPass(file);
  secondPass(file, file_copy);
  
//   printf("Inputline count: %d",INPUT_LINE_COUNT);
//   printf("\n");
//   printf("Outputline count: %d",OUTPUT_LINE_COUNT);
  freeMap(compMap);
  freeMap(jmpMap);
  freeMap(dstMap);
  freeMap(lblvrbMap);
  fclose(file);
  fclose(file_copy);
  return 0;
}
