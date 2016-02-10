# PoundNineNewsWire
Source code for #9 Personal News Wire

The Pound Nine Wire is a wifi enabled (TICC3100), battery operated, hardware device with 3.2" touch screen display. The Pound Nine Wire continually streams live headlines from a database of content. Just glance at the device to stay informed while you stay focused on the tasks at hand.

A full description of the product can be view on the project kickstarter page: https://www.kickstarter.com/projects/1547604553/stream-live-news-updates-on-your-personal-wire

The product was not funded and is not be pursued at this time. A lot of work went into the hardware and firmware design which is posted here. Feel free to leverage the source code in your projects. The project uses a Atmel ATxmega256A3U and TICC3100. Schema and gerber files are available in PCB directory. Firmware to connect device to internet and continually retreive content is posted in Firmware folder. ImageLoader is firmware to load image hex files onto SRAM chip. Not elegant, but functional.

There is a companion android app that operates as a news wire, publishing the same content pushed to the standalone hardware. In the mobile app is a customized version of the CC3100 smart config app. 

The 3d model files are available so you can print your own case.
