# IPV6-chat-in-C


## Projekt ##

Die enthaltenen Quelldateien samt der Header-Datei stellen des Chat Programm, welches im Zuge des RNKS Beleges entwickelt wurde, dar.

## Programmaufruf ##

Vor dem Aufruf müssen sämtliche Dateien im selben Verzeichnis liegen und bereits kompiliert sein.
Die Anwendungen sollten direkt in der Konsole ausgeführt werden.

Der Client wird nach folgendem Schema aufgerufen:<br>
./client [IP] [Portnummer] [sNummer]<br>

Der Host wird nach folgendem Schema aufgerufen:<br>
./host [Portnummer] [sNummer]<br>

Das Programm wird beendet durch die Eingabe von "bye" und [ENTER] .<br>

## Quellen ##

Diese Quellen wurden zur selbstständigen Bearbeitung des Belegs verwendet.<br>

Zum allgemeinen Verständnis:<br>
https://www2.htw-dresden.de/~skuehn/VL/rnks/8_Sockets.pdf;<br>
https://openbook.rheinwerk-verlag.de/c_von_a_bis_z/025_c_netzwerkprogrammierung_004.htm;<br>
https://openbook.rheinwerk-verlag.de/c_von_a_bis_z/025_c_netzwerkprogrammierung_005.htm;<br>
https://openbook.rheinwerk-verlag.de/c_von_a_bis_z/025_c_netzwerkprogrammierung_006.htm;<br>
https://openbook.rheinwerk-verlag.de/c_von_a_bis_z/025_c_netzwerkprogrammierung_007.htm;<br>

Zum weiterführenden Verständnis des Zusammenwirkens unterschiedlicher Funktionen:<br>
http://openbook.rheinwerk-verlag.de/linux_unix_programmierung/Kap11-013.htm#RxxKap11013040003921F03B100;<br>
https://www.ibm.com/docs/en/zos/2.1.0?topic=otap-clientserver-socket-programs-blocking-nonblocking-asynchronous-socket-calls;<br>
https://bitbucket.org/sloankelly/youtube-source-repository/src/39d0e0460016338163d43d9bc01d4a45b1826619/cpp/networking/MultipleClientsBarebonesServer/MultipleClientsBarebonesServer/main.cpp?at=master;<br>


Zum vertiefenden Verständnis verschiedener Funktionen:<br>
https://man7.org/linux/man-pages/man2/socket.2.html;<br>
https://man7.org/linux/man-pages/man2/connect.2.html;<br>
https://man7.org/linux/man-pages/man2/accept.2.html;<br>
https://man7.org/linux/man-pages/man2/select.2.html;<br>
https://man7.org/linux/man-pages/man2/recvmsg.2.html;<br>
https://man7.org/linux/man-pages/man2/send.2.html;<br>
https://manpages.debian.org/buster-backports/manpages-de-dev/FD_SET.3.de.html;<br>
https://www.mkssoftware.com/docs/man3/select.3.asp;<br>
https://manpages.debian.org/testing/manpages-de-dev/FD_ISSET.3.de.html;<br>
https://manpages.debian.org/buster-backports/manpages-de-dev/FD_ZERO.3.de.html;<br>
https://linux.die.net/man/3/fd_zero;<br>
https://man7.org/linux/man-pages/man2/close.2.html;<br>

Zum Erstellen des Headers:<br>
https://www.geeksforgeeks.org/write-header-file-c/;<br>

