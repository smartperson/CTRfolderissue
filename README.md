# Issue Repro

Environment:

* N3DS with 11.2
* unzipped JKSV2 folder placed in root of SD card
* App creates a thread that crawls through a specified directory and prints out folder names and file info. App then creates a new thread to do the same thing.

Steps:

1. Build and deploy 3dsx to 3DS via 3dslink
2. Press A, observe output of folders and file sizes in /JKSV2
3. Press A again to repeat.

Expected:

* Same output will be observed as the first time A is pressed.

Actual:

* Opening one of the folders hangs.

Observations:

* Based on console output, the hang occurs on line 34 of `remote_files.c`.
* Running the print cycle twice during the same thread's execution does not cause any issues. It is only a problem when the first thread is finished and a new thread is created.
