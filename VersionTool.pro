TEMPLATE = app

CONFIG += console
CONFIG -= qt

DEFINES += _CRT_SECURE_NO_WARNINGS

SOURCES += main.cpp



VERSION_TOOL_SCRIPT = ""



!isEmpty(VERSION_TOOL_SCRIPT) {
    ####
    ####   COPY RESULT FILE TO LOGVIEWER FOLDER
    ####

    CONFIG (debug, debug|release) {
        BUILD_MODE = debug
    } else {
        BUILD_MODE = release
    }

    win32 {
        INSTALL_VERSION_TOOL = \"$${VERSION_TOOL_SCRIPT}.bat\" $$BUILD_MODE \"$$replace(PWD, /, \\)\" \"$$replace(OUT_PWD, /, \\)\"
    } else:unix {
        INSTALL_VERSION_TOOL = \"$${VERSION_TOOL_SCRIPT}.sh\"  $$BUILD_MODE \"$$PWD\"                 \"$$OUT_PWD\"
    }



    ### Command to copy files

    installversiontool.commands += $$INSTALL_VERSION_TOOL                                                      \
                                   ||                                                                          \
                                   echo \"ERROR!!! Impossible create separate folder. Please check logs\" 1>&2



    ### It is a MAGIC

    first.depends = $(first) installversiontool
    installversiontool.depends = all

    QMAKE_EXTRA_TARGETS += first installversiontool
}
