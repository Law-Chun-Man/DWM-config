/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systraypadding = 85;  /* systray padding */
static const unsigned int systrayspacing = 1;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono:size=14:weight=bold", "FontAwesome:size=16:antialias=true:autohint=true" };
static const char dmenufont[]       = "JetBrainsMono:size=14:weight=bold";
static const char col_black[]       = "#000000";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_white[]       = "#ffffff";
static const char col_cyan[]        = "#005577";
static const char col_cyan_b[]      = "#00ddff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_black, col_black },
	[SchemeSel]  = { col_black, col_white,  col_white},
};

#define HOME "/home/[user name]"

static const char *const autostart[] = {
	"bash", "-c", HOME"/.config/dwm/bash_script/autostart.sh", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title       tags mask     isfloating   monitor   scratch key */
	{ NULL,                 NULL,       "spterm",   0,            1,           -1,       't' },
	{ NULL,                 NULL,       "spvpn",    0,            1,           -1,       'v' },
	{ NULL,                 NULL,       "spcalc",   0,            1,           -1,       'c' },
	{ "Thunar",             NULL,       NULL,       0,            1,           -1,       'h' },
	{ "Xfce4-appfinder",    NULL,       NULL,       0,            1,           -1,        0 },
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	{ "[]=",      tile },
	{ "[0]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
#define BROWSER "firefox-esr"
#define TERMINAL "xfce4-terminal", "--disable-server"
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "xfce4-terminal", "--disable-server", NULL };

/* commands spawned when clicking statusbar, the mouse button pressed is exported as BUTTON */
static const StatusCmd statuscmds[] = {
	{ "~/.config/dwm/dunst/mode_switcher.sh", 1 },
	{ "~/.config/dwm/dunst/mic_mute.sh", 2 },
	{ "pavucontrol", 3 },
};
static const char *statuscmd[] = { "/bin/sh", "-c", NULL, NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"t", TERMINAL, "-T", "spterm", "--geometry", "60x16", NULL};
static const char *calccmd[] = {"c", TERMINAL, "-T", "spcalc", "--geometry", "60x16+570+0", "-e", "python3 -i linux_computer/jupyter_file/import.py", NULL};
static const char *thunarcmd[] = {"h", "thunar", NULL};
static const char *vpncmd[] = {"v", TERMINAL, "--geometry", "60x16", "-T", "spvpn", "-e", "bash linux_computer/linux_setup/vpn/connect.sh", NULL};

static const Key keys[] = {
	/* modifier                     key        function        argument */
    // monitor
	{ 0,                       XF86XK_Display, spawn,          {.v = (const char*[]){ TERMINAL, "-e", HOME"/.config/dwm/bash_script/monitor.sh", NULL } } },

    // screenshot
	{ 0,                            XK_Print,  spawn,          {.v = (const char*[]){ "bash", "-c", "xfce4-screenshooter -crs Pictures/screenshots/screenshot_$(date +%Y%m%d_%H%M%S).png", NULL } } },
	{ MODKEY,                       XK_Print,  spawn,          {.v = (const char*[]){ "bash", "-c", "xfce4-screenshooter -cfs Pictures/screenshots/screenshot_$(date +%Y%m%d_%H%M%S).png", NULL } } },

    // clipboard manager
	{ MODKEY,                       XK_v,      spawn,          {.v = (const char*[]){ "xfce4-popup-clipman", NULL } } },

    // play-pause
	{ Mod1Mask,                     XK_space,  spawn,          {.v = (const char*[]){ "sh", "-c", "playerctl play-pause || parole --play", NULL } } },

    // volume control
	{ 0,              XF86XK_AudioLowerVolume, spawn,          {.v = (const char*[]){ HOME"/.config/dwm/dunst/volume_decrease.sh", NULL } } },
	{ 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = (const char*[]){ HOME"/.config/dwm/dunst/volume_increase.sh", NULL } } },
	{ 0,                     XF86XK_AudioMute, spawn,          {.v = (const char*[]){ HOME"/.config/dwm/dunst/volume_mute.sh", NULL } } },
	{ 0,                  XF86XK_AudioMicMute, spawn,          {.v = (const char*[]){ HOME"/.config/dwm/dunst/mic_mute.sh", NULL } } },

    // brightness control
	{ 0,               XF86XK_MonBrightnessUp, spawn,          {.v = (const char*[]){ HOME"/.config/dwm/dunst/brightness_increase.sh", NULL } } },
	{ 0,             XF86XK_MonBrightnessDown, spawn,          {.v = (const char*[]){ HOME"/.config/dwm/dunst/brightness_decrease.sh", NULL } } },

    // Poweroff
	{ ControlMask|Mod1Mask, 	    XK_Delete, spawn,          {.v = (const char*[]){ "sudo", "poweroff", NULL } } },

    // appfinder
	{ MODKEY,                       XK_p,      spawn,          {.v = (const char*[]){ "xfce4-appfinder", NULL } } },
	{ MODKEY,                       XK_slash,  spawn,          {.v = (const char*[]){ "xfce4-appfinder", "-c", NULL } } },
	{ MODKEY|ShiftMask,    XF86XK_TouchpadOff, spawn,          {.v = (const char*[]){ "xfce4-appfinder", NULL } } },

    // launch programme
	{ ControlMask|Mod1Mask, 	    XK_f,	   spawn,          {.v = (const char*[]){ BROWSER, NULL } } },
	{ ControlMask|Mod1Mask, 	    XK_h,	   spawn,          {.v = (const char*[]){ "thunar", NULL } } },
	{ ControlMask|Mod1Mask, 	    XK_t,	   spawn,          {.v = termcmd } },
	{ ControlMask|Mod1Mask, 	    XK_g,	   spawn,          {.v = (const char*[]){ "gnome-boxes", NULL } } },
	{ ControlMask|Mod1Mask, 	    XK_k,	   spawn,          {.v = (const char*[]){ "kdeconnect-app", NULL } } },
	{ ControlMask|Mod1Mask, 	    XK_o,	   spawn,          {.v = (const char*[]){ "obs", NULL } } },
	{ ControlMask|Mod1Mask, 	    XK_y,	   spawn,          {.v = (const char*[]){ BROWSER, "--private-window", "youtube.com", NULL } } },
	{ ControlMask|Mod1Mask, 	    XK_j,	   spawn,          {.v = (const char*[]){ HOME"/linux_computer/jupyter_file/jupyter.sh", NULL } } },
	{ ControlMask|Mod1Mask, 	    XK_p,	   spawn,          {.v = (const char*[]){ HOME"/.local/share/JetBrains/Toolbox/apps/pycharm-community/bin/pycharm.sh", NULL } } },
	{ ControlMask|Mod1Mask, 	    XK_w,	   spawn,          {.v = (const char*[]){ HOME"/.local/share/JetBrains/Toolbox/apps/webstorm/bin/webstorm.sh", NULL } } },

    // scratchpad
	{ MODKEY,                       XK_Return, togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                    XK_semicolon, togglescratch,  {.v = thunarcmd } },
	{ MODKEY,                       XK_ntilde, togglescratch,  {.v = thunarcmd } },
	{ ControlMask|Mod1Mask,         XK_c,      togglescratch,  {.v = calccmd } },
	{ 0,                     XF86XK_Favorites, togglescratch,  {.v = calccmd } },
	{ ControlMask|Mod1Mask,         XK_v,      togglescratch,  {.v = vpncmd } },

    // window manager control
	{ MODKEY,                       XK_f,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_comma,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_period, incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_n,      setmfact,       {.f = 1.5} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_m,      zoom,           {0} },
	{ MODKEY,                       XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_Tab,    setlayout,      {0} },
	{ MODKEY,                       XK_t,      togglefloating, {0} },
	{ MODKEY,                       XK_equal,  view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_equal,  tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_i,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_d,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_d,      tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_0,                      9)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_w,	   spawn,          {.v = (const char*[]){ "dm-tool", "lock", NULL }}},
	{ MODKEY|ShiftMask,             XK_r,	   spawn,          {.v = (const char*[]){ "sh", "-c", "kill $(pgrep -f ~/.config/dwm/bash_script/status_bar.sh); ~/.config/dwm/bash_script/status_bar.sh &", NULL }}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[1]} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,              Button3,        spawn,          {.v = statuscmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

