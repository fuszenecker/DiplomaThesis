#!/usr/bin/python

import sys

# ----------------------------------------------------------------------------
# Crypto4o functions start here
# ----------------------------------------------------------------------------

class glfsr:
    def __init__(self, polynom, initial_value):
        self.polynom = polynom | 1
        self.data = initial_value
        tmp = polynom

        self.mask = 1

        while tmp != 0:
            if tmp & self.mask != 0:
                tmp = tmp ^ self.mask;

            if tmp == 0:
                break

            self.mask = self.mask << 1

    def next_state(self):
        self.data = self.data << 1

        retval = 0

        if self.data & self.mask != 0:
            retval = 1
            self.data = self.data ^ self.polynom

        return retval

class sprng:
    def __init__(self, polynom_d, init_value_d, polynom_c, init_value_c):
        self.glfsr_d = glfsr(polynom_d, init_value_d)
        self.glfsr_c = glfsr(polynom_c, init_value_c)

    def next_byte(self):
        byte = 0
        bitpos = 7

        while 1 == 1:
            bit_d = self.glfsr_d.next_state()
            bit_c = self.glfsr_c.next_state()

            if bit_c != 0:
                bit_r = bit_d
                byte = byte | (bit_r << bitpos)

                bitpos = bitpos - 1

                if bitpos < 0:
                    break

        return byte

# ----------------------------------------------------------------------------
# Crypto4o functions end here
# ----------------------------------------------------------------------------

def main():
    prng = sprng(int(sys.argv[3], 16), int(sys.argv[4], 16), 
        int(sys.argv[5], 16), int(sys.argv[6], 16))

    print "GLFSR D0: using polynom 0x%X, initial value: 0x%X." % (int(sys.argv[3], 16), int(sys.argv[4], 16))
    print "GLFSR C0: using polynom 0x%X, initial value: 0x%X." % (int(sys.argv[5], 16), int(sys.argv[6], 16))

    f = open(sys.argv[1], "rb")
    g = open(sys.argv[2], "wb")

    while 1 == 1:
        input_ch = f.read(1)

        if input_ch == "":
            break

        random_ch = prng.next_byte() & 0xff
        g.write(chr(ord(input_ch) ^ random_ch))

    f.close()
    g.close()

main()
