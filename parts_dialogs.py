from tkinter import messagebox as mbox
import tkinter
from utility import Logger
from data_store import DataStore
from dialogs import BaseDialog

class NewPartDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("New Part")

class EditPartDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Edit Part")

class SelectPartDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Select Part")

class DelPartDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Delete Part")

