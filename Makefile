SRC_DIR = ./src
BIN_OUT = ./bin/cmail
DEBUG_OUT = ./bin/cmail_debug

.PHONY: bin
bin:
	gcc $(SRC_DIR)/cmail.c $(SRC_DIR)/email.c -lcurl -o $(BIN_OUT)  

.PHONY: debug
debug:
	gcc -g $(SRC_DIR)/cmail.c $(SRC_DIR)/email.c -lcurl -o $(DEBUG_OUT)
	gdb $(DEBUG_OUT)

.PHONY: test
test:
	gcc $(SRC_DIR)/cmail.c $(SRC_DIR)/email.c -lcurl -o $(BIN_OUT)  
