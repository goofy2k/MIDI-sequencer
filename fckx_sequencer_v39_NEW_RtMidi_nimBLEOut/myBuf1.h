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
Wrote 1257728 bytes (749224 compressed) at 0x00010000 in 18.3 seconds (effective 550.0 kbit/s)...
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
I (103) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x35790 (219024) map
I (204) esp_image: segment 1: paddr=0x000457b8 vaddr=0x3ffbdb60 size=0x042cc ( 17100) load
I (212) esp_image: segment 2: paddr=0x00049a8c vaddr=0x40080000 size=0x00404 (  1028) load
0x40080000: _WindowOverflow4 at C:/Users/Fred/esp-idf/components/freertos/xtensa/xtensa_vectors.S:1730

I (213) esp_image: segment 3: paddr=0x00049e98 vaddr=0x40080404 size=0x06180 ( 24960) load
I (231) esp_image: segment 4: paddr=0x00050020 vaddr=0x400d0020 size=0xdede4 (912868) map
0x400d0020: _stext at ??:?

I (612) esp_image: segment 5: paddr=0x0012ee0c vaddr=0x40086584 size=0x142c8 ( 82632) load
0x40086584: coex_bb_reset_unlock at ??:?

I (667) boot: Loaded app from partition at offset 0x10000
I (667) boot: Disabling RNG early entropy source...
I (668) psram: This chip is ESP32-D0WD
I (673) spiram: Found 64MBit SPI RAM device
I (677) spiram: SPI RAM mode: flash 40m sram 40m
I (682) spiram: PSRAM initialized, cache is in low/high (2-core) mode.
I (689) cpu_start: Pro cpu up.
I (693) cpu_start: Application information:
I (698) cpu_start: Project name:     fckx_sequencer
I (703) cpu_start: App version:      f6f7de9-dirty
I (709) cpu_start: Compile time:     Mar 14 2022 15:08:00
I (715) cpu_start: ELF file SHA256:  544a9f46afa08b5c...
I (721) cpu_start: ESP-IDF:          v4.2-dirty
I (726) cpu_start: Starting app cpu, entry point is 0x40081bc4
0x40081bc4: call_start_cpu1 at C:/Users/Fred/esp-idf/components/esp32/cpu_start.c:287

I (0) cpu_start: App cpu up.
I (1620) spiram: SPI SRAM memory test OK
D (1621) memory_layout: Checking 12 reserved memory ranges:
D (1621) memory_layout: Reserved memory range 0x3f800000 - 0x3fc00000
D (1626) memory_layout: Reserved memory range 0x3ffae000 - 0x3ffae6e0
D (1632) memory_layout: Reserved memory range 0x3ffae6e0 - 0x3ffaff10
D (1639) memory_layout: Reserved memory range 0x3ffb0000 - 0x3ffb6388
D (1645) memory_layout: Reserved memory range 0x3ffb8000 - 0x3ffb9a20
D (1652) memory_layout: Reserved memory range 0x3ffbdb28 - 0x3ffbdb5c
D (1658) memory_layout: Reserved memory range 0x3ffbdb60 - 0x3ffc9d00
D (1665) memory_layout: Reserved memory range 0x3ffe0000 - 0x3ffe0440
D (1672) memory_layout: Reserved memory range 0x3ffe3f20 - 0x3ffe4350
D (1678) memory_layout: Reserved memory range 0x40070000 - 0x40078000
D (1685) memory_layout: Reserved memory range 0x40078000 - 0x40080000
0x40080000: _WindowOverflow4 at C:/Users/Fred/esp-idf/components/freertos/xtensa/xtensa_vectors.S:1730

D (1691) memory_layout: Reserved memory range 0x40080000 - 0x4009a84c
0x40080000: _WindowOverflow4 at C:/Users/Fred/esp-idf/components/freertos/xtensa/xtensa_vectors.S:1730

