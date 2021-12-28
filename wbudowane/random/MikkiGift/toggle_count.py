# import ...

# //frequency in Hz
# static const uint16_t notes[][9] PROGMEM = {        // C D E F G A B : 0-8
#   {27, 55, 110, 220, 440, 880, 1760, 3520, 7040},   //A
#   {30, 61, 123, 246, 493, 987, 1975, 3951, 7902},   //B
#   {16, 32, 65,  130, 261, 523, 1046, 2093, 4186},   //C
#   {18, 36, 73,  146, 293, 587, 1174, 2349, 4698},   //D
#   {20, 41, 82,  164, 329, 659, 1318, 2637, 5274},   //E
#   {22, 43, 87,  174, 349, 698, 1396, 2793, 5587},   //F
#   {24, 49, 98,  196, 392, 783, 1567, 3135, 6271}    //G
# };// ['A'-note]['0'-octave]

FCPU = 4e6
second = 1e6
TEMPO = 160
BEAT = (60*second)/TEMPO

quarter = BEAT/8


# 00 C4 F4 G4 A4 B4 C5 D5
freq = [500, 261, 349, 392, 440, 493, 523, 587]
# freq = [900, 523, 698, 783, 880, 987, 1046, 1174]

toggle = []

for f in freq:
    cycle_len = (second/f)
    toggle.append(int((quarter/cycle_len)*2))

print("toggle\t\t",toggle)

OCRA = []
PRESCALAR = []

for f in freq:
    tmp = FCPU/f
    pre = 1
    if tmp > 256:
        tmp/=8
        pre = 2
        if tmp > 256:
            tmp/=8
            pre = 3
            if tmp > 256:
                tmp/=4
                pre = 4
                if tmp > 256:
                    tmp/=4
                    pre = 5
    OCRA.append(int(tmp-1))
    PRESCALAR.append(pre)
                
print("OCRA\t\t",OCRA)
print("PRESCALAR\t",PRESCALAR)

# FCPU/(64 * (ocra+1)) = 

# 00 C4 F4 G4 A4 B4 C5 D5
song = "C5eC5eC5eB4eA4eG4eF4eF4eF4eC4eF4eA4eC5eC5eC5eB4eA4eG4eF4eF4eF4eC4eF4eA4eC5eD5eC5eB4eA4eB4eC5eD5eC5eB4eA4eB4eC5eC5eD5eD5eC5eC5eB4eB4eF4eF4eC5eC5eF4eF4eF4eF4eF4eF4e"#"F4eF4eF4eC4eF4eG4eA4eA4eA4eG4eA4eB4eC5eD5eC5eC5eB4eB4eA4eA4eG4eG4e00e00eF4eF4eF4eC4eF4eG4eA4eA4eA4eG4eA4eB4eC5eD5eC5eC5eB4eB4eA4eA4eG4eG4e00e00eC5eC5eC5eB4eA4eG4eF4eF4eF4eC4eF4eA4eC5eC5eC5eB4eA4eG4eF4eF4eF4eC4eF4eA4eC5eD5eC5eB4eA4eB4eC5eD5eC5eB4eA4eB4eC5eC5eD5eD5eC5eC5eB4eB4eF4eF4eC5eC5eF4eF4eF4eF4eF4eF4e"

# C5eC5eC5eB4eA4eG4eF4eF4eF4eC4eF4eA4eC5eC5eC5eB4eA4eG4eF4eF4eF4eC4eF4eA4eC5eD5eC5eB4eA4eB4eC5eD5eC5eB4eA4eB4eC5eC5eD5eD5eC5eC5eB4eB4eA4eA4eG4eG4eF4eF4eF4eF4eF4eF4e

parsed = []

i = 0
while i < len(song):
    note = song[i]
    if note == "0":
        parsed.append(0)
    if note == "F":
        parsed.append(2)
    if note == "G":
        parsed.append(3)
    if note == "A":
        parsed.append(4)
    if note == "B":
        parsed.append(5)
    if note == "D":
        parsed.append(7)
    if note == "C":
        octave = song[i+1]
        if octave == "4":
            parsed.append(1)
        if octave == "5":
            parsed.append(6)
    i+=3

print("parsed",parsed)