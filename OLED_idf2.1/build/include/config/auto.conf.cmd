deps_config := \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/bt/Kconfig \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/esp32/Kconfig \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/ethernet/Kconfig \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/freertos/Kconfig \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/log/Kconfig \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/lwip/Kconfig \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/mbedtls/Kconfig \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/spi_flash/Kconfig \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/bootloader/Kconfig.projbuild \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/esptool_py/Kconfig.projbuild \
	/Users/frank/esp/esp-idf-v2.0-rc1/components/partition_table/Kconfig.projbuild \
	/Users/frank/esp/OLED_idf2.1/main/Kconfig.projbuild \
	/Users/frank/esp/esp-idf-v2.0-rc1/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
