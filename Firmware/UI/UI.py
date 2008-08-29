#!/usr/bin/python

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

import Tkinter
from Tkinter import *

import serial
from serial import *

import threading
from threading import *

import time
from time import *

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

def log(line):
    global text

    text.insert(END, line)
    text.see(END)

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

def print_tty():
    global tty

    if tty.inWaiting() > 1:
        line = tty.readline()
        log(line)

    timer = Timer(0.1, print_tty)
    timer.start()

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

def set_filter_func():
    global tty
    global set_filter_number, set_filter_mask, set_filter_id

    filter = int(set_filter_number.get())
    log("f%0X%s\n" % (filter, set_filter_mask.get()))
    tty.write("f%0X%s\n" % (filter, set_filter_mask.get()))
    sleep(1)
    log("i%0X%s\n" % (filter, set_filter_id.get()))
    tty.write("i%0X%s\n" % (filter, set_filter_id.get()))

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

def set_baudrate_func():
    global tty
    global set_baudrate_number

    baudrate = int(set_baudrate_number.get())
    log("b %08X\n" % (baudrate))
    tty.write("b %08X\n" % (baudrate))

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

def send_func():
    global tty
    global send_length, send_id, send_data

    len = int(send_length.get())
    log("s%01X%s%s\n" % (len, send_id.get(), send_data.get()))
    tty.write("s%01X%s%s\n" % (len, send_id.get(), send_data.get()))

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

def init_func():
    global tty

    log("v\n")
    tty.write("v\n")

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

def main():
    global root, text, timer
    global set_baudrate_number
    global set_filter_number, set_filter_mask, set_filter_id
    global send_length, send_id, send_data

    root = Tkinter.Tk()
    root.configure({"width": 500, "height": 400})
    root.title("Serial-CAN converter's user interface")

    text = Text()
    text.grid(row = 0, column = 0, columnspan = 7)
    log("*** INITIALIZING SERIAL-CAN CONVERTER ***\n")

    # Set baud rate
    set_baudrate_button = Button(root, text="Set baud rate", width = 12, command=set_baudrate_func)
    set_baudrate_button.grid(row = 1, column = 0, columnspan = 1, sticky = W)

    Label(root, text="Baud rate:").grid(row = 1, column = 1, sticky = E)
    set_baudrate_number = Entry(root, width = 6)
    set_baudrate_number.grid(row = 1, column = 2, columnspan = 1, sticky = W)

    Label(root, text="bit/s").grid(row = 1, column = 3, sticky = W)
 
    # Set filter and CAN ID
    set_filter_button = Button(root, text="Set filter", width = 12, command=set_filter_func)
    set_filter_button.grid(row = 2, column = 0, columnspan = 1, sticky = W)

    Label(root, text="Filter ID (dec):").grid(row = 2, column = 1, sticky = E)
    set_filter_number = Entry(root, width = 6)
    set_filter_number.grid(row = 2, column = 2, columnspan = 1, sticky = W)
 
    Label(root, text="Mask: 0x").grid(row = 2, column = 3, sticky = E)
    set_filter_mask = Entry(root, width=10)
    set_filter_mask.grid(row = 2, column = 4, columnspan = 1, sticky = W)
 
    Label(root, text="ID: 0x").grid(row = 2, column = 5, sticky = E)
    set_filter_id = Entry(root, width=10)
    set_filter_id.grid(row = 2, column = 6, columnspan = 1, sticky = W)

    # Send message
    send_button = Button(root, text="Send message", width = 12, command=send_func)
    send_button.grid(row = 3, column = 0, columnspan = 1, sticky = W)

    Label(root, text="Length (dec):").grid(row = 3, column = 1, sticky = E)
    send_length = Entry(root, width = 6)
    send_length.grid(row = 3, column = 2, columnspan = 1, sticky = W)
 
    Label(root, text="ID: 0x").grid(row = 3, column = 3, sticky = E)
    send_id = Entry(root, width=10)
    send_id.grid(row = 3, column = 4, columnspan = 1, sticky = W)
 
    Label(root, text="Data: 0x").grid(row = 3, column = 5, sticky = E)
    send_data = Entry(root, width=17)
    send_data.grid(row = 3, column = 6, columnspan = 1, sticky = W)
 
    timer = Timer(0.1, print_tty)
    timer.start()

    # Initializing device
    init_func()

    # Main loop
    root.mainloop()

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

tty = serial.Serial(
        port = "/dev/ttyS0", 
        parity = serial.PARITY_NONE,
        bytesize = serial.EIGHTBITS,
        stopbits = serial.STOPBITS_ONE, 
        timeout = 3,
        xonxoff = 0,
        rtscts = 0,
        baudrate=115200
    )

# -----------------------------------------------------------------------------
#
# -----------------------------------------------------------------------------

main()

# ---- END OF PYTHON CODE -----------------------------------------------------

