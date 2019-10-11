from tkinter import messagebox as mbox
from tkinter import ttk
import tkinter
import math
from utility import Logger, debugger
import utility
from data_store import DataStore

help_text = """
Shop Timer
Chuck Tilbury (c) 2019

This software is open source under the MIT and BSD licenses.

-------------------------------------------
General use.
-------------------------------------------

-------------------------------------------
Saving a file
-------------------------------------------

-------------------------------------------
Loading a file
-------------------------------------------

-------------------------------------------
Reset to default settings
-------------------------------------------

"""

# see: https://effbot.org/tkinterbook/tkinter-dialog-windows.htm
class BaseDialog(tkinter.Toplevel):
    '''
    This class provides common services to simple data dialogs.
    '''

    def __init__(self, parent):# , title = None):

        #init the logger
        self.logger = Logger(self, level=Logger.INFO)
        self.logger.debug("start constructor")

        tkinter.Toplevel.__init__(self, parent)
        self.transient(parent)

        self.parent = parent

        self.result = None
        # get a copy of the data_store for the children
        self.data_store = DataStore.get_instance()

        body = tkinter.Frame(self)
        self.initial_focus = self.body(body)
        body.grid(padx=5, pady=5)

        self.buttonbox()

        self.grab_set()

        if not self.initial_focus:
            self.initial_focus = self

        self.protocol("WM_DELETE_WINDOW", self.cancel)

        self.initial_focus.focus_set()

        self.wait_window(self)
        self.logger.debug("leave constructor")

    #
    # construction hooks
    def body(self, master):
        # create dialog body.  return widget that should have
        # initial focus.  this method should be overridden
        return self

    def buttonbox(self):
        # add standard button box. override if you don't want the
        # standard buttons
        box = tkinter.Frame(self)

        w = tkinter.Button(box, text="OK", width=10, command=self.ok, default=tkinter.ACTIVE)
        w.pack(side=tkinter.LEFT, padx=5, pady=5)
        w = tkinter.Button(box, text="Cancel", width=10, command=self.cancel)
        w.pack(side=tkinter.LEFT, padx=5, pady=5)

        box.grid()

    #
    # standard button semantics
    @debugger
    def ok(self, event=None):
        if not self.validate():
            self.initial_focus.focus_set() # put focus back
            return

        self.withdraw()
        self.update_idletasks()
        self.apply()
        self.cancel()

    @debugger
    def cancel(self, event=None):
        # put focus back to the parent window
        self.parent.focus_set()
        self.destroy()

    #
    # command hooks
    def validate(self):
        return True # override

    def apply(self):
        pass # override


###############################################################################
# Does not use BaseDialog
class helpDialog:

    def __init__(self, parent):
        self.logger = Logger(self, level=Logger.DEBUG)
        self.logger.debug("enter constructer")

        self.top = tkinter.Toplevel(parent)
        self.tx = tkinter.Text(self.top, height=25, width=80)
        self.sb = tkinter.Scrollbar(self.top)
        self.sb.pack(side=tkinter.RIGHT,fill=tkinter.Y)
        self.tx.pack(side=tkinter.LEFT)
        self.sb.config(command=self.tx.yview)
        self.tx.config(yscrollcommand=self.sb.set)
        self.tx.insert(tkinter.END, help_text)
        self.tx.config(state='disabled')

        self.logger.debug("leave constructer")

###############################################################################
class TestDialog(BaseDialog):
    '''
    This implements a minimum dialog using the Base Dialog class.
    '''

    @debugger
    def body(self, master):

        tkinter.Label(master, text="First:").grid(row=0)
        tkinter.Label(master, text="Second:").grid(row=1)

        self.e1 = tkinter.Entry(master)
        self.e2 = tkinter.Entry(master)

        self.e1.grid(row=0, column=1)
        self.e2.grid(row=1, column=1)
        return self.e1 # initial focus

    @debugger
    def validate(self):
        try:
            self.first = int(self.e1.get())
            self.second = int(self.e2.get())
        except ValueError as e:
            self.logger.error("Cannot convert values to ints: (%s, %s)"%(self.e1.get(), self.e2.get()))
            mbox.showerror("ERROR", "Cannot convert value to an int\n(%s, %s)"%(self.e1.get(), self.e2.get()))
            return False
        except Exception as e:
            self.logger.error("Unexpected exception while validating dialog: %s"%(str(e)))
            mbox.showerror("UNKNOWN ERROR", "Cannot convert value to an int\n(%s, %s)"%(self.e1.get(), self.e2.get()))
            return False
        return True


    @debugger
    def apply(self):
        print(self.first, self.second) # or something

###############################################################################
class NotesDialog(BaseDialog):
    '''
    Capture and store arbitrary notes.
    '''

    @debugger
    def body(self, master):
        self.title('Notes')
        self.tx = tkinter.Text(master, height=25, width=80)
        self.sb = tkinter.Scrollbar(master)
        self.sb.pack(side=tkinter.RIGHT,fill=tkinter.Y)
        self.tx.pack(side=tkinter.LEFT)
        self.sb.config(command=self.tx.yview)
        self.tx.config(yscrollcommand=self.sb.set)
        #self.notes = self.data_store.get_notes()
        self.tx.insert(tkinter.END, self.notes)

    @debugger
    def validate(self):
        self.notes = self.tx.get('1.0', tkinter.END)
        return True

    @debugger
    def apply(self):
        #self.data_store.set_notes(self.notes)
        pass
