from tkinter import messagebox as mbox
import tkinter
from utility import Logger
from data_store import DataStore
from dialogs import BaseDialog

class NewTimerDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("New Timer")

class EditTimerDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Edit Timer")

class SelectTimerDialog(BaseDialog):
    '''
    '''
    def body(self, master):
        self.title("Select Timer")

class DelTimerDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Delete Timer")