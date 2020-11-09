/* modifier 0 means no modifier */
static int surfuseragent    = 1;  /* Append Surf version to default WebKit user agent */
static char *fulluseragent  = ""; /* Or override the whole user agent string */
static int startlocbar      = 0;  /* Show location bar on startup (1=yes, 0=no) */
static char *scriptfile     = "~/.surf/script.js";
static char *styledir       = "~/.surf/styles/";
static char *certdir        = "~/.surf/certificates/";
static char *cachedir       = "~/.surf/cache/";
static char *cookiefile     = "~/.surf/cookies.txt";
static char *scriptfiles[]  = {
 "~/.surf/addons/addon-1.js",
 "~/.surf/addons/addon-2.js",
};
static char **plugindirs    = (char*[]){
	"~/.surf/plugins/",
	LIBPREFIX "/mozilla/plugins/",
	NULL
};
#define HOMEPAGE "https://duckduckgo.com/"
/* sc0ttj  added lots of custom search engines:
 *
 * Note: any search in location bar that is not a URL, file
 *       or prefixed custom search (below), will fall back
 *       to using the first search engine in this list.
*/
static SearchEngine searchengines[] = {
	{ "dg",  "https://duckduckgo.com/?q=%s"   },
	{ "ar",  "https://archive.org/search.php?query=%s"   },
	{ "aw",  "https://wiki.archlinux.org/index.php?search=%s"   },
	{ "bbc", "https://www.bbc.co.uk/search?q=%s"   },
	{ "gg",  "http://www.google.com/search?q=%s"   },
	{ "gh",  "https://github.com/search?q=%s"   },
	{ "gist", "https://gist.github.com/search?q=%s"   },
	{ "gi",  "https://www.google.com/search?&q=%s&oq=%s"   },
	{ "im",  "https://www.imdb.com/find?q=%s"   },
	{ "man", "http://manpages.org/%s/1" },
	{ "pf",  "https://forum.puppylinux.com/search.php?keywords=%s" },
	{ "pl",  "https://ww1.put-locker.com/?s=%s" },
	{ "sc",  "https://soundcloud.com/search?q=%s" },
	{ "sp",  "https://startpage.com/%s/search" },
	{ "so",  "https://stackoverflow.com/search?q=%s" },
	{ "sx",  "https://stackexchange.com/search?q=%s" },
	{ "tw",  "https://twitter.com/search?q=%s&src=typed_query" },
	{ "txt",  "https://www.textise.net/showText.aspx?strURL=https%253A//www.google.com/search%253Fq%253D%s" },
	{ "ud",  "https://www.urbandictionary.com/define.php?term=%s" },
	{ "ux",  "https://unix.stackexchange.com/search?q=%s" },
	{ "yt",  "https://www.youtube.com/results?search_query=%s" },
	{ "wb",  "https://web.archive.org/web/*/%s" },
	{ "wk",  "https://en.wikipedia.org/wiki/%s" },
};
/* Webkit default features */
/* Highest priority value will be used.
 * Default parameters are priority 0
 * Per-uri parameters are priority 1
 * Command parameters are priority 2
 */

/* sc0ttj enabled DNSPrefetch, StrictTLS, WebGL */
/* sc0ttj added ClipboardNotPrimary */
static Parameter defconfig[ParameterLast] = {
	/* parameter                    Arg value       priority */
	[AcceleratedCanvas]   =       { { .i = 1 },     },
	[AccessMicrophone]    =       { { .i = 0 },     },
	[AccessWebcam]        =       { { .i = 0 },     },
	[Certificate]         =       { { .i = 0 },     },
	[CaretBrowsing]       =       { { .i = 0 },     },
	[ClipboardNotPrimary] =       { { .i = 1 },     },
	[CookiePolicies]      =       { { .v = "@Aa"   }, },
	[DefaultCharset]      =       { { .v = "UTF-8" }, },
	[DiskCache]           =       { { .i = 1 },     },
	[DNSPrefetch]         =       { { .i = 1 },     },
	[Ephemeral]           =       { { .i = 0 },     },
	[FileURLsCrossAccess] =       { { .i = 0 },     },
	[FontSize]            =       { { .i = 12 },    },
	[FrameFlattening]     =       { { .i = 0 },     },
	[Geolocation]         =       { { .i = 0 },     },
	[HideBackground]      =       { { .i = 0 },     },
	[Inspector]           =       { { .i = 0 },     },
	[Java]                =       { { .i = 1 },     },
	[JavaScript]          =       { { .i = 1 },     },
	[KioskMode]           =       { { .i = 0 },     },
	[LoadImages]          =       { { .i = 1 },     },
	[MediaManualPlay]     =       { { .i = 1 },     },
	[Plugins]             =       { { .i = 1 },     },
	[PreferredLanguages]  =       { { .v = (char *[]){ NULL } }, },
	[RunInFullscreen]     =       { { .i = 0 },     },
	[ScrollBars]          =       { { .i = 1 },     },
	[ShowIndicators]      =       { { .i = 1 },     },
	[SiteQuirks]          =       { { .i = 1 },     },
	[SmoothScrolling]     =       { { .i = 0 },     },
	[SpellChecking]       =       { { .i = 0 },     },
	[SpellLanguages]      =       { { .v = ((char *[]){ "en_US", NULL }) }, },
	[StrictTLS]           =       { { .i = 1 },     },
	[Style]               =       { { .i = 1 },     },
	[WebGL]               =       { { .i = 1 },     },
	[ZoomLevel]           =       { { .f = 1.0 },   },
};