D (1698) memory_layout: Building list of available memory regions:
D (1704) memory_layout: Available memory region 0x3ffaff10 - 0x3ffb0000
D (1710) memory_layout: Available memory region 0x3ffb6388 - 0x3ffb8000
D (1717) memory_layout: Available memory region 0x3ffb9a20 - 0x3ffbdb28
D (1724) memory_layout: Available memory region 0x3ffc9d00 - 0x3ffca000
D (1731) memory_layout: Available memory region 0x3ffca000 - 0x3ffcc000
D (1737) memory_layout: Available memory region 0x3ffcc000 - 0x3ffce000
D (1744) memory_layout: Available memory region 0x3ffce000 - 0x3ffd0000
D (1751) memory_layout: Available memory region 0x3ffd0000 - 0x3ffd2000
D (1757) memory_layout: Available memory region 0x3ffd2000 - 0x3ffd4000
D (1764) memory_layout: Available memory region 0x3ffd4000 - 0x3ffd6000
D (1771) memory_layout: Available memory region 0x3ffd6000 - 0x3ffd8000
D (1777) memory_layout: Available memory region 0x3ffd8000 - 0x3ffda000
D (1784) memory_layout: Available memory region 0x3ffda000 - 0x3ffdc000
D (1791) memory_layout: Available memory region 0x3ffdc000 - 0x3ffde000
D (1797) memory_layout: Available memory region 0x3ffde000 - 0x3ffe0000
D (1804) memory_layout: Available memory region 0x3ffe0440 - 0x3ffe3f20
D (1811) memory_layout: Available memory region 0x3ffe4350 - 0x3ffe8000
D (1817) memory_layout: Available memory region 0x3ffe8000 - 0x3fff0000
D (1824) memory_layout: Available memory region 0x3fff0000 - 0x3fff8000
D (1831) memory_layout: Available memory region 0x3fff8000 - 0x3fffc000
D (1837) memory_layout: Available memory region 0x3fffc000 - 0x40000000
D (1844) memory_layout: Available memory region 0x4009a84c - 0x4009c000
D (1851) memory_layout: Available memory region 0x4009c000 - 0x4009e000
D (1858) memory_layout: Available memory region 0x4009e000 - 0x400a0000
I (1864) heap_init: Initializing. RAM available for dynamic allocation:
D (1872) heap_init: New heap initialised at 0x3ffaff10
I (1877) heap_init: At 3FFAFF10 len 000000F0 (0 KiB): DRAM
D (1883) heap_init: New heap initialised at 0x3ffb6388
I (1888) heap_init: At 3FFB6388 len 00001C78 (7 KiB): DRAM
D (1894) heap_init: New heap initialised at 0x3ffb9a20
I (1899) heap_init: At 3FFB9A20 len 00004108 (16 KiB): DRAM
D (1906) heap_init: New heap initialised at 0x3ffc9d00
I (1911) heap_init: At 3FFC9D00 len 00016300 (88 KiB): DRAM
I (1917) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (1924) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
D (1930) heap_init: New heap initialised at 0x4009a84c
I (1935) heap_init: At 4009A84C len 000057B4 (21 KiB): IRAM
I (1942) cpu_start: Pro cpu start user code
I (1946) spiram: Adding pool of 4096K of external SPI memory to heap allocator
D (1962) clk: RTC_SLOW_CLK calibration value: 3389939
D (1971) intr_alloc: Connected src 46 to int 2 (cpu 0)
D (1972) intr_alloc: Connected src 17 to int 3 (cpu 0)
D (1977) cpu_start: Setting C++ exception workarounds.
D (1978) efuse: coding scheme 0
D (1981) efuse: coding scheme 0
D (1981) efuse: In EFUSE_BLK0__DATA3_REG is used 1 bits starting with 15 bit
D (1984) efuse: In EFUSE_BLK0__DATA5_REG is useD (1995) intr_alloc: Connected src 24 to int 9 (cpu 0)
D (2000) FLASH_HAL: extra_dummy: 1
d 1 bits starting with 20 bit
D (2004) spi_flash: trying chip: issi
D (2004) spi_flash: trying chip: gd
I (20W (2015) spi_flash: Detected size(16384k) larger than the size in the binary image header(8192k). Using the size in the binary image header.
04) spi_flash: detected chip: gd
I (2015) spi_flash: flash io: dio
I (2028) cpu_start: Starting scheduler D (0) efuse: coding scheme 0
D (0) efuse: coding scheme 0
on PRO CPU.
D (0) efuse: In EFUSE_BLK0__DATA3_REG is used 1 bits starting with 15 bit
D (0) efuse: In EFUSE_BLK0__DATA5_D (10) intr_alloc: Connected src 25 to int 2 (cpu 1)
I (20) cpu_start: Starting scheduler on APP CPU.
D (2086) heap_init: New heap initialised at 0x3ffe0440
D (2096) heap_init: New heap initialised at 0x3ffe4350
I (2096) spiram: Reserving pool of 32K of internal memory for DMA/internal allocations
D (2106) spiram: Allocating block of size 32768 bytes
D (2116) intr_alloc: Connected src 16 to int 12 (cpu 0)
REG is used 1 bits starting with 20 bit
W (2116) APP_MAIN: APP_MAIN ENTRY
W (2116) APP_MAIN: APP_MAIN ENTRY
I (2126) APP_MAIN: [APP] Startup..
I (2126) APP_MAIN: [APP] Free memory: 4381372 bytes
I (2126) APP_MAIN: [APP] IDF version: v4.2-dirty
W (2136) APP_MAIN: Initialize WIFI connection here
D (2136) partition: Loading the partition table
D (2196) esp_netif_lwip: LwIP stack has been initialized
D (2196) esp_netif_lwip: esp-netif has been successfully initialized
I (2196) APP_MAIN: ESP_WIFI_MODE_STA
D (2196) esp_netif_lwip: esp-netif has been successfully initialized
D (2216) esp_netif_objects: esp_netif_add_to_list 0x3ffd3f70
D (2216) esp_netif_objects: esp_netif_add_to_list netif added successfully (total netifs: 1)
D (2226) nvs: nvs_open_from_partition misc 1
D (2226) nvs: nvs_get_str_or_blob log
I (2246) wifi:wifi driver task: 3ffd5720, prio:23, stack:3072, core=0
I (2246) system_api: BD (2246) efuse: coding scheme 0
ase MAC address is not set
I (2246) system_api: read default base MAC address from EFUSE
D (2246) efuse: In EFUSED (2256) efuse: coding scheme 0
_BLK0__DATA2_REG is used 8 bits starting with 8 bit
D (2266) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 D (2276) efuse: coding scheme 0
D (2276) efuse: coding scheme 0
bit
D (2276) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (2276) efuse: In EFUSE_BLK0__DATA1_RED (2286) efuse: coding scheme 0
D (2296) efuse: coding scheme 0
G is used 8 bits starting with 16 bit
D (2296) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (229D (2306) efuse: coding scheme 0
6) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 0 bit
D (2316) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
D (2326) nvs: nvs_open_from_partition nvs.net80211 1
D (2326) nvs: nvs_get opmode 1
D (2326) nvs: nvs_get_str_or_blob sta.ssid
D (2336) nvs: nvs_get_str_or_blob sta.mac
D (2336) nvs: nvs_get sta.authmode 1
D (2336) nvs: nvs_get_str_or_blob sta.pswd
D (2346) nvs: nvs_get_str_or_blob sta.pmk
D (2346) nvs: nvs_get sta.chan 1
D (2346) nvs: nvs_get auto.conn 1
D (2356) nvs: nvs_get bssid.set 1
D (2356) nvs: nvs_get_str_or_blob sta.bssid
D (2356) nvs: nvs_get sta.lis_intval 2
D (2366) nvs: nvs_get sta.phym 1
D (2366) nvs: nvs_get sta.phybw 1
D (2366) nvs: nvs_get_str_or_blob sta.apsw
D (2376) nvs: nvs_get_str_or_blob sta.apinfo
D (2386) nvs: nvs_get sta.scan_method 1
D (2386) nvs: nvs_get sta.sort_method 1
D (2386) nvs: nvs_get sta.minrssi 1
D (2386) nvs: nvs_get sta.minauth 1
D (2386) nvs: nvs_get sta.pmf_e 1
D (2396) nvs: nvs_get sta.pmf_r 1
D (2406) nvs: nvs_get_str_or_blob ap.ssid
D (2406) nvs: nvs_get_str_or_blob ap.mac
D (2416) nvs: nvs_get_str_or_blob ap.passwd
D (2416) nvs: nvs_get_str_or_blob ap.pmk
D (2416) nvs: nvs_get ap.chan 1
D (2426) nvs: nvs_get ap.authmode 1
D (2426) nvs: nvs_get ap.hidden 1
D (2426) nvs: nvs_get ap.max.conn 1
D (2436) nvs: nvs_get bcn.interval 2
D (2436) nvs: nvs_get ap.phym 1
D (2436) nvs: nvs_get ap.phybw 1
D (2446) nvs: nvs_get ap.sndchan 1
D (2446) nvs: nvs_get ap.pmf_e 1
D (2446) nvs: nvs_get ap.pmf_r 1
D (2446) nvs: nvs_get lorate 1
D (2456) nvs: nvs_get_str_or_blob country
D (2456) nvs: nvs_set ap.sndchan 1 1
D (2456) nvs: nvs_set_blob sta.mac 6
D (2466) nvs: nvs_set_blob ap.mac 6
I (2466) wifi:wifi firmware version: 1865b55
I (2466) wifi:wifi certification version: v7.0
I (2476) wifi:config NVS flash: enabled
I (2476) wifi:config nano formating: enabled
I (2476) wifi:Init data frame dynamic rx buffer num: 32
I (2486) wifi:Init management frame dynamic rx buffer num: 32
I (2486) wifi:Init management short buffer num: 32
I (2506) wifi:Init static tx buffer num: 16
I (2506) wifi:Init tx cache buffer num: 32
I (2506) wifi:Init static rx buffer size: 1600
I (2516) wifi:Init static rx buffer num: 10
I (2516) wifi:Init dynamic rx buffer num: 32
I (2516) wifi_init: rx ba win: 6
I (2526) wifi_init: tcpip mbox: 32
I (2526) wifi_init: udp mbox: 6
I (2536) wifi_init: tcp mbox: 6
I (2536) wifi_init: tcp tx win: 5744
I (2536) wifi_init: tcp rx win: 5744
I (2546) wifi_init: tcp mss: 1440
I (2546) wifi_init: WiFi/LWIP prefer SPIRAM
D (2546) phy_init: loading PHY init data from application binary
D (2556) nvs: nvs_open_from_partition phy 0
D (2556) nvs: nvs_get calD (2566) efuse: coding scheme 0
_version 4
D (2566) nvs: nvs_get_str_or_blob cal_mac
D (2566) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting wiD (2586) efuse: coding scheme 0
D (2586) efuse: coding scheme 0
th 8 bit
D (2586) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (2586) efuse: In EFUSE_BLK0__DATAD (2596) efuse: coding scheme 0
D (2606) efuse: coding scheme 0
1_REG is used 8 bits starting with 24 bit
D (2606) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 16 bit
DD (2616) efuse: coding scheme 0
 (2606) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (2616) efuse: In EFUSE_BLK0__DATA1_REG is used D (2626) efuse: coding scheme 0
