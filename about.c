#include "includes/kernel.h"

//index for video buffer array
uint32 vga_index;
//counter to store new lines
static uint32 next_line_index = 1;
//fore & back color values
uint8 g_fore_color = WHITE, g_back_color = BLUE;
//digit ascii code for printing integers
int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

/*
16 bit video buffer elements(register ax)
8 bits(ah) higher :
  lower 4 bits - forec olor
  higher 4 bits - back color

8 bits(al) lower :
  8 bits : ASCII character to print
*/
uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color)
{
  uint16 ax = 0;
  uint8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}

//clear video buffer array
void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color)
{
  uint32 i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
  next_line_index = 1;
  vga_index = 0;
}

//initialize vga buffer
void init_vga(uint8 fore_color, uint8 back_color)
{
  vga_buffer = (uint16*)VGA_ADDRESS;
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
  g_fore_color = fore_color;
  g_back_color = back_color;
}

/*
increase vga_index by width of row(80)
*/
void newline_on_terminal()
{
  if(next_line_index >= 55){
    next_line_index = 0;
    clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
  }
  vga_index = 80*next_line_index;
  next_line_index++;
}

//assign ascii character to video buffer
void print_char(char ch)
{
  vga_buffer[vga_index] = vga_entry(ch, g_fore_color, g_back_color);
  vga_index++;
}


uint32 strlen(const char* str)
{
  uint32 length = 0;
  while(str[length])
    length++;
  return length;
}

uint32 digit_count(int num)
{
  uint32 count = 0;
  if(num == 0)
    return 1;
  while(num > 0){
    count++;
    num = num/10;
  }
  return count;
}

void itoa(int num, char *number)
{
  int dgcount = digit_count(num);
  int index = dgcount - 1;
  char x;
  if(num == 0 && dgcount == 1){
    number[0] = '0';
    number[1] = '\0';
  }else{
    while(num != 0){
      x = num % 10;
      number[index] = x + '0';
      index--;
      num = num / 10;
    }
    number[dgcount] = '\0';
  }
}

//print string by calling print_char
void print_on_terminal(char *str)
{
  uint32 index = 0;
  while(str[index]){
    print_char(str[index]);
    index++;
  }
}

//print int by converting it into string
//& then printing string
void print_int(int num)
{
  char str_num[digit_count(num)+1];
  itoa(num, str_num);
  print_on_terminal(str_num);
}

void logo(){
  // print_on_terminal("      _                _     __  __");
  // newline_on_terminal();

  // print_on_terminal("  ___| |__   __ _ _ __| | ___\\\\ \\/ /");
  // newline_on_terminal();

  // print_on_terminal(" / __| '_  \\/ _` | '__| |/ _ \\\\  /");
  // newline_on_terminal();

  // print_on_terminal("| (__| | | | (_| | |  | |  __//  \\");
  // newline_on_terminal();

  // print_on_terminal(" \\___|_| |_|\\__,_|_|  |_| \\___/_/\\_\\");
  // newline_on_terminal();

  // print_on_terminal("        	  ___  ____      ");
  // newline_on_terminal();

  // print_on_terminal("        	 / _ \\/ ___|    ");
  // newline_on_terminal();

  // print_on_terminal("        	| | | \\___ \\   ");
  // newline_on_terminal();

  // print_on_terminal("        	| |_| |___) |    ");
  // newline_on_terminal();

  // print_on_terminal("        	 \\___/|____/    ");

  print_on_terminal("                                                        ,/{}");
  newline_on_terminal();

  print_on_terminal("                                                      ,/  {|");
  newline_on_terminal();

  print_on_terminal("                                                  ,,,/    {|,");
  newline_on_terminal();

  print_on_terminal("                                            __--~~        {| ~-,");
  newline_on_terminal();

  print_on_terminal("                                      __--~~              {     `\\");
  newline_on_terminal();

  print_on_terminal("                                                              ,__ \\");
  newline_on_terminal();

  print_on_terminal("        888                       888           Y88b    /   `,\\{),\\,");
  newline_on_terminal();

  print_on_terminal(" e88~~\\ 888-~88e   /~~~8e  888-~\\ 888  e88~~8e   Y88b  /   __-~  `_ ~-_");
  newline_on_terminal();

  print_on_terminal("d888    888  888       88b 888    888 d888  88b   Y88b/  _-~        ~~-_`~-_");
  newline_on_terminal();

  print_on_terminal("8888    888  888  e88~-888 888    888 8888__888   /Y88b '             `~-_`~-__ ");
  newline_on_terminal();

  print_on_terminal("Y888    888  888 C888  888 888    888 Y888    ,  /  Y88b`,                `~-\\_|");
  newline_on_terminal();

  print_on_terminal(" \"88__/ 888  888  \"88_-888 888    888  \"88___/  /    Y88b`,     _-----___    _,'");
  newline_on_terminal();
  
  print_on_terminal("                                                         / /--__  ~~--__  `~,~");
  newline_on_terminal();

  print_on_terminal("                                                          /     ~~--__  ~-',");
  newline_on_terminal();

  print_on_terminal("------------------------------------------------------' ");
  newline_on_terminal();


}

void kernel_up(){

  //first init vga with fore & back colors
  // init_vga(WHITE, BLACK);

  /*call above function to print something
    here to change the fore & back color
    assign g_fore_color & g_back_color to color values
    g_fore_color = BRIGHT_RED;
  */
  // color of terminal
  init_vga(WHITE, BLACK);
  newline_on_terminal();
  logo();
  print_on_terminal("|------------------|");
  newline_on_terminal();
  print_on_terminal("!Welcome to charleX!");
  newline_on_terminal();
  print_on_terminal("!    version1.0    !");
  newline_on_terminal();
  print_on_terminal("|------------------|");
  newline_on_terminal();
  print_on_terminal("|---------------------------------------|");
  newline_on_terminal();
  print_on_terminal("| Creator : amzy-0 (M.Amin Azimi.K)     |");
  newline_on_terminal();
  print_on_terminal("|---------------------------------------|");

  newline_on_terminal();

}
