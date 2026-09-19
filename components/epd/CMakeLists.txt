idf_component_register(SRCS "Display_EPD_W21.c"
                            "Display_EPD_W21_spi.c"
                            "GUI_Paint.c"
                            "fonts/font8.c"
                            "fonts/font12.c"
                            "fonts/font16.c"
                            "fonts/font20.c"
                            "fonts/font24.c"
                            "fonts/font12CN.c"
                            "fonts/font24CN.c"
                       INCLUDE_DIRS "include" "."
                       REQUIRES driver esp_driver_gpio esp_driver_spi)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wno-missing-braces
    -Wno-unterminated-string-initialization)
