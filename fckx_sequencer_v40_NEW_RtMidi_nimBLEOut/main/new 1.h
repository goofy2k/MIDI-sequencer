rial port COM4
Connecting.....
Chip is ESP32-D0WD-V3 (revision 3)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: 84:cc:a8:0d:3b:70
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Compressed 3072 bytes to 104...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (104 compressed) at 0x00008000 in 0.0 seconds (effective 3497.1 kbit/s)...
Hash of data verified.
Compressed 25456 bytes to 15429...
Writing at 0x00001000... (100 %)
Wrote 25456 bytes (15429 compressed) at 0x00001000 in 0.4 seconds (effective 556.9 kbit/s)...
Hash of data verified.
Compressed 1254672 bytes to 748487...
Writing at 0x00010000... (2 %)
Writing at 0x00014000... (4 %)
Writing at 0x00018000... (6 %)
Writing at 0x0001c000... (8 %)
Writing at 0x00020000... (10 %)
Writing at 0x00024000... (13 %)
Writing at 0x00028000... (15 %)
Writing at 0x0002c000... (17 %)
Writing at 0x00030000... (19 %)
Writing at 0x00034000... (21 %)
Writing at 0x00038000... (23 %)
Writing at 0x0003c000... (26 %)
Writing at 0x00040000... (28 %)
Writing at 0x00044000... (30 %)
Writing at 0x00048000... (32 %)
Writing at 0x0004c000... (34 %)
Writing at 0x00050000... (36 %)
Writing at 0x00054000... (39 %)
Writing at 0x00058000... (41 %)
Writing at 0x0005c000... (43 %)
Writing at 0x00060000... (45 %)
Writing at 0x00064000... (47 %)
Writing at 0x00068000... (50 %)
Writing at 0x0006c000... (52 %)
Writing at 0x00070000... (54 %)
Writing at 0x00074000... (56 %)
Writing at 0x00078000... (58 %)
Writing at 0x0007c000... (60 %)
Writing at 0x00080000... (63 %)
Writing at 0x00084000... (65 %)
Writing at 0x00088000... (67 %)
Writing at 0x0008c000... (69 %)
Writing at 0x00090000... (71 %)
Writing at 0x00094000... (73 %)
Writing at 0x00098000... (76 %)
Writing at 0x0009c000... (78 %)
Writing at 0x000a0000... (80 %)
Writing at 0x000a4000... (82 %)
Writing at 0x000a8000... (84 %)
Writing at 0x000ac000... (86 %)
Writing at 0x000b0000... (89 %)
Writing at 0x000b4000... (91 %)
Writing at 0x000b8000... (93 %)
Writing at 0x000bc000... (95 %)
Writing at 0x000c0000... (97 %)
Writing at 0x000c4000... (100 %)
Wrote 1254672 bytes (748487 compressed) at 0x00010000 in 18.3 seconds (effective 548.7 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
Executing action: monitor
Running idf_monitor in directory c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v39_new_rtmidi_nimbleout
Executing "C:\Users\Fred\.espressif\python_env\idf4.2_py3.8_env\Scripts\python.exe C:\Users\Fred\esp-idf\tools/idf_monitor.py -p COM4 -b 115200 --toolchain-prefix xtensa-esp32-elf- c:\users\fred\esp_projects\midi-sequencer\fckx_sequencer_v39_new_rtmidi_nimbleout\build\fckx_sequencer.elf -m 'C:\Users\Fred\.espressif\python_env\idf4.2_py3.8_env\Scripts\python.exe' 'C:\Users\Fred\esp-idf\tools\idf.py' '--no-ccache' '-p' 'COM4'"...
--- WARNING: GDB cannot open serial ports accessed as COMx
--- Using \\.\COM4 instead...
--- idf_monitor on \\.\COM4 115200 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
ets Jul 29 2019 12:21:46

rst:0x1 (POWERON_RESET),boot:0x17 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:4
load:0x3fff0034,len:7068
load:0x40078000,len:13652
load:0x40080400,len:4632
0x40080400: _init at ??:?

entry 0x400806f4
I (29) boot: ESP-IDF v4.2-dirty 2nd stage bootloader
I (29) boot: compile time 15:08:43
I (29) boot: chip revision: 3
I (32) boot_comm: chip revision: 3, min. bootloader chip revision: 1
I (40) boot.esp32: SPI Speed      : 40MHz
I (44) boot.esp32: SPI Mode       : DIO
I (49) boot.esp32: SPI Flash Size : 8MB
I (53) boot: Enabling RNG early entropy source...
I (59) boot: Partition Table:
I (62) boot: ## Label            Usage          Type ST Offset   Length
I (70) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (77) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (84) boot:  2 factory          factory app      00 00 00010000 00200000
I (92) boot: End of partition table
I (96) boot_comm: chip revision: 3, min. application chip revision: 1
I (103) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x34e28 (216616) map
I (203) esp_image: segment 1: paddr=0x00044e50 vaddr=0x3ffbdb60 size=0x042cc ( 17100) load
I (211) esp_image: segment 2: paddr=0x00049124 vaddr=0x40080000 size=0x00404 (  1028) load
0x40080000: _WindowOverflow4 at C:/Users/Fred/esp-idf/components/freertos/xtensa/xtensa_vectors.S:1730

I (212) esp_image: segment 3: paddr=0x00049530 vaddr=0x40080404 size=0x06ae8 ( 27368) load
I (232) esp_image: segment 4: paddr=0x00050020 vaddr=0x400d0020 size=0xdeb60 (912224) map
0x400d0020: _stext at ??:?

I (612) esp_image: segment 5: paddr=0x0012eb88 vaddr=0x40086eec size=0x13960 ( 80224) load
0x40086eec: coex_unforce_wifi_mode at ??:?

I (666) boot: Loaded app from partition at offset 0x10000
I (666) boot: Disabling RNG early entropy source...
I (666) psram: This chip is ESP32-D0WD
I (672) spiram: Found 64MBit SPI RAM device
I (676) spiram: SPI RAM mode: flash 40m sram 40m
I (681) spiram: PSRAM initialized, cache is in low/high (2-core) mode.
I (688) cpu_start: Pro cpu up.
I (692) cpu_start: Application information:
I (697) cpu_start: Project name:     fckx_sequencer
I (702) cpu_start: App version:      f6f7de9-dirty
I (708) cpu_start: Compile time:     Mar 14 2022 15:08:00
I (714) cpu_start: ELF file SHA256:  6cb35a626755c7b0...
I (720) cpu_start: ESP-IDF:          v4.2-dirty
I (725) cpu_start: Starting app cpu, entry point is 0x40081bc4
0x40081bc4: call_start_cpu1 at C:/Users/Fred/esp-idf/components/esp32/cpu_start.c:287

I (0) cpu_start: App cpu up.
I (1619) spiram: SPI SRAM memory test OK
D (1620) memory_layout: Checking 12 reserved memory ranges:
D (1620) memory_layout: Reserved memory range 0x3f800000 - 0x3fc00000
D (1625) memory_layout: Reserved memory range 0x3ffae000 - 0x3ffae6e0
D (1631) memory_layout: Reserved memory range 0x3ffae6e0 - 0x3ffaff10
D (1638) memory_layout: Reserved memory range 0x3ffb0000 - 0x3ffb6388
D (1644) memory_layout: Reserved memory range 0x3ffb8000 - 0x3ffb9a20
D (1651) memory_layout: Reserved memory range 0x3ffbdb28 - 0x3ffbdb5c
D (1657) memory_layout: Reserved memory range 0x3ffbdb60 - 0x3ffc9fd0
D (1664) memory_layout: Reserved memory range 0x3ffe0000 - 0x3ffe0440
D (1670) memory_layout: Reserved memory range 0x3ffe3f20 - 0x3ffe4350
D (1677) memory_layout: Reserved memory range 0x40070000 - 0x40078000
D (1683) memory_layout: Reserved memory range 0x40078000 - 0x40080000
0x40080000: _WindowOverflow4 at C:/Users/Fred/esp-idf/components/freertos/xtensa/xtensa_vectors.S:1730

D (1690) memory_layout: Reserved memory range 0x40080000 - 0x4009a84c
0x40080000: _WindowOverflow4 at C:/Users/Fred/esp-idf/components/freertos/xtensa/xtensa_vectors.S:1730

D (1696) memory_layout: Building list of available memory regions:
D (1703) memory_layout: Available memory region 0x3ffaff10 - 0x3ffb0000
D (1709) memory_layout: Available memory region 0x3ffb6388 - 0x3ffb8000
D (1716) memory_layout: Available memory region 0x3ffb9a20 - 0x3ffbdb28
D (1723) memory_layout: Available memory region 0x3ffc9fd0 - 0x3ffca000
D (1729) memory_layout: Available memory region 0x3ffca000 - 0x3ffcc000
D (1736) memory_layout: Available memory region 0x3ffcc000 - 0x3ffce000
D (1743) memory_layout: Available memory region 0x3ffce000 - 0x3ffd0000
D (1749) memory_layout: Available memory region 0x3ffd0000 - 0x3ffd2000
D (1756) memory_layout: Available memory region 0x3ffd2000 - 0x3ffd4000
D (1763) memory_layout: Available memory region 0x3ffd4000 - 0x3ffd6000
D (1769) memory_layout: Available memory region 0x3ffd6000 - 0x3ffd8000
D (1776) memory_layout: Available memory region 0x3ffd8000 - 0x3ffda000
D (1783) memory_layout: Available memory region 0x3ffda000 - 0x3ffdc000
D (1789) memory_layout: Available memory region 0x3ffdc000 - 0x3ffde000
D (1796) memory_layout: Available memory region 0x3ffde000 - 0x3ffe0000
D (1803) memory_layout: Available memory region 0x3ffe0440 - 0x3ffe3f20
D (1809) memory_layout: Available memory region 0x3ffe4350 - 0x3ffe8000
D (1816) memory_layout: Available memory region 0x3ffe8000 - 0x3fff0000
D (1823) memory_layout: Available memory region 0x3fff0000 - 0x3fff8000
D (1830) memory_layout: Available memory region 0x3fff8000 - 0x3fffc000
D (1836) memory_layout: Available memory region 0x3fffc000 - 0x40000000
D (1843) memory_layout: Available memory region 0x4009a84c - 0x4009c000
D (1850) memory_layout: Available memory region 0x4009c000 - 0x4009e000
D (1856) memory_layout: Available memory region 0x4009e000 - 0x400a0000
I (1863) heap_init: Initializing. RAM available for dynamic allocation:
D (1870) heap_init: New heap initialised at 0x3ffaff10
I (1875) heap_init: At 3FFAFF10 len 000000F0 (0 KiB): DRAM
D (1882) heap_init: New heap initialised at 0x3ffb6388
I (1887) heap_init: At 3FFB6388 len 00001C78 (7 KiB): DRAM
D (1893) heap_init: New heap initialised at 0x3ffb9a20
I (1898) heap_init: At 3FFB9A20 len 00004108 (16 KiB): DRAM
D (1904) heap_init: New heap initialised at 0x3ffc9fd0
I (1910) heap_init: At 3FFC9FD0 len 00016030 (88 KiB): DRAM
I (1916) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (1922) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
D (1929) heap_init: New heap initialised at 0x4009a84c
I (1934) heap_init: At 4009A84C len 000057B4 (21 KiB): IRAM
I (1940) cpu_start: Pro cpu start user code
I (1945) spiram: Adding pool of 4096K of external SPI memory to heap allocator
D (1960) clk: RTC_SLOW_CLK calibration value: 3396762
D (1970) intr_alloc: Connected src 46 to int 2 (cpu 0)
D (1970) intr_alloc: Connected src 17 to int 3 (cpu 0)
Log out 2:>>>>>>>>>>>>>>>>>>>>>>>>test Log write in constructor<<<<<<<<<<<<<<<<<<<<<<<<<<
D (1979) cpu_start: Setting C++ exception workarounds.
D (1984) efuse: coding scheme 0
D (1987) efuse: coding scheme 0
Hello Back!D (1987) efuse: In EFUSE_BLK0__DATA3_REG is used 1 bits starting with 15 bit
D (1990) efuse: In EFUSE_BLK0__DATA5D (2002) intr_alloc: Connected src 24 to int 9 (cpu 0)
D (2007) FLASH_HAL: extra_dummy: 1
_REG is used 1 bits starting with 20 bit
D (2010) spi_flash: trying chip: issi
D (2010) spi_flash: trying chip: gd
W (2021) spi_flash: Detected size(16384k) larger than the size in the binary image header(8192k). Using the size in the binary image header.
[0;32mI (2010) spi_flash: detected chip: gd
I (2021) spi_flash: flash io: dio
I (2035) cpu_start: StartingD (0) efuse: coding scheme 0
D (0) efuse: coding scheme 0
 scheduler on PRO CPU.
D (0) efuse: In EFUSE_BLK0__DATA3_REG is used 1 bits starting with 15 bit
D (0) efuse: In EFUSE_BD (10) intr_alloc: Connected src 25 to int 2 (cpu 1)
I (20) cpu_start: Starting scheduler on APP CPU.
D (2093) heap_init: New heap initialised at 0x3ffe0440
D (2103) heap_init: New heap initialised at 0x3ffe4350
I (2103) spiram: Reserving pool of 32K of internal memory for DMA/internal allocations
D (2113) spiram: Allocating block of size 32768 bytes
D (2123) intr_alloc: Connected src 16 to int 12 (cpu 0)
LK0__DATA5_REG is used 1 bits starting with 20 bit
W (2123) APP_MAIN: APP_MAIN ENTRY
W (2123) APP_MAIN: APP_MAIN ENTRY
I (2133) APP_MAIN: [APP] Startup..
I (2133) APP_MAIN: [APP] Free memory: 4380652 bytes
I (2133) APP_MAIN: [APP] IDF version: v4.2-dirty
W (2143) APP_MAIN: Initialize WIFI connection here
D (2143) partition: Loading the partition table
D (2203) esp_netif_lwip: LwIP stack has been initialized
D (2203) esp_netif_lwip: esp-netif has been successfully initialized
I (2203) APP_MAIN: ESP_WIFI_MODE_STA
D (2203) esp_netif_lwip: esp-netif has been successfully initialized
D (2223) esp_netif_objects: esp_netif_add_to_list 0x3ffd4240
D (2223) esp_netif_objects: esp_netif_add_to_list netif added successfully (total netifs: 1)
D (2233) nvs: nvs_open_from_partition misc 1
D (2233) nvs: nvs_get_str_or_blob log
I (2253) wifi:wifi driver task: 3ffd59f0, prio:23, stack:3072, core=0
I (2253) syD (2253) efuse: coding scheme 0
stem_api: Base MAC address is not set
I (2253) system_api: read default base MAC address from EFUSE
D (2253) efusD (2263) efuse: coding scheme 0
e: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 8 bit
D (2273) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits startD (2283) efuse: coding scheme 0
D (2283) efuse: coding scheme 0
ing with 0 bit
D (2283) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (2283) efuse: In EFUSE_BLKD (2293) efuse: coding scheme 0
0__DATA1_REG is used 8 bits starting with 16 bit
D (2303) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bitD (2313) efuse: coding scheme 0
D (2313) efuse: coding scheme 0

D (2313) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 0 bit
D (2323) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
D (2333) nvs: nvs_open_from_partition nvs.net80211 1
D (2333) nvs: nvs_get opmode 1
D (2343) nvs: nvs_get_str_or_blob sta.ssid
D (2343) nvs: nvs_get_str_or_blob sta.mac
D (2343) nvs: nvs_get sta.authmode 1
D (2353) nvs: nvs_get_str_or_blob sta.pswd
D (2353) nvs: nvs_get_str_or_blob sta.pmk
D (2353) nvs: nvs_get sta.chan 1
D (2363) nvs: nvs_get auto.conn 1
D (2363) nvs: nvs_get bssid.set 1
D (2363) nvs: nvs_get_str_or_blob sta.bssid
D (2373) nvs: nvs_get sta.lis_intval 2
D (2373) nvs: nvs_get sta.phym 1
D (2373) nvs: nvs_get sta.phybw 1
D (2373) nvs: nvs_get_str_or_blob sta.apsw
D (2383) nvs: nvs_get_str_or_blob sta.apinfo
D (2393) nvs: nvs_get sta.scan_method 1
D (2393) nvs: nvs_get sta.sort_method 1
D (2393) nvs: nvs_get sta.minrssi 1
D (2393) nvs: nvs_get sta.minauth 1
D (2403) nvs: nvs_get sta.pmf_e 1
D (2413) nvs: nvs_get sta.pmf_r 1
D (2413) nvs: nvs_get_str_or_blob ap.ssid
D (2413) nvs: nvs_get_str_or_blob ap.mac
D (2423) nvs: nvs_get_str_or_blob ap.passwd
D (2423) nvs: nvs_get_str_or_blob ap.pmk
D (2433) nvs: nvs_get ap.chan 1
D (2433) nvs: nvs_get ap.authmode 1
D (2433) nvs: nvs_get ap.hidden 1
D (2433) nvs: nvs_get ap.max.conn 1
D (2443) nvs: nvs_get bcn.interval 2
D (2443) nvs: nvs_get ap.phym 1
D (2443) nvs: nvs_get ap.phybw 1
D (2453) nvs: nvs_get ap.sndchan 1
D (2453) nvs: nvs_get ap.pmf_e 1
D (2453) nvs: nvs_get ap.pmf_r 1
D (2463) nvs: nvs_get lorate 1
D (2463) nvs: nvs_get_str_or_blob country
D (2463) nvs: nvs_set ap.sndchan 1 1
D (2463) nvs: nvs_set_blob sta.mac 6
D (2473) nvs: nvs_set_blob ap.mac 6
I (2473) wifi:wifi firmware version: 1865b55
I (2473) wifi:wifi certification version: v7.0
I (2483) wifi:config NVS flash: enabled
I (2483) wifi:config nano formating: enabled
I (2493) wifi:Init data frame dynamic rx buffer num: 32
I (2493) wifi:Init management frame dynamic rx buffer num: 32
I (2493) wifi:Init management short buffer num: 32
I (2513) wifi:Init static tx buffer num: 16
I (2513) wifi:Init tx cache buffer num: 32
I (2523) wifi:Init static rx buffer size: 1600
I (2523) wifi:Init static rx buffer num: 10
I (2523) wifi:Init dynamic rx buffer num: 32
I (2533) wifi_init: rx ba win: 6
I (2533) wifi_init: tcpip mbox: 32
I (2533) wifi_init: udp mbox: 6
I (2543) wifi_init: tcp mbox: 6
I (2543) wifi_init: tcp tx win: 5744
I (2543) wifi_init: tcp rx win: 5744
I (2553) wifi_init: tcp mss: 1440
I (2553) wifi_init: WiFi/LWIP prefer SPIRAM
D (2553) phy_init: loading PHY init data from application binary
D (2563) nvs: nvs_open_from_partition phy 0
D (2563) nvs: D (2573) efuse: coding scheme 0
nvs_get cal_version 4
D (2573) nvs: nvs_get_str_or_blob cal_mac
D (2573) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits D (2593) efuse: coding scheme 0
D (2593) efuse: coding scheme 0
starting with 8 bit
D (2593) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (2593) efuse: In EFUSED (2603) efuse: coding scheme 0
_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (2613) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 1D (2623) efuse: coding scheme 0
D (2623) efuse: coding scheme 0
6 bit
D (2623) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (2623) efuse: In EFUSE_BLK0__DATA1_RD (2633) efuse: coding scheme 0
EG is used 8 bits starting with 0 bit
D (2643) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
D (26D (2663) efuse: coding scheme 0
43) nvs: nvs_get_str_or_blob cal_data
D (2663) nvs: nvs_close 3
D (2663) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits D (2663) efuse: coding scheme 0
D (2673) efuse: coding scheme 0
starting with 8 bit
D (2673) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (2673) efuse: In EFUSED (2683) efuse: coding scheme 0
_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (2683) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 1D (2703) efuse: coding scheme 0
D (2703) efuse: coding scheme 0
6 bit
D (2703) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (2703) efuse: In EFUSE_BLK0__DATA1_RD (2713) efuse: coding scheme 0
I (2803) phy: phy_version: 4500, 0cd6843, Sep 17 2020, 15:37:07, 0, 0
EG is used 8 bits starting with 0 bit
D (2723) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
I (2803) wifi:mode : sta (84:cc:a8:0d:3b:70)
D (2813) wifi_init_default: wifi_start esp-netif:0x3ffd4240 event-id2
D (2813) wifi_init_default: WIFI mac address: 84 cc a8 d 3b 70
D (2823) esp_netif_handlers: esp_netif action has started with netif0x3ffd4240 from event_id=2
D (2833) esp_netif_lwip: check: remote, if=0x3ffd4240 fn=0x40103718
0x40103718: esp_netif_start_api at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:608


D (2833) esp_netif_lwip: esp_netif_start_api 0x3ffd4240
D (2843) esp_netif_lwip: esp_netif_get_hostname esp_netif:0x3ffd4240
D (2843) esp_netif_lwip: check: local, if=0x3ffd4240 fn=0x4010415c
0x4010415c: esp_netif_update_default_netif_lwip at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:163


D (2853) esp_netif_lwip: esp_netif_update_default_netif_lwip 0x3ffd4240
D (2853) esp_netif_lwip: call api in lwip: ret=0x0, give sem
I (2983) wifi:new:<8,0>, old:<1,0>, ap:<255,255>, sta:<8,0>, prof:1
I (2993) wifi:state: init -> auth (b0)
I (3003) wifi:state: auth -> assoc (0)
I (3063) wifi:state: assoc -> run (10)
I (3163) wifi:connected with Verhoeckx_glas, aid = 11, channel 8, BW20, bssid = 96:6a:b0:c3:3b:27
I (3163) wifi:security: WPA2-PSK, phy: bgn, rssi: -46
D (3183) nvs: nvs_set sta.chan 1 8
D (3183) nvs: nvs_set_blob sta.apinfo 700
I (3183) wifi:pm start, type: 1
I (3193) wifi:AP's beacon interval = 102400 us, DTIM period = 1
D (3193) esp_netif_handlers: esp_netif action connected with netif0x3ffd4240 from event_id=4
D (3203) esp_netif_lwip: check: remote, if=0x3ffd4240 fn=0x40103938
0x40103938: esp_netif_up_api at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:1131


D (3203) esp_netif_lwip: esp_netif_up_api esp_netif:0x3ffd4240
D (3213) esp_netif_lwip: check: local, if=0x3ffd4240 fn=0x4010415c
0x4010415c: esp_netif_update_default_netif_lwip at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:163


D (3213) esp_netif_lwip: esp_netif_update_default_netif_lwip 0x3ffd4240
D (3223) esp_netif_lwip: call api in lwip: ret=0x0, give sem
D (3233) esp_netif_lwip: check: remote, if=0x3ffd4240 fn=0x40103510
0x40103510: esp_netif_dhcpc_start_api at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:929


D (3233) esp_netif_lwip: esp_netif_dhcpc_start_api esp_netif:0x3ffd4240
D (3243) esp_netif_lwip: esp_netif_start_ip_lost_timer esp_netif:0x3ffd4240
D (3243) esp_netif_lwip: if0x3ffd4240 start ip lost tmr: no need start because netif=0x3ffd42c4 interval=120 ip=0
D (3253) esp_netif_lwip: starting dhcp client
D (3273) esp_netif_lwip: call api in lwip: ret=0x0, give sem
D (4703) esp_netif_lwip: esp_netif_dhcpc_cb lwip-netif:0x3ffd42c4
D (4703) esp_netif_lwip: if0x3ffd4240 ip changed=1
D (4703) wifi_init_default: Got IP wifi default handler entered
D (4713) esp_netif_handlers: esp_netif action got_ip with netif0x3ffd4240 from event_id=0
I (4713) esp_netif_handlers: sta ip: 192.168.2.5, mask: 255.255.255.0, gw: 192.168.2.254
W (4723) APP_MAIN: Initialize MQTT connection here
E (4733) APP_MAIN: WAITING A WHILE FOR MQTT INITS TO FINISH
D (4733) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=7
I (4743) mqtt_event_handler_cb: Other event id:7
D (4783) TRANS_TCP: [sock=54] Connecting to server. IP: 145.53.71.16, Port: 1883
D (4793) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=1
I (4793) mqtt_event_handler_cb: MQTT_EVENT_CONNECTED
I (4793) mqtt_event_handler_cb: sent publish successful, msg_id=10179
I (4813) mqtt_event_handler_cb: sent subscribe successful, msg_id=33910
I (4823) mqtt_event_handler_cb: sent subscribe successful, msg_id=30678
I (4823) mqtt_event_handler_cb: sent unsubscribe successful, msg_id=22604
I (4833) mqtt_event_handler_cb: sent subscribe successful, msg_id=55236
I (4843) mqtt_event_handler_cb: sent subscribe successful, msg_id=53021
I (4853) mqtt_event_handler_cb: sent subscribe successful, msg_id=3356
I (4853) mqtt_event_handler_cb: sent subscribe successful, msg_id=38295
D (4853) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=5
I (4863) mqtt_event_handler_cb: MQTT_EVENT_PUBLISHED, msg_id=10179
D (4873) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4883) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=33910
I (4883) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4903) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4913) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=30678
I (4913) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4923) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=4
I (4933) mqtt_event_handler_cb: MQTT_EVENT_UNSUBSCRIBED, msg_id=22604
D (4933) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4943) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=55236
I (4953) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4953) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4963) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=53021
I (4963) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4973) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4983) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=7730
I (4983) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (5033) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5033) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=3356
I (5033) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (5123) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5123) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=46505
I (5133) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (5143) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5153) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=38295
I (5153) mqtt_event_handler_cb: sent publish successful, msg_id=0
E (5733) APP_MAIN: CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDER
W (5733) APP_MAIN: ENTERING init_test_recorder()
Log out 2:>>>>>>>>>>>>>>>>>>>>>>>>test Log write in constructor<<<<<<<<<<<<<<<<<<<<<<<<<<
E (5743) FCKX_SEQ: Entering test_recorder of TEST_RECORDER example
E (5743) FCKX_SEQ: CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDER
W (5763) MIDIMANAGER::INIT: Enter
W (5763) MIDIOUTNIMBLE :: INITIALIZE: Initialize nimBLEOutdriver MidiOutNimBLE :: initialize
D (5763) BTDM_INIT: Release DRAM [0x3ffb2730] - [0x3ffb6388]
I (5773) BTDM_INIT: BT controller compile version [3723d5b]
D (5773) BTDM_INIT: .data initialise [0x3ffae6e0] <== [0x4000d890]
D (5783) BTDM_INIT: .bss initialise [0x3ffb0000] - [0x3ffb09a8]
D (5783) BTDM_INIT: .bss initialise [0x3ffb09a8] - [0x3ffb1ddc]
D (5793) BTDM_INIT: .bss initialise [0x3ffb1ddc] - [0x3ffb2730]
D (5813) BTDM_INIT: .bss initialise [0x3ffb8000] - [0x3ffb9a20]
D (5813) BTDM_INIT: .bss initialise [0x3ffbdb28] - [0x3ffbdb5c]
D (5823) phy_init: loading PHY init data from application binary
D (5823) nvs: nvs_open_from_partition phy 0
D (5833) efuse: coding scheme 0
D (5823) nvs: nvs_get cal_version 4
D (5833) nvs: nvs_get_str_or_blob cal_mac
D (5833) efuse: In EFUSE_BLK0__DATA2_REG iD (5843) efuse: coding scheme 0
D (5853) efuse: coding scheme 0
s used 8 bits starting with 8 bit
D (5843) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (5853) eD (5863) efuse: coding scheme 0
fuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (5863) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits sD (5873) efuse: coding scheme 0
D (5883) efuse: coding scheme 0
tarting with 16 bit
D (5883) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (5883) efuse: In EFUSED (5893) efuse: coding scheme 0
_BLK0__DATA1_REG is used 8 bits starting with 0 bit
D (5893) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16D (5913) efuse: coding scheme 0
 bit
