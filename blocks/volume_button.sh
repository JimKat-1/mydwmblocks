#! /bin/zsh

case $BLOCK_BUTTON in
	1) pavucontrol ;;
	2) pamixer -t && pkill -RTMIN+2 dwmblocks ;;
#	3) pamixer --allow-boost -i 5 ;;
	4) pamixer --allow-boost -i 5 && pkill -RTMIN+2 dwmblocks;;
	5) pamixer --allow-boost -d 5 && pkill -RTMIN+2 dwmblocks;;
esac
