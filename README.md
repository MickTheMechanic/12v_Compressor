# 12v_Compressor

## An open-source Arduino controlled 12v mobile compressor using 3d printed and upcycled parts.

<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
#

[<img src="https://i.ibb.co/3T3CrVk/Screenshot-18.png" alt="vid" border="0">](https://youtube.com/playlist?list=PLsk74wXEuKOpL2YGHRVS3v7fIHzCto8o0)

#

## Overview

The compressor is made up of a used BMW air suspension compressor (manufacturer is WABCO, the same part is also used be Mercedes), some empty 0.95l oxygen tanks (those used for small oxy-acetylene torches), an arduino, and diverse electrical components.

The compressor is controlled via an Arduino Uno, which switches a relay on or off depending on the pressure inside the tanks. The current pressure is displayed on a 2.8" TFT screen. A small lithium battery is included, so that a pressure read out can be achieved without 12v power supply.

- If 12v supply is not present, the power button can be pressed and the display will show a realtime pressure readout for 20 seconds and switch off automatically.
- If 12v supply is present and device is switched off the lithium battery will be recharged
- If 12v supply is present and device is switchen on, the compressor will be powered on and off via the relay automatically depending on realtime pressure. The lithium battery will also be recharged. Realtime pressure will be displayed on screen.

### There are also two status LED's
#### Left LED:
- Red = lithium battery is recharging
- Blue = Lithium battery is fully charged
#### Right LED:
- Green = Tanks are full (if 12v is present relay = off)
- Red = Tanks should be refilled (if 12v is present relay = on)

#
<a href="https://ibb.co/Dz1jNbD"><img src="https://i.ibb.co/n6wKT0s/C740-E988-A85-A-4125-8306-F7122-EC01628.jpg" alt="C740-E988-A85-A-4125-8306-F7122-EC01628" border="0"></a>
#
<a href="https://ibb.co/rd78Jnq"><img src="https://i.ibb.co/WVx81S7/0-F9-DC07-B-1-AC6-40-D4-9-CB7-EE59770-C8-E80.jpg" alt="0-F9-DC07-B-1-AC6-40-D4-9-CB7-EE59770-C8-E80" border="0"></a>
#
<a href="https://ibb.co/4fqdKrv"><img src="https://i.ibb.co/590FGHp/IMG-0614.jpg" alt="IMG-0614" border="0"></a>
#
<a href="https://ibb.co/0Q0CyRs"><img src="https://i.ibb.co/Kmt2xpK/IMG-0619.jpg" alt="IMG-0619" border="0"></a>
#

### Extra info

This project relies on a modified version of the GUISlice library. This has been included in the repo
