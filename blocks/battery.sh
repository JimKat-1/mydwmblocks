#! /bin/zsh

zmodload zsh/mathfunc

LB=10

[ ! -f /tmp/.BatteryCritical ] && echo 0 > /tmp/.BatteryCritical

capacity=$(cat /sys/class/power_supply/BAT0/capacity)
statuss=$(cat /sys/class/power_supply/BAT0/status)

( [[ $statuss == Charging ]] || [[ $statuss == Full ]] ) && echo -n "⚡" || echo -n "🔋"
echo -n " $capacity% ^r0,3,9,15^^r2,1,5,3^^c#ff0000^^r1,$((4 + int(10 * (1 - capacity / 100.)))),7,$((4 + int(10 * capacity / 100.)))^^d^^f10^"

[[ capacity -le $LB && $(cat /tmp/.BatteryCritical) == 0 && $statuss == "Discharging" ]] && notify-send -u critical "LOW BATTERY" && echo 1 > /tmp/.BatteryCritical
[[ $statuss == "Charging" && $(cat /tmp/.BatteryCritical) == 1 ]] && echo 0 > /tmp/.BatteryCritical
