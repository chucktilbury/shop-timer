# shop-timer
## General
The purpose of this application is to track how much time it actually takes
to make parts in the shop. The concept of a timer is designed to facilitate
tracking of specific actions on a specific number of parts. To find how much
time it takes to make a part, all of the times for actions are added up and
then divided by the number of parts. For example, to find how long it takes
to make a whistle. The whistle consists of a fipple, which also has a plug.
A whistle also has a body. So all of the actions are added up and output.

The number of parts does not need to be the same for an assembly. The time
for the action is the total timer divided by the number of units that was
made.

Items are arranged in a hirarchy.

At the top level, is an assembly. An assmbly can have other assemblies or
parts connected to it. Assemblies cannot have actions. They exist to make it
easier to find out how much time it takes.

Next in the hirarchy is parts. A part is a physical object that is created by
taking actions on materials. A part must be connected to an assembly and may
not have other parts under it. A part must have at lease one action connected
to it and there is no limit on the number. Similar parts must not be grouped
together. This will facilitate queries and make the measurements more accurate.

Next are actions. An action is a single operation that is applied to a part.
For example, an action might be "cut to length", or "turn to diameter". The
idea is to make each action as small as possible to make measurement of the
time that it takes as accurate as possible. An action must have a timer
connected to it.

Next are timers. A timer is a way to connect multiple timer instances to an
action. A timer must be associated with an action.

Next are timer instances. A timer instance is an actual measurement of time.
There can be any number of timer instances connected to a timer. When a timer
is started, a new instance is created automatically. A timer instance can be
paused and restarted at will. When the instance is stopped, it is closed out
and may not be restarted or reused. Timer instances are only accessible
through the query interface.

## Class Attributes
### Assembly Attributes
* ID  
   Object ID provided by the database.
* Date Created  
   Date on which this record was created.
* Date Modified  
  Date and time of the last modification of the assembly
* Name  
  Display name of the assembly. This is the name that is used in the selection dialogs.
* Descrition  
  This is a free-form description of the assembly. It is displayed as a pop-up when the mouse hovers over the item.
* Notes  
  These are notes that describe things line the purpose of the assembly and/or how to assemble it. Pricing information or whatever is required.
  
### Part Attributes
* ID  
   Object ID provided by the database.
* Date Created  
   Date on which this record was created.
* Date Modified  
  Date and time of the last modification of the part
* Name  
  Display name of the part. This is the name that is used in the selection dialogs.
* Descrition  
  This is a free-form description of the part. It is displayed as a pop-up when the mouse hovers over the item.
* Notes  
  These are notes that describe things line the purpose of the part and any information that may be required.
* Assembly ID  
  The ID of the assembly that this is a part of. Note that the assembly ID and the part ID cannot have values at the same time, but one or the other must have a value.
* Part ID  
  The ID of the part that is is a part of. Note that the assembly ID and the part ID cannot have values at the same time, but one or the other must have a value.

### Action Attributes
* ID  
  Object ID provided by the database.
* Date Created  
  Date on which this record was created.
* Date Modified  
  Date and time of the last modification of the action
* Name  
  Display name of the action. This is the name that is used in the selection dialogs.
* Descrition  
  This is a free-form description of the action. It is displayed as a pop-up when the mouse hovers over the item.
* Notes  
  These are notes that describe things line the purpose of the action and/or how to perform it, what materials are needed, and the like.
* Part ID    
  The ID of the part that is is a part of. This field must have a value.

### Timer Attributes
* ID  
  Object ID provided by the database.
* Date Created  
  Date on which this record was created.
* Date Modified  
  Date and time of the last modification of the timer
* Name  
  Display name of the timer. This is the name that is used in the selection dialogs.
* Descrition  
  This is a free-form description of the timer. It is displayed as a pop-up when the mouse hovers over the item.
* Notes  
  These are notes that describe things line the purpose of the action and/or how to perform it, what materials are needed, and the like.
* Part ID    
  The ID of the part that is is a part of. This field must have a value.

### Timer Instance Attributes
* ID  
  Object ID provided by the database.
* Start Date and time
  Date on which this record was created.
* End Date and time
  Date and time of when this timer was stopped. (note that pauses are not tracked.
* Name  
  Display name of the instance. This will be the name of the timer that is connected to it with a serial number appended to it.
* Timer ID    
  The ID of the part that is is a part of. This field must have a value.


## Other information
Selection comboboxes work such that items above them in the hirarchy are selected as well as the item.

Selection combo boxes work such that, if a part is selected, only actions
that pertain to that part appear in the selection box. If there is no
current part, but there is a current assembly, then actions that qualify
include all of the parts in that assembly. When an action is selected,
the part is selected as well. If there is no selection in the hirarchy, then
all of the actions are presented.

