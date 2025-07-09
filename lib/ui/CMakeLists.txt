SET(SOURCES ui_ConsoleScreen.c
    ui_VehicleScreen.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_img_tgv_png.c)

add_library(ui ${SOURCES})
