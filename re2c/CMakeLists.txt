cmake_minimum_required(VERSION 2.8)

project(re2c)

include(ExternalProject)

ExternalProject_Add(re2c
	URL ${CMAKE_CURRENT_SOURCE_DIR}/re2c-0.14.3.tar.gz
	URL_MD5 9d6914369494ea924a1dd7d96715cdc0
	CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
	PREFIX artifacts
	INSTALL_DIR ${CMAKE_BUILD_DIR})
