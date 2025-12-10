# Phantom-Persistence

An educational Linux malware sample demonstrating Advanced Persistent Threat (APT) techniques used by nation-state actors. This project includes both the malware implant and complete reverse engineering analysis.

## Overview

Phantom-Persistence is a Linux persistence implant that demonstrates real-world malware techniques including:
- Automatic persistence across reboots
- Command and Control (C2) beacon communication
- System information exfiltration
- Anti-debugging and anti-analysis measures
- String obfuscation to evade detection

This project was created for educational purposes to understand how sophisticated malware operates and how to detect and analyze it.

## MITRE ATT&CK Techniques

This malware demonstrates the following techniques:

- **T1053.003** - Scheduled Task/Job: Cron (Persistence)
- **T1071.001** - Application Layer Protocol: Web Protocols (C2 Communication)
- **T1082** - System Information Discovery
- **T1041** - Exfiltration Over C2 Channel
- **T1027** - Obfuscated Files or Information
- **T1497** - Virtualization/Sandbox Evasion
- **T1059.004** - Command and Scripting Interpreter: Unix Shell

## Project Components

### Malware Implant (`src/implant.c`)
The main persistence implant written in C. Includes:
- Cron-based persistence mechanism
- HTTP-based C2 beacon
- System reconnaissance capabilities
- Basic anti-analysis features

### C2 Server (`c2-server/c2_server.py`)
Python-based command and control server that:
- Receives beacon check-ins from infected systems
- Logs victim information
- Sends commands to implants

### Analysis Documentation (`analysis/`)
Complete reverse engineering analysis including:
- Ghidra walkthrough
- Static analysis findings
- Dynamic analysis results
- Indicators of Compromise (IOCs)
- YARA rules for detection

## Quick Start

### Building the Implant
```bash
cd src/
make
```

### Running the C2 Server
```bash
cd c2-server/
python3 c2_server.py
```

### Testing (Safe Environment Only)

See `docs/testing-guide.md` for complete testing instructions in an isolated VM environment.

## Educational Purpose & Ethics

**WARNING: This is malware. Use only in isolated lab environments.**

This project is created solely for:
- Learning malware analysis techniques
- Understanding APT tactics and procedures
- Practicing reverse engineering skills
- Improving defensive security capabilities

**Never deploy this on systems you do not own or have explicit permission to test.**

## Skills Demonstrated

- C programming and Linux system programming
- Understanding of persistence mechanisms
- Network protocol implementation
- Reverse engineering and binary analysis
- Threat intelligence and ATT&CK framework mapping
- Security documentation and reporting

## Analysis Results

For complete analysis, see `analysis/ghidra-analysis.md` which includes:
- Decompiled code examination
- String extraction and decoding
- Control flow analysis
- IOC identification
- Detection recommendations

## Detection & Defense

### IOCs (Indicators of Compromise)
See `analysis/iocs.txt` for complete list including:
- File hashes
- Network indicators
- Persistence mechanisms
- Behavioral indicators

### Detection Methods
- Monitor cron job modifications for unusual entries
- Network traffic analysis for C2 beacons
- File integrity monitoring on system directories
- Process behavior analysis

## Author

Mohammad - U.S. Army Veteran transitioning to Federal Cybersecurity
- Learning malware analysis and reverse engineering
- Building portfolio!
- MS in Cybersecurity Technology (in progress)

## Disclaimer

This software is provided for educational and research purposes only. The author is not responsible for any misuse or damage caused by this program. Use only in controlled laboratory environments with proper authorization.

---

**This project demonstrates understanding of both offensive techniques (how malware works) and defensive techniques (how to analyze and detect it).**