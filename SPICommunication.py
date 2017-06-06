#!/usr/bin/env python

# mini-spi.py
# 2016-03-18
# Public Domain

import time

import pigpio # http://abyz.co.uk/rpi/pigpio/python.html

pi = pigpio.pi()

if not pi.connected:
   exit(0)

h = pi.spi_open(0, 40000)

stop = time.time() + 120.0

n = 0

while time.time() < stop:

   n += 1
   pi.spi_xfer(h, "This is message number {}\n".format(n))
   time.sleep(1)

pi.spi_close(h)

pi.stop()
