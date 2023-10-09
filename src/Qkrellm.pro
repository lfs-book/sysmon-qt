QT       += core gui widgets network

TARGET   = Qkrellm 
TEMPLATE = app

HEADERS += Qkrellm.h    \
           qk_widgets.h \
           qk_config.h  \
           qk_color.h   \
           qk_font.h    \
           qk_temps.h    \
			  QKrellm_version.h

SOURCES += Qkrellm.cpp    \
           qk_widgets.cpp \
           qk_config.cpp  \
           qk_color.cpp   \
           qk_temps.cpp   \
           qk_font.cpp

#MAKEFILE = QK_Makefile
