/**
 * @file tty_ioctl.c
 * @author Sascha Klick (dev@sklick.de)
 * @brief Control DTR and RTS signals on tty (terminal) devices
 * @version 0.1.2
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>

int run = 1;

void INThandler() {
   run = 0;
}

int main(int argc, char* args[]) {
   if(argc < 3){ printf(" Control DTR and RTS signals on tty (terminal) devices.\n\n Usage:\n\n  tty_ioctl <port> [-vv] <R,r,D,d,B,b,S,s,l>\n\n"); }

   signal(SIGINT, INThandler);

   int verbose = 0;

   int fd;
   fd = open(args[1], O_RDWR | O_NOCTTY );

   if(fcntl(fd, F_GETFD) != F_OK){
      printf("! could not open \"%s\"\n", args[1]);
      return 1;
   }

   for(unsigned int i = 2; i < argc; i++){
      char *cmd = args[i];
      
      if(*(cmd++) == '-' && *(cmd++) == 'v'){
         verbose++;
         while(*(cmd++) == 'v'){
            verbose++;
         }
         continue;
      }
      
      cmd = args[i];
      while(run == 1 && *cmd != '\0'){
         int flags = 0;
         
         switch(*cmd){
            case 'b': { flags = TIOCM_RTS | TIOCM_DTR; ioctl(fd, TIOCMBIC, &flags); if(verbose >= 1) printf("0 [r]TS/[d]TR\n"); } break;
            case 'B': { flags = TIOCM_RTS | TIOCM_DTR; ioctl(fd, TIOCMBIS, &flags); if(verbose >= 1) printf("1 [R]TS/[D]TR\n"); } break;
            case 'R': { flags = TIOCM_RTS; ioctl(fd, TIOCMBIC, &flags); if(verbose >= 1) printf("1 [R]TS\n"); } break;
            case 'r': { flags = TIOCM_RTS; ioctl(fd, TIOCMBIC, &flags); if(verbose >= 1) printf("0 [r]TS\n"); } break;
            case 'D': { flags = TIOCM_DTR; ioctl(fd, TIOCMBIS, &flags); if(verbose >= 1) printf("1 [D]TR\n"); } break;
            case 'd': { flags = TIOCM_DTR; ioctl(fd, TIOCMBIC, &flags); if(verbose >= 1) printf("0 [d]TR\n"); } break;
            case 'S': { usleep(500000); if(verbose >= 2) printf("* [S]leep for 0.5s\n"); } break;
            case 's': { usleep(100000); if(verbose >= 2) printf("* [s]leep for 0.1s\n"); } break;
            case 'l': { cmd = args[i]; if(verbose >= 2) printf("* [l]oop to start\n"); } continue;
            default:  { if(verbose >= 1) printf("! [%c] BAD CMD\n", *cmd); } break;
         }

         cmd++;
      }
   }

   if(verbose >= 1) printf("* exit\n"); 

   close(fd);
   return 0;
}
