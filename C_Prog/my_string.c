#include <stdio.h>
// #include <string.h>
#include <stdlib.h>

#define MAX_SIZE 255

// длина строки по указателю
int my_strlen(char *str);
// копирование строки
char *my_strcpy(char *dest, char *src);

char *my_strcat(char *str1, char *str2);

int my_strcmp(char *s1, char *s2);

char *my_strchr(char *str, char ch);

char *my_strstr(char *str, char *substr);

int main()
{
  
  char string_1[MAX_SIZE + 1] = { 0 };
  char string_2[MAX_SIZE + 1] = { 0 };
  
  printf("Input string:");
  scanf("%255s", string_1);

  // copy string
  my_strcpy(string_2, string_1);
  printf("my_strcpy\n");
  printf("First  string: %s\n", string_1);  
  printf("Second string: %s\n", string_2); 
  
  printf("Input second string:");
  scanf("%255s", string_2);
  
  
  // compare strings

  int cmp_result = my_strcmp(string_1, string_2);
  printf("my_strcmp\n");
  printf("First  string: %s\n", string_1);  
  printf("Second string: %s\n", string_2); 
  printf("Compare result: %d\n", cmp_result);
  
  // cat strings
  my_strcat(string_1, string_2);
  printf("Concat result: %s\n", string_1);

  

  // find char
  char search_char = '\0';
  printf("Enter char to search for: ");

  scanf("%c", &search_char);
  scanf("%c", &search_char);

  // while (!search_char && search_char == '\n')

  char *found_char = my_strchr(string_1, search_char); 
  if (found_char)
  {
    printf("Found on pos %ld \n", found_char - string_1);
    printf("Str from char: %s \n", found_char);
  }
  else
    printf("Char not found \n");
  
  printf("First  result: %s\n", string_1);  
  printf("Second result: %s\n", string_2);  
  
  
  // TODO

  printf("Input substring: ");
  scanf("%255s", string_2);
  char *result_strstr = my_strstr(string_1, string_2);
  if (!result_strstr)
    printf("Substr not found \n");
  else
  {
    printf("substr starts from %ld \n", result_strstr - string_1);
    printf("substr in string: %s \n", result_strstr);
  }
  
}




int my_strlen(char *str) // длина строки по указателю на нее
{
  int len = 0;
  
  while (str[len++]) 
      ;  
  return len;    
}

char *my_strcpy(char *dest, char *src) // копирование строки, 
{
     // Возращает указатель на новую строку
    int i = 0;
    while ((dest[i++] = src[i]))
        ;
    return dest;
}


char *my_strcat(char *str1, char *str2) // сложение строк
{
  char *work_str = str1;
  
  while(*work_str)
    work_str++;
    
  while(*str2) 
  {
    *work_str = *str2;
    work_str++;
    str2++;
  }
  
  return str1;  
}

int my_strcmp(char *s1, char *s2) // сравнивание строк
{
  for ( ;*s1 == *s2 && *s1; s1++, s2++);
    
  return *s1 - *s2;
}


char *my_strchr(char *str, char ch) //  поиск первого вхождения элемента в строку
{
  while (*str && *str != ch)
    str++;

  return *str == ch ? str : NULL;
}

char *my_strstr(char *str, char *substr)
{  
  char *result = NULL;
  while (*str && !result)
  {
    char *s1 = str;
    char *s2 = substr;
    for ( ;*s1 == *s2 && *s1; s1++, s2++);
    result = !*s2 ? str : NULL;
    str++;
  }
  
  return result;
}