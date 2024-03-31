#!/usr/bin/env bash

CLIENT_COUNT=20
WAIT_SEC=5

echo "Run tests:"

PID=$(pidof ttm)
if [ -z "$PID" ]; then
    echo "[FAILED] Server 'ttm' must be run"
    exit 1
else
    echo "[PASSED] Server running"
fi

for iter in $(seq 1 $CLIENT_COUNT); do
    pos1=$((1000+iter*10))
    echo ".. prepare client $iter"
    cat /var/log/syslog -n | head -n $pos1 | tail -n 1000 > "telnet$iter.txt"
done

for iter in $(seq 1 $CLIENT_COUNT); do
    echo ".. run client $iter"
    netcat localhost 5000 < "telnet$iter.txt" >/dev/null &
done

echo ".. waiting $WAIT_SEC seconds"
sleep $WAIT_SEC

PID=$(pidof ttm)
if [ -z "$PID" ]; then
    echo "[FAILED] Server 'ttm' was crashed"
    exit 1
else
    echo "[PASSED] Server still running"
fi
