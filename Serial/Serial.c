#include "Serial.h"
void openSerialPort(char *portDir,unsigned int baudRate){

serial_port = open(portDir, O_RDWR | O_NOCTTY | O_NDELAY);

struct termios tty;
struct sigaction saio;

memset(&tty, 0, sizeof(tty));

// Read in existing settings, and handle any error
if(tcgetattr(serial_port, &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
}
 /* install the signal handler before making the device asynchronous */
saio.sa_handler = signal_handler_IO;
saio.sa_flags = 0;
saio.sa_restorer = NULL;
sigaction(SIGIO,&saio,NULL);
fcntl(serial_port, F_SETFL, FNDELAY);
fcntl(serial_port, F_SETOWN, getpid());
fcntl(serial_port, F_SETFL, FNDELAY|FASYNC);

tcgetattr(serial_port,&tty);

tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
tty.c_cflag |= CS8; // 8 bits per byte (most common)
tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

tty.c_lflag &= ~ICANON;
tty.c_lflag &= ~ECHO; // Disable echo
tty.c_lflag &= ~ECHOE; // Disable erasure
tty.c_lflag &= ~ECHONL; // Disable new-line echo
tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

// tty.c_cc[VTIME] = 0;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
tty.c_cc[VMIN] = 100;

// Set in/out baud rate to be 9600
cfsetispeed(&tty, baudRate);
cfsetospeed(&tty, baudRate);

// Save tty settings, also checking for error
if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
}


}
void closeSerialPort(){
close(serial_port);
}
void signal_handler_IO (int status)
    {   char buf[255];
         printf("received data from UART.\n");
         int res = read(serial_port,buf,5); 
            buf[res]=0;             /* set end of string, so we can printf */
            printf(":%s:%d\n", buf, res);
    }