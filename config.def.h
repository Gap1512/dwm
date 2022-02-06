/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 8;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = {
  "JetBrainsMono Nerd Font:style:medium:size=10"
};
static const char dmenufont[]       = "JetBrainsMono Nerd Font:style:medium:size=10";

static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_purple1[]     = "#2D2A2E";

/* Terminal Fonts */
static const char terminal_primary_bg[]     = "#2D2A2E";
static const char terminal_primary_fg[]     = "#FCFCFA";

static const char terminal_normal_black[]   = "#403E41";
static const char terminal_normal_red[]     = "#FF6188";
static const char terminal_normal_green[]   = "#A9DC76";
static const char terminal_normal_yellow[]  = "#FFD866";
static const char terminal_normal_orange[]  = "#FC9867";
static const char terminal_normal_magenta[] = "#AB9DF2";
static const char terminal_normal_cyan[]    = "#78DCE8";
static const char terminal_normal_white[]   = "#FCFCFA";

static const char terminal_bright_black[]   = "#727072";
static const char terminal_bright_red[]     = "#FF6188";
static const char terminal_bright_green[]   = "#A9DC76";
static const char terminal_bright_yellow[]  = "#FFD866";
static const char terminal_bright_orange[]  = "#FC9867";
static const char terminal_bright_magenta[] = "#AB9DF2";
static const char terminal_bright_cyan[]    = "#78DCE8";
static const char terminal_bright_white[]   = "#FCFCFA";

static const char *colors[][3]      = {
        /*               fg                      bg                   border         */
	[SchemeNorm] = { terminal_primary_fg,    terminal_primary_bg, terminal_primary_bg  },
	[SchemeSel]  = { terminal_normal_orange, terminal_primary_bg, terminal_primary_fg  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define PrintScreenDWM 0x0000ff61
#define RaiseVolumeDWM XF86XK_AudioRaiseVolume
#define LowerVolumeDWM XF86XK_AudioLowerVolume
#define MuteVolumeDWM  XF86XK_AudioMute


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", terminal_primary_bg, "-nf", terminal_primary_fg, "-sb", terminal_primary_bg, "-sf", terminal_normal_orange, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *cmdprintscreen[]  = { "scrot", "/home/gap1512/Screenshots/%Y-%m-%d-%s_$wx$h.jpg", NULL };
static const char *raisevolume[] = { "sh", "-c", "pactl set-sink-mute 1 false ; pactl set-sink-volume 1 +10%", NULL };
static const char *lowervolume[] = { "sh", "-c", "pactl set-sink-mute 1 false ; pactl set-sink-volume 1 -10%", NULL };
static const char *mutevolume[] = { "pactl", "set-sink-mute", "1", "toggle", NULL };

static Key keys[] = {
	/* modifier                     key             function        argument */
	{ MODKEY,                       XK_p,           spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,           togglebar,      {0} },
	{ MODKEY,                       XK_j,           focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,           focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,           incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,           incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,           setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,           setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,      zoom,           {0} },
	{ MODKEY,                       XK_Tab,         view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,           killclient,     {0} },
	{ MODKEY,                       XK_t,           setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,           setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,       setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
	{ MODKEY,                       XK_0,           view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,           tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,       focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,       tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,      tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                           0)
	TAGKEYS(                        XK_2,                           1)
	TAGKEYS(                        XK_3,                           2)
	TAGKEYS(                        XK_4,                           3)
	TAGKEYS(                        XK_5,                           4)
	TAGKEYS(                        XK_6,                           5)
	TAGKEYS(                        XK_7,                           6)
	TAGKEYS(                        XK_8,                           7)
	TAGKEYS(                        XK_9,                           8)
	{ MODKEY|ShiftMask,             XK_q,           quit,           {0} },
	{ 0,                            PrintScreenDWM, spawn,          {.v = cmdprintscreen } },
	{ 0,                            RaiseVolumeDWM, spawn,          {.v = raisevolume } },
	{ 0,                            LowerVolumeDWM, spawn,          {.v = lowervolume } },
	{ 0,                            MuteVolumeDWM,  spawn,          {.v = mutevolume } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

