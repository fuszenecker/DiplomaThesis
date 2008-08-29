
#include <config.h>
#include <sched.h>
#include <queue.h>
#include <serv_usart.h>
#include <taskman.h>
#include <taskman_messages.h>
#include <stringpp.h>

#define SEND_TO_USART(_command, _param, _next_state) \
            msg.command = _command; \
            msg.param = _param; \
            \
            if (queue_put_non_blocking(&queue4usart, &msg) == QUEUE_OK) { \
                taskman_state = _next_state; \
            }

#define SKIP_SPACES()   \
            for (; (c_str[i] == ' ') || (c_str[i] == '\t') || \
                   (c_str[i] == '\f'); i++);
            
/* Process descriptor table, that contains the data about processes */
extern t_process_descriptor process_table[PROC_TAB_SIZE];

t_queue queue4taskman;

volatile unsigned int taskman_state;
volatile char ch;
volatile unsigned char pid_index;
String str;
char *c_str;

void taskman() {
    t_message msg;

    switch (taskman_state) {
        // Initialize the process
        case TM_INIT:
            SEND_TO_USART(USART_RECV, (unsigned int)&queue4taskman, TM_LIST1);
            break;
    
        // Print header: Welcome to ...
        case TM_LIST1:
            SEND_TO_USART(USART_PRINT, (unsigned int)str_welcome, TM_LIST2);
            break;

        // Header of the process list
        case TM_LIST2:
            pid_index = 0;
            SEND_TO_USART(USART_PRINT, (unsigned int)str_processes, TM_LIST3);
            break;

        // Items of the process list
        case TM_LIST3:
            if (process_table[pid_index].status != SCHED_FREE) {
                str.reset();
                str + pid_index + "  " + process_table[pid_index].name + "  " + 
                    (unsigned char)process_table[pid_index].priority_preload + "   " +  
                    (unsigned char)process_table[pid_index].status + "\r\n";
            }


            if (pid_index < PROC_TAB_SIZE) {
                if (process_table[pid_index].status != SCHED_FREE) {
                    SEND_TO_USART(USART_PRINT, (unsigned int)(str.c_str()), TM_LIST3);
                }
            } else
                taskman_state = TM_LIST4;

            pid_index++;

            break;
           
        // Available commands
        case TM_LIST4: 
            SEND_TO_USART(USART_PRINT, (unsigned int)str_commands, TM_READLINE);
            break;

        case TM_READLINE:
            str.reset();
            taskman_state = TM_READLINE2;
            break;
            
        case TM_READLINE2:
            if (queue_get_non_blocking(&queue4taskman, &msg) == QUEUE_OK) {
                if (msg.command == USART_RECVD) {
                    taskman_state = TM_PRINT_CH1;
                    ch = (char)msg.param;
                    str + ch;
                }
            }

            break;

        case TM_PRINT_CH1:
            SEND_TO_USART(USART_SEND, ch, TM_PRINT_CH2);
            break;

        case TM_PRINT_CH2:
            if ((ch == '\r') || (ch == '\n')) {
                int i = 0;
                int pid = 0;
                int new_prio = 0;
                c_str = str.c_str();

                SKIP_SPACES();

                switch (c_str[i]) {
                    case 's':
                        i++;

                        SKIP_SPACES();

                        for (; (c_str[i] != ' ') && (c_str[i] != '\r') && (c_str[i] != '\n'); i++) {
                            pid *= 10;
                            pid += c_str[i] - '0';
                        }

                        sched_set_status(process_table, pid, SCHED_NOT_RUNNING);
                        taskman_state = TM_LIST1;

                        break;
                    case 'c':
                        i++;

                        SKIP_SPACES();

                        for (; (c_str[i] != ' ') && (c_str[i] != '\r') && (c_str[i] != '\n'); i++) {
                            pid *= 10;
                            pid += c_str[i] - '0';
                        }

                        sched_set_status(process_table, pid, SCHED_RUNNING);
                        taskman_state = TM_LIST1;

                        break;
                    case 'i':
                        taskman_state = TM_SEND;
                        break;
                    case 'p':
                        i++;

                        SKIP_SPACES();

                        for (; (c_str[i] != ' ') && (c_str[i] != '\r') && (c_str[i] != '\n'); i++) {
                            pid *= 10;
                            pid += c_str[i] - '0';
                        }

                        SKIP_SPACES();

                        for (; (c_str[i] != ' ') && (c_str[i] != '\r') && (c_str[i] != '\n'); i++) {
                            new_prio *= 10;
                            new_prio += c_str[i] - '0';
                        }

                        sched_set_priority(process_table, pid, new_prio);
                        // process_table[pid].priority_preload = new_prio;
                        taskman_state = TM_LIST1;

                        break;
                    default:
                        msg.command = USART_SEND;
                        msg.param = '\a';
                        queue_put_non_blocking(&queue4usart, &msg);
                        taskman_state = TM_LIST1;
                        break;
                }

            } else
                taskman_state = TM_READLINE2;
                 
            break;

        case TM_SEND:
            taskman_state = TM_LIST1;
            break;

        case TM_STOP:
            break;
    }
}

