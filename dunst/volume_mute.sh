#!/bin/bash

MUTED=$(pactl get-sink-mute @DEFAULT_SINK@ | grep -oP 'yes|no')
VOLUME=$(pactl get-sink-volume @DEFAULT_SINK@ | grep -oP '\d+%' | head -n 1)

pactl set-sink-mute @DEFAULT_SINK@ toggle

if [ "$MUTED" = "yes" ]; then
  dunstify -r 1234 -h int:value:"$VOLUME" -I ~/.config/dwm/dunst/audio-volume-high.png -t 3000 "$VOLUME"
  echo "" > "/dev/shm/tmp.MUTE"
else
  dunstify -r 1234 -h int:value:"$VOLUME" -I ~/.config/dwm/dunst/audio-volume-muted.png -t 3000 "Muted"
  echo "" > "/dev/shm/tmp.MUTE"
fi

kill $(ps -eo ppid,pid,comm | awk -v parent=$(pgrep -of .config/dwm/bash_script/status_bar.sh) '$1 == parent && $3 == "sleep" {print $2}' | head -n 1)

