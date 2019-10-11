
TARGET	=	shop-timer
DEPS	=	main.py \
			main_frame.py \
			data_store.py \
			utility.py


all: $(TARGET)

$(TARGET): $(DEPS)
	pyinstaller -F -n $(TARGET) main.py

clean:
	rm -rf *.spec dist build __pycache__ *.db