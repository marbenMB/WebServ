#!/bin/bash

dependencies () {


	PACK_MAN=
	echo -n "Enter Name of Your Package Manager (brew , apt, apt-get...): "
	read PACK_MAN

	GOEXEC="alias goExec=\"~/.$PACK_MAN/bin/go\""
	STAT=
	pid=
	atention="🅰 🆃 🅴 🅽 🆃 🅸 🅾 🅽"

	echo "\x1b[33m       " "+> Please Wait Until Installation Is Done" 

	loading &
	pid=$!

	command -v $PACK_MAN &> /dev/null

	if [ $? = "1" ]
	then
		echo "\x1b[31m" "+> Need To Install $PACK_MAN to Run this project Perfectly" "\033[0m"
		exit 1
	fi

	command -v python &> /dev/null
	if [ $? = "1" ]
	then
		$PACK_MAN update &> /dev/null
		$PACK_MAN install python
	fi

	command -v go &> /dev/null
	if [ $? = "1" ]
	then
		$PACK_MAN update &> /dev/null
		$PACK_MAN install go
	fi

	kill $pid
	wait $pid 2>/dev/null
	echo ""
	echo ""

	cat ~/.zshrc | grep -c goExec 1> /dev/null
	if [ $? = "1" ]
	then
		echo "\x1b[32m       " "+> Installing GO alias" "\033[0m"
		echo $GOEXEC >> ~/.zshrc
		echo $GOEXEC >> ~/.bashrc
	else
		echo "\x1b[33m       " "+> GO alias already Installed" "\033[0m"
	fi

	echo ""
	/bin/sleep 1
	echo "\x1b[32m          " "Environement Set Completely" "\033[0m"
	printf "\n\n\n"

	echo "\x1b[31m             " $atention
	printf "\n\n\n"
	echo "\x1b[31m" "Now, befre runing WebServ , run this command in your Terminal : " 
	echo "\x1b[32m         " "If you use zsh : " "source ~/.zshrc"
	echo "\x1b[32m         " "If you use bash : " "source ~/.bashrc"
	echo "" "\033[0m"
}

loading () {

	echo ""
	while [ 1 ]
	do
		echo -ne "█"
		sleep 0.1
	done
}

bound="▄▄ ▄▄ ▄▄ ▄▄ ▄▄ ▄▄ ▄▄ ▄▄ ▄▄ ▄▄ ▄▄
░░ ░░ ░░ ░░ ░░ ░░ ░░ ░░ ░░ ░░ ░░"

logo="░██╗░░░░░░░██╗███████╗██████╗░░██████╗███████╗██████╗░██╗░░░██╗
░██║░░██╗░░██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║░░░██║
░╚██╗████╗██╔╝█████╗░░██████╦╝╚█████╗░█████╗░░██████╔╝╚██╗░██╔╝
░░████╔═████║░██╔══╝░░██╔══██╗░╚═══██╗██╔══╝░░██╔══██╗░╚████╔╝░
░░╚██╔╝░╚██╔╝░███████╗██████╦╝██████╔╝███████╗██║░░██║░░╚██╔╝░░
░░░╚═╝░░░╚═╝░░╚══════╝╚═════╝░╚═════╝░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░
"

clear
echo "\033[0;34m" $logo "\033[0m"
dependencies
echo "\033[0;34m" $bound "\033[0m"
exit 0