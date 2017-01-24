#-------------------------------------------------
#
# Project created by QtCreator 2017-01-23T04:08:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtgo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        ntiss.cpp \
    frame_painter.cpp

HEADERS  += ntiss.h \
    graph/lpp_algorithm/ec_sae.h \
    graph/lpp_algorithm/plpca.h \
    graph/lpp_algorithm/replay_attack.h \
    graph/lpp_algorithm/sa.h \
    graph/display.h \
    graph/draw_util.h \
    graph/edge_cluster_edge.h \
    graph/edge_cluster_graph.h \
    graph/edge_cluster_node.h \
    graph/graph.h \
    graph/graph_edge.h \
    graph/graph_node.h \
    graph/lbs_user.h \
    graph/poi.h \
    graph/random_generator.h \
    graph/util.h \
    frame_painter.h

FORMS    += ntiss.ui

DISTFILES += \
    graph/lpp_algorithm/out1.txt \
    graph/map_data/graph_config_oldenburgGen.txt \
    graph/map_data/graph_config_sanfrancisco.txt \
    graph/map_data/oldenburgGen_edge.txt \
    graph/map_data/oldenburgGen_node.txt \
    graph/map_data/sanfrancisco_edge.txt \
    graph/map_data/sanfrancisco_node.txt \
    graph/map_data/sanfrancisco_node_bak.txt
