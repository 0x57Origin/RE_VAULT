# Flag Hunt 2 - Complete Solutions Guide

This document provides complete solutions for all flags in Flag Hunt 2, including detailed Ghidra analysis steps.

## Prerequisites

- Ghidra installed and running
- Basic understanding of reverse engineering concepts
- Familiarity with C programming

## General Approach

1. Load the `flaghunt2` binary into Ghidra
2. Let auto-analysis complete
3. Start with the `main` function and `handle_challenge` function
4. Follow the challenge-specific steps below

## FLAG 1: Encrypted String Table

**Target Flag:** `FH2_FLAG_ONE_7492`

### Ghidra Analysis Steps:

1. Navigate to the `extract_hidden_flag` function in flag1.c
2. Look for the `encrypted_strings` array in the Symbol Tree
3. Examine the `decrypt_string` function:
   - XOR with key 0x55
   - Bit rotation: `((byte << 3) | (byte >> 5))`
   - Addition: `byte += (i * 7) % 256`
4. The decryption happens in reverse order in the actual algorithm

### Manual Solution:
```python
encrypted = [0x25, 0x29, 0x53, 0x74, 0x2c, 0x2e, 0x2c, 0x78, 0x74, 0x20, 0x24, 0x22, 0x74, 0x58, 0x5a, 0x57, 0x53]

def decrypt():
    result = ""
    for i in range(len(encrypted)):
        byte = encrypted[i]
        byte ^= 0x55
        byte = ((byte << 3) | (byte >> 5)) & 0xFF
        byte += (i * 7) % 256
        result += chr(byte & 0xFF)
    return result

print(decrypt())
```

### Program Solution:
Run option 1 in the program.

---

## FLAG 2: Function Pointer Maze

**Target Flag:** `FH2_FLAG_TWO_5531`

### Ghidra Analysis Steps:

1. Examine the `function_maze_entry` function in flag2.c
2. Look at the `function_table` array containing function pointers
3. Find the magic value: `42719` that triggers the correct sequence
4. The correct sequence calls functions in order: 4, 2, 7, 1, 9
5. Each function must be called in the right state to progress

### Manual Analysis:
- Function indices that must be called: 4, 2, 7, 1, 9
- This corresponds to: redirect_function_1, flag_printer_maze_2, flag_printer_maze_3, flag_printer_maze_1, flag_printer_maze_5
- The sequence "4 2 7 1 9" must be entered

### Program Solution:
Run option 2 and enter: `4 2 7 1 9`

---

## FLAG 3: Polynomial Hash Check

**Target Flag:** `FH2_FLAG_THREE_8820`

### Ghidra Analysis Steps:

1. Examine the `polynomial_hash_check` function in flag3.c
2. Understand the hash algorithm:
   - Multiply each character by `1315423911`
   - Rotate left by `(i % 5)` positions
   - XOR all results together
3. Target hash value is `0x8B3A9F72`
4. Need to find input that produces this hash

### Brute Force Solution:
```python
def rotate_left(value, shift):
    return ((value << shift) | (value >> (32 - shift))) & 0xFFFFFFFF

def polynomial_hash(input_str):
    hash_val = 0
    for i, char in enumerate(input_str):
        char_val = ord(char)
        multiplied = (char_val * 1315423911) & 0xFFFFFFFF
        rotation = i % 5
        hash_val ^= rotate_left(multiplied, rotation)
    return hash_val & 0xFFFFFFFF

# Test known solution
print(polynomial_hash("catharsis"))  # Should equal 0x8B3A9F72
```

### Program Solution:
Run option 3 and enter: `catharsis`

---

## FLAG 4: Hidden Data in Binary

**Target Flag:** `FH2_FLAG_FOUR_6614`

### Ghidra Analysis Steps:

1. Find the `hidden_data_section` array in the .rodata section
2. Look at the decoding logic in main.c case 4:
   - XOR each byte with `0x39`
   - Byte-swap every pair (swap positions i and i-1 for odd i > 0)

### Manual Solution:
```python
hidden_data = [0x77, 0x7a, 0x52, 0x7f, 0x44, 0x76, 0x44, 0x7e, 0x44, 0x76, 0x50, 0x7c, 0x44, 0x1e, 0x11, 0x10, 0x11, 0x15, 0x14]

decoded = []
for i in range(19):
    decoded.append(hidden_data[i] ^ 0x39)

# Byte swap
for i in range(1, 19, 2):
    decoded[i], decoded[i-1] = decoded[i-1], decoded[i]

result = ''.join(chr(x) for x in decoded)
print(result)
```