8 bits starting with 0 bit
D (2636) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
D (2636) nvs: nvD (2656) efuse: coding scheme 0
s_get_str_or_blob cal_data
D (2656) nvs: nvs_close 3
D (2656) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting wiD (2656) efuse: coding scheme 0
D (2666) efuse: coding scheme 0
th 8 bit
D (2666) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (2666) efuse: In EFUSE_BLK0__DATAD (2676) efuse: coding scheme 0
D (2676) efuse: coding scheme 0
1_REG is used 8 bits starting with 24 bit
D (2676) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 16 bit
DD (2696) efuse: coding scheme 0
 (2686) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (2696) efuse: In EFUSE_BLK0__DATA1_REG is used D (2706) efuse: coding scheme 0
I (2796) phy: phy_version: 4500, 0cd6843, Sep 17 2020, 15:37:07, 0, 0
8 bits starting with 0 bit
D (2716) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
I (2796) wifi:mode : sta (84:cc:a8:0d:3b:70)
D (2806) wifi_init_default: wifi_start esp-netif:0x3ffd3f70 event-id2
D (2806) wifi_init_default: WIFI mac address: 84 cc a8 d 3b 70
D (2816) esp_netif_handlers: esp_netif action has started with netif0x3ffd3f70 from event_id=2
D (2826) esp_netif_lwip: check: remote, if=0x3ffd3f70 fn=0x401036e0
0x401036e0: esp_netif_start_api at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:608


