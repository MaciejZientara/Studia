# TUTORIAL 1
# zmień ustawienie sieci maszyn w virtualBox!

# wyświetla interfejsy sieciowe:
Vi> ip link

# zlokalizuj interfejs podłączony do sieci remote i local1 i zmień ich nazwy:
V1> sudo ip link set enpxsy name enp-rem
V1> sudo ip link set enpwsz name enp-loc

V2> sudo ip link set enpxsy name enp-rem
V2> sudo ip link set enpwsz name enp-loc

# ip link set up - włącz interfejs
# ip addr add przypisz ip do interfejsu
V1> sudo ip link set up dev enp-rem
V1> sudo ip addr add 172.16.0.1/16 dev enp-rem
V2> sudo ip link set up dev enp-rem
V2> sudo ip addr add 172.16.0.2/16 dev enp-rem

# wyswietl skonfigurowane interfejsy
Vi> ip addr

# wyświetl tablice routing
V1> ip route
V1> route -n

V1> ping -c 1 172.16.0.2
V2> ping -c 1 172.16.0.1

V1> ping -c 1 127.0.0.1
# tego ping nie widac, zamiast ip route:
V1> ip route list table local

                        adresy rozgłoszeniowe i scope???

# włącz wireshark

V1> ping -c 1 127.0.0.1
V1> ping -c 1 172.16.0.1
V1> ping -c 1 172.16.0.2

V1> ping -c 1 -b 172.16.255.255
# jeśli nie dochodzi to:
V1> sysctl net.ipv4.icmp echo ignore broadcasts 
# jeśli równe 1 to:
V1> sudo sysctl net.ipv4.icmp echo ignore broadcasts=0

V1> ping -c 1 172.16.0.0
V1> ping -c 1 10.10.10.10


# TUTORIAL 2

# na każdej maszynie:
Vi> sysctl net.ipv4.ip_forward
# jeśli równe 0 to:
Vi> sudo sysctl net.ipv4.ip_forward=1

V1> sudo ip link set up dev enp-loc
V1> sudo ip addr add 192.168.11.1/24 dev enp-loc
V3> sudo ip link set up dev enp-loc
V3> sudo ip addr add 192.168.11.2/24 dev enp-loc

V2> sudo ip link set up dev enp-loc
V2> sudo ip addr add 192.168.12.1/24 dev enp-loc
V4> sudo ip link set up dev enp-loc
V4> sudo ip addr add 192.168.12.2/24 dev enp-loc


V1> ping -c 1 192.168.11.2
V1> ping -c 1 172.16.0.2

V2> ping -c 1 192.168.12.2
V2> ping -c 1 172.16.0.1

V3> ping -c 1 192.168.11.1

V4> ping -c 1 192.168.12.1



V1> ping -c 1 192.168.12.1
V2> ping -c 1 192.168.11.1

V1> sudo ip route add 192.168.12.0/24 via 172.16.0.2
V2> sudo ip route add 192.168.11.0/24 via 172.16.0.1

V1> ping -c 1 192.168.12.1
V2> ping -c 1 192.168.11.1



V1> ping -c 1 192.168.12.2

V4> sudo ip route add default via 192.168.12.1
V3> sudo ip route add default via 192.168.11.1

V1> ping -c 1 192.168.12.2

V3> ip route
V4> ip route

V3> ping -c 1 192.168.12.2

V3> sudo traceroute 192.168.12.2
V4> sudo traceroute 192.168.11.2


# WYZWANIE
# zmień ustawienie sieci maszyn w virtualBox!

# wyświetla interfejsy sieciowe:
Vi> ip link

# zlokalizuj interfejs podłączony do sieci remote i local1 i zmień ich nazwy:
V0> sudo ip link set enpxsy name enp-loc0

V1> sudo ip link set enpxsy name enp-loc0
V1> sudo ip link set enpwsz name enp-loc1

V2> sudo ip link set enpwsz name enp-loc1

# włącz interfejsy
V0> sudo ip link set up dev enp-loc0

V1> sudo ip link set up dev enp-loc0
V1> sudo ip link set up dev enp-loc1

V2> sudo ip link set up dev enp-loc1

# nadaj interfejsom adresy IP
V0> sudo ip addr add 192.168.0.1/24 dev enp-loc0

V1> sudo ip addr add 192.168.0.2/24 dev enp-loc0
V1> sudo ip addr add 192.168.1.1/24 dev enp-loc1

V2> sudo ip addr add 192.168.1.2/24 dev enp-loc1

# konfiguracja sieciowa
V0> sudo dhclient -v enp-ext


V0> ping -c 1 192.168.0.2

V1> ping -c 1 192.168.0.1
V1> ping -c 1 192.168.1.2

V2> ping -c 1 192.168.1.1

# konfiguracja routingu
V0> sudo ip route add 192.168.1.0/24 via 192.168.0.2

V1> sudo ip route add default via 192.168.0.1

V2> sudo ip route add default via 192.168.1.1


V0> ping -c 1 192.168.1.2
V2> sudo traceroute 192.168.0.1

V0> ping -c 1 8.8.8.8

V2> ping -c 1 8.8.8.8

# trzeba by poinformowac jakis główny router? bo serwer 8.8.8.8 nie będzie wiedział gdzie to wysłać
# bo nie zna naszej lokalnej sieci, więc wysłałby na swój adres default i to właśnie tam należało by dodać?

