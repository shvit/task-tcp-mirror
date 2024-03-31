#!/usr/bin/env bash

echo "Run tests:"

PID=$(pidof ttm)
if [ -z "$PID" ]; then
    echo "[FAILED] Server 'ttm' must be run"
    exit 1
else
    echo "[PASSED] Server running"
fi