D (2826) esp_netif_lwip: esp_netif_start_api 0x3ffd3f70
D (2836) esp_netif_lwip: esp_netif_get_hostname esp_netif:0x3ffd3f70
D (2836) esp_netif_lwip: check: local, if=0x3ffd3f70 fn=0x40104124
0x40104124: esp_netif_update_default_netif_lwip at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:163


D (2846) esp_netif_lwip: esp_netif_update_default_netif_lwip 0x3ffd3f70
D (2846) esp_netif_lwip: call api in lwip: ret=0x0, give sem
I (2976) wifi:new:<8,0>, old:<1,0>, ap:<255,255>, sta:<8,0>, prof:1
I (2976) wifi:state: init -> auth (b0)
I (2996) wifi:state: auth -> assoc (0)
I (3056) wifi:state: assoc -> run (10)
I (3166) wifi:connected with Verhoeckx_glas, aid = 7, channel 8, BW20, bssid = 96:6a:b0:c3:3b:27
I (3166) wifi:security: WPA2-PSK, phy: bgn, rssi: -49
D (3166) nvs: nvs_set sta.chan 1 8
D (3176) nvs: nvs_set_blob sta.apinfo 700
I (3176) wifi:pm start, type: 1
D (3176) esp_netif_handlers: esp_netif action connected with netif0x3ffd3f70 from event_id=4
D (3186) esp_netif_lwip: check: remote, if=0x3ffd3f70 fn=0x40103900
0x40103900: esp_netif_up_api at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:1131


