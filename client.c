#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "struktur.h"

int main(int argc, char *argv[]) {
	//verschiedene ints für sockets, select, receive, send
    int socksd, portno, ready, rc, snd, sockmax = -1;

	//Strukturen für Adresse und Hostinformationen
    struct sockaddr_in6 serv_addr;
    struct hostent *server;
	
	//filedescriptors erstellen und null setzen
    fd_set master;
	fd_set read_fds;
	FD_ZERO(&master);
	
	//Paketspeicher
    packet *pc = malloc(sizeof(packet));
    packet *ph = malloc(sizeof(packet));
	
	//Prüfe Anzahl der Argumente
    if (argc != 4) {
        fprintf(stderr, "Fehler! Bitte so aufrufen: ./client IP Portnummer sNummer\n");
        exit(0);
    }
    
	//Ausgabe der Verbindungsinformationen
    printf("Zu verbindende IP:\t%s\n", argv[1]);
    printf("Portnr:\t\t\t%s\n", argv[2]);
    printf("Ihre sNummer:\t\t%s\n", argv[3]);
	
	//portno in int konvertieren
    portno = atoi(argv[2]);
    
	//sNummer dem Packet zuweisen
    strcpy(pc->snummer, argv[3]); 
	
    
	//Erzeugung Socket-Deskriptor
    socksd = socket(AF_INET6, SOCK_STREAM, 0);
    if (socksd < 0){
        perror("Fehler beim Erzeugen des Sockets!\n");
    }
	
	//Wandelt Servernamen in IP-Adresse um
    server = gethostbyname2(argv[1], AF_INET6);
    if (server == NULL) {
        fprintf(stderr, "\n");
        exit(0);
    }
	
    memset((char *) &serv_addr, 0, sizeof(serv_addr));												//Speicher für serv_addr reservieren
    serv_addr.sin6_flowinfo = 0;                                                                    //Flusskontrolle
    serv_addr.sin6_family = AF_INET6;                                                               //Internetprotokollfamilie IPV6
    memmove((char *) &serv_addr.sin6_addr.s6_addr, (char *) server->h_addr, server->h_length);      //server_addr.sin6_addr speichert Serverinformationen
    serv_addr.sin6_port = htons(portno);                                                            //portno-struct mit Portnummer belegen

	//Baut Verbindung zum Server auf
    if (connect(socksd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("Fehler bein connect()!\n");
    }else{
        printf("\nErfolgreich verbunden!\n");
    }
    
	//listenersock und stdin ins set einfügen
    FD_SET(socksd, &master);
    FD_SET(fileno(stdin), &master);
	//Speichern des höchsten descriptors
    sockmax = socksd; 																				

    printf("\nSie können nun chatten!\n\n");
	
	//Hauptschleife
    for(;;){         
		//Kopie von master anlegen
        read_fds = master;
		
		//Ankunft von Daten erwarten
        ready = select(sockmax+1, &read_fds, NULL, NULL, NULL);
        //Fehler
        if(ready == -1){
            perror("Fehler bei select()\n");
        }

		//Verbindung auf Ankunft neuer Daten prüfen
        if(FD_ISSET(socksd, &read_fds)){
			//Nachricht empfangen und auf packet schreiben
            rc = recv(socksd, ph, sizeof(packet), 0);
            if (rc <= 0){
                if(rc == 0){ 
					//Host hat beendet
                    printf("Host hat sich beendet...");
					//Client beenden
                    close(socksd);
                    return 0;
                } else {
                    perror("Fehler beim Empfagen!\n");
                }
            } else {
			//Nachrichtenausgabe
            printf(">%s : %s", ph->snummer, ph->buffer);
            }
        }
		
		//stdin auf Inputs prüfen
        if(FD_ISSET(fileno(stdin), &read_fds)){                                                     
			//Eingaben holen und ins Paket legen
            fgets(pc->buffer, sizeof(pc->buffer), stdin);
            //packet verschicken
            snd = send(socksd, pc, sizeof(packet), 0);
		    if (snd < 0){
			    perror("Fehler beim senden!\n");
            }
			//Bei Eingabe von "bye" wird das Programm beendet
            if(strcmp(pc->buffer, "bye\n") == 0){
                close(socksd);
                return 0;
            }
        }
    }
	//Socket wird geschlossen
    close(socksd);
        
    return 0;
}
