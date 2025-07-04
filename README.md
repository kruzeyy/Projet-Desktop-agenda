# Agenda (Qt Example)

This is a simple desktop agenda application built with Qt Widgets.

It allows you to select dates from a calendar, add events for each day, and
stores them in a JSON file (`agenda_events.json`) in your home directory.

The project uses qmake and can be opened directly in Qt Creator.

## Building

1. Open `Agenda.pro` with Qt Creator.
2. Configure the project with your installed Qt kit.
3. Build and run from Qt Creator.

Events will be saved automatically when you close the application. If you share
the JSON file between machines, you can emulate a basic shared agenda.
