#!/bin/bash

# XFCE
/usr/libexec/at-spi-bus-launcher --launch-immediately &
nm-applet &
/usr/lib/policykit-1-gnome/polkit-gnome-authentication-agent-1 &
/usr/lib/x86_64-linux-gnu/libexec/kdeconnectd &
xfce4-power-manager &
light-locker &
xfsettingsd &
xiccd &  #for colours

# Other settings
xmodmap .Xmodmap
xfce4-clipman &
/usr/bin/sudo tlp start
picom --backend glx --vsync -b
kill $(pgrep -f ~/.config/dwm/bash_script/status_bar.sh)
~/.config/dwm/bash_script/status_bar.sh &
#picom --backend glx --vsync --corner-radius 10 --inactive-opacity 0.8 -b
#xset s 600
dunst &
xset r rate 250 40 # set keyboard repeat rate
pactl get-source-mute @DEFAULT_SOURCE@ | grep -q 'no' && pactl set-source-mute @DEFAULT_SOURCE@ toggle &
#/usr/bin/ibus-daemon &
#blueman-applet &
#/bin/bash -c "sudo rfkill unblock bluetooth"
#echo schedutil | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor

