#define PATH "/home/jimkat/.scripts/dwmblocks/"

//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon*/	/*Command*/								/*Update Interval*/	/*Update Signal*/
	{"",	PATH"earbuds.sh",		PATH"earbuds_button.sh",	60,	3},
	{"",	PATH"volume.sh",		PATH"volume_button.sh",		0,	2},
	{"",	PATH"battery.sh",		PATH"battery_button.sh",	1,	1},
	{"",	"date '+%a %d/%m/%Y %H:%M'",	"",				30,	0},
};

//Send signal with pkill -RTMIN+<Update Signal> dwmblocks

//Sets delimiter between status commands. NULL character ('\0') means no delimiter.
static char *delim = " │ "; //│

// Have dwmblocks automatically recompile and run when you edit this file in
// vim with the following line in your vimrc/init.vim:

// autocmd BufWritePost ~/.local/src/dwmblocks/config.h !cd ~/.local/src/dwmblocks/; sudo make install && { killall -q dwmblocks;setsid dwmblocks & }
