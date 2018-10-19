#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>

// REMEMBER TO FREE MEMORY
//Helper function to reverse bytes in a short
char* reverse_byte_order( unsigned short* shorts, int length ) {
  char* helper = "help";
  FILE* outFS = fopen(helper, "wb");
  printf("LENF: %d\n", length);
  fwrite(shorts, length * 2, 1, outFS);
  fclose(outFS);
  FILE* inFS = fopen(helper, "rb");
  fseek(inFS, 0, SEEK_END); 
  long size = ftell(inFS);
  rewind(inFS);
  char* bytes = (char*)malloc(size + 1); //Allocated buffer of the size of the file
  char* betterBytes = (char*)malloc(size + 1);
  fread(bytes, size, 1, inFS);
  for (int i = 0; i < size; i+=2) {
    printf("bytes[i] = <0x%x>\tbytes[i+1] = <0x%x>\n", bytes[i], bytes[i + 1]);
    betterBytes[i] = bytes[i + 1];
    betterBytes[i + 1] = bytes[i];
  } 
  fclose(inFS);
  //remove(helper);
  free(bytes);
  return betterBytes;
}
//Converts digit representation to ascii
char digit_to_ascii(char digit){
  switch(digit) {
    case 0:
      return 0x30;
    case 1:
      return 0x31;
    case 2:
      return 0x32;
    case 3:
      return 0x33;
    case 4:
      return 0x34;
    case 5:
      return 0x35;
    case 6:
      return 0x36;
    case 7:
      return 0x37;
    case 8:
      return 0x38;
    case 9:
      return 0x39;
  }
}
//Converts ascii representation to actual integer representation
int ascii_to_int(char byte) {
  switch(byte) {
    case 0x30:
      return 0;
    case 0x31:
      return 1;
    case 0x32:
      return 2;
    case 0x33:
      return 3;
    case 0x34:
      return 4;
    case 0x35:
      return 5;
    case 0x36:
      return 6;
    case 0x37:
      return 7;
    case 0x38:
      return 8;
    case 0x39:
      return 9;
    //CONSIDER RETURNING ERROR VALUE IF E.G. "!" == BYTE
  }
}

struct asciiArrayInfo
{
  char arraySize;
  bool positive;
  char* asciiArray;
};

//Gets size of file referenced by fd
int get_file_size(int fd) {
  struct stat stat_struct;
  if(fstat(fd, &stat_struct) == -1)
    return -1;
  return (int) stat_struct.st_size;
}
struct asciiArrayInfo* int_to_ascii_array(int number) {
  //RETURN POINTER TO STRUCT WITH SIZE, SIGN, ARRAY
  //Will need to report back or calculate size of array
  //Deal with negative case
  bool sign  = true;
  if (number < 0) { sign = false; }
  number = abs(number);
  struct asciiArrayInfo* ptr;
  struct asciiArrayInfo info;
  if (number < 10) {
    char* asciiChars = (char*)malloc(1);
    asciiChars[0] = digit_to_ascii((char)number);
    info.arraySize = 1;
    info.positive = sign;
    info.asciiArray = asciiChars;
    ptr = &info;
    return ptr;
  }
  else if (number < 100) {
    char* asciiChars = (char*)malloc(2);
    char ones = number % 10;
    char tens = (number - (number % 10)) / 10;
    asciiChars[0] = digit_to_ascii(tens);
    asciiChars[1] = digit_to_ascii(ones);
    info.arraySize = 2;
    info.positive = sign;
    info.asciiArray = asciiChars;
    ptr = &info;
    return ptr;
  }
  else if (number < 1000) {
    char* asciiChars = (char*)malloc(3);
    char ones = number % 10;
    char tens = ((number % 100) - (number % 10)) / 10;
    char hundreds = (number - (number % 100)) / 100;
    asciiChars[0] = digit_to_ascii(hundreds);
    asciiChars[1] = digit_to_ascii(tens);
    asciiChars[2] = digit_to_ascii(ones);
    info.arraySize = 3;
    info.positive = sign;
    info.asciiArray = asciiChars;
    ptr = &info;
    return ptr;
  } 
  else if (number < 10000) {
    char* asciiChars = (char*)malloc(4);
    char ones = number % 10;
    char tens = ((number % 100) - (number % 10)) / 10;
    char hundreds = ((number % 1000) - (number % 100)) / 100;
    char thousands = (number - (number % 1000)) / 1000;
    asciiChars[0] = digit_to_ascii(thousands);
    asciiChars[1] = digit_to_ascii(hundreds);
    asciiChars[2] = digit_to_ascii(tens);
    asciiChars[3] = digit_to_ascii(ones);
    info.arraySize = 4;
    info.positive = sign;
    info.asciiArray = asciiChars;
    ptr = &info;
    return ptr;
  } 

