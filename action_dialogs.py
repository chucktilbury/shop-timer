from tkinter import messagebox as mbox
import tkinter
from utility import Logger
#from data_store import DataStore
from dialogs import BaseDialog


# Enter Action name
# Description
# notes
# After action is created, it is selected along with the part and assembly
class NewActionDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("New Action")

# Select action to edit
# Select which part the action applies to
# Edit description
# Edit notes
# After action is edited, the action, part, and assembly are selected
class EditActionDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Edit Action")

# Select action, part and assembly
class SelectActionDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Select Action")

# Select action to delete
# Confirm delete
class DelActionDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Delete Action")

