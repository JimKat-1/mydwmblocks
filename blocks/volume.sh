#! /bin/zsh

volume=$((`pamixer --get-volume` / 5 ))
[[ $(pamixer --get-mute) == false ]] && echo -n "🔊 " || echo -n "🔇 "
echo -n "$volume%"
(( $volume > 100 )) && echo -n " (CAN YOU NOT)"
