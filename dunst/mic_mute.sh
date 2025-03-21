#!/bin/bash

MUTED=$(pactl get-source-mute @DEFAULT_SOURCE@ | grep -oP 'yes|no')

pactl set-source-mute @DEFAULT_SOURCE@ toggle

if [ "$MUTED" = "yes" ]; then
  dunstify -r 1232 -I ~/.config/dwm/dunst/microphone-sensitivity-high.png -t 3000 ""
  echo  > /dev/shm/tmp.MIC
else
  dunstify -r 1232 -I ~/.config/dwm/dunst/microphone-sensitivity-muted.png -t 3000 ""
  echo  > /dev/shm/tmp.MIC
fi

kill $(ps -eo ppid,pid,comm | awk -v parent=$(pgrep -of .config/dwm/bash_script/status_bar.sh) '$1 == parent && $3 == "sleep" {print $2}' | head -n 1)

