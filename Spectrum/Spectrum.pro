QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++23

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


TARGET = Spectrum

RESOURCES += \
    resources.qrc


# Include directories
INCLUDEPATH +=  ../Source/TextEditor \
                ../Source/Functions \
                ../Source/Lexer \
                ../Source/MenuBar

SOURCES += \
    ../Source/Lexer/AlifComplete.cpp \
    ../Source/Lexer/AlifLexer.cpp \
    ../Source/MenuBar/SPMenu.cpp \
    ../Source/TextEditor/SPEditor.cpp \
    ../Source/TextEditor/SPHighlighter.cpp \
    Spectrum.cpp \
    main.cpp

HEADERS += \
    ../Source/Lexer/AlifComplete.h \
    ../Source/Lexer/AlifLexer.h \
    ../Source/MenuBar/SPMenu.h \
    ../Source/TextEditor/SPEditor.h \
    ../Source/TextEditor/SPHighlighter.h \
    Spectrum.h



# Add the application icon (Windows)
win32:RC_ICONS += Resources/TaifLogo.ico

# Add the application icon (macOS/Linux)
macx:ICON = Resources/TaifLogo.icns
unix:!macx:ICON = Resources/TaifLogo.png

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




