# BattleBeasts

![PCBWAY][PCBWAYLOGO]

Just before Christmas, I had a kind offer from the folks at [PCBWay][PCBWAY] to work on a small maker project. I saw this as a great opportunity to experiment with some new tech and designed a new board around the Microchip SAMD21G microcontroller.

I've always wanted to build a 'Tamagotchi'-type device, so I designed a simple development board the size of a standard playing card with an 8x8 LED matrix and some capacitative-touch buttons. I knocked my design up in KiCAD in a couple of hours:

![KiCAD PCB Layout][KiCAD]

and uploaded the files to the PCB website.

Not long afterwards I got these beauties in the post:

![PCBWay PCBs][BOARDS]

I'd also asked for a solder paste stencil. I was a little bit lazy and just added a comment for it to be made the same size as the board and the guys at PCBWay did it for me which was really great. The boards were really good quality - I poked at them with my multimeter to check continuity and everything was fine.

The black soldermask looks so good on bare PCBs.

Using some old PCBs from another project, I taped the stencil down to my workbench:

![Stencil taped to PCB][STENCIL]

and applied the paste. I think my paste might be a little bit old but it still looks good. I'd really like to get a little USB microscope so I can take proper solder paste glamour shots!

![Solder paste porn][PASTE]

I then very patiently placed most of the components - I don't have any RGB LEDs or coin cell holders so I left those off. This is easily the highest component count I've assembled so far and it went really well. I was paranoid about the LEDs being the right way around so I check every single one out of the reel-strip!

After blasting it with my hot air gun, I think it looks pretty good:

![Finished Board][FINISHED].

I can power the board via the USB connector whilst I'm developing and I can add the RGBs to it later once I've got the matrix working.

My main goal with this board is to build up my Arm MCU development toolchain so getting some code up and running is the next step!

[PCBWAY]: http://www.pcbway.com
[PCBWAYLOGO]: http://www.subdimension.co.uk/files/2020-01-10-New-Year-New-Gear/pcbway.png
[KiCAD]: http://www.subdimension.co.uk/files/2020-01-10-New-Year-New-Gear/KiCAD.png
[BOARDS]: http://www.subdimension.co.uk/files/2020-01-10-New-Year-New-Gear/boards.jpeg
[STENCIL]: http://www.subdimension.co.uk/files/2020-01-10-New-Year-New-Gear/stencil.jpeg
[PASTE]: http://www.subdimension.co.uk/files/2020-01-10-New-Year-New-Gear/paste.jpeg
[FINISHED]: http://www.subdimension.co.uk/files/2020-01-10-New-Year-New-Gear/finished.jpeg
[BB]: https://github.com/MalphasWats/BattleBeasts