D (5913) nvs: nvs_get_str_or_blob cal_data
D (5913) nvs: nvs_close 4
D (5913) efuse: In EFUSE_BLK0__DATA2_REG iD (5923) efuse: coding scheme 0
D (5923) efuse: coding scheme 0
s used 8 bits starting with 8 bit
D (5923) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (5933) eD (5943) efuse: coding scheme 0
fuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (5943) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits sD (5953) efuse: coding scheme 0
D (5953) efuse: coding scheme 0
tarting with 16 bit
D (5953) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (5963) efuse: In EFUSED (5973) efuse: coding scheme 0
_BLK0__DATA1_REG is used 8 bits starting with 0 bit
D (5973) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
D (6223) nvs: nvs_open_from_partition nimble_bond 1
D (6223) nvs: nvs_get_str_or_blob our_sec_1
D (6223) nvs: nvs_close 5
D (6233) nvs: nvs_open_from_partition nimble_bond 1
D (6233) nvs: nvs_get_str_or_blob our_sec_2
D (6233) nvs: nvs_close 6
D (6243) nvs: nvs_open_from_partition nimble_bond 1
D (6243) nvs: nvs_get_str_or_blob our_sec_3
D (6243) nvs: nvs_close 7
D (6253) NIMBLE_NVS: ble_store_config_our_secs restored 0 bonds
D (6253) nvs: nvs_open_from_partition nimble_bond 1
D (6263) nvs: nvs_get_str_or_blob peer_sec_1
D (6263) nvs: nvs_close 8
D (6263) nvs: nvs_open_from_partition nimble_bond 1
D (6273) nvs: nvs_get_str_or_blob peer_sec_2
D (6273) nvs: nvs_close 9
D (6273) nvs: nvs_open_from_partition nimble_bond 1
D (6283) nvs: nvs_get_str_or_blob peer_sec_3
D (6283) nvs: nvs_close 10
D (6283) NIMBLE_NVS: ble_store_config_peer_secs restored 0 bonds
D (6293) nvs: nvs_open_from_partition nimble_bond 1
D (6293) nvs: nvs_get_str_or_blob cccd_sec_1
D (6303) nvs: nvs_close 11
D (6303) nvs: nvs_open_from_partition nimble_bond 1
D (6303) nvs: nvs_get_str_or_blob cccd_sec_2
D (6313) nvs: nvs_close 12
D (6313) nvs: nvs_open_from_partition nimble_bond 1
D (6333) nvs: nvs_get_str_or_blob cccd_sec_3
D (6333) nvs: nvs_close 13
D (6333) nvs: nvs_open_from_partition nimble_bond 1
D (6343) nvs: nvs_get_str_or_blob cccd_sec_4
D (6343) nvs: nvs_close 14
D (6343) nvs: nvs_open_from_partition nimble_bond 1
D (6353) nvs: nvs_get_str_or_blob cccd_sec_5
D (6353) nvs: nvs_close 15
D (6353) nvs: nvs_open_from_partition nimble_bond 1
D (6363) nvs: nvs_get_str_or_blob cccd_sec_6
D (6363) nvs: nvs_close 16
D (6363) nvs: nvs_open_from_partition nimble_bond 1
D (6373) nvs: nvs_get_str_or_blob cccd_sec_7
D (6373) nvs: nvs_close 17
D (6373) nvs: nvs_open_from_partition nimble_bond 1
D (6383) nvs: nvs_get_str_or_blob cccd_sec_8
D (6383) nvs: nvs_close 18
D (6383) NIMBLE_NVS: ble_store_config_cccds restored 0 bonds
D (6393) nvs: nvs_open_from_partition nimble_bond 1
D (6393) nvs: nvs_get_str_or_blob p_dev_rec_1
D (6403) nvs: nvs_close 19
D (6403) nvs: nvs_open_from_partition nimble_bond 1
D (6403) nvs: nvs_get_str_or_blob p_dev_rec_2
D (6413) nvs: nvs_close 20
D (6413) nvs: nvs_open_from_partition nimble_bond 1
D (6433) nvs: nvs_get_str_or_blob p_dev_rec_3
D (6433) nvs: nvs_close 21
D (6433) nvs: nvs_open_from_partition nimble_bond 1
D (6443) nvs: nvs_get_str_or_blob p_dev_rec_4
D (6443) nvs: nvs_close 22
D (6443) NIMBLE_NVS: peer_dev_rec restored 0 records
ble_hs_hci_cmd_send: ogf=0x03 ocf=0x0003 len=0
0x03 0x0c 0x00
Command complete: cmd_pkts=5 ogf=0x3 ocf=0x3 status=0
ble_hs_hci_cmd_send: ogf=0x04 ocf=0x0001 len=0
0x01 0x10 0x00
Command complete: cmd_pkts=5 ogf=0x4 ocf=0x1 status=0 hci_ver=8 hci_rev=782 lmp_ver=8 mfrg=96 lmp_subver=782
ble_hs_hci_cmd_send: ogf=0x04 ocf=0x0003 len=0
0x03 0x10 0x00
Command complete: cmd_pkts=5 ogf=0x4 ocf=0x3 status=0 supp_feat=0x877bffdbfecdeebf
ble_hs_hci_cmd_send: ogf=0x03 ocf=0x0001 len=8
0x01 0x0c 0x08 0x90 0x80 0x00 0x02 0x00 0x80 0x00 0x20
Command complete: cmd_pkts=5 ogf=0x3 ocf=0x1 status=0
ble_hs_hci_cmd_send: ogf=0x03 ocf=0x0063 len=8
0x63 0x0c 0x08 0x00 0x00 0x80 0x00 0x00 0x00 0x00 0x00
Command complete: cmd_pkts=5 ogf=0x3 ocf=0x63 status=0
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0001 len=8
0x01 0x20 0x08 0x7f 0x06 0x00 0x00 0x00 0x00 0x00 0x00
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x1 status=0
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0002 len=0
0x02 0x20 0x00
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x2 status=0
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0003 len=0
0x03 0x20 0x00
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x3 status=0
ble_hs_hci_cmd_send: ogf=0x04 ocf=0x0009 len=0
0x09 0x10 0x00
Command complete: cmd_pkts=5 ogf=0x4 ocf=0x9 status=0 bd_addr=84:cc:a8:d:3b:72
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0018 len=0
0x18 0x20 0x00
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x18 status=0 rand=0x4f75a2877f4fa8ea
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0018 len=0
0x18 0x20 0x00
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x18 status=0 rand=0x3de2188bbe899f5d
Device added to RL, Resolving list count = 1
ble_hs_hci_cmd_send: ogf=0x03 ocf=0x0031 len=1
0x31 0x0c 0x01 0x01
Command complete: cmd_pkts=5 ogf=0x3 ocf=0x31 status=0
ble_hs_hci_cmd_send: ogf=0x03 ocf=0x0033 len=7
0x33 0x0c 0x07 0xff 0x00 0x00 0x0c 0x00 0x00 0x00
Command complete: cmd_pkts=5 ogf=0x3 ocf=0x33 status=17
ble_hs_hci_cmd_send: ogf=0x03 ocf=0x0031 len=1
0x31 0x0c 0x01 0x00
Command complete: cmd_pkts=5 ogf=0x3 ocf=0x31 status=0
looking up peer sec;
W (6633) MIDIOUTNIMBLE :: INITIALIZE: NimBLEDevice created
W (6633) MIDIOUTNIMBLE :: INITIALIZE: BLE server created
W (6643) MIDIOUTNIMBLE :: INITIALIZE: BLE server callbacks created
W (6643) MIDIOUTNIMBLE :: INITIALIZE: BLE server service created
W (6653) MIDIOUTNIMBLE :: INITIALIZE: BLE server MIDI characteristic created
W (6663) MIDIOUTNIMBLE :: INITIALIZE: portName[0] MIDIport
W (6663) MIDIOUTNIMBLE :: INITIALIZE: BLE service started
E (6673) NICMIDI DRIVER: start creation of MidiOutNimBLE port
W (6673) MIDIOUTNIMBLE :: INITIALIZE: Initialize nimBLEOutdriver MidiOutNimBLE :: initialize
W (6683) MIDIOUTNIMBLE :: INITIALIZE: NimBLEDevice created
W (6683) MIDIOUTNIMBLE :: INITIALIZE: BLE server created
W (6693) MIDIOUTNIMBLE :: INITIALIZE: BLE server callbacks created
W (6713) MIDIOUTNIMBLE :: INITIALIZE: BLE server service created
W (6713) MIDIOUTNIMBLE :: INITIALIZE: BLE server MIDI characteristic created
W (6723) MIDIOUTNIMBLE :: INITIALIZE: portName[0] MIDIport
W (6723) MIDIOUTNIMBLE :: INITIALIZE: BLE service started
E (6733) NICMIDI DRIVER: executed creation of MidiOutNimBLE port
W (6743) MQTTDRIVER: MQTTMidiIn instantiation
W (6743) MQTTDRIVER: MQTTMidiIn :: initialize
E (6753) NICMIDI DRIVER INPUT: start creation of MQTTMidiIn port
W (6753) MQTTDRIVER: MQTTMidiIn instantiation
W (6763) MQTTDRIVER: MQTTMidiIn :: initialize
E (6763) MQTTDRIVER: MQTTMidiIn::setCallback: STORING CALLBACK IN inputData_
E (6773) NICMIDI DRIVER INPUT: executed creation of MQTTMidiIn port
W (6773) MIDIMANAGER::INIT: Pos1
W (6783) MQTTDRIVER: Closed MQTTMidiIn NO CLEANUP: BEWARE of MEMORY LEAKS
W (6783) MIDIMANAGER::INIT: Pos2
W (6793) MIDIMANAGER::INIT: Pos3 (exit)
E (6803) SEQUENCER: track_states.size() 17
E (6813) SEQUENCER: track_states.size() 17
E (6813) SEQUENCER: track_states.size() 17
E (6823) SEQUENCER: track_states.size() 17
E (6823) SEQUENCER: track_states.size() 17
W (6843) MQTTDRIVER: open MQTTMidiIn port
E (6843) NICMIDI DRIVER: MIDIOutDriver::OpenPort() port_id 0
W (6843) MIDIOUTNIMBLE :: OPENPORT: Initialize nimBLEOutdriver MidiOutNimBLE :: openPort nr 0
OPENPORT2 ENTEREDE (6843) MIDIOUTNIMBLE :: OPENPORT: A valid connection already exists nevertheless start advertising (DIRTYOPEN!)
E (6863) MIDIOUTNIMBLE :: OPENPORT: >>>>>take the opportunity to list all available Characteristics somewhere over here <<<<<<<
I (6873) MIDIOUTNIMBLE :: OPENPORT: Prepare advertising
I (6883) MIDIOUTNIMBLE :: OPENPORT: Start advertising
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0008 len=32
0x08 0x20 0x20 0x1f 0x02 0x01 0x06 0x11 0x07 0x4b 0x91 0x31 0xc3 0xc9 0xc5 0xcc 0x8f 0x9e 0x45 0xb5 0x1f 0x01 0xc2 0xaf 0x4f 0x09 0x09 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x8 status=0
GAP procedure initiated: advertise; disc_mode=2 adv_channel_map=0 own_addr_type=0 adv_filter_policy=0 adv_itvl_min=0 adv_itvl_max=0
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0006 len=15
0x06 0x20 0x0f 0x30 0x00 0x60 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x07 0x00
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x6 status=0
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x000a len=1
0x0a 0x20 0x01 0x01
Command complete: cmd_pkts=5 ogf=0x8 ocf=0xa status=0
I (6953) MIDIOUTNIMBLE :: OPENPORT: OPENPORT Waiting for a client connection to notify...
LE connection complete. handle=0 role=1 paddrtype=0 addr=84.cc.a8.d.3b.ba local_rpa=0.0.0.0.0.0 peer_rpa=0.0.0.0.0.0 itvl=40 latency=0 spvn_tmo=256 mca=1
W (8253) NIMBLE_ONCONNECT: -----
GAP procedure initiated: advertise; disc_mode=2 adv_channel_map=0 own_addr_type=0 adv_filter_policy=0 adv_itvl_min=0 adv_itvl_max=0
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0006 len=15
0x06 0x20 0x0f 0x30 0x00 0x60 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x07 0x00
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x6 status=0
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x000a len=1
0x0a 0x20 0x01 0x01
Command complete: cmd_pkts=5 ogf=0x8 ocf=0xa status=0
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0016 len=2
0x16 0x20 0x02 0x00 0x00
Command Status: status=0 cmd_pkts=5 ocf=0x16 ogf=0x8
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=7 data=0x03 0x00 0x04 0x00 0x02 0x00 0x01
rxed att command: mtu req; conn=0 mtu=256
txed att command: mtu rsp; conn=0 mtu=256
host tx hci data; handle=0 length=7
ble_hs_hci_acl_tx(): 0x00 0x00 0x07 0x00 0x03 0x00 0x04 0x00 0x03 0x00 0x01
LE Remote Used Features. handle=0 feat=ff 00 00 00 00 00 00 00
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=27 data=0x17 0x00 0x04 0x00 0x06 0x01 0x00 0xff 0xff 0x00 0x28 0x4b 0x91 0x31 0xc3 0xc9 0xc5 0xcc 0x8f 0x9e 0x45 0xb5 0x1f 0x01 0xc2 0xaf 0x4f
rxed att command: find type value req; conn=0 start_handle=0x0001 end_handle=0xffff attr_type=10240
txed att command: find type value rsp; conn=0
host tx hci data; handle=0 length=13
ble_hs_hci_acl_tx(): 0x00 0x00 0x0d 0x00 0x09 0x00 0x04 0x00 0x07 0x0a 0x00 0x0d 0x00 0x0e 0x00 0x11 0x00
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=27 data=0x17 0x00 0x04 0x00 0x06 0x12 0x00 0xff 0xff 0x00 0x28 0x4b 0x91 0x31 0xc3 0xc9 0xc5 0xcc 0x8f 0x9e 0x45 0xb5 0x1f 0x01 0xc2 0xaf 0x4f
rxed att command: find type value req; conn=0 start_handle=0x0012 end_handle=0xffff attr_type=10240
txed att command: error rsp; conn=0 req_op=6 handle=0x0012 error_code=10
host tx hci data; handle=0 length=9
ble_hs_hci_acl_tx(): 0x00 0x00 0x09 0x00 0x05 0x00 0x04 0x00 0x01 0x06 0x12 0x00 0x0a
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=11 data=0x07 0x00 0x04 0x00 0x08 0x0e 0x00 0x11 0x00 0x03 0x28
rxed att command: read type req; conn=0 start_handle=0x000e end_handle=0x0011
txed att command: read type rsp; conn=0 length=21
host tx hci data; handle=0 length=27
ble_hs_hci_acl_tx(): 0x00 0x00 0x1b 0x00 0x17 0x00 0x04 0x00 0x09 0x15 0x0f 0x00 0x1a 0x10 0x00 0xa8 0x26 0x1b 0x36 0x07 0xea 0xf5 0xb7 0x88 0x46 0xe1 0x36 0x3e 0x48 0xb5 0xbe
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=11 data=0x07 0x00 0x04 0x00 0x08 0x10 0x00 0x11 0x00 0x03 0x28
rxed att command: read type req; conn=0 start_handle=0x0010 end_handle=0x0011
txed att command: error rsp; conn=0 req_op=8 handle=0x0010 error_code=10
host tx hci data; handle=0 length=9
ble_hs_hci_acl_tx(): 0x00 0x00 0x09 0x00 0x05 0x00 0x04 0x00 0x01 0x08 0x10 0x00 0x0a
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=7 data=0x03 0x00 0x04 0x00 0x0a 0x10 0x00
rxed att command: read req; conn=0 handle=0x0010
host tx hci data; handle=0 length=5
ble_hs_hci_acl_tx(): 0x00 0x00 0x05 0x00 0x01 0x00 0x04 0x00 0x0b
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=11 data=0x07 0x00 0x04 0x00 0x08 0x10 0x00 0x11 0x00 0x03 0x28
rxed att command: read type req; conn=0 start_handle=0x0010 end_handle=0x0011
txed att command: error rsp; conn=0 req_op=8 handle=0x0010 error_code=10
host tx hci data; handle=0 length=9
ble_hs_hci_acl_tx(): 0x00 0x00 0x09 0x00 0x05 0x00 0x04 0x00 0x01 0x08 0x10 0x00 0x0a
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=9 data=0x05 0x00 0x04 0x00 0x04 0x11 0x00 0x11 0x00
rxed att command: find info req; conn=0 start_handle=0x0011 end_handle=0x0011
txed att command: find info rsp; conn=0 format=1
host tx hci data; handle=0 length=10
ble_hs_hci_acl_tx(): 0x00 0x00 0x0a 0x00 0x06 0x00 0x04 0x00 0x05 0x01 0x11 0x00 0x02 0x29
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=9 data=0x05 0x00 0x04 0x00 0x52 0x11 0x00 0x01 0x00
rxed att command: write cmd; conn=0 handle=0x0011
I (11963) MIDIOUTNIMBLE :: OPENPORT: Number of connected peers: 1
E (11963) FCKX_SEQ: PROCEEDING with TEST_RECORDER example (a connection should be available here, because tested before)
E (11973) FCKX_SEQ: thru = new MIDIThru;
E (11983) FCKX_SEQ: Entering Idle Loop of test_recorder
E (11983) FCKX_SEQ: USE GUI TO ISSUE COMMANDS