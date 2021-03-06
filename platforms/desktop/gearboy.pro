TEMPLATE = app
TARGET = gearboy

QT += core gui widgets multimedia qml quick
CONFIG += c++11

RESOURCES += gearboy.qrc

QML_FILES += $$files(*.qml,true) \
             $$files(*.js,true)

CONF_FILES +=  gearboy.png 

OTHER_FILES += $${CONF_FILES} \
               $${QML_FILES} \
               gearboy.rpattison.desktop 

#specify where the qml/js files are installed to
qml_files.path = /gearboy
qml_files.files += $${QML_FILES}

#specify where the config files are installed to
config_files.path = /
config_files.files += $${CONF_FILES}

desktop_file.path = /
desktop_file.files = $$OUT_PWD/gearboy.rpattison.desktop 
desktop_file.CONFIG += no_check_exist 

SOURCES += \
    ../../src/audio/Blip_Buffer.cpp \
    ../../src/audio/Gb_Apu_State.cpp \
    ../../src/audio/Gb_Apu.cpp \
    ../../src/audio/Gb_Oscs.cpp \
    ../../src/audio/Multi_Buffer.cpp \
    ../../src/audio/Sound_Queue.cpp \
    ../../src/Audio.cpp \
    ../../src/Cartridge.cpp \
    ../../src/CommonMemoryRule.cpp \
    ../../src/GearboyCore.cpp \
    ../../src/Input.cpp \
    ../../src/IORegistersMemoryRule.cpp \
    ../../src/MBC1MemoryRule.cpp \
    ../../src/MBC2MemoryRule.cpp \
    ../../src/MBC3MemoryRule.cpp \
    ../../src/MBC5MemoryRule.cpp \
    ../../src/Memory.cpp \
    ../../src/MemoryRule.cpp \
    ../../src/MultiMBC1MemoryRule.cpp \
    ../../src/opcodes_cb.cpp \
    ../../src/opcodes.cpp \
    ../../src/Processor.cpp \
    ../../src/RingBuffer.cpp \
    ../../src/RomOnlyMemoryRule.cpp \
    ../../src/Video.cpp \
    ../../src/miniz/miniz.c \
    ../../src/PixelRenderer.cpp \
    ../../src/GBEmulator.cpp \
    ../../src/EmulationRunner.cpp \
    ../../src/main.cpp 

HEADERS  += \
    ../../src/audio/blargg_common.h \
    ../../src/audio/blargg_config.h \
    ../../src/audio/blargg_source.h \
    ../../src/audio/Blip_Buffer.h \
    ../../src/audio/Blip_Synth.h \
    ../../src/audio/Gb_Apu.h \
    ../../src/audio/Gb_Oscs.h \
    ../../src/audio/Multi_Buffer.h \
    ../../src/audio/Sound_Queue.h \
    ../../src/Audio.h \
    ../../src/boot_roms.h \
    ../../src/Cartridge.h \
    ../../src/RingBuffer.h \
    ../../src/CommonMemoryRule.h \
    ../../src/definitions.h \
    ../../src/EightBitRegister.h \
    ../../src/gearboy.h \
    ../../src/GearboyCore.h \
    ../../src/Input.h \
    ../../src/IORegistersMemoryRule.h \
    ../../src/MBC1MemoryRule.h \
    ../../src/MBC2MemoryRule.h \
    ../../src/MBC3MemoryRule.h \
    ../../src/MBC5MemoryRule.h \
    ../../src/Memory_inline.h \
    ../../src/Memory.h \
    ../../src/MemoryRule.h \
    ../../src/MultiMBC1MemoryRule.h \
    ../../src/opcode_names.h \
    ../../src/opcode_timing.h \
    ../../src/Processor_inline.h \
    ../../src/Processor.h \
    ../../src/RomOnlyMemoryRule.h \
    ../../src/SixteenBitRegister.h \
    ../../src/EmulationRunner.h \
    ../../src/Video.h \
    ../../src/GBEmulator.h \
    ../../src/PixelRenderer.h 

target.path = ./build

INSTALLS += target config_files qml_files desktop_file 
