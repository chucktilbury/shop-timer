import time
from utility import debugger, Logger
from tables import Tables

class AssemblyTable(Tables):

    def __init__(self):
        super().__init__('assembly')
        self.logger.debug(self.get_column_names())
        self.logger.debug(self.get_column_types())

    @debugger
    def create(self, name, number, description, notes):
        ''' Create a new record '''
        self.insert(['name', 'number', 'description', 'notes', 'created', 'modified'],
                        [name, number, description, notes, time.time(), time.time()])

    @debugger
    def update_by_id(self, id, name=None, number=None, description=None, notes=None):
        ''' Modify one or more fields, given the ID '''
        changed = False
        if not name is None:
            self.update('name', name, 'ID = %d'%(id))
            changed = True

        if not description is None:
            self.update('description', description, 'ID=%d'%(id))
            changed = True

        if not notes is None:
            self.update('notes', notes, 'ID=%d'%(id))
            changed = True

        if not number is None:
            self.update('number', number, 'ID=%d'%(id))
            changed = True

        if changed:
            self.update('modified', time.time(), 'ID=%d'%(id))

    @debugger
    def update_by_name(self, name, number=None, description=None, notes=None):

        changed = False
        if not number is None:
            self.update('number', number, 'name=\'%s\''%(name))
            changed = True

        if not description is None:
            self.update('description', description, 'name=\'%s\''%(name))
            changed = True

        if not notes is None:
            self.update('notes', notes, 'name=\'%s\''%(name))
            changed = True

        if changed:
            self.update('modified', time.time(), 'name=\'%s\''%(name))

