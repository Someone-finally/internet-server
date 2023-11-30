idf_component_register(
    SRCS 
    "main.c"
    "toggle_led.c"
    "push_btn.c"
    INCLUDE_DIRS 
    "."
    
    )

    fatfs_create_rawflash_image(storage ../site/dist FLASH_IN_PROJECT)