D (3196) esp_netif_lwip: esp_netif_up_api esp_netif:0x3ffd3f70
D (3196) esp_netif_lwip: check: local, if=0x3ffd3f70 fn=0x40104124
0x40104124: esp_netif_update_default_netif_lwip at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:163


D (3206) esp_netif_lwip: esp_netif_update_default_netif_lwip 0x3ffd3f70
D (3206) esp_netif_lwip: call api in lwip: ret=0x0, give sem
D (3216) esp_netif_lwip: check: remote, if=0x3ffd3f70 fn=0x401034d8
0x401034d8: esp_netif_dhcpc_start_api at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:929


D (3226) esp_netif_lwip: esp_netif_dhcpc_start_api esp_netif:0x3ffd3f70
D (3226) esp_netif_lwip: esp_netif_start_ip_lost_timer esp_netif:0x3ffd3f70
D (3236) esp_netif_lwip: if0x3ffd3f70 start ip lost tmr: no need start because netif=0x3ffd3ff4 interval=120 ip=0
D (3256) esp_netif_lwip: starting dhcp client
D (3256) esp_netif_lwip: call api in lwip: ret=0x0, give sem
I (3256) wifi:AP's beacon interval = 102400 us, DTIM period = 1
D (4696) esp_netif_lwip: esp_netif_dhcpc_cb lwip-netif:0x3ffd3ff4
D (4696) esp_netif_lwip: if0x3ffd3f70 ip changed=1
D (4696) wifi_init_default: Got IP wifi default handler entered
D (4706) esp_netif_handlers: esp_netif action got_ip with netif0x3ffd3f70 from event_id=0
I (4706) esp_netif_handlers: sta ip: 192.168.2.5, mask: 255.255.255.0, gw: 192.168.2.254
W (4716) APP_MAIN: Initialize MQTT connection here
E (4726) APP_MAIN: WAITING A WHILE FOR MQTT INITS TO FINISH
D (4736) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=7
I (4736) mqtt_event_handler_cb: Other event id:7
D (4776) TRANS_TCP: [sock=54] Connecting to server. IP: 145.53.71.16, Port: 1883
D (4796) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=1
I (4796) mqtt_event_handler_cb: MQTT_EVENT_CONNECTED
I (4796) mqtt_event_handler_cb: sent publish successful, msg_id=41801
I (4806) mqtt_event_handler_cb: sent subscribe successful, msg_id=24511
I (4816) mqtt_event_handler_cb: sent subscribe successful, msg_id=8388
I (4816) mqtt_event_handler_cb: sent unsubscribe successful, msg_id=41678
I (4826) mqtt_event_handler_cb: sent subscribe successful, msg_id=57639
I (4826) mqtt_event_handler_cb: sent subscribe successful, msg_id=12857
I (4836) mqtt_event_handler_cb: sent subscribe successful, msg_id=44235
I (4846) mqtt_event_handler_cb: sent subscribe successful, msg_id=62754
D (4846) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=5
I (4856) mqtt_event_handler_cb: MQTT_EVENT_PUBLISHED, msg_id=41801
D (4866) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4866) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=24511
I (4876) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4896) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4896) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=8388
I (4906) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4916) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=4
I (4916) mqtt_event_handler_cb: MQTT_EVENT_UNSUBSCRIBED, msg_id=41678
D (4926) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4936) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=57639
I (4936) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4946) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4956) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=12857
I (4956) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (5026) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5026) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=22533
I (5026) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (5106) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5116) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=44235
I (5116) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (5126) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5136) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=36364
I (5136) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (5146) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5156) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=62754
I (5156) mqtt_event_handler_cb: sent publish successful, msg_id=0
E (5726) APP_MAIN: CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDER
W (5726) APP_MAIN: ENTERING init_test_recorder()
MyNotifier::NotifyRECEIVED EVENT of type 1  with number 314132345
INIT_TEST_RECORDER
Testing Outbuf
31 HEXADECIMAL: 1F
Tested Outbuf
E (5736) FCKX_SEQ: Entering test_recorder of TEST_RECORDER example
E (5736) FCKX_SEQ: CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDER
W (5766) MIDIMANAGER::INIT: Enter
Executing MIDIManager::Init()
Going to create temp MidiOutNimBLE for detecting nr of ports
W (5766) MIDIOUTNIMBLE :: INITIALIZE: Initialize nimBLEOutdriver MidiOutNimBLE :: initialize
D (5766) BTDM_INIT: Release DRAM [0x3ffb2730] - [0x3ffb6388]
I (5776) BTDM_INIT: BT controller compile version [3723d5b]
D (5776) BTDM_INIT: .data initialise [0x3ffae6e0] <== [0x4000d890]
D (5796) BTDM_INIT: .bss initialise [0x3ffb0000] - [0x3ffb09a8]
D (5796) BTDM_INIT: .bss initialise [0x3ffb09a8] - [0x3ffb1ddc]
D (5806) BTDM_INIT: .bss initialise [0x3ffb1ddc] - [0x3ffb2730]
D (5816) BTDM_INIT: .bss initialise [0x3ffb8000] - [0x3ffb9a20]
D (5816) BTDM_INIT: .bss initialise [0x3ffbdb28] - [0x3ffbdb5c]
D (5826) phy_init: loading PHY init data from application binary
D (5826) nvs: nvs_open_from_partition phy 0
D (5836) efuse: coding scheme 0
D (5826) nvs: nvs_get cal_version 4
D (5836) nvs: nvs_get_str_or_blob cal_mac
D (5836) efuse: In EFUSE_BLK0__DATA2_REG iD (5846) efuse: coding scheme 0
D (5856) efuse: coding scheme 0
s used 8 bits starting with 8 bit
D (5856) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (5856) eD (5866) efuse: coding scheme 0
fuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (5866) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits sD (5886) efuse: coding scheme 0
D (5886) efuse: coding scheme 0
tarting with 16 bit
D (5886) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (5886) efuse: In EFUSED (5896) efuse: coding scheme 0
_BLK0__DATA1_REG is used 8 bits starting with 0 bit
D (5906) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16D (5926) efuse: coding scheme 0
 bit