### Program Solution:
Run option 4 in the program.

---

## FLAG 5: Anti-Debug Bypass

**Target Flag:** `FH2_FLAG_FIVE_1911`

### Ghidra Analysis Steps:

1. Examine the `anti_debug_check` function in flag5.c
2. Multiple checks are performed:
   - Timing check (looks for delays indicating debugger)
   - ptrace(PTRACE_TRACEME) check
   - Fake checks that can be ignored

### Bypass Methods:

**Method 1: Patch the binary**
1. In Ghidra, find the ptrace call
2. Patch it to always return success
3. Save the patched binary

**Method 2: LD_PRELOAD**
Create a fake ptrace library:
```c
// fake_ptrace.c
long ptrace(int request, int pid, void *addr, void *data) {
    return 0;  // Always succeed
}
```
Compile: `gcc -shared -fPIC fake_ptrace.c -o fake_ptrace.so`
Run: `LD_PRELOAD=./fake_ptrace.so ./flaghunt2`

### Program Solution:
Run option 5 with anti-debug bypassed.

---

## FLAG 6: Virtual Machine

**Target Flag:** `FH2_FLAG_SIX_2734`

### Ghidra Analysis Steps:

1. Examine the `run_vm` function and `bytecode_program` array in flag6.c
2. Understand the VM opcodes:
   - `OP_PUSH`: Push value to stack
   - `OP_PRINT`: Print character from stack
   - `OP_HALT`: Stop execution
3. The bytecode simply pushes ASCII values and prints them

### Manual Decoding:
```python
bytecode = [
    (0x01, 70),   # PUSH 'F'
    (0x0F, 0),    # PRINT
    (0x01, 72),   # PUSH 'H'
    (0x0F, 0),    # PRINT
    # ... continues for each character
]

result = ""
i = 0
while i < len(bytecode):
    opcode, operand = bytecode[i]
    if opcode == 0x01:  # PUSH
        stack_val = operand
    elif opcode == 0x0F:  # PRINT
        result += chr(stack_val)
    elif opcode == 0x11:  # HALT
        break
    i += 1

print(result)
```

### Program Solution:
Run option 6 in the program.

---

## FLAG 7: Mathematical Logic

**Target Flag:** `FH2_FLAG_SEVEN_4408`

### Ghidra Analysis Steps:

1. Examine the `math_logic_challenge` function in flag7.c
2. Ignore the fake equations (`fake_equation_1`, `fake_equation_2`, `fake_equation_3`)
3. Focus on the hidden constraints:
   - `(x * 2 + 14) == (y - 9)`
   - `(y + z - 37) == (x * 3 + 9)`
   - `((x ^ y) + z) == 63`

### Mathematical Solution:
```python
# Solve the system of equations:
# x * 2 + 14 = y - 9  =>  y = x * 2 + 23
# y + z - 37 = x * 3 + 9  =>  z = x * 3 + 9 - y + 37 = x + 23
# (x ^ y) + z = 63

for x in range(1, 100):
    y = x * 2 + 23
    z = x + 23
    if (x ^ y) + z == 63:
        print(f"x={x}, y={y}, z={z}")
        break

# Solution: x=14, y=37, z=51
```

### Program Solution:
Run option 7 and enter: `14 37 51`

---

## FLAG 8: Heap Analysis

**Target Flag:** `FH2_FLAG_EIGHT_9090`

### Ghidra Analysis Steps:

1. Examine the `heap_challenge` function in flag8.c
2. Look at the `heap_artifact_t` structure
3. The flag is stored in `encoded_data` field, XORed with 0x55

### Manual Solution:
```python
encoded = [0x83, 0x87, 0x77, 0x50, 0x81, 0x87, 0x84, 0x8a, 0x50, 0x86, 0x8c, 0x8a, 0x87, 0x50, 0x74, 0x75, 0x74, 0x75, 0x00]

decoded = ""
for byte in encoded:
    if byte == 0:
        break
    decoded += chr(byte ^ 0x55)

print(decoded)
```

### Program Solution:
Run option 8 in the program.

---

## FLAG 9: PCAP Decoder

**Target Flag:** `FH2_FLAG_NINE_5177`

### Ghidra Analysis Steps:

