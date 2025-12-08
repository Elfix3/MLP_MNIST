CXX = g++
SRC_MAIN = main.cpp
SRC_DIR  = sources
OBJ_DIR  = obj
TARGET = program


CXXFLAGS = -Wall -std=c++11 -Iincludes -Iexternals 
LDFLAGS  = -L$(PYTHON_LIB) -lpython314

SOURCES  = $(SRC_MAIN) $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SOURCES)))

all: $(TARGET)

# Edition de liens
$(TARGET): $(OBJECTS)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation automatique de chaque .cpp vers obj/
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
