# default: dirs openblas suitesparse stb fast glfw
# .PHONY: dirs openblas suitesparse stb fast glfw

default: dirs install
.PHONY: dirs install

clean:
	@rm -rf bin
	@rm -rf include
	@rm -rf lib
	@rm -rf log
	@rm -rf share
	@rm -rf src

dirs:
	@mkdir -p bin
	@mkdir -p include
	@mkdir -p lib
	@mkdir -p log
	@mkdir -p share
	@mkdir -p src

install: dirs
	@sudo bash install.bash
