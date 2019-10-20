import time
from utility import debugger, Logger
from tables import Tables

class PartTable(Tables):

    def __init__(self):
        super().__init__('part')
        self.logger.debug(self.get_column_names())
        self.logger.debug(self.get_column_types())

    @debugger
    def create(self, name, number, description, notes, assembly_id):
        ''' Create a new record '''
        self.insert(['name', 'number', 'description', 'notes', 'created', 'modified', 'assembly_ID'],
                        [name, number, description, notes, time.time(), time.time(), assembly_id])

    @debugger
    def update_by_id(self, id, name=None, number=None, description=None, notes=None, assembly_id=None):
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

        if not assembly_id is None:
            self.update('assembly_id', assembly_id, 'ID=%d'%(id))
            changed = True

        if changed:
            self.update('modified', time.time(), 'ID=%d'%(id))

    @debugger
    def update_by_name(self, name, number=None, description=None, notes=None, assembly_id=None):

        changed = False
        if not description is None:
            self.update('description', description, 'name=\'%s\''%(name))
            changed = True

        if not notes is None:
            self.update('notes', notes, 'name=\'%s\''%(name))
            changed = True

        if not number is None:
            self.update('number', number, 'name=\'%s\''%(name))
            changed = True

        if not assembly_id is None:
            self.update('assembly_id', assembly_id, 'name=\'%s\''%(name))
            changed = True

        if changed:
            self.update('modified', time.time(), 'name=\'%s\''%(name))

    @debugger
    def get_assembly_id_by_name(self, name):
        '''
        Return a list of part IDs that match the name.
        '''
        return self.select('assembly_id', 'name=\'%s\''%(name))

    @debugger
    def get_assembly_id_by_id(self, id):
        '''
        Return the part ID as an int.
        '''
        curs = self.select('assembly_id', 'id=\'%d\''%(id))
        return curs[0][0]

    @debugger
    def get_part_number_by_name(self, name):
        '''
        Return a list of part IDs that match the name.
        '''
        return self.select('number', 'name=\'%s\''%(name))

    @debugger
    def get_part_number_by_id(self, id):
        '''
        Return the part ID as an int.
        '''
        curs = self.select('number', 'id=\'%d\''%(id))
        return curs[0][0]