D (5916) nvs: nvs_get_str_or_blob cal_data
D (5926) nvs: nvs_close 4
D (5926) efuse: In EFUSE_BLK0__DATA2_REG iD (5926) efuse: coding scheme 0
D (5936) efuse: coding scheme 0
s used 8 bits starting with 8 bit
D (5936) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (5936) eD (5946) efuse: coding scheme 0
fuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (5956) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits sD (5966) efuse: coding scheme 0
D (5966) efuse: coding scheme 0
tarting with 16 bit
D (5966) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (5966) efuse: In EFUSED (5976) efuse: coding scheme 0
_BLK0__DATA1_REG is used 8 bits starting with 0 bit
D (5986) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
D (6236) nvs: nvs_open_from_partition nimble_bond 1
D (6236) nvs: nvs_get_str_or_blob our_sec_1
D (6236) nvs: nvs_close 5
D (6236) nvs: nvs_open_from_partition nimble_bond 1
D (6236) nvs: nvs_get_str_or_blob our_sec_2
D (6236) nvs: nvs_close 6
D (6246) nvs: nvs_open_from_partition nimble_bond 1
D (6246) nvs: nvs_get_str_or_blob our_sec_3
D (6246) nvs: nvs_close 7
D (6256) NIMBLE_NVS: ble_store_config_our_secs restored 0 bonds
D (6256) nvs: nvs_open_from_partition nimble_bond 1
D (6266) nvs: nvs_get_str_or_blob peer_sec_1
D (6266) nvs: nvs_close 8
D (6266) nvs: nvs_open_from_partition nimble_bond 1
D (6276) nvs: nvs_get_str_or_blob peer_sec_2
D (6276) nvs: nvs_close 9
D (6286) nvs: nvs_open_from_partition nimble_bond 1
D (6296) nvs: nvs_get_str_or_blob peer_sec_3
D (6296) nvs: nvs_close 10
D (6296) NIMBLE_NVS: ble_store_config_peer_secs restored 0 bonds
D (6306) nvs: nvs_open_from_partition nimble_bond 1
D (6306) nvs: nvs_get_str_or_blob cccd_sec_1
D (6316) nvs: nvs_close 11
D (6316) nvs: nvs_open_from_partition nimble_bond 1
D (6316) nvs: nvs_get_str_or_blob cccd_sec_2
D (6326) nvs: nvs_close 12
D (6326) nvs: nvs_open_from_partition nimble_bond 1
D (6336) nvs: nvs_get_str_or_blob cccd_sec_3
D (6336) nvs: nvs_close 13
D (6336) nvs: nvs_open_from_partition nimble_bond 1
D (6346) nvs: nvs_get_str_or_blob cccd_sec_4
D (6346) nvs: nvs_close 14
D (6346) nvs: nvs_open_from_partition nimble_bond 1
D (6356) nvs: nvs_get_str_or_blob cccd_sec_5
D (6356) nvs: nvs_close 15
D (6356) nvs: nvs_open_from_partition nimble_bond 1
D (6366) nvs: nvs_get_str_or_blob cccd_sec_6
D (6366) nvs: nvs_close 16
D (6366) nvs: nvs_open_from_partition nimble_bond 1
D (6376) nvs: nvs_get_str_or_blob cccd_sec_7
D (6376) nvs: nvs_close 17
D (6386) nvs: nvs_open_from_partition nimble_bond 1
D (6396) nvs: nvs_get_str_or_blob cccd_sec_8
D (6396) nvs: nvs_close 18
D (6396) NIMBLE_NVS: ble_store_config_cccds restored 0 bonds
D (6406) nvs: nvs_open_from_partition nimble_bond 1
D (6406) nvs: nvs_get_str_or_blob p_dev_rec_1
D (6416) nvs: nvs_close 19
D (6416) nvs: nvs_open_from_partition nimble_bond 1
D (6416) nvs: nvs_get_str_or_blob p_dev_rec_2
D (6426) nvs: nvs_close 20
D (6426) nvs: nvs_open_from_partition nimble_bond 1
D (6436) nvs: nvs_get_str_or_blob p_dev_rec_3
D (6436) nvs: nvs_close 21
D (6436) nvs: nvs_open_from_partition nimble_bond 1
D (6446) nvs: nvs_get_str_or_blob p_dev_rec_4
D (6446) nvs: nvs_close 22
D (6446) NIMBLE_NVS: peer_dev_rec restored 0 records
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
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x18 status=0 rand=0x8346bb733e306936
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0018 len=0
0x18 0x20 0x00
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x18 status=0 rand=0x2faf3c5a278f57df
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
W (6636) MIDIOUTNIMBLE :: INITIALIZE: NimBLEDevice created
W (6636) MIDIOUTNIMBLE :: INITIALIZE: BLE server created
W (6646) MIDIOUTNIMBLE :: INITIALIZE: BLE server callbacks created
W (6646) MIDIOUTNIMBLE :: INITIALIZE: BLE server service created
W (6656) MIDIOUTNIMBLE :: INITIALIZE: BLE server MIDI characteristic created
W (6666) MIDIOUTNIMBLE :: INITIALIZE: portName[0] MIDIport
W (6666) MIDIOUTNIMBLE :: INITIALIZE: BLE service started
Created temp MidiOutNimBLE for detecting nr of ports
E (6686) NICMIDI DRIVER: start creation of MidiOutNimBLE port
W (6686) MIDIOUTNIMBLE :: INITIALIZE: Initialize nimBLEOutdriver MidiOutNimBLE :: initialize
W (6696) MIDIOUTNIMBLE :: INITIALIZE: NimBLEDevice created
W (6696) MIDIOUTNIMBLE :: INITIALIZE: BLE server created
W (6706) MIDIOUTNIMBLE :: INITIALIZE: BLE server callbacks created
W (6726) MIDIOUTNIMBLE :: INITIALIZE: BLE server service created
W (6726) MIDIOUTNIMBLE :: INITIALIZE: BLE server MIDI characteristic created
W (6736) MIDIOUTNIMBLE :: INITIALIZE: portName[0] MIDIport
W (6736) MIDIOUTNIMBLE :: INITIALIZE: BLE service started
E (6746) NICMIDI DRIVER: executed creation of MidiOutNimBLE port
Going to create temp MQTTMidiIn for detecting nr of ports
W (6756) MQTTDRIVER: MQTTMidiIn instantiation
W (6756) MQTTDRIVER: MQTTMidiIn :: initialize
Created temp MQTTMidiIn for detecting nr of ports
E (6776) NICMIDI DRIVER INPUT: start creation of MQTTMidiIn port
W (6776) MQTTDRIVER: MQTTMidiIn instantiation
W (6786) MQTTDRIVER: MQTTMidiIn :: initialize
E (6786) MQTTDRIVER: MQTTMidiIn::setCallback: STORING CALLBACK IN inputData_
E (6796) NICMIDI DRIVER INPUT: executed creation of MQTTMidiIn port
W (6796) MIDIMANAGER::INIT: Pos1
W (6806) MQTTDRIVER: Closed MQTTMidiIn NO CLEANUP: BEWARE of MEMORY LEAKS
W (6806) MIDIMANAGER::INIT: Pos2
Timer resolution 10
portTICK_PERIOD_MS 10
Calculated periodTicks 1
Timer started with 1 system ticks resolution
Timer open with 10 msecs resolution
Exiting MIDIManager::Init() Found 1 midi out and 1 midi in
W (6836) MIDIMANAGER::INIT: Pos3 (exit)
111 AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
E (6846) SEQUENCER: track_states.size() 17
GUI EVENT: All       GENERAL RESET
E (6856) SEQUENCER: track_states.size() 17
E (6856) SEQUENCER: track_states.size() 17
E (6866) SEQUENCER: track_states.size() 17
GUI EVENT: All       GENERAL RESET
E (6876) SEQUENCER: track_states.size() 17
GUI EVENT: All       GENERAL RESET
11a AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
11b AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
DEBUG MIDIThru constructor
11c AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
11d AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
11e AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
W (6916) MQTTDRIVER: open MQTTMidiIn port
IN Port MQTT_In open
E (6926) NICMIDI DRIVER: MIDIOutDriver::OpenPort() port_id 0
W (6926) MIDIOUTNIMBLE :: OPENPORT: Initialize nimBLEOutdriver MidiOutNimBLE :: openPort nr 0
OPENPORT2 ENTEREDE (6936) MIDIOUTNIMBLE :: OPENPORT: A valid connection already exists nevertheless start advertising (DIRTYOPEN!)
E (6946) MIDIOUTNIMBLE :: OPENPORT: >>>>>take the opportunity to list all available Characteristics somewhere over here <<<<<<<
I (6956) MIDIOUTNIMBLE :: OPENPORT: Prepare advertising
I (6966) MIDIOUTNIMBLE :: OPENPORT: Start advertising
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
I (7036) MIDIOUTNIMBLE :: OPENPORT: OPENPORT Waiting for a client connection to notify...
LE connection complete. handle=0 role=1 paddrtype=0 addr=84.cc.a8.d.3b.ba local_rpa=0.0.0.0.0.0 peer_rpa=0.0.0.0.0.0 itvl=40 latency=0 spvn_tmo=256 mca=1
W (8696) NIMBLE_ONCONNECT: -----
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
I (12046) MIDIOUTNIMBLE :: OPENPORT: Number of connected peers: 1
OUT Port MIDIport open
WAIT FOR PEER1
WAIT FOR PEER on MIDI out
set Midi Characteristic callbacks
Timer stopped by MIDITimer::HardStop()
MIDITimer::freeRTOSTimerCallback() num_open: 0 **STOPPING freeRTOSTimer**
Timer resolution 10
portTICK_PERIOD_MS 10
Calculated periodTicks 1
Timer started with 1 system ticks resolution
Timer open with 10 msecs resolution
E (12076) FCKX_SEQ: PROCEEDING with TEST_RECORDER example (a connection should be available here, because tested before)
E (12086) FCKX_SEQ: thru = new MIDIThru;
DEBUG MIDIThru constructor
E (12096) FCKX_SEQ: Entering Idle Loop of test_recorder
E (12096) FCKX_SEQ: USE GUI TO ISSUE COMMANDS