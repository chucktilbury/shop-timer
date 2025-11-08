# Scripts
This directory contains the management scripts for the project.

## scripts run while not in the shell

### start
Run this script from the project root directory such as ``./scripts/setup`` to create a sub-shell with git macros and some new path items in it. To exit the shell, just type ``exit``.

This script accepts the words ``debug``, ``release``, and ``profile`` on the command like to control the type of the build. The default is ``debug``. The compiler can be switched out by editing the last line of ``./project_env`` and commenting out the one one you don't want and commenting in the one that you do want. 

### clean
This script deletes all files and directories that were created as a result of starting and/or building the project. Run such as ``./scripts/clean``.

## scripts that can be run inside the shell.
These scripts check to see if the shell is operational and publish an error if the shell is not active.

### build
Build the project as quickly as possible. 

### rebuild
Make clean and then build the project as quickly as possible.

### showdocs
Starts firefox and loads the Doxygen documentation. 

## macros
These macros are implemented as aliases or actual shell functions in the file ``./project_env``. 

### git macros

#### add
Adds all changes

#### commit
Commits all changes with text editor

#### push
Push all changes

#### status
Git ``status`` command

#### all
Runs ``add``, ``commit``, and ``push``.

#### dbg
Runs ``kdbg <path-to-exe>``. Requires that kdbg from the KDE repository be present. 