static UriParameters uriparams[] = {
	{ "(://|\\.)suckless\\.org(/|$)", {
	  [JavaScript] = { { .i = 0 }, 1 },
	  [Plugins]    = { { .i = 0 }, 1 },
	}, },
/* sc0ttj added site specific settings, as examples */
	{ "(://|\\.)google\\.com(/|$)", {         // The big bad Google
	  [JavaScript] = { { .i = 0 }, 1 },       // no JavaScript
	  [CookiePolicies] = { { .v = "@" }, 1 }, // Don't accept third party
	  [Geolocation] = { { .i = 0 }, 1 },      // No Geolocation! Google knows anyway
	}, },
	{ "(://|\\.)gmail\\.com(/|$)", {
	  [JavaScript] = { { .i = 0 }, 1 },
	  [Geolocation] = { { .i = 0 }, 1 },
	}, },
	{ "(://|\\.)youtube\\.com(/|$)", {
	  [JavaScript] = { { .i = 1 }, 1 },
	  [Geolocation] = { { .i = 0 }, 1 },
	}, },
	{ "(://|\\.)facebook\\.com(/|$)", {
	  [Plugins] = { { .i = 0 }, 1 },
	  [CookiePolicies] = { { .v = "a" }, 1 }, // No cookies at all
	}, },
	{ "(://|\\.)tumblr\\.com(/|$)", {
	  [Plugins] = { { .i = 0 }, 1 },
	  [StrictTLS] = { { .i = 0 }, 1 },
	  [CookiePolicies] = { { .v = "a" }, 1 }, // No cookies at all
	}, },
	{ "(://|\\.)wikipedia\\.org(/|$)", {
	  [JavaScript] = { { .i = 1 }, 1 },
	  [ZoomLevel] = { { .f = 1.20 }, 1 }, // custom zoom level
	  [FontSize] = { { .i = 13 }, 1 },    // custom font size
	}, },
	{ "(://|\\.)wiki\\.archlinux\\.org(/|$)", {
	  [JavaScript] = { { .i = 1 }, 1 },
	  [ZoomLevel] = { { .f = 1.20 }, 1 }, // custom zoom level
	  [FontSize] = { { .i = 14 }, 1 },    // custom font size
	}, },
};

/* default window size: width, height */
static int winsize[] = { 800, 600 };

static WebKitFindOptions findopts = WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE |
                                    WEBKIT_FIND_OPTIONS_WRAP_AROUND;

/* sc0ttj location bar now accepts URLs, custom search engine cmds, and
*         arbitrary search terms, so rename the label to reflect this */
#define PROMPT_GO   "Web search:"
#define PROMPT_FIND "Find:"

/* SETPROP(readprop, setprop, prompt)*/
#define SETPROP(r, s, p) { \
        .v = (const char *[]){ "/bin/sh", "-c", \
             "prop=\"$(printf '%b' \"$(xprop -id $1 $2 " \
             "| sed \"s/^$2(STRING) = //;s/^\\\"\\(.*\\)\\\"$/\\1/\" \
              | grep -v '_SURF_FIND' \
              && tac ~/.surf/bookmarks)\" " \
             "| dmenu -l 10 -p \"$4\" -w $1)\" && " \
             "xprop -id $1 -f $3 8u -set $3 \"$prop\"", \
             "surf-setprop", winid, r, s, p, NULL \
        } \
}

/* sc0ttj this is not used - doesn't support custom search engines */
#define SEARCH() { \
        .v = (const char *[]){ "/bin/sh", "-c", \
             "xprop -id $1 -f $2 8s -set $2 \"" \
             "$(dmenu -p 'Web Search:' -w $1 < /dev/null)\"", \
             "surf-search", winid, "_SURF_SEARCH", NULL \
        } \
}

