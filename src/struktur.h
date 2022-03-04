#define socket_t int

//Struktur für die zu sendenden Pakete
typedef struct packet{
    char snummer[7], buffer[1024];
} packet;
