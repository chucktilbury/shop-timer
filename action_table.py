import time
from utility import debugger, Logger
from tables import Tables

class ActionTable(Tables):

    def __init__(self):
        super().__init__('action')
        self.logger.debug(self.get_column_names())
        self.logger.debug(self.get_column_types())

    @debugger
    def create(self, name, order, description, notes, part_id):
        ''' Create a new record '''
        self.insert(['name', 'order', 'description', 'notes', 'created', 'modified', 'part_ID'],
                        [name, order, description, notes, time.time(), time.time(), part_id])

    @debugger
    def update_by_id(self, id, name=None, order=None, description=None, notes=None, part_id=None):
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

        if not part_id is None:
            self.update('part_id', part_id, 'ID=%d'%(id))
            changed = True

        if not order is None:
            self.update('order', order, 'ID=%d'%(id))
            changed = True

        if changed:
            self.update('modified', time.time(), 'ID=%d'%(id))

    @debugger
    def update_by_name(self, name, order=None, description=None, notes=None, part_id=None):

        changed = False
        if not description is None:
            self.update('description', description, 'name=\'%s\''%(name))
            changed = True

        if not notes is None:
            self.update('notes', notes, 'name=\'%s\''%(name))
            changed = True

        if not part_id is None:
            self.update('part_id', part_id, 'name=\'%s\''%(name))
            changed = True

        if not order is None:
            self.update('order', order, 'name=\'%s\''%(name))
            changed = True

        if changed:
            self.update('modified', time.time(), 'name=\'%s\''%(name))

    @debugger
    def get_part_id_by_name(self, name):
        '''
        Return a list of part IDs that match the name.
        '''
        return self.select('part_id', 'name=\'%s\''%(name))

    @debugger
    def get_part_id_by_id(self, id):
        '''
        Return the part ID as an int.
        '''
        curs = self.select('part_id', 'id=\'%d\''%(id))
        return curs[0][0]

    @debugger
    def get_order_id_by_name(self, name):
        '''
        Return a list of part IDs that match the name.
        '''
        return self.select('order', 'name=\'%s\''%(name))

    @debugger
    def get_order_id_by_id(self, id):
        '''
        Return the part ID as an int.
        '''
        curs = self.select('order', 'id=\'%d\''%(id))
        return curs[0][0]
