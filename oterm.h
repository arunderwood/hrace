#ifndef __oterm_h__
#define __oterm_h__

#define oRESET     0
#define oBRIGHT    1
#define oDIM       2
#define oUNDERLINE 4
#define oBLINK     5
#define oREVERSE   7
#define oHIDDEN    8

#define oBLACK   0
#define oRED     1
#define oGREEN   2
#define oYELLOW  3
#define oBLUE    4
#define oMAGENTA 5
#define oCYAN    6
#define oWHITE   7




void o_term_save_cursor(int i_fd);
void o_term_unsave_cursor(int i_fd);
void o_term_clear(int i_fd);
void o_term_gotoxy(int i_fd,int i_xpos,int i_ypos);
void o_term_set_fg(int i_fd,int fg);
void o_term_set_bg(int i_fd,int bg);
void o_term_set_attr(int i_fd,int attr);
void o_term_show_cursor(int i_fd);
void o_term_hide_cursor(int i_fd);
void o_term_clear_line_from_cursor_right(int i_fd);
void o_term_clear_line_from_cursor_left(int i_fd);
void o_term_clear_entire_line(int i_fd);
void o_term_set_auto_wrap_mode(int i_fd);
void o_term_reset_auto_wrap_mode(int i_fd);
void o_term_echo_off(int i_fd);
void o_term_echo_on(int i_fd);
void o_term_canon_off(int i_fd);
void o_term_canon_on(int i_fd);
void o_term_delay_off(void);
void o_term_delay_on(void);
void o_term_no_block_off(void);
void o_term_no_block_on(void);
void o_term_get_cols_rows(int *ip_cols,
                          int *ip_rows);
#endif
