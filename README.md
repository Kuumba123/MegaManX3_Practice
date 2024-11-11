# MegaMan X3 Practice
This is a hack to assist with practicing speedruns of the PSX version of Rockman X3.
It primarly adds save states which are not normally possible on PSX games due to the nature of the Disc Games.

## Compile
1st your gonna need a total of 3 repros (including this one)
 * Red-Hot's PSX Modding Repro https://github.com/mateusfavarin/psx-modding-toolchain
 * My MMX3 PSX Repro https://github.com/Kuumba123/MegaManX3_PS1_Modding

Once you have all 3 go back to the github with Red-Hot's PSX modding repro and following the instructions there. If you can compile any of the examples included in that repro you should be able to compile this one.

Then put the MegaMan X3 PSX repro inside the `games` folder and then this repro inside your `mods` folder and put your MegaMan X3 ISO files inside the `build` folder (must be named `RMX3.cue` and `RMX3.bin`).

Once your done setting up all the files go back to the MMX3 Practice repro and run the MOD.BAT file. The only options that you should really be interested in are the option `1` , `3` and `4`. 1st you should do option 3 to extract the game files for editing then option 1 to compile then option 4 to actually build the game disc.

## How to Play
If you have a modded PS2 you should be able to play this using POPS or if you have a PS1 use an Xstation.