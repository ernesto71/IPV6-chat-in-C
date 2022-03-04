#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "struktur.h"

int main(int argc, char *argv[]) {

	//ints für recv, send, select, setsockopt
	int rc, snd, ready, on = 1;
	
	//Socketdeskriptoren
    int listenersock, accept_sock;
    int portno, sockmax = -1;
    
	//Adr. für Client/Server
    socklen_t clilen;
    struct sockaddr_in6 serv_addr, cli_addr;     
	
	//Client IPV6 Adresse
    char client_addr_ipv6[100];
			
	//Packetspeicher reservieren
    packet *ph = malloc(sizeof(packet));							//Host-Packet
    packet *pc = malloc(sizeof(packet));							//Client-Packet
	
    //filedescriptors erstellen und null setzen
    fd_set master;													//Verwaltung eingehender Connections(Sockets)
	FD_ZERO(&master); 												//alle Sockets auf Null setzen
    fd_set read_fds;												//Verwaltung leseberechtigter Connections
	FD_ZERO(&read_fds); 											//leseberechtigte Sockets auf Null setzen
	//listenersocket wiederverwendbar setzen
    setsockopt(listenersock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	
	//Anzahl der übergebenen Variablen prüfen
    if (argc != 3) {
        fprintf(stderr, "Fehler! Bitte so aufrufen: ./host Portnummer sNummer");
        exit(0);
    }

    //Ausgabe Port und sNummer
    printf("Portnr:\t%s\n", argv[1]);
    printf("sNr:\t%s\n", argv[2]);
    printf("\nHost wird gestartet...\n");
    
    //sNummer dem Packet zuweisen
    strcpy (ph->snummer, argv[2]); 
    
    //Erzeugung eines listenersocket
    listenersock = socket(AF_INET6, SOCK_STREAM, 0);
    //Fehlermeldung
    if (listenersock < 0){
        perror("Fehler beim Erzeugen des Sockets!");
    }else{
        printf("Socket erzeugt!\n");
    }

    //serv_addr clearen und Portnr, Adressfamilie und Serveradresse festlegen
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin6_flowinfo = 0;									//Flusskontrolle
    serv_addr.sin6_family = AF_INET6;								//Internetprotokollfamilie IPV6
    serv_addr.sin6_addr = in6addr_any;								//server_addr.sin6_addr speichert Serverinformationen
    serv_addr.sin6_port = htons(portno);							//portno-struct mit Portnummer belegen
    	
	
    //Bind - festlegen der IP-Adresse und Portnummer für listenersocket
    if (bind(listenersock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("\nFehler bei bind()g!\n");
    }
    
    //"horchen" auf Verbindungen
    listen(listenersock, 5);
	//listenersock und stdin ins set einfügen
    FD_SET(listenersock, &master);
    FD_SET(fileno(stdin), &master);
    
    //Speichern des höchsten descriptors
    sockmax = listenersock;
    
    printf("Warten auf Verbindung...!\n");
    
    //Akzeptieren
    clilen = sizeof(cli_addr);
    accept_sock = accept(listenersock, (struct sockaddr*)&cli_addr, &clilen);
    //Fehlermeldung
    if (accept_sock < 0){
        perror("\nFehler bei accept()\n");
    }
	
    FD_SET(accept_sock,&master);
	
	//max prüfen
    if (accept_sock > sockmax) {
            sockmax = accept_sock;
        }
    //Verbindungsinformation
    inet_ntop(AF_INET6, &(cli_addr.sin6_addr),client_addr_ipv6, 100);
    printf("\nEingehende Connection von der IP-Adresse: %s\n",client_addr_ipv6);
    printf("\nSie können nun kommunizieren.\n\n");

	//Hauptschleife
    for(;;){
        //Kopie von master anlegen zur Übergabe ins select()
        read_fds = master;

        //Ankunft von Daten oder neue Verbindung von Clients erwarten
        ready = select(sockmax+1, &read_fds, NULL, NULL, NULL);
        //Fehler
        if(ready == -1){
            perror("Fehler bei select()\n");
        }
        
        //Verbindung auf Ankunft neuer Daten prüfen
        if(FD_ISSET(accept_sock, &read_fds)) {
			//Nachricht empfangen
            rc = recv(accept_sock, pc, sizeof(packet), 0);
            if (rc <= 0) {
                if(rc == 0) {
					//Client beendet
                    printf("Der Client hat sich beendet...");
                    close(accept_sock);								//Socket schliessen
                    FD_CLR(accept_sock, &read_fds);					//aus set löschen
                    break;
                } else {
                    perror("Fehler bei receive()\n");
                    FD_CLR(accept_sock, &read_fds);
                }
            } else {
                //Nachrichtenausgabe 
                printf(">%s : %s", pc->snummer, pc->buffer);
                fflush(stdout);
            }
        }

        //stdin auf Eingaben prüfen
        if(FD_ISSET(fileno(stdin), &read_fds)) {
            //Eingaben holen und ins Paket legen
            fgets(ph->buffer, sizeof(ph->buffer), stdin);
            //Versenden
            snd = send(accept_sock, ph, sizeof(packet), 0);
			//Ausgabebuffer bei jedem Durchlauf leeren leeren
            fflush(stdout);
            //Fehler
            if(snd < 0){  
                perror("Fehler bei send()");
            }
        }
    }
    
    return 0;
}