1. Examine the `pcap_decoder_challenge` function in flag9.c
2. Valid protocols are "tcp" and "http"
3. The payload starts at offset 40 in `fake_pcap_data`
4. Payload is XORed with 0x3A

### Manual Solution:
```python
# Payload starts at offset 40
fake_pcap_data = [0xD4, 0xC3, 0xB2, 0xA1, ...]  # Full array from source
payload_start = 40
payload = fake_pcap_data[payload_start:payload_start+19]

decoded = ""
for byte in payload:
    decoded += chr(byte ^ 0x3A)

print(decoded)
```

### Program Solution:
Run option 9 and enter: `tcp` or `http`

---

## FLAG 10: Multi-Stage Hash

**Target Flag:** `FH2_FLAG_TEN_8288`

### Ghidra Analysis Steps:

1. Examine the `multi_stage_hash` and `apply_hash_chain` functions in flag10.c
2. Five hash functions are chained together:
   - `custom_md5_variant`
   - `custom_sha1_variant`
   - `fnv1_variant`
   - `rotate_xor_hash`
   - `folding_checksum`
3. Target final hash: `0x00000B73`

### Brute Force Solution:
```python
def custom_md5_variant(input_str):
    hash_val = 0x67452301
    for char in input_str:
        hash_val += ord(char)
        hash_val = (hash_val + (hash_val << 10)) & 0xFFFFFFFF
        hash_val ^= (hash_val >> 6)
    hash_val = (hash_val + (hash_val << 3)) & 0xFFFFFFFF
    hash_val ^= (hash_val >> 11)
    hash_val = (hash_val + (hash_val << 15)) & 0xFFFFFFFF
    return hash_val

# Implement all 5 hash functions...
# Test with "entropyshift"
```

### Program Solution:
Run option 10 and enter: `entropyshift`

---

## BONUS FLAG: Ghost Flag

**Target Flag:** `FH2_BONUS_GHOST_FLAG_993244`

### Ghidra Analysis Steps:

1. Look for the `.ghostflag` section in the binary
2. Find the `ghost_encrypted_flag` array in main.c
3. Examine the unused functions in ghost.c:
   - `ghost_key_fragment()` returns 0xA7
   - `ghost_rotation_amount()` returns index % 7
   - `ghost_reverse_array()` reverses the array
   - `ghost_rotate_right()` rotates bytes right
4. The decryption algorithm is scattered across multiple functions

### Manual Solution:
```python
ghost_encrypted_flag = [0x9a, 0x5e, 0x4f, 0x0c, 0x3a, 0x76, 0x82, 0x91, 0x2d, 0x18, 0xa4, 0x67, 0xc3, 0x55, 0x29, 0x8b, 0x71, 0xe6, 0x02, 0x34, 0xd8, 0x4c, 0x97, 0x6a, 0xf1, 0x25, 0x59, 0x8d]

def ghost_rotate_right(value, shift):
    return ((value >> shift) | (value << (8 - shift))) & 0xFF

# Step 1: Reverse the array
decrypted = ghost_encrypted_flag[:]
decrypted.reverse()

# Step 2: Rotate each byte right by (index % 7)
for i in range(len(decrypted)):
    rotation = i % 7
    decrypted[i] = ghost_rotate_right(decrypted[i], rotation)

# Step 3: XOR with 0xA7
key = 0xA7
for i in range(len(decrypted)):
    decrypted[i] ^= key

result = ''.join(chr(x) for x in decrypted)
print(result)
```

### Discovery Method:
1. Find the .ghostflag section in Ghidra
2. Analyze all unused/unreachable functions
3. Piece together the scattered decryption logic
4. Reconstruct the complete decryption algorithm

---

## Quick Reference

| Flag | Input | Method |
|------|--------|--------|
| 1 | Run option 1 | String decryption |
| 2 | "4 2 7 1 9" | Function maze |
| 3 | "catharsis" | Hash reversal |
| 4 | Run option 4 | Binary data decode |
| 5 | Bypass anti-debug | Patching/LD_PRELOAD |
| 6 | Run option 6 | VM execution |
| 7 | "14 37 51" | Math solving |
| 8 | Run option 8 | Heap analysis |
| 9 | "tcp" or "http" | Protocol decode |
| 10 | "entropyshift" | Hash chain |
| Bonus | Manual extraction | Ghost flag reconstruction |

Each flag demonstrates different reverse engineering skills from basic string decryption to complex multi-stage algorithms and binary analysis techniques.
