
CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall

# Source directories to search for .cpp files
SOURCEDIRS := dynamic-programing brute-force greedy
SOURCES := $(foreach d,$(SOURCEDIRS),$(wildcard $(d)/*.cpp))

# Binaries are source paths without .cpp suffix
BINS := $(patsubst %.cpp,%,$(SOURCES))

.PHONY: all clean run-all run-%

all: $(BINS)

# Generic rule: build executable from corresponding .cpp
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	@rm -f $(BINS)

run-all: all
	@for b in $(BINS); do \
		echo '---- Running' $$b '----'; \
		./$$b || true; \
	done

# Run a specific binary by name, e.g. `make run-cloud_computing`
run-%: all
	@./$*

csv: all
	@python3 generate_metrics_csv.py