/* sc0ttj always save to ~/Downloads, slightly nicer pop up  */
/* DOWNLOAD(URI, referer) */
#define DOWNLOAD(u, r) { \
        .v = (const char *[]){ "st", "-g", "84x8", "-e", "/bin/sh", \
             "-c", "cd ~/Downloads; curl -g -L -J -O -A \"$1\" -b \"$2\" -c \"$2\"" \
             " -e \"$3\" \"$4\"; echo; echo 'Finished.'; read", \
             "surf-download", useragent, cookiefile, r, u, NULL \
        } \
}

/* PLUMB(URI) */
/* This called when some URI which does not begin with "about:",
 * "http://" or "https://" should be opened.
 */
#define PLUMB(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "xdg-open \"$0\"", u, NULL \
        } \
}

/* VIDEOPLAY(URI) */
#define VIDEOPLAY(u) {\
        .v = (const char *[]){ "/bin/sh", "-c", \
             "mpv --really-quiet --x11-name 'mpv via surf' \"$0\"", u, NULL \
        } \
}

/* sc0ttj added Ctrl-D, Ctrl-B dmenu-based bookmarking, no sh scripts */
#define BM_PICK {\
   .v = (char *[]){ "/bin/sh", "-c", \
     "xprop -id $0 -f _SURF_GO 8s -set _SURF_GO 2>/dev/null \
     $(tac ~/.surf/bookmarks \
      | dmenu -p 'Load Bookmark' -i -l 10 -w $0 \
      | awk '{print $1}' || exit 0)", \
    winid, NULL } }

#define BM_ADD {\
    .v = (const char *[]){ "/bin/sh", "-c", \
         "(echo $(xprop -id $0 _SURF_URI) | cut -d '\"' -f2 " \
         "&& sort -u ~/.surf/bookmarks) " \
         "| dmenu -i -p 'Save Bookmark' -i -l 10 -w $0 "  \
         ">> ~/.surf/bookmarks", \
         winid, NULL } }

/* styles */
/*
 * The iteration will stop at the first match, beginning at the beginning of
 * the list.
 */
static SiteSpecific styles[] = {
	/* regexp               file in $styledir */
	{ ".*",                 "default.css" },
/* sc0ttj added site specific CSS, as examples */
//	{ ".*github.com.*",    "github.css" },
//	{ ".*reddit.com.*",    "reddit.css" },
//	{ ".*wikipedia.org.*", "wikipedia.css" },
};

/* certificates */
/*
 * Provide custom certificate for urls
 */
static SiteSpecific certs[] = {
	/* regexp               file in $certdir */
	{ "://suckless\\.org/", "suckless.org.crt" },
};

#define MODKEY GDK_CONTROL_MASK

/* hotkeys */
/*
 * If you use anything else but MODKEY and GDK_SHIFT_MASK, don't forget to
 * edit the CLEANMASK() macro.
 */

/*
 * sc0ttj changed some hotkeys:
 *   - Ctrl-l    =  open location bar (omnibar)
 *   - Ctrl-q    =  exit (close window)
 *   - Ctrl-=    =  zoom+1
 *   - Ctrl-b    =  open bookmarks menu
 *   - Ctrl-d    =  bookmark current page
 *   - Alt-left  =  go back a page
 *   - Alt-right =  go forward a page
 *
 */