  else if (number < 65536) {
    char* asciiChars = (char*)malloc(5);
    char ones = number % 10;
    char tens = ((number % 100) - (number % 10)) / 10;
    char hundreds = ((number % 1000) - (number % 100)) / 100;
    char thousands = ((number % 10000) - (number % 1000)) / 1000;
    char tenThousands = (number - (number % 10000)) / 10000;
    asciiChars[0] = digit_to_ascii(tenThousands);
    asciiChars[1] = digit_to_ascii(thousands);
    asciiChars[2] = digit_to_ascii(hundreds);
    asciiChars[3] = digit_to_ascii(tens);
    asciiChars[4] = digit_to_ascii(ones);
    info.arraySize = 5;
    info.positive = sign;
    info.asciiArray = asciiChars;
    ptr = &info;
    return ptr;
  } 

}
//Calculates length of "ascii array" structure;
  int len_of_ascii_array(unsigned char* chars ) {
    char length = 0;
    for (int i = 0; chars[i] != 255 && i < 5; i++){
      length++;
    }
    return (int)length;
      }

/*Returns true is input is in valid format, false otherwise. Additionally translates
  data into human readable form and prints to console. */
bool isValidFile(const char* path){
  char* bytes; //Pointer to byte array

  //READ FILE INTO A BYTE ARRAY
  FILE* inputFS = fopen(path, "rb");
  fseek(inputFS, 0, SEEK_END); 
  long size = ftell(inputFS);
  rewind(inputFS);
  bytes = (char*)malloc(size + 1); //Allocated buffer of the size of the file
  fread(bytes, size, 1, inputFS);

  //PARSE BYTE ARRAY
    for (int i = 0; i < size;) {
      if (bytes[i] == 0x0) {
        printf("Type 0 unit parsed:\n");
        i++;
        int amount = bytes[i];
        printf("%03d ", amount);
        i++;
        int count = 0;
        short* nums = (short*)malloc(amount * sizeof(short));
        while(count < amount) {
          nums[count] = (((short)bytes[i]) << 8) | (0x00ff & bytes[++i]);
          printf("%d ", nums[count]);
          i++;
          count++;
        }
        printf("\n");
        if(bytes[i] != 0x0 && bytes[i] != 0x1 && i != size) {
          printf("Type 0 formatting error\n");
          printf("Error byte: <0x%x>\n", bytes[i]);
          printf("Previous byte: <0x%x>\n", bytes[i - 1]);
          printf("Previous, Previous byte: <0x%x>\n", bytes[i - 2]);
          printf("Previous, Previous Previous byte: <0x%x>\n", bytes[i - 3]);
          printf("Previous 4x byte: <0x%x>\n", bytes[i - 4]);
          free(nums);
          free(bytes);
          fclose(inputFS);
          return false;
        }
      }
      else if (bytes[i] == 0x1) {
        
        printf("Type 1 unit parsed:\n");
        //printf("%d, %d, %d\n", bytes[i+1], bytes[i+2], bytes[i+3]);
        int hundreds = ascii_to_int(bytes[++i]);
        int tens = ascii_to_int(bytes[++i]);
        int ones = ascii_to_int(bytes[++i]);
        //printf("%d, %d, %d\n", hundreds, tens, ones);
        int amount = ones + (tens * 10) + (hundreds * 100);
        printf("%03d ", amount);
        i++;
        
        int nums[amount];
        for (int j = 0; j < amount; j++) { nums[j] = 0; }
        unsigned char comma = 0x2c;
        unsigned char* ptr;
       
        for (int count = 0; count < amount; count++){
          unsigned char asciiNums[5] = {255,255,255,255,255};
          ptr = asciiNums;
          int length = 0;
          while (length <= 5) {
            if(bytes[i] == comma) {
              i++;
              break;
            }
            else if(bytes[i] == 0x0 || bytes[i] == 0x1) /* && (count == amount - 1)) */ {
              break;
            }
            else if(length != 5){
            
            asciiNums[length] = bytes[i];
            i++;
            length++;
            }
          }

          int len = len_of_ascii_array(ptr);
          int offset = (int)pow(10.0, (double)len - 1);
          for (int j = 0; j < len; j++) {
            nums[count] += ascii_to_int((char)asciiNums[j]) * offset;
            offset /= 10;
          }
        }

        for (int j = 0; j < amount; j++) {
          if (j == amount - 1) {
            printf("%d\n", nums[j]);
          }
          else {
            printf("%d, ", nums[j]);
          }
        }

        if(bytes[i] != 0x0 && bytes[i] != 0x1 && i != size) {
            printf("Type 1 formatting error\n");
            printf("Error byte: <0x%x>\n", bytes[i]);
            printf("Previous byte: <0x%x>\n", bytes[i - 1]);
            printf("Previous, Previous byte: <0x%x>\n", bytes[i - 2]);
            free(bytes);
            fclose(inputFS);
            return false;
          }
      }
      else {
        perror("Invalid File Format\n");
        free(bytes);
        fclose(inputFS);
        return false;
      }
    }
    free(bytes);
    fclose(inputFS); 
    return true;
  }

