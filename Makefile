
TARGET	=	shop-timer
DEPS	=	main.py \
			main_frame.py \
			database.py \
			utility.py \
			action_table.py \
			action_dialogs.py \
			assembly_dialogs.py \
			assembly_table.py \
			dialogs.py \
			part_dialogs.py \
			part_table.py \
			tables.py \
			timer_dialogs.py \
			timer_table.py \
			timer_instance_table.py



all: $(TARGET)

$(TARGET): $(DEPS)
	pyinstaller -F -n $(TARGET) main.py

clean:
	rm -rf *.spec dist build __pycache__ *.db