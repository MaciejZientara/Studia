# zmień ustawienie sieci maszyn w virtualBox!
dla kazdej maszyny ustawiam 3 karty sieciowe z siecia local

# zlokalizuj interfejs podłączony do sieci remote i local1 i zmień ich nazwy:
Vi> sudo ip link set enp#s# name enp-rem#
Vi> sudo ip link set enp#s# name enp-rem#
Vi> sudo ip link set enp#s# name enp-all

# ip link set up - włącz interfejs
# ip addr add przypisz ip do interfejsu
V1> sudo ip link set up dev enp-rem1 && sudo ip addr add 192.168.1.1/24 dev enp-rem1
V1> sudo ip link set up dev enp-rem4 && sudo ip addr add 192.168.4.1/24 dev enp-rem4

V2> sudo ip link set up dev enp-rem1 && sudo ip addr add 192.168.1.2/24 dev enp-rem1
V2> sudo ip link set up dev enp-rem2 && sudo ip addr add 192.168.2.2/24 dev enp-rem2

V3> sudo ip link set up dev enp-rem2 && sudo ip addr add 192.168.2.3/24 dev enp-rem2
V3> sudo ip link set up dev enp-rem3 && sudo ip addr add 192.168.3.3/24 dev enp-rem3

V4> sudo ip link set up dev enp-rem3 && sudo ip addr add 192.168.3.4/24 dev enp-rem3
V4> sudo ip link set up dev enp-rem4 && sudo ip addr add 192.168.4.4/24 dev enp-rem4

# wyświetl tablice routing (2 wpisy)
Vi> ip route

Vi> ping -c 1 192.168.#.#

# TUTORIAL 1

# dodanie bramek domyslnych do tablicy routingu
V1> sudo ip route add default via 192.168.1.2
V2> sudo ip route add default via 192.168.2.3
V3> sudo ip route add default via 192.168.3.4
V4> sudo ip route add default via 192.168.4.1

# wyświetl tablice routing (3 wpisy)
Vi> ip route

Vi> ping -c 1 192.168.#.#

V1> sudo traceroute 192.168.#.#

# usuniecie bramek domyslnych z tablicy routingu
V1> sudo ip route del default via 192.168.1.2
V2> sudo ip route del default via 192.168.2.3
V3> sudo ip route del default via 192.168.3.4
V4> sudo ip route del default via 192.168.4.1

# TUTORIAL 2

Vi> sudo touch /etc/quagga/ospfd.conf && sudo touch /etc/quagga/zebra.conf && sudo touch /etc/quagga/vtysh.conf
Vi> sudo systemctl start ospfd

Vi> sudo vtysh
# pomoc z klawiszem '?'

# w terminalu vtysh, dla kazdej maszyny:
virbian# show ip route

# przeniesienie sie do trybu konfiguracji
virbian# configure terminal
virbian(config)# router ospf

# x i y to sieci dla danej maszyny, np dla V2 to 1,2
virbian(config-router)# network 192.168.x.0/24 area 0
virbian(config-router)# network 192.168.y.0/24 area 0

# Wyjdź z trybu konfiguracji i wyświetl aktualną konfigurację poleceniami
virbian(config-router)# end
virbian# show running-config

# w wireshark:
# Internet Protocol -> Network Layer -> warstwa sieci
# Transmission Control Protocol -> Transport Layer -> warstwa transportowa
# Nie korzysta z warstwy transportowej.

virbian# show ip route
Vi> sudo ip route

# sprawdz polaczenia:
Vi> ping -c 1 192.168.#.#
Vi> sudo traceroute 192.168.#.#

V1> sudo ip link set up dev enp-all && sudo ip addr add 172.16.16.1/16 dev enp-all
V2> sudo ip link set up dev enp-all && sudo ip addr add 172.16.16.2/16 dev enp-all
V3> sudo ip link set up dev enp-all && sudo ip addr add 172.16.16.3/16 dev enp-all
V4> sudo ip link set up dev enp-all && sudo ip addr add 172.16.16.4/16 dev enp-all

# na kazdej maszynie w vtysh:
virbian# configure terminal
virbian(config)# router ospf
virbian(config-router)# network 172.16.0.0/16 area 0

virbian(config-router)# end
virbian# show running-config
virbian# show ip route

# zakoncz usluge ospf
Vi> sudo systemctl stop ospfd



# WYZWANIE
# zmień ustawienie sieci maszyn w virtualBox!

# zlokalizuj interfejs podłączony do sieci remote i local1 i zmień ich nazwy:
V1> sudo ip link set enp0s3 name enp0
V2> sudo ip link set enp0s3 name enp0
V3> sudo ip link set enp0s3 name enp0 && sudo ip link set enp0s8 name enp1
V4> sudo ip link set enp0s3 name enp-out

# ip link set up - włącz interfejs
# ip addr add przypisz ip do interfejsu
V1> sudo ip link set up dev enp0 && sudo ip addr add 192.168.1.1/24 dev enp0
V2> sudo ip link set up dev enp0 && sudo ip addr add 192.168.1.2/24 dev enp0
V3> sudo ip link set up dev enp0 && sudo ip addr add 192.168.1.3/24 dev enp0 && sudo ip link set up dev enp1 && sudo ip addr add 192.168.2.1/24 dev enp1
V4> sudo ip link set up dev enp-out && sudo ip addr add 192.168.2.2/24 dev enp-out

# sprawdz polaczenia:
Vi> ping -c 1 192.168.#.#

# dodanie bramek domyslnych do tablicy routingu
V1> sudo ip route add default via 192.168.1.2
V2> sudo ip route add default via 192.168.1.3
V4> sudo ip route add default via 192.168.2.1

# WIRESHARK
# filter: icmp

# Wybieramy pakiet Redirect
# Internet Control Message Protocol:
# Gateway Address: 192.168.1.3

# Virbian 2 sugeruje aby wysyłać pakiety przez Virbian3.

# Skróci to trasę przesyłania pakietu.
# Teraz V1->V2->V3->V4
# Po zmianie V1->V3->V4

# V2 dostał pakiet przez enp0/local0 i wysyła przez ten sam interfejs
