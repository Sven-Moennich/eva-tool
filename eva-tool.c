
#include "eva-tool.h"


int add_conntrack(int sd, char *ipaddress) {
    struct sockaddr_in dst = {0};
    char packet[] = "a";

    dst.sin_family = AF_INET;
    dst.sin_port = htons(EVA_PORT);
    memcpy(&dst.sin_addr, ipaddress, IP4_ADDRESS_SIZE);

    return sendto(sd, packet, sizeof(packet), 0, (const void *) &dst, sizeof(dst));
}

char *receive_response(int sd, char *out) {
    char ip_buf[IP4_ADDRESS_SIZE] = {0};
    char rcv_buf[RCV_BUF_SIZE] = {0};

    int ret = recv(sd, &rcv_buf, RCV_BUF_SIZE, 0);
    if (ret <= 0) return 0;

    reverse(&rcv_buf[EVA_PACKET_IP_OFF], ip_buf, IP4_ADDRESS_SIZE);
    inet_ntop(AF_INET, ip_buf, out, INET_ADDRSTRLEN);
    return out;
}

void help () {
	printf("FRITZ!Box EVA tool %s\n",VERSION);
  printf("\n");
	printf("\t-d\t\t\t network device\n");
	printf("\t-i\t\t\t set FRITZ!Box to ip \n");
	printf("\t--branding <brand>\t set branding to <brand>\n");
	printf("\t--boot-partition <0|1>\t set boot-partition\n");
  printf("\t--no-reboot\t\t no reboot after all\n");
  printf("\n");
}



