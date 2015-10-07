

#include <termios.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <errno.h>

#define    UART_BASE   0x20201000 // PL011 USART --> B+

int fd = 0;
int mem_fd = 0;
int errsv = 0;

volatile unsigned* uart = NULL;
void* uart_map = NULL;
void* IBRD = NULL;
void* FBRD = NULL;
void* LCRH = NULL;
void* UARTCR = NULL;
void* UARTFR = NULL;

uint32_t brd = 0;

struct termios termios_p;
 
int main() {

fd = open( "/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NONBLOCK );

if ( fd > 0 ) // set the baud rate and other configurations
{
   tcgetattr( fd, &termios_p );

   termios_p.c_lflag &= ~ISIG;      // no signals
   termios_p.c_lflag &= ~ICANON;   // no canonical mode
   termios_p.c_lflag &= ~ECHO;   // no echo input
   termios_p.c_lflag &= ~NOFLSH;   // no flushing on SIGINT
   termios_p.c_lflag &= ~IEXTEN;   // no input processing
   termios_p.c_cc[VMIN] = 0;
   termios_p.c_cc[VTIME] = 0;

   tcsetattr( fd, TCSANOW, &termios_p );

   // Now set the uart baud rate divisors
   if( ( mem_fd = open( "/dev/mem", O_RDWR | O_SYNC ) ) < 0 )
   {
      fprintf( stdout, "can't open /dev/mem for mmap(). Did you use 'sudo' ?\n" );
      return;
   }

   uart_map = mmap( NULL, BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, UART_BASE );

   close( mem_fd );

   if ( uart_map == MAP_FAILED )
   {
      errsv = errno;
      fprintf( stdout,"uart mmap failed\n" );
      fprintf( stdout, "error %d %s\n", errsv, strerror( errsv ) );
   }
   else
   {
      uart = (volatile unsigned*)uart_map;
      UARTFR = (void*)( (uint32_t)uart + 0x18 );
      IBRD = (void*)( (uint32_t)uart + 0x24 );
      FBRD = (void*)( (uint32_t)uart + 0x28 );
      LCRH = (void*)( (uint32_t)uart + 0x2C );
      UARTCR = (void*)( (uint32_t)uart + 0x30 );

      // first, disable the uart and flush the fifos
      brd = 0x0B00;
      memcpy( UARTCR, &brd, 4 );

      // check for completion of any tx/rx
      memcpy( &brd, UARTFR, 4 );
      while( brd & 0x08 )
      {
         usleep( 1 ); // wait a bit
         memcpy( &brd, UARTFR, 4 );
      }

      brd = 6; // 3000000 / ( 16 * 31250 ) = 6.0
      memcpy( IBRD, &brd, 4 );
      brd = 0;
      memcpy( FBRD, &brd, 4 );
      brd = 0x70; // 8 bit data, FIFO enabled
      memcpy( LCRH, &brd, 4 );

      // enable uart, tex, rex etc.
      brd = 0x0B01;
      memcpy( UARTCR, &brd, 4 );

      // close mmap once done here
      munmap( uart_map, BLOCK_SIZE );
   }
}
else
{
   errsv = errno;
   fprintf( stdout,"/dev/ttyAMA0 access failed\n" );
   fprintf( stdout, "error %d %s\n", errsv, strerror( errsv ) );
   fd = 0;
}
}
