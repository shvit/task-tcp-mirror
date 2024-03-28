APP:=ttm

DIR_SRC:=src
DIR_BIN:=bin

CFLAGS += -Wall -fPIC -std=c++17 -pthread -MMD -g -O0
LDFLAGS += -lstdc++ -lpthread -levent_core

OBJ=$(patsubst $(DIR_SRC)/%.cpp,$(DIR_BIN)/%.o,$(wildcard $(DIR_SRC)/*.cpp))
DEPS:=$(OBJ:.o=.d) 

all: $(DIR_BIN)/$(APP)

-include $(DEPS)


$(DIR_BIN)/$(APP): $(OBJ)
	@echo "Linking $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

$(DIR_BIN)/%.o: $(DIR_SRC)/%.cpp | dir_bin
	@echo "Compile $@"
	@$(CXX) -c $(CFLAGS) $< -o $@

show:
	@echo "OBJ=$(OBJ)"
	@echo "DEPS=$(DEPS)"

dir_bin:
	@mkdir -p $(DIR_BIN)

clean:
	@rm -rf $(DIR_BIN)

.PHONY: all show clean
