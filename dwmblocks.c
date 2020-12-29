#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <X11/Xlib.h>

#define LENGTH(X)               (sizeof(X) / sizeof (X[0]))

typedef struct {
	char *icon;
	char *execget;
	char *exec;
	unsigned int interval;
	unsigned int signal;
} Block;

void buttonhandler(int sig, siginfo_t *si, void *ucontext);
void exec_get_refresh(int block, int button);
void construct();
void sighandler(int sig);
void setroot();
void setupsignals();

#include "config.h"

char status[2][1024]; //0new 1old
char blocktext[LENGTH(blocks)][256];

void
buttonhandler(int sig, siginfo_t *si, void *ucontext){
	sig = si->si_value.sival_int >> 8;
	int i = 0;
	while(blocks[i++].signal != sig && i < LENGTH(blocks));
	exec_get_refresh(i-1, si->si_value.sival_int & 0xff);
}

void
construct(){
	status[0][0] = '\0';
	int i = -1;
	while(++i < LENGTH(blocks)){
		if(blocktext[i][0]){
			strcat(status[0],blocktext[i]);
			strcat(status[0],delim);
		}
	}
	if(status[0][0]){
		status[0][strlen(status[0])-strlen(delim)] = 0;
	}
}

void
exec_get_refresh(int blockn, int buttonn){
	char *bt = blocktext[blockn];
	Block block = blocks[blockn];

	if (block.signal){
	    bt[0] = block.signal;
	    bt++;
	}

	if(buttonn){
		if(fork() == 0){
			char button[10];
			sprintf(button,"%d",buttonn);
			setenv("BLOCK_BUTTON",button,0);
			execl(block.exec,block.exec,NULL);
		}
	}

	FILE *proc = popen(block.execget,"r");
	if(proc){
		fgets(bt,256,proc);
	
		int i=-1;
		while(bt[++i]){
			if(bt[i] == '\n'){
				bt[i] = '\0';
				break;
			}
		}
		pclose(proc);
	}

	construct();
	setroot();
}

void
sighandler(int sig){
	int i = 0;
	for(; i < LENGTH(blocks); i++)
		if(sig - SIGRTMIN == blocks[i].signal)
			break;

	exec_get_refresh(i,0);
}

void
setroot()
{
	if(!strcmp(status[0],status[1]))
		return;
	strcpy(status[1], status[0]);

	Display *dpy, *d = XOpenDisplay(NULL);
	if (d) {
	    dpy = d;
	}
	int screen = DefaultScreen(dpy);
	Window root = RootWindow(dpy, screen);
	XStoreName(dpy, root, status[0]);
	XCloseDisplay(dpy);
}

void
setupsignals(){
	struct sigaction sa;
	for(int i = 0; i < LENGTH(blocks); i++)
	{
		if (blocks[i].signal > 0)
		{
			signal(SIGRTMIN+blocks[i].signal, sighandler);
			sigaddset(&sa.sa_mask, SIGRTMIN+blocks[i].signal);
		}
	}
	sa.sa_sigaction = buttonhandler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	struct sigaction sigchld_action = {
		.sa_handler = SIG_DFL,
		.sa_flags = SA_NOCLDWAIT
	};
	sigaction(SIGCHLD, &sigchld_action, NULL);
}

int
main(int argc, char** argv){
	setupsignals();
	unsigned int time = 0;
	for(int i = 0; i < LENGTH(blocks); i++){
		exec_get_refresh(i,0);
	}

	while(1){
		for(int i = 0; i < LENGTH(blocks); i++){
			if(blocks[i].interval && time % blocks[i].interval == 0){
				exec_get_refresh(i,0);
			}
		}
		time++;
		sleep(1);
	}
}
