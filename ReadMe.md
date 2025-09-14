# ZaiDAQ



### Formerly

[ZaidaScope](https://github.com/ZaidaTek/ZaidaScope/blob/master/scope/ReadMe.md) - Use your PC and Arduino as an Oscilloscope! ([Download](https://github.com/ZaidaTek/ZaiDAQ/releases/tag/zaidascope-v210603))  



## Latest

`zdaq` - A free set of minimalist DAQ-tools and -utilities ([Download](https://github.com/ZaidaTek/ZaiDAQ/releases/tag/zdaq-v250831))

Note: Presently, only the ATmega328p controllers are supported, and currently only 10-bit analog input on all eight channels at a maximum combined sampling rate of 100kS/s is supported. See the existing [ZaidaScope](https://github.com/ZaidaTek/ZaidaScope/blob/master/scope/ReadMe.md) documentation for more in-depth details and limitations, as zdaq merely strips away the GUI and gives the user direct access to the measurement data.



### Development

`2025-09-14` -- The updated code in its entirety should have been pushed with the commit that includes this edit. You can build (non-release/internal-debug-only) ZaidaScope-v250906 with it like in the screenshot below, though this presently offers no extra features beyond being compatible with the latest firmware again.

To do so with Code::Blocks, place both ZDK and ZaiDAQ folders in the same folder and then build the `*.cbp` projects in *order* of ZTM, ZTL, ZTK, ZTX (needs to be linked against zlib, libpng and libjpeg), ZDX, ZUI and finally ZS/ZaidaScope.

The Code::Blocks project files I used in 2021 to compile zlib, libpng and libjpeg from source can be found [here](https://github.com/ZaidaTek/zmisc). These were built inside/the `ZTX.cbp` project file expects them to be in e.g. `C:\__lib\zlib\zlib.(h|dll)`.  
Note: Code::Blocks project files are XML-files (and can be opened in text editors).

`2025-09-14` -- I have adapted ZDK in so far, as that it has partial (ZTM, ZTL, ZTX, ZDX) Linux support and now also supports its entire original Windows build as well again (at least according to how it worked with Windows 7/10). This has allowed me to rebuild ZaidaScope with all the current updates, refactors and code, compiler and linker optimizations on its native platform. This makes *that* (not yet publicly available) build compatible again with the current embedded firmware, and no longer with the older one (to which ZaidaScope-v210603 and zdaq-v250831 are compatible to). On Windows, the original hardware and the up-to-date software appear to function faultlessly again at 100 kHz. This possibly means that either some code in the Linux chain with regards to serial USB communication is not emulating Windows in its entirety, or that maybe it is a Windows or driver issue (as one requires a third party driver on Windows; these often add vendor-specific code, that may be missing on Linux), or maybe something else altogether.

Nevertheless, this is promising.

I'm aiming for a small update to the Windows build this year and am **open to bug reports or feature requests**.  
Addendum, to clarfiy: Small feature updates, i.e. e.g. what's inside the *.csv.meta file can be put in the actual filename of the recording (it was to split header/payload, to simplify parsing), which I have planned, things like that, tiny quality-of-life improvements, that can go a long way.

![image doc/blog/2025-09-14_enter-das-ding.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-09-14_enter-das-ding.png "...wait, why are the paths full of escape characters? :P")




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



