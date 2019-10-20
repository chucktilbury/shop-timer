import time
from utility import debugger, Logger
from tables import Tables

class TimerTable(Tables):

    def __init__(self):
        super().__init__('timer')
        self.logger.debug(self.get_column_names())
        self.logger.debug(self.get_column_types())

    @debugger
    def create(self, name, description, notes, action_id):
        ''' Create a new record '''
        self.insert(['name', 'description', 'notes', 'created', 'modified', 'action_ID'],
                        [name, description, notes, time.time(), time.time(), action_id])

    @debugger
    def update_by_id(self, id, name=None, description=None, notes=None, action_id=None):
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

        if not action_id is None:
            self.update('action_id', action_id, 'ID=%d'%(id))
            changed = True

        if changed:
            self.update('modified', time.time(), 'ID=%d'%(id))

    @debugger
    def update_by_name(self, name, description=None, notes=None, action_id=None):

        changed = False
        if not description is None:
            self.update('description', description, 'name=\'%s\''%(name))
            changed = True

        if not notes is None:
            self.update('notes', notes, 'name=\'%s\''%(name))
            changed = True

        if not action_id is None:
            self.update('action_id', action_id, 'name=\'%s\''%(name))
            changed = True

        if changed:
            self.update('modified', time.time(), 'name=\'%s\''%(name))

    @debugger
    def get_action_id_by_name(self, name):
        '''
        Return a list of part IDs that match the name.
        '''
        return self.select('action_id', 'name=\'%s\''%(name))

    @debugger
    def get_action_id_by_id(self, id):
        '''
        Return the part ID as an int.
        '''
        curs = self.select('action_id', 'id=\'%d\''%(id))
        return curs[0][0]

