# Console Text Re-Color (CTRC)
Console text re-coloring

## Purpose
Working in Windows console is made easier with some coloring. CTRC scans its standard input and sends it back to its standard aumented with coloring according to detected tags.

Typically CTRC should be piped with another program or command for which we need coloring.

## Syntax
Basic command line

`crtc -c=COLOR~PATTERN`

Where COLOR is one of the available color names and PATTERN is the text to detect, e,g. :

`ctrc -c=red~error -c=yellow~warning`

Available colors (d_ stands for dark) :
black, d_red, d_green, d_yellow, d_blue, d_magenta, d_cyan, d_gray, gray, red, green, yellow, blue, magenta, cyan, white.

### Pause (-p)

To enable easy debugger attachment, CRTC can be paused at start using '-p' argument, e.g. :

`ctrc -c=red~error -c=yellow~warning -p`

### Verbose (-v)

Log more information upon errors and successes using '-v' argument, e.g. :

`ctrc -c=red~error -c=yellow~warning -v`

Informations are outputted to standard output (std::out).
Errors are outputted to stand error (std::err).

### Default color (-d)


### Separtor (-s)

## Future enhancements
- Enable only partial line coloring
- Enable use of regular expressions
- Enable background colors
