#include <stdio.h>
#include <stdlib.h>

int main ()
{
  FILE* gpio_956;
  FILE* gpio_957;
  FILE* value_956;
  FILE* export;
  int close_error;
  char *value;
  int taster_value;
  size_t num_of_bytes = 1;
  int i = 0;
  export = fopen ("/sys/class/gpio/export", "w");
  if(!export)
    printf("file could not be opened\n");
  else
    printf("file succesfully opened\n");
  // creating gpio956 folder
  fwrite ("956", sizeof(char), 4, export);
  //**************************
  close_error = fclose(export);
  if(!close_error)
    printf("file closed\n");
  else
    printf("file could not be closed\n");

  gpio_956 = fopen ("/sys/class/gpio/gpio956/direction", "w");
  if(!gpio_956)
    printf("file could not be opened\n");
  else
    printf("file succesfully opened\n");
  
  // creating gpio956 folder
  fwrite ("in", sizeof(char), 3, gpio_956);
  //****************************
  
  close_error = fclose(gpio_956);
  if(!close_error)
    printf("file closed\n");
  else
    printf("file could not be closed\n");
  
  value_956 = fopen ("/sys/class/gpio/gpio956/value", "r");
  if(!value_956)
    printf("file could not be opened\n");
  else
    printf("file succesfully opened\n");
  
  //reading taster


  //**************************

  
  while(1)
  {
    value = (char *)malloc(num_of_bytes+1); 
    getline(&value, &num_of_bytes, value_956); 
    sscanf(value, "%d", &taster_value); 
    printf("\nvalue is: %d", taster_value);
    for (i =0; i<10000; i++);
    value_956 = fopen ("/sys/class/gpio/gpio956/value", "r");
    fflush(value_956);
    free(value);
  }
  
  
  
  return 0;
}
