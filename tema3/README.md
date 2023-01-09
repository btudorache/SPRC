# Tema 3 - Platformă IoT folosind Microservicii

344C3 Tudorache Bogdan Mihai

Tema contine implementarea unei platforme pentru colectarea, stocarea si vizualizarea datelor provenite
de la dispozitive IoT. 

Solutia se porneste ruland ./run.sh

In cea mai mare parte am respectat indicatiile/cerintele din enuntul temei, cu cateva exceptii:

- versiunea de InfluxDB pe care o folosesc (2.6) are suport pentru limbajul de query flux, 
asa ca nu mai e nevoie de expresii regulate pentru actualizarea dashboard-urilor cu cele mai recente date
 in query-urile din grafana

- query-urile flux din grafana sunt corecte, insa pare ca baza de date nu suporta refresh-urile 
query-urilor atat de rapide pe cat se doreste in enuntul bazei de date, si imi "crapa" baza de date cand grafana
incearca sa faca prea multe request-uri

- dashboard-ul cu durata de viata a bateriilor nu este tocmai functional

- docker swarm (din cate am inteles din documentaitii oficiale/sugestii de pe internet) nu este facut sa ruleze cu imagini de docker locale, numai cu imagini care exista deja pe niste registry-uri (fie ele remote sau locale). Astfel, am creat propiul registry local si am pus acolo imaginea de docker
construita cu fisierul Dockerfile, ca sa poata fi vazuta de swarm. Scriptul run.sh intializeaza registry-ul si porneste stack-ul swarm