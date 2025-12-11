# Phantom-Persistence

Linux persistence malware for educational analysis.

## What it does

**Implant (`src/implant.c`)**:
- Copies itself to `/tmp/.system_daemon`
- Creates cron job to run every 5 minutes
- Daemonizes (runs in background)
- Sends system info to C2 server every 10 seconds
- Basic anti-debugging with ptrace

**C2 Server (`c2-server/c2_server.py`)**:
- Listens on port 8080
- Receives beacon data via HTTP GET
- Logs victim info to `c2_beacons.log`
- Shows real-time connections

## Build and Run

**Install dependencies:**
```bash
sudo apt-get install libcurl4-openssl-dev build-essential
```

**Build implant:**
```bash
cd src/
make
```

**Start C2 server:**
```bash
cd c2-server/
python3 c2_server.py
```

**Run implant:**
```bash
cd src/
./phantom-implant
```

## Data collected

- Hostname
- Username  
- OS info (`uname -a`)
- Source IP
- Timestamps

## Files created

- `/tmp/.system_daemon` - Copied implant
- Cron entry: `*/5 * * * * /tmp/.system_daemon >/dev/null 2>&1`
- `c2_beacons.log` - Beacon log file

## Clean up

```bash
# Remove cron job
crontab -e  # Delete the phantom line manually

# Remove files
rm -f /tmp/.system_daemon
rm -f c2_beacons.log
```

**Warning: Use only in isolated lab environments.**
