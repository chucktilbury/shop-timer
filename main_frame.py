from tkinter import ttk
from tkinter import messagebox
from tkinter import filedialog
import tkinter
import sys
import os
import time

from database import Database
from utility import Logger, debugger, raise_event
from assembly_table import AssemblyTable
import utility
import actions_dialogs
import parts_dialogs
import timer_dialogs
import assembly_dialogs
import dialogs


class MainFrame(tkinter.Frame):
    '''
    This is the main frame that "contains" the other frames.
    '''

    def __init__(self, master=None):
        self.logger = Logger(self, Logger.DEBUG)
        self.logger.debug(sys._getframe().f_code.co_name)

        self.data = Database.get_instance()

        tkinter.Frame.__init__(self, master)
        self.master = master
        self.master.protocol("WM_DELETE_WINDOW", self.close_window)

        self.timer_running_flag = False

        tkinter.Label(self.master, text="Shop Timer", font=(
            "Helvetica", 14)).grid(row=0, column=0)

        self.assembly = AssemblyTable()

        self.upper_frame = tkinter.LabelFrame(self.master, text="Parameters", padx=55)
        self.upper_frame.grid(row=1, column=0)
        self.lower_frame = tkinter.LabelFrame(self.master, text="Timer", padx=35)
        self.lower_frame.grid(row=2, column=0)

        menu = tkinter.Menu(self.master, tearoff=0)
        self.master.config(menu=menu)

        fileMenu = tkinter.Menu(menu, tearoff=0)
        fileMenu.add_command(label="Load", command=self.loadCommand)
        fileMenu.add_command(label="Save", command=self.saveCommand)
        fileMenu.add_command(label="Save As", command=self.saveasCommand)
        #fileMenu.add_command(label="Export", command=self.exportCommand)
        fileMenu.add_separator()
        fileMenu.add_command(label="Quit", command=self.close_window)
        menu.add_cascade(label="File", menu=fileMenu)

        # settingsMenu = tkinter.Menu(menu, tearoff=0)
        # # , command=self.constCommand)
        # settingsMenu.add_command(label="Constants")
        # # , command=self.emboCommand)
        # settingsMenu.add_command(label="Embouchure")
        # settingsMenu.add_command(label="Notes")  # , command=self.notesCommand)
        # menu.add_cascade(label="Settings", menu=settingsMenu)

        assemblyMenu = tkinter.Menu(menu, tearoff=0)
        assemblyMenu.add_command(label="New", command=self.newAssembly)
        assemblyMenu.add_command(label="Edit", command=self.editAssembly)
        assemblyMenu.add_command(label="Select", command=self.selectAssembly)
        assemblyMenu.add_command(label="Delete", command=self.delAssembly)
        menu.add_cascade(label="Assemblies", menu=assemblyMenu)

        partMenu = tkinter.Menu(menu, tearoff=0)
        partMenu.add_command(label="New", command=self.newPart)
        partMenu.add_command(label="Edit", command=self.editPart)
        partMenu.add_command(label="Select", command=self.selectPart)
        partMenu.add_command(label="Delete", command=self.delPart)
        menu.add_cascade(label="Parts", menu=partMenu)

        actionMenu = tkinter.Menu(menu, tearoff=0)
        actionMenu.add_command(label="New", command=self.newAction)
        actionMenu.add_command(label="Edit", command=self.editAction)
        actionMenu.add_command(label="Select", command=self.selectAction)
        actionMenu.add_command(label="Delete", command=self.delAction)
        menu.add_cascade(label="Actions", menu=actionMenu)

        timerMenu = tkinter.Menu(menu, tearoff=0)
        timerMenu.add_command(label="New", command=self.newTimer)
        timerMenu.add_command(label="Edit", command=self.editTimer)
        timerMenu.add_command(label="Select", command=self.selectTimer)
        timerMenu.add_command(label="Delete", command=self.delTimer)
        menu.add_cascade(label="Timers", menu=timerMenu)

        editMenu = tkinter.Menu(menu, tearoff=0)
        editMenu.add_command(label="Help", command=self.helpCommand)
        editMenu.add_command(label="About", command=self.aboutCommand)
        editMenu.add_command(label="Dump", command=self.dumpInternalData)
        menu.add_cascade(label="Help", menu=editMenu)

        # Upper Frame
        tkinter.Label(self.upper_frame, text="Current Assembly:").grid(row=0, column=0, padx=4, pady=4, sticky=tkinter.E)
        self.assemblyString = tkinter.StringVar()
        tkinter.Label(self.upper_frame, textvariable=self.assemblyString).grid(row=0, column=1, columnspan=3)
        self.assemblyString.set("none selected")

        tkinter.Label(self.upper_frame, text="Current Part:").grid(row=1, column=0, padx=4, pady=4, sticky=tkinter.E)
        self.partString = tkinter.StringVar()
        tkinter.Label(self.upper_frame, textvariable=self.partString).grid(row=1, column=1, columnspan=3)
        self.partString.set("none selected")

        tkinter.Label(self.upper_frame, text="Current Action:").grid(row=2, column=0, padx=4, pady=4, sticky=tkinter.E)
        self.actionString = tkinter.StringVar()
        tkinter.Label(self.upper_frame, textvariable=self.actionString).grid(row=2, column=1, columnspan=3)
        self.actionString.set("none selected")

        tkinter.Label(self.upper_frame, text="Current Timer:").grid(row=3, column=0, padx=4, pady=4, sticky=tkinter.E)
        self.timerString = tkinter.StringVar()
        tkinter.Label(self.upper_frame, textvariable=self.timerString).grid(row=3, column=1, columnspan=3)
        self.timerString.set("none selected")



        # Lower Frame
        self.showTimerString = tkinter.StringVar()
        tkinter.Label(self.lower_frame, textvariable=self.showTimerString, width=8, fg='red', font=("Helvetica", 40)).grid(row=0, column=0, columnspan=3)
        self.showTimerString.set("00:00:00")

        self.startButton = tkinter.Button(self.lower_frame, text="Start", padx=20, command=self.startButtonCommand)
        self.startButton.grid(row=1, column=0)
        self.pauseButton = tkinter.Button(self.lower_frame, text="Pause", padx=20, command=self.pauseButtonCommand)
        self.pauseButton.grid(row=1, column=1)
        self.stopButton = tkinter.Button(self.lower_frame, text="Stop", padx=20, command=self.stopButtonCommand)
        self.stopButton.grid(row=1, column=2)


    @debugger
    def set_state(self):
        pass

    @debugger
    def get_state(self):
        pass

    @debugger
    def close_window(self):
        if self.timer_running_flag:
            messagebox.showinfo("Quit", "A timer is running.\nStopping the timer.")
            self.logger.debug('save')
            self.saveCommand()

        self.data.close()
        self.master.destroy()

    # Assembly commands
    @debugger
    def newAssembly(self):
        assembly_dialogs.NewAssemblyDialog(self.master)
        self.assembly.create('chuckie', 'description', 'Notes.')

    @debugger
    def editAssembly(self):
        assembly_dialogs.EditAssemblyDialog(self.master)

    @debugger
    def selectAssembly(self):
        assembly_dialogs.SelectAssemblyDialog(self.master)
        id = self.assembly.get_ids_by_name('chuckie')[0][0]
        self.logger.debug(self.assembly.get_all())
        self.assembly.update_by_id(id, description='hoopa loopa')
        self.logger.debug(self.assembly.get_all())
        # print(self.assembly.get_list())
        # self.assembly.update_by_id(1, name='a differet name')
        # print(self.assembly.get_list())
        # self.assembly.update_by_name('a differet name', description="bla bla BLART!")
        # print(self.assembly.get_list())
        # print(self.assembly.get_id_list())
        # print(self.assembly.get_name_list())

    @debugger
    def delAssembly(self):
        assembly_dialogs.DelAssemblyDialog(self.master)

    # Part commands
    @debugger
    def newPart(self):
        parts_dialogs.NewPartDialog(self.master)

    @debugger
    def editPart(self):
        parts_dialogs.EditPartDialog(self.master)

    @debugger
    def selectPart(self):
        parts_dialogs.SelectPartDialog(self.master)

    @debugger
    def delPart(self):
        parts_dialogs.DelPartDialog(self.master)

    # Action commands
    @debugger
    def newAction(self):
        actions_dialogs.NewActionDialog(self.master)

    @debugger
    def editAction(self):
        actions_dialogs.EditActionDialog(self.master)

    @debugger
    def selectAction(self):
        actions_dialogs.SelectActionDialog(self.master)

    @debugger
    def delAction(self):
        actions_dialogs.DelActionDialog(self.master)

    # Timer commands
    @debugger
    def newTimer(self):
        timer_dialogs.NewTimerDialog(self.master)

    @debugger
    def editTimer(self):
        timer_dialogs.EditTimerDialog(self.master)

    @debugger
    def selectTimer(self):
        timer_dialogs.SelectTimerDialog(self.master)

    @debugger
    def delTimer(self):
        timer_dialogs.DelTimerDialog(self.master)

    @debugger
    def startButtonCommand(self):
        pass

    @debugger
    def pauseButtonCommand(self):
        pass

    @debugger
    def stopButtonCommand(self):
        pass

    @debugger
    def loadCommand(self):
        pass

    @debugger
    def saveCommand(self):
        pass

    @debugger
    def saveasCommand(self):
        pass

    @debugger
    def aboutCommand(self):
        messagebox.showinfo(
            "About", "Tilbury Woodwinds Company\nShop Timer\nChuck Tilbury (c) 2019\nVersion: 1.0\nData Version: %s" % (self.data.get_version()))

    @debugger
    def dumpInternalData(self):
        pass

    @debugger
    def helpCommand(self):
        # messagebox.showinfo(
        #    "Help",
        dialogs.helpDialog(self.master)
