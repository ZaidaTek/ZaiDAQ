# ZaiDAQ



### Formerly

[ZaidaScope](https://github.com/ZaidaTek/ZaidaScope/blob/master/scope/ReadMe.md) - Use your PC and Arduino as an Oscilloscope! ([Download](https://github.com/ZaidaTek/ZaiDAQ/releases/tag/zaidascope-v210603))  



## Latest

`zdaq` - A free set of minimalist DAQ-tools and -utilities ([Download](https://github.com/ZaidaTek/ZaiDAQ/releases/tag/zdaq-v250831))

Note: Presently, only the ATmega328p controllers are supported, and currently only 10-bit analog input on all eight channels at a maximum combined sampling rate of 100kS/s is supported. See the existing [ZaidaScope](https://github.com/ZaidaTek/ZaidaScope/blob/master/scope/ReadMe.md) documentation for more in-depth details and limitations, as zdaq merely strips away the GUI and gives the user direct access to the measurement data.



### Development

`2025-09-25` -- [zmake](https://github.com/ZaidaTek/zmake) is a Python3 script I'e continuously written and expanded, that's basically... well, a (very basic) 'make-system', I guess. A 'zmake'-project is defined with a JSON `project.zmk` file, that typically includes an array of sources and a target, and possibly some optional compiling or linking flags, usually to optimize. A project is built by calling e.g. `zmake -n project.zmk`, which in this case would perform a dry-run and solely print out the commands on the screen. The volume of code I've written meant I either had to start using Code::Blocks on Linux as well (still might, if I need autocomplete, etc.), 'get acquainted' with one of the known build systems, or, I guess, just churn out some script that'll get the job done. The complexity of such a make system makes Python(3) a preferable choice to e.g. using bash-scripts or similar, furthermore, Python itself is a dependency of many modern Linux systems, and zmake has no extra dependencies apart from the Python Standard-Library, meaning zmake should run OOTB on basically any Linux system (and Windows as well, as Python is/can be platform-independent).

![image doc/blog/2025-09-25_zmake.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-09-25_zmake.png "")



`2025-09-23` -- I've started building `ZTK/ZTXCB`, which is the library component intended to be used for creating and event handling application windows, and drawing to them, in Linux. Once this component is functionally complete, it should be possible to build a native Linux ZaidaScope as well.

![image doc/blog/2025-09-23_xcb.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-09-23_xcb.png "'...hinter dieser Tür, da steht ein Klavier.' :x")



`2025-09-15` -- I've managed to get an x86-64 build done. Some interesting notes: x32/gcc5.1 avg. ca. 960 fps, x32/gcc14.2 avg. ca. 1000 fps, x64/gcc14.2 avg. ca. 1050 fps. Note that these are only interesting as comparative values, as the rendering system of ZaidaScope is actually unoptimized (i.e. it redraws the entire frame, it could only just draw the incremental, but one would need to split one big 2D texture into smaller segments, etc.).

![image doc/blog/2025-09-15_x64.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-09-15_x64.png "")

*Unfortunately*, juggling around two (four) build targets is slowly ...caving in my head? I can't just set `-m32` (or the other way around), at least not with the MingW OOTB from CB on either x63 or x32, it seems. Assuming one uses 64-bit as the default, one should install that, and then copy the 32-bit `-nosetup.zip` into its `Program Files (x86)` Code::Blocks folder, then from the 64-bit CB IDE under Compiler settings, point the *toolchain* to the x86 version. The auto-detect will "lock on" to a valid folder once it's found, regardless if the IDE or the compiler is 32- or 64-bit.

I'll provide and still use .cbp project files myself, but going forward, it seems, I'll need to make [zmake](https://github.com/ZaidaTek/zmake) work on Windows as well, which I was intending to do for completeness, but I'm not unhappy I have a valid reason to do so as well.



`2025-09-15` -- There is a bug, indeed, and it was "intentionally accepted" by me, as it was a quick workaround solution to just get some basic info out for the user. In other words: The NG- and frequency-counters going haywire from the posts of yesterday are absolutely to be expected, once a certain number of packets have been processed. This just happens the fastest if the user actually uses 100kS/s.  
Note: This did and does not affect the *actual operation* of the program, just its info display.

![image doc/blog/2025-09-15_cosmetic-bug-no-impact-operation.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-09-15_cosmetic-bug-no-impact-operation.png "")

Due to the cosmetic, non-functional-nature of this bug, it is no longer a top priority bug-chase, but a to-do-item when expanding/reworking *zdaq-scope/ZaiDAQ-Scope*'s feature set.

ZaidaScope's (and, in the process, ZDK's) code-bases have been made compatible/conformant with Code::Blocks v25.03 and to its corresponding MinGW gcc/g++ compilers/linkers (the project files themselves are unchanged with regards to CB version number).

Regarding 64-bit builds: I have noticed that ZTM and ZTL basically compile without issue (`-flto` is giving me warnings in ZTM now with gcc v14.2 from CB v25.03 vs. gcc v5.1 from CB 17.12), but ZTK will require `#define` guards and discerning 32- and 64-bit builds, as Windows uses ...Hungarian Notation for its typedefs, and so, the data types themselves are hardcoded into the variable typedefs, meaning that e.g. what is a "LPSTR" in 32-bit Windows becomes an "LPPTR" in 64-bit Windows (or whatever they're actually called), meaning you can't just switch without having to accomodate the switch in the code.



`2025-09-14` -- There must be a bug somewhere, I had a reference soak going on, that seemed to mirror what I believed to have seen and expect to see up to about 20,000s runtime:

![image doc/blog/2025-09-14_soak-reference.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-09-14_soak-reference.png "")

but then this happened sometime between t:21142s and t:21768s, less than ten minutes after uploading that:

![image doc/blog/2025-09-14_some-overflow-or-bit-toggle-probably.png](https://github.com/ZaidaTek/ZaidaScope/blob/master/doc/blog/2025-09-14_some-overflow-or-bit-toggle-probably.png "")

I'm not sure what happened there, but there are other sketchy issues with the NG-packet-fail-counter, I suspect there must be a bug in there, somewhere. But apart from the frequency, RX and NG counters going haywire, the program and the measurements appear to happily chug along (though maybe this is only coincidental when it is one channel)...

I've downloaded Code::Blocks v20.03 and v25.03; the former seemingly was the version that bridged the gap between the default going from 32- to 64-bit defaults. (Note: There are 32- and 64-bit versions of the Code::Blocks *IDE*, of the mingw *compiler/linker*, and then there are versions of a build's *target architectures*, too.) This is, in part, one of the reasons I've never *just* gone with the next version since v17.12, especially since otherwise the program should work good enough (and arguably never require to actually address more than 4GiB of memory in RAM). There could be performance improvements to expect from a native width build, however, as some otherwise 32-to-64-bit wrapping method likely is not being called everytime memory is addressed.

Because, oh yeah, the standard included gcc/g++ in Code::Blocks v17.12 is v5.1, and it's not impossible this could also be due to an optimization error/bug on that old compiler version with `-O3 -flto` flags, so it seems now is the time to take that step. Nevertheless, I want to keep a 32-bit binary versions of ZaiDAQ-(Scope) available, but the default is now likely to switch to 64-bit as well (I mean, I suppose my Linux builds automatically are, hm...)



`2025-09-14` -- The updated code in its entirety should have been pushed with the commit that includes this edit. You can build (non-release/internal-debug-only) ZaidaScope-v250906 with it like in the screenshot below, though this presently offers no extra features beyond being compatible with the latest firmware again.

To do so with Code::Blocks, place both ZDK and ZaiDAQ folders in the same folder and then build the `*.cbp` projects in *order* of ZTM, ZTL, ZTK, ZTX (needs to be linked against zlib, libpng and libjpeg), ZDX, ZUI and finally ZS/ZaidaScope.

The Code::Blocks project files I used in 2021 to compile zlib, libpng and libjpeg from source can be found [here](https://github.com/ZaidaTek/zmisc). These were built inside/the `ZTX.cbp` project file expects them to be in e.g. `C:\__lib\zlib\zlib.(h|dll)`.  
Note: Code::Blocks project files are XML-files (and can be opened in text editors).

`2025-09-14` -- I have adapted ZDK in so far, as that it has partial (ZTM, ZTL, ZTX, ZDX) Linux support and now also supports its entire original Windows build as well again (at least according to how it worked with Windows 7/10). This has allowed me to rebuild ZaidaScope with all the current updates, refactors, and code, compiler and linker optimizations on its native platform. This makes *that* (not yet publicly available) build compatible again with the current embedded firmware, and no longer with the older one (to which ZaidaScope-v210603 and zdaq-v250831 are compatible to). On Windows, the original hardware and the up-to-date software appear to function faultlessly again at 100 kHz. This possibly means that either some code in the Linux chain with regards to serial USB communication is not emulating Windows in its entirety, or that maybe it is a Windows or driver issue (as one requires a third party driver on Windows; these often add vendor-specific code, that may be missing on Linux), or maybe something else altogether.

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



