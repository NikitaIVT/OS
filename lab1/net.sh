#!/bin/bash

com=$1
arg=$2
arg2=$3
argnum=$#
if [[ -z "$com" ]]
then
echo -e "\nЗапустите скрипт с аргументом -h для получения более подробной информации\n"
fi
if [ "$com" = "-h" ]
then
echo -e "\nАвторы: Мяконьких Дмитрий, Шиндель Эдуард, Полищук Никита"
echo -e "\nДоступны следующие аргументы:"
echo -e "\nВывод справки: -h"
echo -e "\nПример: ./net.sh -h"
echo -e "\nВывод активных сетевых интерфейсов: display"
echo -e "\nПример: ./net.sh display"
echo -e "\nВывод активных сетевых интерфейсов: display all"
echo -e "\nПример: ./net.sh display all"
echo -e "\nВключение сетевого интерфейса: tu <имя_интерфейса2> <имя_интерфейса2>"
echo -e "\nПример: ./net.sh tu lo enp0s3"
echo -e "\nВыключение сетевого интерфейса: td <имя_интерфейса2> <имя_интерфейса2>"
echo -e "\nПример: ./net.sh td lo enp0s3"
echo -e "\nУстановка ip: cip <имя_интерфейса> <новый_ip>"
echo -e "\nПример: ./net.sh cip lo 127.0.0.1"
echo -e "\nУстановка mask: cmask <имя_интерфейса> <новый_mask>"
echo -e "\nПример: ./net.sh cmask lo 255.255.255.0"
echo -e "\nУстановка gateway: cgate <имя_интерфейса> <новый_gateway>"
echo -e "\nПример: ./net.sh cgate enp0s3 192.168.0.1"
fi
if [ "$com" = "display" ] && [ "$arg" = "all"  ] 
then
echo -e "$(sudo ifconfig -a)"
elif [ "$com" = "display" ] && [ -z "$arg" ] 
then 
echo -e "$(sudo ifconfig)"
fi

if [ -n "$arg" ] && [ "$com" = "tu" ]
then
for(( a = 2; a <= argnum; a++))
do
echo -e "$(sudo ifconfig ${2} up)"
shift
done
fi


if [ -n "$arg" ] && [ "$com" = "td" ]
then
for(( a = 2; a <= argnum; a++))
do
echo -e "$(sudo ifconfig ${2} down)"
shift
done
fi

if [ -n "$arg" ] && [ "$com" = "cmask" ] &&
then
echo "$(sudo ifconfig ${2} netmask ${3})"
fi

if [ -n "$arg" ] && [ "$com" = "cip" ] &&
then
echo "$(sudo ifconfig ${2} ${3})"
fi

if [ -n "$arg" ] && [ "$com" = "cgate" ] &&
then
echo "$(sudo route add default gw ${2} ${3})"
fi
