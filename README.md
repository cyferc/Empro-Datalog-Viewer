# Empro Datalog Viewer
I started this project to quickly view .csv data logs that I saved from my home made ECU. It is a highly efficient data series renderer, implementing binary search (worst case O(log n)) to find the axis limits within the data series. O(n) to render everything between the axis limits.

## Latest Release Binary:
| Platform | Binary |
|---|---|
| Win32 | [Empro Datalog Viewer v0.2.0.zip](https://github.com/cyferc/Empro-Datalog-Viewer/tree/master/Release/Win32/EmproDatalogViewer_0.2.0.zip) |
| Linux | open with QtCreator and compile, no other dependencies. Tested with Qt5.8.0 |

## Supported Formats
- .csv
- .msl (standard megasquirt format)

(see [here](https://github.com/cyferc/Empro-Datalog-Viewer/tree/master/docs/sampleDatalogs) for sample data logs):

## Screenshots
![screenshot 1](https://raw.githubusercontent.com/cyferc/Empro-Datalog-Viewer/master/docs/screenshots/1.png)

![screenshot 2](https://raw.githubusercontent.com/cyferc/Empro-Datalog-Viewer/master/docs/screenshots/2.png)

### Todo:
- add "do not show again" option to error dialog when opening file
- render flat line data series
- y-axis tick labels
- semi-transparent background for text in plot
- right click; single selection / multi selection
- snap to points / interpolate
- use megasquirt .msl units from log file
- play/pause for realtime playback
- show current value in channel list
- show cursor coordinates, not snapped to line
- implement screenshot button
- make zoom easier to use
- overview view, make scrolling easier
