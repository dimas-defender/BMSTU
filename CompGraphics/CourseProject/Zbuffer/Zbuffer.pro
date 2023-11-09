QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

#TARGET = Zbuffer

HEADERS = \
   $$PWD/include/ActiveEdge.h \
   $$PWD/include/ActivePolygon.h \
   $$PWD/include/ActiveSegment.h \
   $$PWD/include/Edge.h \
   $$PWD/include/ImageLabel.h \
   $$PWD/include/Model.h \
   $$PWD/include/NaiveHierarchicalZBuffer.h \
   $$PWD/include/NaiveZBuffer.h \
   $$PWD/include/Octree.h \
   $$PWD/include/OctreeHierarchicalZBuffer.h \
   $$PWD/include/Pixel.h \
   $$PWD/include/Polygon.h \
   $$PWD/include/QuadTree.h \
   $$PWD/include/RenderWidget.h \
   $$PWD/include/ScanlineZBuffer.h \
   $$PWD/include/Segment.h \
   $$PWD/include/Vertex.h \
   $$PWD/include/ZBuffer.h

SOURCES = \
   $$PWD/src/ActiveEdge.cpp \
   $$PWD/src/ActivePolygon.cpp \
   $$PWD/src/ActiveSegment.cpp \
   $$PWD/src/Edge.cpp \
   $$PWD/src/ImageLabel.cpp \
   $$PWD/src/main.cpp \
   $$PWD/src/Model.cpp \
   $$PWD/src/NaiveHierarchicalZBuffer.cpp \
   $$PWD/src/NaiveZBuffer.cpp \
   $$PWD/src/Octree.cpp \
   $$PWD/src/OctreeHierarchicalZBuffer.cpp \
   $$PWD/src/Pixel.cpp \
   $$PWD/src/Polygon.cpp \
   $$PWD/src/QuadTree.cpp \
   $$PWD/src/RenderWidget.cpp \
   $$PWD/src/ScanlineZBuffer.cpp \
   $$PWD/src/Segment.cpp \
   $$PWD/src/Vertex.cpp \
   $$PWD/src/ZBuffer.cpp

INCLUDEPATH = \
    $$PWD/include

INCLUDEPATH += "$$PWD\assimp"
LIBS += -L"$$PWD\lib"
LIBS += -lassimp

#DEFINES = 