static Key keys[] = {
	/* modifier              keyval          function    arg */
	{ MODKEY,                GDK_KEY_l,      spawn,      SETPROP("_SURF_URI", "_SURF_GO", PROMPT_GO) },
	{ MODKEY,                GDK_KEY_g,      spawn,      SETPROP("_SURF_URI", "_SURF_GO", PROMPT_GO) },
	{ MODKEY,                GDK_KEY_f,      spawn,      SETPROP("_SURF_FIND", "_SURF_FIND", PROMPT_FIND) },
	{ MODKEY,                GDK_KEY_slash,  spawn,      SETPROP("_SURF_FIND", "_SURF_FIND", PROMPT_FIND) },
  /* sc0ttj disable this search - does not support custom search engines
 	{ MODKEY,                GDK_KEY_s,      spawn,      SEARCH() },
  */

	/* sc0ttj nice bookmarks menu */
	{ MODKEY,                GDK_KEY_b,      spawn,      BM_PICK },
	{ MODKEY,                GDK_KEY_d,      spawn,      BM_ADD },

	{ MODKEY,                GDK_KEY_q,      exit,       { 0 } },

	{ 0,                     GDK_KEY_Escape, stop,       { 0 } },
	{ MODKEY,                GDK_KEY_c,      stop,       { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_r,      reload,     { .i = 1 } },
	{ MODKEY,                GDK_KEY_r,      reload,     { .i = 0 } },

	{ MODKEY,                GDK_KEY_l,      navigate,   { .i = +1 } },
	{ MODKEY,                GDK_KEY_h,      navigate,   { .i = -1 } },

	/* sc0ttj use Alt-left and Alt-right to go back/forward in history */
	{ GDK_MOD1_MASK,         GDK_KEY_Right,  navigate,   { .i = +1 } },
	{ GDK_MOD1_MASK,         GDK_KEY_Left,   navigate,   { .i = -1 } },

	/* vertical and horizontal scrolling, in viewport percentage */
/*
	{ MODKEY,                GDK_KEY_j,      scrollv,    { .i = +10 } },
	{ MODKEY,                GDK_KEY_k,      scrollv,    { .i = -10 } },
	{ MODKEY,                GDK_KEY_space,  scrollv,    { .i = +50 } },
	{ MODKEY,                GDK_KEY_b,      scrollv,    { .i = -50 } },
	{ MODKEY,                GDK_KEY_i,      scrollh,    { .i = +10 } },
	{ MODKEY,                GDK_KEY_u,      scrollh,    { .i = -10 } },
*/

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_j,      zoom,       { .i = -1 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_k,      zoom,       { .i = +1 } },

	{ MODKEY,                GDK_KEY_0,      zoom,       { .i = 0  } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_q,      zoom,       { .i = 0  } },
	{ MODKEY,                GDK_KEY_minus,  zoom,       { .i = -1 } },
	{ MODKEY,                GDK_KEY_plus,   zoom,       { .i = +1 } },
	{ MODKEY,                GDK_KEY_equal,  zoom,       { .i = +1 } }, /* sc0ttj use 'equal' and 'plus' */

	{ MODKEY,                GDK_KEY_p,      clipboard,  { .i = 1 } },
	{ MODKEY,                GDK_KEY_y,      clipboard,  { .i = 0 } },

	{ MODKEY,                GDK_KEY_n,      find,       { .i = +1 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_n,      find,       { .i = -1 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_p,      print,      { 0 } },
	{ MODKEY,                GDK_KEY_t,      showcert,   { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_a,      togglecookiepolicy, { 0 } },
	{ 0,                     GDK_KEY_F11,    togglefullscreen, { 0 } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_o,      toggleinspector, { 0 } },

	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_c,      toggle,     { .i = CaretBrowsing } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_f,      toggle,     { .i = FrameFlattening } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_g,      toggle,     { .i = Geolocation } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_s,      toggle,     { .i = JavaScript } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_i,      toggle,     { .i = LoadImages } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_v,      toggle,     { .i = Plugins } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_b,      toggle,     { .i = ScrollBars } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_t,      toggle,     { .i = StrictTLS } },
	{ MODKEY|GDK_SHIFT_MASK, GDK_KEY_m,      toggle,     { .i = Style } },
	/* sc0ttj added hotkey to call externel player */
	{ MODKEY,                GDK_KEY_w,      playexternal, { 0 } },

  /* sc0ttj support special keyboard keys.. see this link: */
  /* https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent/key/Key_Values */
	{ 0,                     GDK_KEY_Find,    spawn,      SETPROP("_SURF_FIND", "_SURF_FIND", PROMPT_FIND) },
	{ 0,                     GDK_KEY_Search,  spawn,      SETPROP("_SURF_URI", "_SURF_GO", PROMPT_GO) },
	{ 0,                     GDK_KEY_ZoomOut, zoom,       { .i = -1 } },
	{ 0,                     GDK_KEY_ZoomIn,  zoom,       { .i = +1 } },
	{ 0,                     GDK_KEY_Back,    navigate,   { .i = -1 } },
	{ 0,                     GDK_KEY_Forward, navigate,   { .i = +1 } },
	{ 0,                     GDK_KEY_Refresh, reload,     { .i = 0 } },
	{ 0,                     GDK_KEY_Reload,  reload,     { .i = 0 } },
	{ 0,                     GDK_KEY_Stop,    stop,       { 0 } },
	{ 0,                     GDK_KEY_Favorites, spawn,    BM_PICK },
	{ 0,                     GDK_KEY_MySites, spawn,      BM_PICK },
};

/* button definitions */
/* target can be OnDoc, OnLink, OnImg, OnMedia, OnEdit, OnBar, OnSel, OnAny */
static Button buttons[] = {
	/* target       event mask      button  function        argument        stop event */
	{ OnLink,       0,              2,      clicknewwindow, { .i = 0 },     1 },
	{ OnLink,       MODKEY,         2,      clicknewwindow, { .i = 1 },     1 },
	{ OnLink,       MODKEY,         1,      clicknewwindow, { .i = 1 },     1 },
	{ OnAny,        0,              8,      clicknavigate,  { .i = -1 },    1 },
	{ OnAny,        0,              9,      clicknavigate,  { .i = +1 },    1 },
	{ OnMedia,      MODKEY,         1,      clickexternplayer, { 0 },       1 },
};

