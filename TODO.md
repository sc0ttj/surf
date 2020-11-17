# TODO

Here's a list of things I want to add to this fork of `surf`:

1. Enable smooth scrolling by default (if not already)

  - see `enable-smooth-scrolling`

2. More powerful "add-ons"

  - enable `allow-file-access-from-file-urls` for AJAX
  - enable `allow-universal-access-from-file-urls` for LocalStorage
  - enable these settings for JS files run from `~/.surf/addons/`

3. Add a `--fullscreen|-fs` option to the command-line

  - users can toggle fullscreen with F11 once running
  - but users cant _start_ in fullscreen
  - so add an option to do this

4. Add cli option to enable developer console

  - otherwise disable it by default
  - if enabled, also set:
    - cache disabled
    - dev tools enabled and open by default
    - enable `enable-write-console-messages-to-stdout`
      - this will put console.log() stuff to stdout

5. Toggle dark mode

  - get the dark mode CSS from `surfer`, adjust it a bit
  - add a toggle, using `xprops`
  - add a hot key to toggle it
  - add a cli option to enable it on start up
  - add to docs

6. ...

7. Add a "save file" dialog

  - pops up when choosing where to save a file
  - use whatever works

8. Add a download page/tab (or popup, if need be)

  - open with Ctrl-j
  - show progress bars
  - show completed downloads
  - show button: "open"
  - show button: "open parent folder"
  - show button: "cancel"

9. Enable "private browsing" (is-ephemeral) mode

  - enable setting all these options in one go:
    - cookie policy deny all
    - no geolocation
    - ephemeral = true
    - disable plugins(?) (..separate dir for addons if in privmode?)
    - java = false
    - javascript = false
    - enable-javascript-markup = false
    - strictTLS = true
    - dnsPrefetching = false
    - cachedir = /dev/null  (if possible)
    - enable-page-cache = false
    - enable developer console

10. Extend the right-click menu:

  10a. Right click anywhere on a page
    - "Save element as screenshot"
    - "Save page as HTML"
    - "Save page as plain text"
    - "Save page as screenshot"

  10b. Right-click on link to web page:
    - "Open in new Window"
    - "Save page as HTML"
    - "Save page as plain text"

  10c. Right-click on link to file:
    - "Download"
    - "Download to.." (opens a "save file" dialog)
    - "View as text"

  10d. Right-click selected text:
    - "Open in Editor"
    - "Save to file.." (opens a "save file" dialog)

  10e. Right-click an image, or a link to an image:
    - "Download"
    - "Download to.." (opens a "save file" dialog)
    - "Open in editor" (opens image in an image editor)
    - "Open in editor, no background" (uses removebg.com)

  10f. Right-click link to video/audio file or Youtube video page
    - "Play in media player"

  10g. Right-click magnet link:
    - "Open in torrent client"

11. Enable "developer" mode:

  - console.log() a nice message on page load
  - watch local files for changes, auto refresh
    - only if $1 is localhost, 127.0.0.1, or file://
  - bundle `tea` into every page?
    - if `tea` running in browser, it saves your tests to LocalStorage
    - tests you write are persistent between page refreshes


12. Allow settings in config file (see wyeb)

  - no need to recompile
  - set many underlying webkit settings

13. Allow live monitoring of config files (see wyeb)

    - update config file, settings will change immediately

14. Expose these settings in config file to cli  (see wyeb), using a new command: 
    - `surf set enable-smooth-scrolling true`
  
15. More user-fiendly window title

  - do format like this: `[no JS|images|..] <title>  <url>`

      
