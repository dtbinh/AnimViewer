
# 15-462 project makefile
# EDIT THIS FILE TO ADD NEW SOURCE FILES

SRCS = \
	application/application.cpp \
	application/imageio.cpp \
	application/camera_roam.cpp \
	application/scene_loader.cpp \
	math/math.cpp \
	math/color.cpp \
	math/vector.cpp \
	math/quaternion.cpp \
	math/matrix.cpp \
	math/camera.cpp \
    math/ray.cpp \
	scene/material.cpp \
	scene/mesh.cpp \
	scene/scene.cpp \
	scene/sphere.cpp \
	scene/triangle.cpp \
	scene/model.cpp \
	scene/bounding_box.cpp \
	tinyxml/tinyxml.cpp \
	tinyxml/tinyxmlerror.cpp \
	tinyxml/tinyxmlparser.cpp \
	raytracer/main.cpp \
	raytracer/raytracer.cpp \
    raytracer/raycasting.cpp 

TARGET = raytracer

