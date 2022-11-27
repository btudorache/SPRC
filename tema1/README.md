# Tema 1 - Implementare protocol OAuth prin RPC

344C3 Tudorache Bogdan Mihai

Tema contine implementarea protocolului **OAuth** printr-o 
aplicatie client-server peste protocolul **RPC**

In mare parte am respectat pasii descrisi in etapele 
de rezolvare specificate in enunt. 

Tema a fost implementata exclusiv in **C** si am folosit
structurile de date (map si array redimensionabil) de la
acest repo de github: https://github.com/tezc/sc

Am modificat stub-ul generat pentru server, pentru
a putea face initializarea structurilor de date necesare
si scrierea din fisierele primite ca argumente in linia
de comanda. Tot ce am facut este sa adaug o functie de 
initializare pe care o apelez in main, si declar si niste 
containere globale pe care le accesez in fisierul unde sunt 
implementate functiile din server prin directiva ```extern```.

Am folosit varianta checker-ului care adauga un timp de sleep 
de o secunda dupa ce porneste server-ul pentru ca serverul sa aiba
timp sa faca toate initializarile necesare pana sa porneasca client-ul.
De asemenea am ales abordarea de a citi timpii de expirare ai token-urilor
dintr-un fisier pentru consistenta.

Pentru compilare nu mai trebuie generate stub-urile de la RPC. Daca
se vrea recompilarea stub-urilor, trebuie modificat iar fisierul server

## Detalii de implementare

Tema contine 4 etape mari:

1. Definirea interfetei RPC
2. Initializat structurile necesare pe partea de server
3. Implementat functiile interfetei in server
4. Implementat clientul care se foloseste de functiile din server

Am folost un map pe partea de server cu maparea de la numele utilizatorului 
la o structura cu toate datele pe care trebuie sa le avem despre el: nume, tokeni, 
permisiuni etc... .

Mai folosesc de asemenea map-uri pentru a face trecere de la nume -> token de access 
si invers. 

Unele dintre valorile de return ale functiile definite in interfata RPC contin flag-uri
pentru a specifica valori de return obtionale (de exemplu, daca apelul s-a terminat cu eroare,
sau daca serverul a facut prelucrari aditionale de care trebuie sa fie notificat client-ul).

In rest, fluxul de executie este in mare parte cel descris in specificatia temei.