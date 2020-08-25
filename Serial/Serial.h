// Linux headers
#ifndef __USE_POSIX
#define __USE_POSIX
#endif
#define __USE_MISC
#include "../CommonInc.h"
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sys/signal.h>

int serial_port;
GtkWidget **widgetsToUpdate;

void openSerialPort(char *portDir,unsigned int baudRate);
void addWidgetToUpdate(GtkWidget *widget);
void signal_handler_IO (int status);   /* definition of signal handler */
void closeSerialPort();

