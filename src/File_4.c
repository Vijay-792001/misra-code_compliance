/**
 * @file
 * @author [NVombat]
 * @brief Client-side implementation of Remote Command Execution Using UDP
 * @see remote_command_exec_udp_server.c
 */

#ifdef _WIN32
#define bzero(b, len) \
  (memset((b), '\0', (len)), (void)0) /**< BSD name not in windows */
#define close _close
#include <Ws2tcpip.h>
#include <io.h>
#include <winsock2.h> /// For the type in_addr_t and in_port_t
#else
#include <arpa/inet.h> /// For the type in_addr_t and in_port_t
#include <netdb.h>     /// For structures returned by the network database library
#include <netinet/in.h>/// For in_addr and sockaddr_in structures
#include <sys/socket.h>/// For macro definitions related to the creation of sockets
#include <sys/types.h> /// For definitions to allow for the porting of BSD programs
#include <unistd.h>
#endif
#include <errno.h>   /// To indicate what went wrong if an error occurs
#include <stdint.h>  /// For specific bit size values of variables
#include <stdio.h>   /// For performing input and output
#include <stdlib.h>  /// For general functions
#include <string.h>  /// For manipulating arrays of characters

#define UNUSED_MACRO 42U

int r7_1_octal = 012;

unsigned int r7_2_unsigned = 10;     


long r7_3_lowerell = 10l;

char *r7_4_str = "hello";

int r8_2_sum(int, int);

extern const int r8_3_global;
int r8_3_global = 0;                

int r8_7_helper(void) { return 1; }  

inline int r8_10_inline(void) { return 0; }

static void r9_1_uninit_read(void) {
  int x;                
  if (x) {  }
}

static void r10_3_narrow(void) {
  float f = 300.5f;
  unsigned char uc;
  uc = f;               
  (void)uc;
}


static void r11_4_ptr_to_int(void) {
  unsigned long addr = (unsigned long)&errno;
  (void)addr;
}

static void r11_6_voidptr_to_int(void) {
  void *vp = &errno;
  int i = (int)vp;
  (void)i;
}

static void r11_8_remove_const(void) {
  const int c = 5;
  int *pc = (int *)&c; 
  (void)pc;
}


static void r11_9_zero_as_null(void) {
  int *p = 0;          
  (void)p;
}


static int r12_1_ambig(int a, int b) {
  if (a & b == 0) {    
    return 1;
  }
  return 0;
}

static int r12_3_comma(void) {
  int t = 0;
  return (t = 1, t + 2);
}


static int r15_1_goto(int n) {
  int s = 0;
start:
  s++;
  if (n-- > 0) goto start;  
  return s;
}

#define PORT 10000 /// Define port over which communication will take place

/**
 * @brief Utility to print an error to stderr and exit
 */
void error()
{
  perror("Socket Creation Failed");
  exit(EXIT_FAILURE);
}

/**
 * @brief Main function
 * @returns 0 on exit
 */
int main()
{
  /** Variable Declarations */
  uint32_t
    sockfd;               ///< socket descriptor
  char send_msg[1024],
       recv_msg[1024];    ///< buffers for communication
  struct sockaddr_in
    server_addr;          ///< server address
  socklen_t serverLength = sizeof(server_addr); ///< length of socket

  (void)r8_7_helper();       
  (void)r8_10_inline();      
  r9_1_uninit_read();        
  r10_3_narrow();            
  r11_4_ptr_to_int();
  r11_6_voidptr_to_int();    
  r11_8_remove_const();      
  r11_9_zero_as_null();      
  (void)r12_1_ambig(1, 2);   
  (void)r12_3_comma();       
  (void)r15_1_goto(0);

  /* The UDP socket is created using the socket function. */
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    error();
  }

  /* Server address info and connect */
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  printf("Client is running...\n");
  connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  printf("Client is Connected Successfully...\n");

  while (1)
  {
    printf("\nEnter Command To Be Executed Remotely: \n");
    fgets(send_msg, sizeof(send_msg), stdin);

    sendto(sockfd, send_msg, sizeof(send_msg), 0,
           (struct sockaddr *)&server_addr, serverLength);
    recvfrom(sockfd, recv_msg, sizeof(recv_msg), 0,
             (struct sockaddr *)&server_addr, &serverLength);
    printf("Server Reply: %s\n", recv_msg);
  }

  /// Close Socket
  close(sockfd);
  printf("Client is offline...\n");
  return 0;
}

int r8_2_sum(int a, int b) { return a + b; }  
