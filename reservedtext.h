#ifndef __RESERVED_TEXT_H__
#define __RESERVED_TEXT_H__

/*
 * There are sentences reserved in the game system,
 * including title, menu, instructions, etc.
 */
#define MENU_POS_MAX	5
#define HOWTO_POS_MAX	7

char title[16];
char menu_str[MENU_POS_MAX][16];
char howto_str[HOWTO_POS_MAX][16];

#endif
