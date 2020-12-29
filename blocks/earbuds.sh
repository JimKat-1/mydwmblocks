#! /bin/zsh

echo -n "🎧"
[[ "$(bluetoothctl info | grep "Name:" | sed 's/.*Name: //')" != "Galaxy Buds+ (4A97)" ]] && exit

earbuds -d 2>&-

earbuds info | grep Equalizer: | awk '{printf " "$2" "}' | sed 's/Boost//'
case=$(earbuds info | grep Case: | awk '{printf $2" "}')
earbuds info | grep Battery: | awk '{printf "❮"$3 $5"❯"}' | sed "s/,/ $case/"
