#!/bin/bash

CURRENT_GOVERNOR=$(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor)

# Determine the new governor
if [ "$CURRENT_GOVERNOR" = "schedutil" ]; then
    NEW_GOVERNOR="performance"
    echo "" > "/dev/shm/status_bar/tmp.MODE"
else
    NEW_GOVERNOR="schedutil"
    echo "" > "/dev/shm/status_bar/tmp.MODE"
fi

kill $(ps -eo ppid,pid,comm | awk -v parent=$(pgrep -of .config/dwm/bash_script/status_bar.sh) '$1 == parent && $3 == "sleep" {print $2}' | head -n 1)

echo $NEW_GOVERNOR | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

dunstify -r 1236 -I ~/.config/dwm/dunst/uninterruptible-power-supply.png -t 3000 "$NEW_GOVERNOR"

