#pragma once

typedef struct _stack_t 
{
  struct _stack_t *next;
  char text[1]; // this should be the length of string_size
} stack_t;

enum
{
INST_0,         // $0
INST_1,         // $1
INST_2,         // $2
INST_3,         // $3
INST_4,         // $4
INST_5,         // $5
INST_6,         // $6
INST_7,         // $7
INST_8,         // $8
INST_9,         // $9
INST_R0,        // $R0
INST_R1,        // $R1
INST_R2,        // $R2
INST_R3,        // $R3
INST_R4,        // $R4
INST_R5,        // $R5
INST_R6,        // $R6
INST_R7,        // $R7
INST_R8,        // $R8
INST_R9,        // $R9
INST_CMDLINE,   // $CMDLINE
INST_INSTDIR,   // $INSTDIR
INST_OUTDIR,    // $OUTDIR
INST_EXEDIR,    // $EXEDIR
__INST_LAST
};

#define DEFAULT_RECT 1018

// sent to the last child window to tell it that the install thread is done
#define WM_NOTIFY_INSTPROC_DONE (WM_USER+0x4)

// sent to every child window to tell it it can start executing NSIS code
#define WM_NOTIFY_START (WM_USER+0x5)

// sent to the outer window to tell it to go to the next inner window
#define WM_NOTIFY_OUTER_NEXT (WM_USER+0x8)

// sent to every child window to tell it it is closing soon
#define WM_NOTIFY_INIGO_MONTOYA (WM_USER+0xb)

// update message used by DirProc and SelProc for space display
#define WM_IN_UPDATEMSG (WM_USER+0xf)

// custom pages should send this message to let NSIS know they're ready
#define WM_NOTIFY_CUSTOM_READY (WM_USER+0xd)

// simulates clicking on the tree
#define WM_TREEVIEW_KEYHACK (WM_USER+0x13)

// notifies a component selection change (.onMouseOverSection)
#define WM_NOTIFY_SELCHANGE (WM_USER+0x19)

// Notifies the installation type has changed by the user
#define WM_NOTIFY_INSTTYPE_CHANGED (WM_USER+0x20)

#define NOTIFY_BYE_BYE 'x'

extern int			g_stringsize;
extern stack_t**	g_stacktop;
extern char*		g_variables;

int popstring(char *str);
void pushstring(char *str);
char *getuservariable(int varnum);
