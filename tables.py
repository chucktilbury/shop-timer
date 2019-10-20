import time
from utility import debugger, Logger
from database import Database

class Tables(object):

    def __init__(self, name):
        self.name = name
        self.logger = Logger(self, Logger.DEBUG)
        self.logger.debug("enter constructor")
        self.db = Database.get_instance()

        self.col_names = []
        self.col_types = []
        cols = self.db.get_columns(self.name)
        for item in cols:
            self.col_names.append(item[1])
            if item[2] == 'INTEGER':
                self.col_types.append(int)
            elif item[2] == 'REAL':
                self.col_types.append(float)
            elif item[2] == 'TEXT':
                self.col_types.append(str)
            else:
                self.col_types.append(str)
        self.logger.debug("leave constructor")

    @debugger
    def get_column_names(self):
        return self.col_names

    @debugger
    def get_column_types(self):
        return self.col_types

    @debugger
    def _convert_cursor(self, cursor):
        '''
        Take the cursor and convert it to a list of lists where the internal
        lists are the values. i.e. [[v1, v2], [v1, v2]]
        '''
        retv = []
        for item in cursor:
            val = []
            for thing in item:
                val.append(thing)
            retv.append(val)

        return retv

    @debugger
    def get_all(self):
        '''
        Return all of the rows in the table
        '''
        return self._convert_cursor(self.db._select(self.name))

    @debugger
    def get_id_list(self, where=None):
        '''
        Return all of the IDs in the table
        [id1, id2, ...]
        '''
        cur = self.db._select(self.name, 'ID', where)
        retv = []
        for item in cur:
            retv.append(item[0])
        return retv

    @debugger
    def get_name_list(self, where=None):
        '''
        Return all of the names in the table
        [name1, name2, ...]
        '''
        cur = self.db._select(self.name, 'name', where)
        retv = []
        for item in cur:
            retv.append(item[0])
        return retv

    # NOTE that I will add code to make names unique within a table.
    @debugger
    def get_all_by_name(self, name):
        '''
        Return the row(s) specified by the name
        [[ID, name, c2, ...], [ID, name, c2, ...], ...]
        '''
        return self._convert_cursor(self.db._select(self.name, '*', 'name=\'%s\''%(name)))

    @debugger
    def get_ids_by_name(self, name):
        '''
        Return the ID(s), given the name
        [id1, id2, ...]
        '''
        cur = self._convert_cursor(self.db._select(self.name, 'ID', 'name=\'%s\''%(name)))
        retv = []
        for item in cur:
            retv.append(item[0])
        return retv

    @debugger
    def insert(self, fields, values):
        self.db._insert(self.name, fields, values)

    @debugger
    def select(self, fields=None, where=None):
        return self.db._select(self.name, fields, where)

    @debugger
    def update(self, field, value, where):
        self.db._update(self.name, field, value, where)

    @debugger
    def delete(self, id):
        self.db._delete(self.name, id)

