# ZaiDAQ



### Formerly

[ZaidaScope](https://github.com/ZaidaTek/ZaidaScope/blob/master/scope/ReadMe.md) - Use your PC and Arduino as an Oscilloscope! ([Download](https://github.com/ZaidaTek/ZaiDAQ/releases/tag/zaidascope-v210603))  



## Latest

`zdaq` - A free set of minimalist DAQ-tools and -utilities ([Download](https://github.com/ZaidaTek/ZaiDAQ/releases/tag/zdaq-v250831))

Note: Presently, only the ATmega328p controllers are supported, and currently only 10-bit analog input on all eight channels at a maximum combined sampling rate of 100kS/s is supported. See the existing [ZaidaScope](https://github.com/ZaidaTek/ZaidaScope/blob/master/scope/ReadMe.md) documentation for more in-depth details and limitations, as zdaq merely strips away the GUI and gives the user direct access to the measurement data.



### Development

`2025-09-12` -- I happen to have run *ZaidaScope*  for a somewhat extended period of time in Wine on Linux without issue, bar the as-of-yet unresolved issue of increased packet losses...

![image doc/blog/2025-09-12_short-soak.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-09-12_short-soak.png "Having to reupload a sketch to set the rate is admiteddly a bit of a drag...")



`2025-08-31` -- A full expansion of this ReadMe to include installation instructions remains at large  
`2025-08-31` -- A Windows [preliminary-experimental ZaiDAQ release](https://github.com/ZaidaTek/ZaiDAQ/releases) has also been added to the repository

![image doc/blog/2025-08-31_HelloMyOldFriend.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-08-31_HelloMyOldFriend.png "Having avoided Vista, I was late to 7, convinced it was just as messy; I made up for that, I think.")

`2025-08-31` -- A full expansion of this ReadMe to include installation instructions is to follow  
`2025-08-31` -- A (presently Linux-only) [preliminary-experimental ZaiDAQ release](https://github.com/ZaidaTek/ZaiDAQ/releases) has been added to the repository

![image doc/blog/2025-08-31_das-kraken.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-08-31_das-kraken.png "2004 and 2006, the first live giant squid footages in the world; only recently, the colossal squid now too!")



`2025-08-30` -- Polishing for first experimental release...

![image doc/blog/2025-08-30_version.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-08-30_version.png "...'YYDDD 'is even shorter! :P")



`2025-08-29` -- Experimental teaser

![image doc/blog/2025-08-29_experimental.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-08-29_experimental.png "There are a lot of implications pertaining non-ANSI data output...")



`2025-08-17` -- Not sure if EMI is causing packet loss, or if changing signals' packets interpreted within Wine somehow result in packets interpreted as loss

![image doc/blog/2025-08-17_packet-loss.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-08-17_packet-loss.png "It's still that one PCB you see in the pic in the Electrical part of the ReadMe.")



`2025-08-15` -- ZaidaScope appears to be functional in [Linux with Wine](scope/ReadMe.md#linux)



`2025-08-14` -- Renamed to 'ZaiDAQ'

![image doc/blog/2025-08-15_floating.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-08-15_floating.png "Coming soon: 'zdaq' a native Linux console version! Also, that first ADC-sample is a 'wraparound discardable.'")



