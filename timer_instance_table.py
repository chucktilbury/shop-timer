import time
from utility import debugger, Logger
from tables import Tables

class TimerInstanceTable(Tables):

    def __init__(self):
        super().__init__('timer_instance')
        self.logger.debug(self.get_column_names())
        self.logger.debug(self.get_column_types())

    @debugger
    def create(self, name, timer_id):
        ''' Create a new record '''
        self.insert(['name', 'start', 'end', 'total', 'timer_ID'],
                        [name, 0.0, 0.0, 0.0, timer_id])

    @debugger
    def update_by_id(self, id, name=None, start=None, end=None, total=None, timer_id=None):
        ''' Modify one or more fields, given the ID '''
        changed = False
        if not name is None:
            self.update('name', name, 'ID = %d'%(id))
            changed = True

        if not start is None:
            self.update('start', start, 'ID=%d'%(id))
            changed = True

        if not end is None:
            self.update('end', end, 'ID=%d'%(id))
            changed = True

        if not total is None:
            self.update('total', total, 'ID=%d'%(id))
            changed = True

        if not timer_id is None:
            self.update('timer_id', timer_id, 'ID=%d'%(id))
            changed = True

        if changed:
            self.update('modified', time.time(), 'ID=%d'%(id))

    @debugger
    def update_by_name(self, name, start=None, end=None, total=None, timer_id=None):

        changed = False
        if not start is None:
            self.update('start', start, 'name=\'%s\''%(name))
            changed = True

        if not end is None:
            self.update('end', end, 'name=\'%s\''%(name))
            changed = True

        if not total is None:
            self.update('total', total, 'name=\'%s\''%(name))
            changed = True

        if not timer_id is None:
            self.update('timer_id', timer_id, 'name=\'%s\''%(name))
            changed = True

        if changed:
            self.update('modified', time.time(), 'name=\'%s\''%(name))

