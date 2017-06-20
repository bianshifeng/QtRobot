OTHER_FILES += \
    $$PWD/user/scripts/* \




user_scripts.files += \
    $$PWD/user/scripts/demo.js \
    $$PWD/user/scripts/mode2.js \
user_scripts.path = F:/Projects/bin/debug/user/scriptbsf


message($$user_scripts.path)


INSTALLS += user_scripts
