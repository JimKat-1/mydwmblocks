#! /bin/zsh

case $BLOCK_BUTTON in
    1)
	setsid st -e ncmpcpp & ;;
    2)
	modes=("normal" "bass" "soft" "dynamic" "clear" "treble")
	current=$(earbuds info | grep Equalizer: | awk '{print $2}' | sed 's/Boost//' | tr '[:upper:]' '[:lower:]')
	i=1
	while [[ $modes[i++] != $current ]]; do;done
	((i<7)) && earbuds set equalizer $modes[i] || earbuds set equalizer $modes[1]
    ;;
    7)
        [[ "$(bluetoothctl info | grep "Name:" | sed 's/.*Name: //')" != "Galaxy Buds+ (4A97)" ]] \
        && (echo "power on\nconnect 80:7B:3E:EF:4A:97" | bluetoothctl >&- && sleep 2 && pkill -RTMIN+3 dwmblocks && exit) \
        || echo "disconnect 80:7B:3E:EF:4A:97" | bluetoothctl >&- && sleep 2 && pkill -RTMIN+3 dwmblocks && exit 
    ;;
esac
