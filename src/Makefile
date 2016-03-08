SER_DIR=server
CLT_DIR=client
COMM_DIR=comm
SER_OBJ=$(SER_DIR)
CLT_OBJ=$(CLT_DIR)
INSTALL_DIR=../install
DICTIONARY=dictionary.txt

export SER_OBJ CLT_OBJ
.PHONY:all clean install uninstall

all:
	-make -C $(COMM_DIR)
	-make -C $(SER_DIR)
	-make -C $(CLT_DIR)
clean:
	-cd ./$(SER_DIR) && $(MAKE) clean
	-cd ./$(CLT_DIR) && $(MAKE) clean
	-cd ./$(COMM_DIR) && $(MAKE) clean
	-make uninstall
install:
	-mkdir $(INSTALL_DIR)/bin -p
	-cp ./$(SER_DIR)/$(SER_OBJ) $(INSTALL_DIR)/bin
	-cp ./$(CLT_DIR)/$(CLT_OBJ) $(INSTALL_DIR)/bin
	-cp $(INSTALL_DIR)/../doc/$(DICTIONARY) $(INSTALL_DIR)/bin/$(DICTIONARY)
uninstall:
	rm -rf $(INSTALL_DIR)

