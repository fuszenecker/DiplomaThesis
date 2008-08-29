#ifndef __TASKMAN_MESSAGES_H__
#define __TASKMAN_MESSAGES_H__

#define TAB         "\t"
#define FORMFEED    "\f"
#define NL          "\r\n"
#define NLNL        "\r\n\n"

#define NORMAL      "\033[0m"
#define BOLD        "\033[1m"

#define RESET       "\033c"
#define HOME        "\033[1;1H"
#define CLR_EOL     "\033[K"

#define FG_WHITE    "\033[37m"
#define BG_BLACK    "\033[40m"
#define BG_BLUE     "\033[44m"


const char str_welcome[] = BG_BLACK FORMFEED RESET HOME BOLD BG_BLUE FG_WHITE "  Welcome to the Portable Cooperative Multitasking Framework" CLR_EOL BG_BLACK NL;
const char str_processes[] = NL "Process table:" NLNL "PID  Name              Prio  Status" NL "-----------------------------------" NL;
const char str_commands[] = NL "Available commands:" NL 
                                    TAB BOLD "s" NORMAL " <pid>" TAB TAB "Stop process" NL
                                    TAB BOLD "c" NORMAL " <pid>" TAB TAB "Continue process" NL
                                    TAB BOLD "i" NORMAL " <pid>" TAB TAB "Send init command to process" NL
                                    TAB BOLD "p" NORMAL " <pid> <prio>" TAB "Set priority of process" NL
                                    NL BOLD "Command: " NORMAL;

#endif
