/*
       _______                    .___.__  __
  ____ \   _  \   ____   ____   __| _/|__|/  |_  ____ ______
 /    \/  /_\  \ /    \_/ __ \ / __ | |  \   __\/  _ \_  __ \
|   |  \  \_/   \   |  \  ___// /_/ | |  ||  | (  <_> )  | \/
|___|  /\_____  /___|  /\___  >____ | |__||__|  \____/|__|
     \/       \/     \/     \/     \/

		[ made by github.com/n0nexist ]
*/

#include <sys/ioctl.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "n0config.h"

int number_of_files;
int window_offset = 0;
int woffset;

void goodbye(int code)
{
     endwin();
     system("clear");
     printf("Exiting with code: %i\n", code);
     exit(0);
}


int countFilesinDir(char *dirtoOpen){
	struct dirent **namelist;
        int n;
        int number = 0;

        n = scandir(dirtoOpen, &namelist, 0, alphasort);
        if (n < 0)
                perror("scandir");
                else {
                while (n--) {
                number += 1;
                free(namelist[n]);
        }
                free(namelist);
        }

     return number;
}


int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}


int listDir(WINDOW *win, int index, int fileIndex, char *dirtoOpen, char *operation, char* main_cwd, int statusbarindex){

	struct dirent **namelist;
        int n;
        char *filename;
        char *status;
        char trashcmd[PATH_MAX];
        char editorcmd[PATH_MAX];
        char *selectedfilename;
        int isdir;
	chdir(dirtoOpen);

	index += window_offset;

        n = scandir(dirtoOpen, &namelist, 0, alphasort);
        if (n < 0)
                perror("scandir");
                else {
                while (n--) {

                 if (index < statusbarindex){
                    if (index > 0){
	                filename = namelist[n]->d_name;
	                if (fileIndex == index+woffset){
	                     if(operation == "list"){
		                     wattron(win, COLOR_PAIR(2));
		                     selectedfilename = filename;
		             }else if (operation == "goforward"){
	                        isdir = isDirectory(filename);
		                if (isdir == 0){
			           	sprintf(editorcmd, "%s %s",editor_cmd, filename);
			           	system(editorcmd);
			        } else {
				        number_of_files = countFilesinDir(filename);
	        			moveIndex(win, 0, fileIndex, filename, statusbarindex);
			        }
		             	return 1;
		             }else if (operation == "delfile"){
				sprintf(trashcmd, "%s %s",trash_cmd, filename);
		           	system(trashcmd);
		           	return 1;
		             }
		      }
	           }
	        }
                mvwprintw(win, index, 1, filename);
                wattron(win, COLOR_PAIR(1));

                isdir = isDirectory(selectedfilename);
                if(isdir != 0){
	                status = dir_string;
        	        setStatus(win,status,1,statusbarindex);
        	} else {
	                status = file_string;
        	        setStatus(win,status,1,statusbarindex);
        	}

            	wrefresh(win);
                index += 1;

                free(namelist[n]);
        }
                free(namelist);
        }

     return index;

}


void setTitle(WINDOW *win){

	char cwd[PATH_MAX];
        getcwd(cwd, sizeof(cwd));
        char mytitle[sizeof(cwd)];
        sprintf(mytitle, "[%s]", cwd);
        mvwprintw(win, 0, 1, mytitle);

}


void setStatus(WINDOW *win, char* status, int x, int y){
	mvwprintw(win, y, x, status);
	refresh();

}


void clearWindow(WINDOW *win){
	wclear(win);
	box(win,0,0);
	refresh();
	curs_set(0);
	setTitle(win);
	wattron(win, COLOR_PAIR(1));
}

void moveIndex(WINDOW *win, int index, int fileIndex, char *dirtoOpen, int statusbarindex){
	clearWindow(win);
	index = listDir(win,index,fileIndex,dirtoOpen,"list","",statusbarindex);
	refresh();
}


void windowFunc(WINDOW *win, int wsrow, char* dirtoOpen, char* main_cwd){

	chdir(dirtoOpen);
	int index = 1;
	int fileIndex = 1;
	DIR *d;
	DIR *directory;
	int statusbarindex = wsrow-1;
	char* selectedItem;
	char* dirchanged;
	curs_set(0);

	moveIndex(win, index, fileIndex, dirtoOpen, statusbarindex);

	for (;;)
	{
	int c = wgetch(win);
//	goodbye(c);
	switch (c){

		case keyb_exit:
			goodbye(c);
		break;

		case keyb_go_down:
			if (fileIndex < (number_of_files-window_offset)){
	        		fileIndex += 1;
	        	} else {
	        		fileIndex = 1;
	        	}
		break;

		case keyb_go_up:
			if (fileIndex > 1){
				fileIndex -= 1;
			}else {
				fileIndex = number_of_files;
			}
		break;

		case keyb_go_forw:
			woffset = window_offset;
			window_offset = 0;
			index = listDir(win,index,fileIndex,dirtoOpen,"goforward", main_cwd, statusbarindex);
			fileIndex = 1;
		break;

		case keyb_go_backw:
			window_offset = 0;
			dirchanged = "..";
		        number_of_files = countFilesinDir(dirchanged);
			moveIndex(win, index, fileIndex, dirchanged, statusbarindex);
			fileIndex = 1;
		break;

		case keyb_l_offset:
			if (window_offset < 0){
				window_offset ++;
				fileIndex = 1;
			}
		break;

		case keyb_u_offset:
			window_offset --;
			fileIndex = 1;
		break;

		case keyb_blank_file:
			system(editor_cmd);
		break;


		case keyb_trash:
			woffset = window_offset;
			window_offset = 0;
			index = listDir(win,index,fileIndex,dirtoOpen,"delfile", main_cwd, statusbarindex);
			fileIndex = 1;
			woffset = 0;
			window_offset = 0;
		break;


	}
	wclear(win);
	moveIndex(win, index, fileIndex, dirtoOpen, statusbarindex);

	}


}


int main(int argc, char *argv[])
{

	(void) signal(SIGINT, goodbye);

	(void) initscr();
	keypad(stdscr, TRUE);
	(void) nonl();
	(void) cbreak();
	(void) noecho();

	if (!has_colors())
	{
		goodbye(2);
	}


	char* main_cwd;
        getcwd(main_cwd, PATH_MAX);


	start_color();

	use_default_colors();
	init_pair(1, fg_color, bg_color);
	init_pair(2, fg_color_selected, bg_color_selected);

	curs_set(0);

	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	WINDOW *win = newwin(w.ws_row, w.ws_col, 0, 0);

	char* dirtoOpen = ".";
	number_of_files = countFilesinDir(dirtoOpen);
	windowFunc(win, w.ws_row, dirtoOpen, main_cwd);


	goodbye(0);
}