int main(int argc, char *argv[]) {
    char packet[EVA_PACKET_SIZE] = {0x00, 0x00, 0x12, 0x01, 0x01, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0xb2, 0x01, 0x00, 0x00, 0x00, 0x00};
    char *packet_ip = &packet[EVA_PACKET_IP_OFF];
    char box_ip_str[INET_ADDRSTRLEN] = {0};
    struct sockaddr_in dst = {0};
		netbuf *nbuf;


    unsigned int command = 0;

    char *ipaddress;
    char *ifname;

		char *branding;
		int bootpartition = -1;
    char *filename;
    char *quote;

    int sd;

    dst.sin_family = AF_INET;
    dst.sin_port = htons(EVA_PORT);



    /* check for capture device name on command-line */
      if (argc >= 2) {

  			//int i;
  			ifname = "wlp1s0"; // default output name
        ipaddress = "192.168.179.1";
				branding = "avm";
				bootpartition = 0;
        filename = "image.bin";
        //quote = "";

  			for( int i = 1; i < argc; i++ ) {
  				if( strcmp(argv[i], "-d") == 0  && argv[i+1] ){
  					ifname = argv[++i];
  				} else
  				if( strcmp(argv[i], "-i") == 0  && argv[i+1] ){
  					ipaddress = argv[++i];
  				} else
  				if( strcmp(argv[i], "--branding") == 0  && argv[i+1] ){
  					branding = argv[++i];
		        command |= 1UL << CMD_BRANDING;
  				} else
  				if( strcmp(argv[i], "--boot-partition") == 0  && argv[i+1] ){
  					bootpartition = atoi(argv[++i]);
            command |= 1UL << CMD_BOOTPARTITION;
  				}	else
          if( strcmp(argv[i], "--no-reboot") == 0 ){
            command |= 1UL << CMD_NO_REBOOT;
  				}	else

          if( strcmp(argv[i], "--quote") == 0 && argv[++i] ){
            command |= 1UL << CMD_QUOTE;
            quote = argv[++i];
  				}	else
          if( strcmp(argv[i], "--flash-backup") == 0 && argv[++i] ){
            command |= 1UL << CMD_FLASH_BACKUP;
            filename = argv[++i];
  				}	else
          if( strcmp(argv[i], "--flash-update") == 0 && argv[++i] ){
            command |= 1UL << CMD_FLASH_UPDATE;
            filename = argv[++i];
  				}	else



  				if( strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--h") || strcmp(argv[i], "--help") || strcmp(argv[i], "/?")) {
						help();
  					exit(EXIT_SUCCESS);
  				}	else {
  					exit(EXIT_FAILURE);
  				}
  			}
      } else {
				help();
  			exit(EXIT_FAILURE);
  		}



    if (inet_pton(AF_INET, ipaddress, packet_ip) != 1) {
        printf("%s is not a valid IPv4 address\n", ipaddress);
        return 1;
    }

    if (inet_pton(AF_INET, EVA_ADDRESS, &dst.sin_addr) != 1) {
        perror("Error converting EVA IP address");
        return 1;
    }

    sd = create_socket(ifname, EVA_PORT);
    if (sd < 0) {
        /* Error was already given to the user */
        return 1;
    }

    if (add_conntrack(sd, packet_ip) < 0) {
            perror("Error adding conntrack");
            close(sd);
            return 1;
    }

    printf("Search for FRITZ!Box...\n");
    for (int i = 1; 1; i++) {
        if (sendto(sd, packet, EVA_PACKET_SIZE, 0, (const void *) &dst, sizeof(dst)) < 0) {
            perror("Error sending message");
            close(sd);
            return 1;
        }

        receive_response(sd, box_ip_str);
        if (receive_response(sd, box_ip_str) > 0) {
            break;
        }
    }

    printf("FRITZ!Box ready at %s\n", box_ip_str);


		if (command != 0){

EvaFtplib_debug = 1;

      EvaFtpVersion();
    	printf("evaftp: init\n");
    //	EvaFtpInit();
    	printf("evaftp: connect\n");
    	EvaFtpConnect(box_ip_str, &nbuf);
    	printf("evaftp: login\n");
    	EvaFtpLogin("adam2", "adam2", nbuf);

      printf("evaftp: SYST\n");
      EvaFtpQuote("SYST", nbuf);
      printf("%s",FtpLastResponse(nbuf));


			if ((command >> CMD_BOOTPARTITION) & 1U) {
				// BootPartition ändern
        printf("evaftp: GETENV linux_fs_start\n");
    		EvaFtpGETENV("linux_fs_start",nbuf);
    		printf("%s",FtpLastResponse(nbuf));

        char buf[255];
			  printf("evaftp: SETENV linux_fs_start %d\n",bootpartition);
        sprintf(buf,"linux_fs_start %d",bootpartition);
				EvaFtpSETENV(buf,nbuf);
        printf("%s",FtpLastResponse(nbuf));
			}


			if ((command >> CMD_BRANDING) & 1U) {
				// Branding entfernen
        printf("evaftp: UNSETENV provider\n");
				EvaFtpUNSETENV("provider",nbuf);
        printf("%s",FtpLastResponse(nbuf));

        char buf[255];
        printf("evaftp: SETENV firmware_version %s\n",branding);
        sprintf(buf,"firmware_version %s",branding);
        EvaFtpSETENV(buf,nbuf);
        printf("%s",FtpLastResponse(nbuf));
			}


      if ((command >> CMD_QUOTE) & 1U) {
      char buf[255];
      sprintf(buf,"%s",quote);
        EvaFtpQuote(buf,nbuf);
        printf("%s",FtpLastResponse(nbuf));
			}


      if ((command >> CMD_FLASH_BACKUP) & 1U) {
        printf("evaftp: MEDIA SDRAM\n");
        EvaFtpQuote("MEDIA SDRAM",nbuf);
        printf("%s",FtpLastResponse(nbuf));
        printf("evaftp: GET env env\n");
        EvaFtpGet("env.txt", "env", EvaFtpLIB_ASCII, nbuf);
        printf("%s",FtpLastResponse(nbuf));

			}
      if ((command >> CMD_FLASH_UPDATE) & 1U) {
        printf("evaftp: MEDIA FLSH\n");
        EvaFtpQuote("MEDIA FLSH",nbuf);
        printf("%s",FtpLastResponse(nbuf));
        printf("evaftp: PUT mtd1 mtd1\n");
        EvaFtpPut("env.txt", "mtd1", EvaFtpLIB_IMAGE, nbuf);
        printf("%s",FtpLastResponse(nbuf));
      }





			if (!(command >> CMD_NO_REBOOT) & 1U) {
        printf("evaftp: REBOOT\n");
        EvaFtpREBOOT(nbuf);
      } else {
        printf("evaftp: QUIT\n");
        EvaFtpQuit(nbuf);
      }
      printf("%s",FtpLastResponse(nbuf));

			if (!(command >> CMD_NO_REBOOT) & 1U) {
         printf("FRITZ!Box reboots now\n");
       } else {
         printf("use 'ftp %s' to connect to FRITZ!Box\n",box_ip_str);
       }


		}


    close(sd);
    return 0;
}
