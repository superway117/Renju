EXECUTABLE = Renju

SOURCES = ../Classes/MainMenuLayer.cpp \
	../Classes/AppDelegate.cpp \
	../Classes/SmartRes.cpp \
	../Classes/BaseLayer.cpp \
	../Classes/Bead.cpp \
	../Classes/BeadBoardLayer.cpp \
	../Classes/Arithmetic.cpp \
	../Classes/BoardKeeper.cpp \
	../Classes/User.cpp \
	../Classes/DialogLayer.cpp \
	../Classes/SettingLayer.cpp \
	../Classes/GameRulerLayer.cpp \
	../Classes/StringRes.cpp \
	main.cpp

SHAREDLIBS = -lcocos2d -lcocosdenshion -lcurl
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so $(LIB_DIR)/libcocosdenshion.so

include ../../../../cocos2dx/proj.linux/cocos2dx.mk


INCLUDES +=	 -I../ \
	-I../../ \
	-I../Classes \
	-I$(COCOS_ROOT)/CocosDenshion/include \
	-I$(COCOS_ROOT)/extensions/ \
	-I$(COCOS_ROOT)/external/ \
	-I$(COCOS_ROOT)/external/chipmunk/include/chipmunk/ \
	-I$(COCOS_ROOT)/external/json-c/

STATICLIBS += \
	$(STATICLIBS_DIR)/libcurl.a \
	$(LIB_DIR)/libextension.a \
	$(LIB_DIR)/libjsonc.a

####### Build rules
$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS) $(LIBS)

####### Compile
$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(OBJ_DIR)/%.o: %.c $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CC)$(CC) $(CCFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@
