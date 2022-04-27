# zmień ustawienie sieci maszyn w virtualBox!
# TUTORIAL 1
# zmień nazwę i włącz karte sieciową
V1> sudo ip link set enp0s3 name enp0 && sudo ip link set up dev enp0

# Wireshark na enp0
V1> sudo dhclient enp0
# komunikaty w wireshark

# adres IP 0.0.0.0
# z wikipedii:
# W protokole internetowym w wersji 4 adres 0.0.0.0 
# jest nieroutowalnym metadresem używanym do oznaczania 
# nieprawidłowego, nieznanego lub niemającego zastosowania celu.

# Usuń konfigurację interfejsu enp0 poleceniem:
V1> sudo dhclient -r enp0


# zmień ustawienie sieci maszyn w virtualBox!
# TUTORIAL 2
V1> sudo ip link set enp0s3 name enp0 && sudo ip link set up dev enp0 && sudo ip addr add 192.168.0.1/24 dev enp0
V2> sudo ip link set enp0s3 name enp0 && sudo ip link set up dev enp0 && sudo ip addr add 192.168.0.2/24 dev enp0

# Wireshark na enp0
V1> ip link
V2> ip link

V1> ping -c 1 192.168.0.2
# komunikaty w wireshark
V1> ping -c 1 192.168.0.255
# komunikaty w wireshark

# obejrzyj tablicę ARP 
V1> ip neigh
# REACHABLE - można dotrzeć/osiągnąć
# STALE - gdy długo jest osiągalna (długo jest stan REACHABLE)

# wyczyść tablicę ARP 
V1> sudo ip neigh flush all
V2> sudo ip neigh flush all

V1> ping -c 1 192.168.0.2
# komunikaty w wireshark obu maszyn
V1> ip neigh
V2> ip neigh

# dane w ramce ARP: nic konkretnego?
# zapytania ARP -> rozgloszeniowy
# odpowiedzi ARP -> konkretny komputer


# TUTORIAL 3

# Wireshark na enp0
V1> ping 192.168.0.2
# i zostaw dzialajace

# zmien adres ip maszyny V2 kiedy V1 dalej wysyla ping
V2> sudo ip addr del 192.168.0.2/24 dev enp0 && sudo ip addr add 192.168.0.123/24 dev enp0

# wylacz ping na V1 po paru sekundach
# komunikaty w wireshark


# zmień ustawienie sieci maszyn w virtualBox! 
# pamietaj o usunieciu starych adresow ip maszyn V1 i V2 (jesli wczesniej byl tutorial robiony)     !!!
# WYZWANIE

V1> sudo ip link set enp0s3 name enp0 && sudo ip link set up dev enp0 && sudo ip addr add 192.168.1.1/24 dev enp0
V2> sudo ip link set enp0s3 name enp0 && sudo ip link set up dev enp0 && sudo ip addr add 192.168.1.2/25 dev enp0
V3> sudo ip link set enp0s3 name enp0 && sudo ip link set up dev enp0 && sudo ip addr add 192.168.1.129/24 dev enp0
V4> sudo ip link set enp0s3 name enp0 && sudo ip link set up dev enp0 && sudo ip addr add 192.168.1.130/25 dev enp0


# 192.168.1.1/24 - Virbian 1
# Adres sieci - 192.168.1.0
# Adres rozgłoszeniowy - 192.168.1.255
# Zakres adresów 192.168.1.1 - 192.168.1.254 (1)
# 192.168.1.2/25 - Virbian 2
# Adres sieci - 192.168.1.0
# Adres rozgłoszeniowy - 192.168.1.127
# Zakres adresów 192.168.1.1 - 192.168.1.126 (2)
# 192.168.1.129/24 - Virbian 3
# Adres sieci - 192.168.1.0
# Adres rozgłoszeniowy - 192.168.1.255
# Zakres adresów 192.168.1.1 - 192.168.1.254 (1)
# 192.168.1.130/25 - Virbian 4
# Adres sieci - 192.168.1.128
# Adres rozgłoszeniowy - 192.168.1.255
# Zakres adresów 192.168.1.129 - 192.168.1.254 (3)

# V1,V2,V3,V4 należą do (1) - maska 24
# V1,V2 należą do (2)
# V3,V4 należą do (3)

# Wireshark na enp0
V1> ping -c 1 192.168.1.255
# Odpowiedział V3, wszyscy dostali.
V2> ping -c 1 192.168.1.127
# Nikt nie odpowiedział, wszyscy dostali.
V3> ping -c 1 192.168.1.255
# Odpowiedział V1,V4, wszyscy dostali.
V4> ping -c 1 192.168.1.255
# Odpowiedział V1,V3, wszyscy dostali.
