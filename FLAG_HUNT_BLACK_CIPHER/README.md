# Flag Hunt 2 – Black Cipher Edition

A deeply hostile reverse engineering challenge written in C

This project is a more advanced successor to the original Flag Hunt. This time, the program contains ten flags, each hidden behind a different type of reverse engineering challenge. Some involve encrypted data, others include a small custom VM, function pointer obfuscation, heap artifacts, and even a fake PCAP block. The goal is to push beginners into intermediate territory and let advanced users enjoy something more complicated than the typical CTF warmup.

Every flag requires a different method to solve, and the binary is intentionally messy in places. A few functions exist purely to waste your time. Others hide their behavior behind roundabout control flow or strange arithmetic. The decompiler will help, but only partially. The rest requires proper understanding of what the program is actually doing.

The tenth flag is the most difficult one that is still intended to be solvable. It combines a multi-stage hashing chain meant to force you to read the logic carefully and replicate each step by hand or script.

There is also a bonus flag. This flag is not documented, not hinted at anywhere in the program, and is not printed by the binary under any circumstance. It is stored in an unusual way, and the logic for extracting it does not appear in one place. If you want to find it, you will need to understand the binary at a deeper level than what is required for the normal challenges.

Hint for the bonus flag: Some answers are not printed. They are preserved.

## Build instructions

```bash
gcc *.c -o flaghunt2 -O2
```

No other build steps are required.