int main(void) {
  const char* inFile = "practice_project_test_file_1";   const char* outFile = "garbo";
  //const char* inFile = "out";
  //const char* inFile = "garbo"; const char* outFile = "out";

  //const char* outFile = "collector";
  char to = 3;
  printf("Recieved file\n-------------------------\n");
  bool valid = isValidFile(inFile);
  if (valid){
    printf("-------------------------\nFile is valid. Translating and saving to %s\n", outFile);
    printf("\n-------------------------\n");
    char* bytes; 
    FILE* inputFS = fopen(inFile, "rb");
    FILE* outFS = fopen(outFile, "wb");
    fseek(inputFS, 0, SEEK_END); 
    long size = ftell(inputFS);
    rewind(inputFS);
    bytes = (char*)malloc(size + 1);
    fread(bytes, size, 1, inputFS);

    switch(to) { //SWITCH STATEMENT TO HANDLE USER PASSED ARGUEMENT
      case 0:
        ;
        char typeZero = 0x0;
        const char* toTypeZero = &typeZero;
        for( int i = 0; i < size;) {
          if(bytes[i] == 0x0) {
            printf("Writing Type 0 unit:\n");
            fwrite(toTypeZero, 1, 1, outFS); // Write out type byte
            char amount = bytes[++i];
            char* amountPtr = &amount;
            fwrite(amountPtr, 1, 1, outFS); //Write out amount
            int numsLength = (int)amount * 2;
            char numsArray[numsLength];
            for (int g = 0; g < numsLength; g++) {
              numsArray[g] = bytes[++i];
            }
            i++;
            char* numsArrayOut = numsArray;
            fwrite(numsArrayOut, numsLength, 1, outFS); //Write out two byte nums
          }
          else if(bytes[i] == 0x1 ) { 
            printf("Translating Type 1 unit to Type 0:\n");
            fwrite(toTypeZero, 1, 1, outFS); // Write out type byte
            int hundreds = ascii_to_int(bytes[++i]);
            int tens = ascii_to_int(bytes[++i]);
            int ones = ascii_to_int(bytes[++i]);
            char amount = (char)(ones + (tens * 10) + (hundreds * 100));
            i++;
            printf("Amount: %d\n", amount);
            char* amountPtr = &amount;
            fwrite(amountPtr, 1, 1, outFS);
            int nums[amount];
            for (int j = 0; j < amount; j++) { nums[j] = 0; }
            unsigned char comma = 0x2c;
            unsigned char* ptr;
          
            for (int count = 0; count < amount; count++){
              unsigned char asciiNums[5] = {255,255,255,255,255};
              ptr = asciiNums;
              int length = 0;
              while (length <= 5) {
                if(bytes[i] == comma) {
                  i++;
                  break;
                }
                else if(bytes[i] == 0x0 || bytes[i] == 0x1) /* && (count == amount - 1)) */ {
                  //count = amount;  //BREAK OUT OF BOTH LOOPS
                  break;
                }
                else if(length != 5){
                
                asciiNums[length] = bytes[i];
                i++;
                length++;
                }
              }
              int len = len_of_ascii_array(ptr);
              int offset = (int)pow(10.0, (double)len - 1);
              for (int j = 0; j < len; j++) {
                nums[count] += ascii_to_int((char)asciiNums[j]) * offset;
                offset /= 10;
              }
            }
            
            unsigned short numsOut[amount];
            for (int j = 0; j < amount; j++) {
              numsOut[j] = (short)nums[j];
              printf("Nums %d\n", numsOut[j]);
            }
            unsigned short* numsOutPtr = numsOut;
            char* betterBytes = reverse_byte_order(numsOutPtr, amount);
            fwrite(betterBytes, 2 * amount, 1, outFS); //Write out nums
            free(betterBytes);
          }
        }
        break; 
      case 1:
        ;
        char typeOne = 0x1;
        const char* toTypeOne = &typeOne;
        for (int i = 0; i < size;) {
          if (bytes[i] == 0x0) {
            printf("Translating Type 0 unit to Type 1:\n");
            fwrite(toTypeOne, 1, 1, outFS); // Write out type byte
            int amount = bytes[++i];
            i++;
            int count = 0;
            short* nums = (short*)malloc(amount * sizeof(short));
            while(count < amount) {
              nums[count] = (((short)bytes[i]) << 8) | (0x00ff & bytes[++i]);
              i++;
              count++;
            }

            struct asciiArrayInfo* info = int_to_ascii_array(amount);
            const char* asciiArray = info -> asciiArray;
            char size = info -> arraySize;
            bool isPositive = info -> positive;
            char zeros[3] = {0x30, 0x30, 0x30};
            const char* zerosPtr = zeros;
            char numZeros = 3 - size;
            printf("numZeros: %d\n", numZeros);
            fwrite(zerosPtr, numZeros, 1, outFS); //Write out padding zeros 
            fwrite(asciiArray, size, 1, outFS); //Write out amount ascii bytes
            for (char i = 0; i < size; i++) {
              printf("%c ", asciiArray[i]);
            }
            printf("\n");
            char comma = 0x2c;
            const char* commaPtr = &comma;
            for (int i = 0; i < amount; i++){
              info = int_to_ascii_array((int)nums[i]);
              asciiArray = info -> asciiArray;
              size = info -> arraySize;
              isPositive = info -> positive;
              fwrite(asciiArray, size, 1, outFS); //Write out short array ascii bytes
              if ( i != amount - 1) { fwrite(commaPtr, 1, 1, outFS);}
              for (char i = 0; i < size; i++) {
                printf("%c ", asciiArray[i]);
            }
          }
        }
        else if (bytes[i] == 0x1) {
            
            printf("Writing Type 1 unit:\n");
            fwrite(toTypeOne, 1, 1, outFS); // Write out type byte
            char amountOut[3];
            for (char g = 0; g < 3; g++) { amountOut[g] = bytes[i + g + 1];}
            const char* amountPtr = amountOut;
            fwrite(amountPtr, 3, 1, outFS); //Write out amount bytes as they are
            int hundreds = ascii_to_int(bytes[++i]);
            int tens = ascii_to_int(bytes[++i]);
            int ones = ascii_to_int(bytes[++i]);
            //printf("%d, %d, %d\n", hundreds, tens, ones);
            int amount = ones + (tens * 10) + (hundreds * 100);
            printf("%03d ", amount);
            i++;
            unsigned char comma = 0x2c;
            unsigned char* ptr;
          
            for (int count = 0; count < amount; count++){
              unsigned char asciiNums[5] = {255,255,255,255,255};
              ptr = asciiNums;
              int length = 0;
              while (length <= 5) {
                if(bytes[i] == comma) {
                  i++;
                  break;
                }
                else if(bytes[i] == 0x0 || bytes[i] == 0x1) /* && (count == amount - 1)) */ {
                  break;
                }
                else if(length != 5){
                  asciiNums[length] = bytes[i];
                  i++;
                  length++;
                }
              }
              int len = len_of_ascii_array(ptr);
              int offset = (int)pow(10.0, (double)len - 1);
              unsigned char* asciiArrayOut = (unsigned char*)malloc(len);
              for (int j = 0; j < len; j++) {
                asciiArrayOut[j] = asciiNums[j];
                printf("%c ", asciiArrayOut[j]);
              }
              printf("\n");
              fwrite(asciiArrayOut, len, 1, outFS);
              free(asciiArrayOut);
              if (count != (amount - 1)) { 
                const char* commaPtr = &comma;
                fwrite(commaPtr, 1, 1, outFS);
                }

              }


          }

        }
        break;
      case 3:
        ;
        char One = 0x1; char Zero = 0x0;
        const char* toOne = &One;
        const char* toZero = &Zero;
        for (int i = 0; i < size;) {
        if (bytes[i] == 0x0) {
            printf("Translating Type 0 unit to Type 1:\n");
            fwrite(toOne, 1, 1, outFS); // Write out type byte
            int amount = bytes[++i];
            i++;
            int count = 0;
            short* nums = (short*)malloc(amount * sizeof(short));
            while(count < amount) {
              nums[count] = (((short)bytes[i]) << 8) | (0x00ff & bytes[++i]);
              i++;
              count++;
            }

            struct asciiArrayInfo* info = int_to_ascii_array(amount);
            const char* asciiArray = info -> asciiArray;
            char size = info -> arraySize;
            bool isPositive = info -> positive;
            char zeros[3] = {0x30, 0x30, 0x30};
            const char* zerosPtr = zeros;
            char numZeros = 3 - size;
            printf("numZeros: %d\n", numZeros);
            fwrite(zerosPtr, numZeros, 1, outFS); //Write out padding zeros 
            fwrite(asciiArray, size, 1, outFS); //Write out amount ascii bytes
            for (char i = 0; i < size; i++) {
              printf("%c ", asciiArray[i]);
            }
            printf("\n");
            char comma = 0x2c;
            const char* commaPtr = &comma;
            for (int i = 0; i < amount; i++){
              info = int_to_ascii_array((int)nums[i]);
              asciiArray = info -> asciiArray;
              size = info -> arraySize;
              isPositive = info -> positive;
              fwrite(asciiArray, size, 1, outFS); //Write out short array ascii bytes
              if ( i != amount - 1) { fwrite(commaPtr, 1, 1, outFS);}
              for (char i = 0; i < size; i++) {
                printf("%c ", asciiArray[i]);
            }
          }
        } 
        else if(bytes[i] == 0x1 ) { 
            printf("Translating Type 1 unit to Type 0:\n");
            fwrite(toZero, 1, 1, outFS); // Write out type byte
            int hundreds = ascii_to_int(bytes[++i]);
            int tens = ascii_to_int(bytes[++i]);
            int ones = ascii_to_int(bytes[++i]);
            char amount = (char)(ones + (tens * 10) + (hundreds * 100));
            i++;
            printf("Amount: %d\n", amount);
            char* amountPtr = &amount;
            fwrite(amountPtr, 1, 1, outFS);
            int nums[amount];
            for (int j = 0; j < amount; j++) { nums[j] = 0; }
            unsigned char comma = 0x2c;
            unsigned char* ptr;
          
            for (int count = 0; count < amount; count++){
              unsigned char asciiNums[5] = {255,255,255,255,255};
              ptr = asciiNums;
              int length = 0;
              while (length <= 5) {
                if(bytes[i] == comma) {
                  i++;
                  break;
                }
                else if(bytes[i] == 0x0 || bytes[i] == 0x1) /* && (count == amount - 1)) */ {
                  //count = amount;  //BREAK OUT OF BOTH LOOPS
                  break;
                }
                else if(length != 5){
                
                asciiNums[length] = bytes[i];
                i++;
                length++;
                }
              }
              int len = len_of_ascii_array(ptr);
              int offset = (int)pow(10.0, (double)len - 1);
              for (int j = 0; j < len; j++) {
                nums[count] += ascii_to_int((char)asciiNums[j]) * offset;
                offset /= 10;
              }
            }
            
            unsigned short numsOut[amount];
            for (int j = 0; j < amount; j++) {
              numsOut[j] = (short)nums[j];
              printf("Nums %d\n", numsOut[j]);
            }
            unsigned short* numsOutPtr = numsOut;
            char* betterBytes = reverse_byte_order(numsOutPtr, amount);
            fwrite(betterBytes, 2 * amount, 1, outFS); //Write out nums
            free(betterBytes);
          }
        }
        break;
    }
}
else {
  printf("FILE INVALID");
}
 return 0;
}


