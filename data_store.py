import sqlite3
from tkinter import messagebox as mbox
from utility import Logger, debugger, register_event
import utility

class DataStore(object):

    __instance = None

    @staticmethod
    def get_instance():
        '''
        This static method is used to get the singleton object for this class.
        '''
        if DataStore.__instance == None:
            DataStore()
        return DataStore.__instance

    def __init__(self):

        # gate the accress to __init__()
        if DataStore.__instance != None:
            raise Exception("DataStore class is a singleton. Use get_instance() instead.")
        else:
            DataStore.__instance = self

        # Continue with init exactly once.
        self.logger = Logger(self, Logger.DEBUG)
        self.logger.debug("enter constructor")
        self.change_flag = False
        self.data_version = '1.0'

        self.logger.debug("leave constructor")

    @debugger
    def get_change_flag(self):
        return self.change_flag

    @debugger
    def set_change_flag(self):
        self.change_flag = True

    @debugger
    def clear_change_flag(self):
        self.change_flag = False

    @debugger
    def get_version(self):
        return self.data_version

    # Utilities
    @debugger
    def validate_type(self, var, t):
        '''
        Validate the type of the srguement. If it cannot be converted, then the program cannot continue.
        This is considered a developer error. The exceptions here only happen if the input validation
        from the GUI has failed.
        '''
        if type(var) != t:
            if t is float:
                try:
                    tmp = float(var)
                    return tmp
                except ValueError:
                    mbox.showerror("FATAL ERROR", "expected type %s but got type %s"%(str(t), str(type(var))))
                    self.logger.fatal("expected type %s but got type %s"%(str(t), str(type(var))))
                except:
                    mbox.showerror("FATAL ERROR", "float type error.")
                    self.logger.fatal("float type error.")

            elif t is int:
                try:
                    tmp = int(var)
                    return tmp
                except ValueError:
                    mbox.showerror("FATAL ERROR", "expected type %s but got type %s"%(str(t), str(type(var))))
                    self.logger.fatal("expected type %s but got type %s"%(str(t), str(type(var))))
                except:
                    mbox.showerror("FATAL ERROR", "int type error.")
                    self.logger.fatal("int type error.")

            elif t is bool:
                try:
                    tmp = bool(var)
                    return tmp
                except ValueError:
                    mbox.showerror("FATAL ERROR", "expected type %s but got type %s"%(str(t), str(type(var))))
                    self.logger.fatal("expected type %s but got type %s"%(str(t), str(type(var))))
                except:
                    mbox.showerror("FATAL ERROR", "bool type error.")
                    self.logger.fatal("bool type error.")

            elif t is str:
                # anything can be converted to a str()
                return str(var)
            else:
                mbox.showerror("FATAL ERROR", "attempt to validate an unexpected type %s as type %s."%(str(type(var)), str(t)))
                self.logger.fatal("attempt to validate an unexpected type %s as type %s."%(str(type(var)), str(t)))
        else:
            return var

