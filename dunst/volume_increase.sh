#!/bin/bash

pactl set-sink-volume @DEFAULT_SINK@ +5%

MUTED=$(pactl get-sink-mute @DEFAULT_SINK@ | grep -oP 'yes|no')
VOLUME=$(pactl get-sink-volume @DEFAULT_SINK@ | grep -oP '\d+%' | head -n 1)

echo "$VOLUME" > /dev/shm/status_bar/tmp.VOLUME
kill $(ps -eo ppid,pid,comm | awk -v parent=$(pgrep -of .config/dwm/bash_script/status_bar.sh) '$1 == parent && $3 == "sleep" {print $2}' | head -n 1)

if [ "$MUTED" = "yes" ]; then
  dunstify -r 1234 -h int:value:"$VOLUME" -I ~/.config/dwm/dunst/audio-volume-muted.png -t 3000 "Muted"
else
  dunstify -r 1234 -h int:value:"$VOLUME" -I ~/.config/dwm/dunst/audio-volume-high.png -t 3000 "$VOLUME"
fi

