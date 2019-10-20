from tkinter import messagebox as mbox
import tkinter
from utility import Logger
#from data_store import DataStore
from dialogs import BaseDialog

class NewAssemblyDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("New Assembly")

class EditAssemblyDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Edit Assembly")

class SelectAssemblyDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Select Assembly")

class DelAssemblyDialog(BaseDialog):
    '''
    '''

    def body(self, master):
        self.title("Delete Assembly")

