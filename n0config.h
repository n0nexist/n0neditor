/*
n0-neditor's config file
*/


/*
MAIN
*/

// foreground color
#define fg_color COLOR_WHITE
// background color
#define bg_color -1
// foreground color for selected item
#define fg_color_selected COLOR_BLACK
// background color for selected item
#define bg_color_selected COLOR_WHITE
// command to launch the editor
#define editor_cmd "nano"
// command to move file to trash
#define trash_cmd "gio trash"
// string to identify a directory
#define dir_string "[Selected: DIR]"
// string to identify a file
#define file_string "[Selected: FIL]"


/*
KEYBINDS
*/

// keybind to exit (q)
#define keyb_exit 113
// keybind to go forward (l)
#define keyb_go_forw 108
// keybind to go backwards (j)
#define keyb_go_backw 106
// keybind to go up (n)
#define keyb_go_up 110
// keybind to go down (m)
#define keyb_go_down 109
// keybind to move offset down (b)
#define keyb_l_offset 98
// keybind to move offset up (v)
#define keyb_u_offset 118
// keybind to open a new file (f)
#define keyb_blank_file 102
//keybind to move file to trash (t)
#define keyb_trash 116