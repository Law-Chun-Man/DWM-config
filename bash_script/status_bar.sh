#!/bin/bash

mkdir /dev/shm/status_bar
INTERNET=$(mktemp -p /dev/shm/status_bar)
WIFI=$(mktemp -p /dev/shm/status_bar)
BAT=$(mktemp -p /dev/shm/status_bar)
CPU=$(mktemp -p /dev/shm/status_bar)
MEM=$(mktemp -p /dev/shm/status_bar)
trap 'rm -f "$INTERNET" "$WIFI" "$BAT" "$CPU" "$MEM"' EXIT
VOLUME="/dev/shm/status_bar/tmp.VOLUME"
MUTE="/dev/shm/status_bar/tmp.MUTE"
MIC="/dev/shm/status_bar/tmp.MIC"
DATE=$(date +"%a %d %b")
echo "$(pactl get-sink-volume @DEFAULT_SINK@ | awk '{print $5}')" > $VOLUME
echo "" > $MUTE
echo "" > $MIC

# battery
(
while true; do
    status=$(cat "/sys/class/power_supply/BAT0/status" 2>/dev/null)
    capacity=$(cat "/sys/class/power_supply/BAT0/capacity" 2>/dev/null)
    
    case "$status" in
        "Charging")     symbol="" ;;
        "Discharging")  symbol="" ;;
        "Full")         symbol="" ;;
        "Not charging") symbol="" ;;
        *)              symbol="?" ;;
    esac
    
    echo "${symbol} ${capacity}%" > "$BAT"
    sleep 60
done
) &

# wifi
(
while true; do
    INTERFACE="wlp2s0"
    
    get_rx_bytes() {
        awk -v intf="$INTERFACE:" '$1 == intf {print $2}' /proc/net/dev
    }
    
    old_rx=$(get_rx_bytes)
    if [ -z "$old_rx" ]; then
        echo "Error: Interface $INTERFACE not found!"
        exit 1
    fi
    
    sleep 1
    
    new_rx=$(get_rx_bytes)
    if [ -z "$new_rx" ]; then
        echo "Error: Interface $INTERFACE disappeared!"
        exit 1
    fi
    
    bytes_delta=$((new_rx - old_rx))
    speed_kb=$(echo "scale=2; $bytes_delta / 1024" | bc)
    
    if (( $(echo "$speed_kb >= 1024" | bc -l) )); then
        speed=$(echo "scale=2; $speed_kb / 1024" | bc)
        unit="MB/s"
    else
        speed=$speed_kb
        unit="KB/s"
    fi
    
    printf "%5.1f %-4s\n" "$speed" "$unit" > "$WIFI"
    sleep 2
done
) &

# wifi speed
(
while true; do
    if [ "$(cat /sys/class/net/w*/operstate 2>/dev/null)" = 'up' ] ; then
    	wifi=""
    elif [ "$(cat /sys/class/net/w*/operstate 2>/dev/null)" = 'down' ] ; then
    	[ "$(cat /sys/class/net/w*/flags 2>/dev/null)" = '0x1003' ] && wifi="" || wifi=""
    fi
    
    [ -n "$(cat /sys/class/net/tun*/operstate 2>/dev/null)" ] && wifi=""
    
    [ "$(cat /sys/class/net/e*/operstate 2>/dev/null)" = 'up' ] && wifi=""
    
    echo $wifi > "$INTERNET"
    sleep 2
done
) &

# cpu
(
while true; do
    read -r -a prev < /proc/stat
    prev_total=$((prev[1] + prev[2] + prev[3] + prev[4] + prev[5] + prev[6] + prev[7] + prev[8] + prev[9] + prev[10]))
    prev_idle=$((prev[4] + prev[5]))
    
    sleep 1
    
    read -r -a current < /proc/stat
    current_total=$((current[1] + current[2] + current[3] + current[4] + current[5] + current[6] + current[7] + current[8] + current[9] + current[10]))
    current_idle=$((current[4] + current[5]))
    
    total_diff=$((current_total - prev_total))
    idle_diff=$((current_idle - prev_idle))
    
    cpu_usage=$(bc <<< "scale=4; (($total_diff - $idle_diff)/$total_diff)*100")
    cpu_usage=$(printf "%.1f" "$cpu_usage" | awk '{ 
      printf "%s%.1f%%", ($1 < 10 ? " " : ""), $1 
    }')
    echo " $cpu_usage" > "$CPU"
    sleep 2
done
) &

# memory
(
while true; do
    mem_usage=$(awk '/MemTotal/ { total = $2 }
         /MemAvailable/ { avail = $2 }
         END {
             usage = ((total - avail) / total) * 100;
             printf "%4.1f%%\n", usage
         }' /proc/meminfo)
    echo " $mem_usage"  > "$MEM"
    sleep 2
done
) &

# Volume
(
while true; do
    echo "$(pactl get-sink-volume @DEFAULT_SINK@ | grep -oP '\d+%' | head -n 1)" > /dev/shm/status_bar/tmp.VOLUME
    sleep 60
done
) &

while true; do
    xsetroot -name "$(cat $MEM)  $(cat $INTERNET) $(cat $WIFI)  $(cat $CPU)  $(cat $MIC)  $(cat $MUTE) $(cat $VOLUME)  $(cat $BAT)  $DATE $(date +"%H:%M:%S")"
    sleep 1
done

