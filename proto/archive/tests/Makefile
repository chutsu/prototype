include ../config.mk

LIBPROTO = $(BLD_DIR)/libproto.a

default: test_data
# default: test_data $(BLD_DIR)/test_se
# default: test_data $(BLD_DIR)/test_euroc
# default: test_data $(BLD_DIR)/test_frontend
.PHONY: test_data

test_data:
	@rm -rf $(BLD_DIR)/test_data
	@cp -rf ./test_data $(BLD_DIR)/test_data

$(BLD_DIR)/test_atl: test_atl.cpp $(LIBPROTO)
	$(MAKE_TEST)

$(BLD_DIR)/test_calib: test_calib.cpp $(LIBPROTO)
	$(MAKE_TEST)

$(BLD_DIR)/test_core: test_core.cpp $(LIBPROTO)
	$(MAKE_TEST)

$(BLD_DIR)/test_dense: test_dense.cpp $(LIBPROTO)
	$(MAKE_TEST)

$(BLD_DIR)/test_euroc: test_euroc.cpp $(LIBPROTO)
	$(MAKE_TEST)

$(BLD_DIR)/test_se: test_se.cpp $(LIBPROTO)
	$(MAKE_TEST)

$(BLD_DIR)/test_frontend: test_frontend.cpp $(LIBPROTO)
	$(MAKE_TEST)

$(BLD_DIR)/test_kitti: test_kitti.cpp $(LIBPROTO)
	$(MAKE_TEST)
