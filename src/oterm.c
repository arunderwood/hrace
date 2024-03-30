#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include "oterm.h"


void o_term_save_cursor(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[s",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_unsave_cursor(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[u",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_clear(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[2J",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_gotoxy(int i_fd,int i_xpos,int i_ypos)
{
 char cb[80];
 sprintf(cb,"%c[%i;%iH",27,i_ypos,i_xpos); 
 write(i_fd,cb,strlen(cb));
}

void o_term_set_fg(int i_fd,int fg)
{
 char cb[80];
 sprintf(cb,"%c[%dm",27,(fg&7)+30);
 write(i_fd,cb,strlen(cb));
}

void o_term_set_bg(int i_fd,int bg)
{
 char cb[80];
 sprintf(cb,"%c[%dm",27,(bg&7)+40);
 write(i_fd,cb,strlen(cb));
}

void o_term_set_attr(int i_fd,int attr)
{
 char cb[80];
 sprintf(cb,"%c[%dm",27,attr); 
 write(i_fd,cb,strlen(cb));
}

void o_term_show_cursor(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[?25h",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_hide_cursor(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[?25l",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_clear_line_from_cursor_right(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[0K",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_clear_line_from_cursor_left(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[1K",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_clear_entire_line(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[2K",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_set_auto_wrap_mode(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[?7h",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_reset_auto_wrap_mode(int i_fd)
{
 char cb[80];
 sprintf(cb,"%c[?7l",27); 
 write(i_fd,cb,strlen(cb));
}

void o_term_echo_off(int i_fd)
{
 struct termios t;
 tcgetattr(i_fd,&t);
 t.c_lflag &= ~ECHO;
 tcsetattr(i_fd,TCSANOW,&t);
}

void o_term_echo_on(int i_fd)
{
 struct termios t;
 tcgetattr(i_fd,&t);
 t.c_lflag |= ECHO;
 tcsetattr(i_fd,TCSANOW,&t);
}

void o_term_canon_off(int i_fd)
{
 struct termios t;
 tcgetattr(i_fd,&t);
 t.c_lflag &= ~ICANON;
 tcsetattr(i_fd,TCSANOW,&t);
}

void o_term_canon_on(int i_fd)
{
 struct termios t;
 tcgetattr(i_fd,&t);
 t.c_lflag |= ICANON;
 tcsetattr(i_fd,TCSANOW,&t);
}

void o_term_delay_off(void)
{
 int termflags;
 termflags=fcntl(0,F_GETFL);
 termflags&=~O_NDELAY;
 fcntl(0,F_SETFL,termflags);
}

void o_term_delay_on(void)
{
 int termflags;
 termflags=fcntl(0,F_GETFL);
 termflags|=O_NDELAY;
 fcntl(0,F_SETFL,termflags);
}

void o_term_no_block_off(void)
{
 int termflags;
 termflags=fcntl(0,F_GETFL);
 termflags&=~O_NONBLOCK;
 fcntl(0,F_SETFL,termflags);
}

void o_term_no_block_on(void)
{
 int termflags;
 termflags=fcntl(0,F_GETFL);
 termflags|=O_NONBLOCK;
 fcntl(0,F_SETFL,termflags);
}

void o_term_get_cols_rows(int *ip_cols,
                          int *ip_rows)
{
 struct winsize ws;
 ioctl(1, TIOCGWINSZ, &ws);

 *ip_cols=ws.ws_col;
 *ip_rows=ws.ws_row;
}





