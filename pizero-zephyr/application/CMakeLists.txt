cmake_minimum_required(VERSION 3.20.0)

set(SHIELD waveshare_rgb_led_hat)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(pizero_test)

target_sources(app PRIVATE src/main.c)
