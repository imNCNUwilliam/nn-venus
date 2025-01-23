
TARGET := encoder

all: 
	$(CC) -o $(TARGET) $(TARGET).c

clean:
	$(RM) $(TARGET)
	$(RM) input
	$(RM) L1_weights
	$(RM) L2_weights
	$(RM) L3_weights
	$(RM) L4_weights

PRINTF = env printf
PASS_COLOR = \e[32;01m
NO_COLOR = \e[0m
pass = $(PRINTF) "$(PASS_COLOR)$1 Passed [-]$(NO_COLOR)\n"

check: all
	make clean || true > /dev/null
	make
#	$(RM) out || true > /dev/null
	./$(TARGET)

