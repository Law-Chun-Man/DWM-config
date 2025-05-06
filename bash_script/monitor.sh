#!/bin/bash

echo "Choose an option:"
echo "1) Extend display"
echo "2) Only external monitor"
echo "3) Only built-in monitor"
echo "4) Only built-in monitor (1920x1080)"

read -p "Enter your choice (1-4): " choice

case $choice in
  1)
    xrandr --output HDMI-A-0 --auto --primary --output eDP --auto --below HDMI-A-0
    ;;
  2)
    xrandr --output HDMI-A-0 --auto --output eDP --off
    ;;
  3)
    xrandr --output HDMI-A-0 --off --output eDP --auto
    ;;
  4)
    xrandr --output HDMI-A-0 --off --output eDP --mode 1920x1080
    ;;
  *)
esac

