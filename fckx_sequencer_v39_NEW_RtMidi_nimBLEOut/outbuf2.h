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
I (103) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x34d1c (216348) map
I (203) esp_image: segment 1: paddr=0x00044d44 vaddr=0x3ffbdb60 size=0x042cc ( 17100) load
I (211) esp_image: segment 2: paddr=0x00049018 vaddr=0x40080000 size=0x00404 (  1028) load
0x40080000: _WindowOverflow4 at C:/Users/Fred/esp-idf/components/freertos/xtensa/xtensa_vectors.S:1730

I (211) esp_image: segment 3: paddr=0x00049424 vaddr=0x40080404 size=0x06bf4 ( 27636) load
I (232) esp_image: segment 4: paddr=0x00050020 vaddr=0x400d0020 size=0xde8b0 (911536) map
0x400d0020: _stext at ??:?

I (612) esp_image: segment 5: paddr=0x0012e8d8 vaddr=0x40086ff8 size=0x13854 ( 79956) load
0x40086ff8: coex_timer_ts_end_disalarm at ??:?

I (665) boot: Loaded app from partition at offset 0x10000
I (666) boot: Disabling RNG early entropy source...
I (666) psram: This chip is ESP32-D0WD
I (671) spiram: Found 64MBit SPI RAM device
I (675) spiram: SPI RAM mode: flash 40m sram 40m
I (680) spiram: PSRAM initialized, cache is in low/high (2-core) mode.
I (688) cpu_start: Pro cpu up.
I (691) cpu_start: Application information:
I (696) cpu_start: Project name:     fckx_sequencer
I (702) cpu_start: App version:      f6f7de9-dirty
I (707) cpu_start: Compile time:     Mar 14 2022 15:08:00
I (713) cpu_start: ELF file SHA256:  3092d4030ab9fb0a...
I (719) cpu_start: ESP-IDF:          v4.2-dirty
I (725) cpu_start: Starting app cpu, entry point is 0x40081bc4
0x40081bc4: call_start_cpu1 at C:/Users/Fred/esp-idf/components/esp32/cpu_start.c:287

I (0) cpu_start: App cpu up.
I (1619) spiram: SPI SRAM memory test OK
D (1619) memory_layout: Checking 12 reserved memory ranges:
D (1620) memory_layout: Reserved memory range 0x3f800000 - 0x3fc00000
D (1625) memory_layout: Reserved memory range 0x3ffae000 - 0x3ffae6e0
D (1631) memory_layout: Reserved memory range 0x3ffae6e0 - 0x3ffaff10
D (1638) memory_layout: Reserved memory range 0x3ffb0000 - 0x3ffb6388
D (1644) memory_layout: Reserved memory range 0x3ffb8000 - 0x3ffb9a20
D (1651) memory_layout: Reserved memory range 0x3ffbdb28 - 0x3ffbdb5c
D (1657) memory_layout: Reserved memory range 0x3ffbdb60 - 0x3ffc9d00
D (1664) memory_layout: Reserved memory range 0x3ffe0000 - 0x3ffe0440
D (1670) memory_layout: Reserved memory range 0x3ffe3f20 - 0x3ffe4350
D (1677) memory_layout: Reserved memory range 0x40070000 - 0x40078000
D (1683) memory_layout: Reserved memory range 0x40078000 - 0x40080000
0x40080000: _WindowOverflow4 at C:/Users/Fred/esp-idf/components/freertos/xtensa/xtensa_vectors.S:1730

D (1690) memory_layout: Reserved memory range 0x40080000 - 0x4009a84c
0x40080000: _WindowOverflow4 at C:/Users/Fred/esp-idf/components/freertos/xtensa/xtensa_vectors.S:1730

D (1696) memory_layout: Building list of available memory regions:
D (1702) memory_layout: Available memory region 0x3ffaff10 - 0x3ffb0000
D (1709) memory_layout: Available memory region 0x3ffb6388 - 0x3ffb8000
D (1716) memory_layout: Available memory region 0x3ffb9a20 - 0x3ffbdb28
D (1722) memory_layout: Available memory region 0x3ffc9d00 - 0x3ffca000
D (1729) memory_layout: Available memory region 0x3ffca000 - 0x3ffcc000
D (1736) memory_layout: Available memory region 0x3ffcc000 - 0x3ffce000
D (1742) memory_layout: Available memory region 0x3ffce000 - 0x3ffd0000
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
D (1829) memory_layout: Available memory region 0x3fff8000 - 0x3fffc000
D (1836) memory_layout: Available memory region 0x3fffc000 - 0x40000000
D (1843) memory_layout: Available memory region 0x4009a84c - 0x4009c000
D (1849) memory_layout: Available memory region 0x4009c000 - 0x4009e000
D (1856) memory_layout: Available memory region 0x4009e000 - 0x400a0000
I (1863) heap_init: Initializing. RAM available for dynamic allocation:
D (1870) heap_init: New heap initialised at 0x3ffaff10
I (1875) heap_init: At 3FFAFF10 len 000000F0 (0 KiB): DRAM
D (1881) heap_init: New heap initialised at 0x3ffb6388
I (1887) heap_init: At 3FFB6388 len 00001C78 (7 KiB): DRAM
D (1893) heap_init: New heap initialised at 0x3ffb9a20
I (1898) heap_init: At 3FFB9A20 len 00004108 (16 KiB): DRAM
D (1904) heap_init: New heap initialised at 0x3ffc9d00
I (1910) heap_init: At 3FFC9D00 len 00016300 (88 KiB): DRAM
I (1916) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (1922) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
D (1929) heap_init: New heap initialised at 0x4009a84c
I (1934) heap_init: At 4009A84C len 000057B4 (21 KiB): IRAM
I (1940) cpu_start: Pro cpu start user code
I (1945) spiram: Adding pool of 4096K of external SPI memory to heap allocator
D (1960) clk: RTC_SLOW_CLK calibration value: 3390336
D (1970) intr_alloc: Connected src 46 to int 2 (cpu 0)
D (1970) intr_alloc: Connected src 17 to int 3 (cpu 0)
D (1975) cpu_start: Setting C++ exception workarounds.
D (1976) efuse: coding scheme 0
D (1980) efuse: coding scheme 0
D (1979) efuse: In EFUSE_BLK0__DATA3_REG is used 1 bits starting with 15 bit
D (1982) efuse: In EFUSE_BLK0__DATA5_REG is useD (1994) intr_alloc: Connected src 24 to int 9 (cpu 0)
D (1999) FLASH_HAL: extra_dummy: 1
d 1 bits starting with 20 bit
D (2002) spi_flash: trying chip: issi
D (2002) spi_flash: trying chip: gd
I (20W (2013) spi_flash: Detected size(16384k) larger than the size in the binary image header(8192k). Using the size in the binary image header.
02) spi_flash: detected chip: gd
I (2013) spi_flash: flash io: dio
I (2027) cpu_start: Starting scheduler D (0) efuse: coding scheme 0
D (0) efuse: coding scheme 0
on PRO CPU.
D (0) efuse: In EFUSE_BLK0__DATA3_REG is used 1 bits starting with 15 bit
D (0) efuse: In EFUSE_BLK0__DATA5_D (10) intr_alloc: Connected src 25 to int 2 (cpu 1)
I (20) cpu_start: Starting scheduler on APP CPU.
D (2085) heap_init: New heap initialised at 0x3ffe0440
D (2095) heap_init: New heap initialised at 0x3ffe4350
I (2095) spiram: Reserving pool of 32K of internal memory for DMA/internal allocations
D (2105) spiram: Allocating block of size 32768 bytes
D (2115) intr_alloc: Connected src 16 to int 12 (cpu 0)
REG is used 1 bits starting with 20 bit
W (2115) APP_MAIN: APP_MAIN ENTRY
W (2115) APP_MAIN: APP_MAIN ENTRY
I (2125) APP_MAIN: [APP] Startup..
I (2125) APP_MAIN: [APP] Free memory: 4381372 bytes
I (2125) APP_MAIN: [APP] IDF version: v4.2-dirty
W (2135) APP_MAIN: Initialize WIFI connection here
D (2135) partition: Loading the partition table
D (2195) esp_netif_lwip: LwIP stack has been initialized
D (2195) esp_netif_lwip: esp-netif has been successfully initialized
I (2195) APP_MAIN: ESP_WIFI_MODE_STA
D (2195) esp_netif_lwip: esp-netif has been successfully initialized
D (2215) esp_netif_objects: esp_netif_add_to_list 0x3ffd3f70
D (2215) esp_netif_objects: esp_netif_add_to_list netif added successfully (total netifs: 1)
D (2225) nvs: nvs_open_from_partition misc 1
D (2225) nvs: nvs_get_str_or_blob log
I (2245) wifi:wifi driver task: 3ffd5720, prio:23, stack:3072, core=0
I (2245) system_api: BD (2245) efuse: coding scheme 0
ase MAC address is not set
I (2245) system_api: read default base MAC address from EFUSE
D (2245) efuse: In EFUSED (2255) efuse: coding scheme 0
_BLK0__DATA2_REG is used 8 bits starting with 8 bit
D (2265) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 D (2275) efuse: coding scheme 0
D (2275) efuse: coding scheme 0
bit
D (2275) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (2275) efuse: In EFUSE_BLK0__DATA1_RED (2285) efuse: coding scheme 0
D (2295) efuse: coding scheme 0
G is used 8 bits starting with 16 bit
D (2295) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (229D (2305) efuse: coding scheme 0
5) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 0 bit
D (2315) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
D (2325) nvs: nvs_open_from_partition nvs.net80211 1
D (2325) nvs: nvs_get opmode 1
D (2325) nvs: nvs_get_str_or_blob sta.ssid
D (2335) nvs: nvs_get_str_or_blob sta.mac
D (2335) nvs: nvs_get sta.authmode 1
D (2335) nvs: nvs_get_str_or_blob sta.pswd
D (2345) nvs: nvs_get_str_or_blob sta.pmk
D (2345) nvs: nvs_get sta.chan 1
D (2345) nvs: nvs_get auto.conn 1
D (2355) nvs: nvs_get bssid.set 1
D (2355) nvs: nvs_get_str_or_blob sta.bssid
D (2355) nvs: nvs_get sta.lis_intval 2
D (2365) nvs: nvs_get sta.phym 1
D (2365) nvs: nvs_get sta.phybw 1
D (2365) nvs: nvs_get_str_or_blob sta.apsw
D (2375) nvs: nvs_get_str_or_blob sta.apinfo
D (2385) nvs: nvs_get sta.scan_method 1
D (2385) nvs: nvs_get sta.sort_method 1
D (2385) nvs: nvs_get sta.minrssi 1
D (2385) nvs: nvs_get sta.minauth 1
D (2385) nvs: nvs_get sta.pmf_e 1
D (2395) nvs: nvs_get sta.pmf_r 1
D (2405) nvs: nvs_get_str_or_blob ap.ssid
D (2405) nvs: nvs_get_str_or_blob ap.mac
D (2415) nvs: nvs_get_str_or_blob ap.passwd
D (2415) nvs: nvs_get_str_or_blob ap.pmk
D (2415) nvs: nvs_get ap.chan 1
D (2425) nvs: nvs_get ap.authmode 1
D (2425) nvs: nvs_get ap.hidden 1
D (2425) nvs: nvs_get ap.max.conn 1
D (2435) nvs: nvs_get bcn.interval 2
D (2435) nvs: nvs_get ap.phym 1
D (2435) nvs: nvs_get ap.phybw 1
D (2445) nvs: nvs_get ap.sndchan 1
D (2445) nvs: nvs_get ap.pmf_e 1
D (2445) nvs: nvs_get ap.pmf_r 1
D (2445) nvs: nvs_get lorate 1
D (2455) nvs: nvs_get_str_or_blob country
D (2455) nvs: nvs_set ap.sndchan 1 1
D (2455) nvs: nvs_set_blob sta.mac 6
D (2465) nvs: nvs_set_blob ap.mac 6
I (2465) wifi:wifi firmware version: 1865b55
I (2465) wifi:wifi certification version: v7.0
I (2475) wifi:config NVS flash: enabled
I (2475) wifi:config nano formating: enabled
I (2475) wifi:Init data frame dynamic rx buffer num: 32
I (2485) wifi:Init management frame dynamic rx buffer num: 32
I (2485) wifi:Init management short buffer num: 32
I (2505) wifi:Init static tx buffer num: 16
I (2505) wifi:Init tx cache buffer num: 32
I (2505) wifi:Init static rx buffer size: 1600
I (2515) wifi:Init static rx buffer num: 10
I (2515) wifi:Init dynamic rx buffer num: 32
I (2515) wifi_init: rx ba win: 6
I (2525) wifi_init: tcpip mbox: 32
I (2525) wifi_init: udp mbox: 6
I (2535) wifi_init: tcp mbox: 6
I (2535) wifi_init: tcp tx win: 5744
I (2535) wifi_init: tcp rx win: 5744
I (2545) wifi_init: tcp mss: 1440
I (2545) wifi_init: WiFi/LWIP prefer SPIRAM
D (2545) phy_init: loading PHY init data from application binary
D (2555) nvs: nvs_open_from_partition phy 0
D (2555) nvs: nvs_get calD (2565) efuse: coding scheme 0
_version 4
D (2565) nvs: nvs_get_str_or_blob cal_mac
D (2565) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting wiD (2585) efuse: coding scheme 0
D (2585) efuse: coding scheme 0
th 8 bit
D (2585) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (2585) efuse: In EFUSE_BLK0__DATAD (2595) efuse: coding scheme 0
D (2605) efuse: coding scheme 0
1_REG is used 8 bits starting with 24 bit
D (2605) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 16 bit
DD (2615) efuse: coding scheme 0
 (2605) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (2615) efuse: In EFUSE_BLK0__DATA1_REG is used D (2625) efuse: coding scheme 0
8 bits starting with 0 bit
D (2635) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
D (2635) nvs: nvD (2655) efuse: coding scheme 0
s_get_str_or_blob cal_data
D (2655) nvs: nvs_close 3
D (2655) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting wiD (2655) efuse: coding scheme 0
D (2665) efuse: coding scheme 0
th 8 bit
D (2665) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (2665) efuse: In EFUSE_BLK0__DATAD (2675) efuse: coding scheme 0
D (2675) efuse: coding scheme 0
1_REG is used 8 bits starting with 24 bit
D (2675) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 16 bit
DD (2695) efuse: coding scheme 0
 (2685) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (2695) efuse: In EFUSE_BLK0__DATA1_REG is used D (2705) efuse: coding scheme 0
I (2795) phy: phy_version: 4500, 0cd6843, Sep 17 2020, 15:37:07, 0, 0
8 bits starting with 0 bit
D (2715) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
I (2795) wifi:mode : sta (84:cc:a8:0d:3b:70)
D (2805) wifi_init_default: wifi_start esp-netif:0x3ffd3f70 event-id2
D (2805) wifi_init_default: WIFI mac address: 84 cc a8 d 3b 70
D (2815) esp_netif_handlers: esp_netif action has started with netif0x3ffd3f70 from event_id=2
D (2825) esp_netif_lwip: check: remote, if=0x3ffd3f70 fn=0x40103630
0x40103630: esp_netif_start_api at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:608


D (2825) esp_netif_lwip: esp_netif_start_api 0x3ffd3f70
D (2835) esp_netif_lwip: esp_netif_get_hostname esp_netif:0x3ffd3f70
D (2835) esp_netif_lwip: check: local, if=0x3ffd3f70 fn=0x40104074
0x40104074: esp_netif_update_default_netif_lwip at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:163


D (2845) esp_netif_lwip: esp_netif_update_default_netif_lwip 0x3ffd3f70
D (2845) esp_netif_lwip: call api in lwip: ret=0x0, give sem
I (2975) wifi:new:<8,0>, old:<1,0>, ap:<255,255>, sta:<8,0>, prof:1
I (2985) wifi:state: init -> auth (b0)
I (2995) wifi:state: auth -> assoc (0)
I (3055) wifi:state: assoc -> run (10)
I (3165) wifi:connected with Verhoeckx_glas, aid = 7, channel 8, BW20, bssid = 96:6a:b0:c3:3b:27
I (3165) wifi:security: WPA2-PSK, phy: bgn, rssi: -50
D (3165) nvs: nvs_set sta.chan 1 8
D (3175) nvs: nvs_set_blob sta.apinfo 700
I (3175) wifi:pm start, type: 1
D (3175) esp_netif_handlers: esp_netif action connected with netif0x3ffd3f70 from event_id=4
D (3185) esp_netif_lwip: check: remote, if=0x3ffd3f70 fn=0x40103850
0x40103850: esp_netif_up_api at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:1131


D (3195) esp_netif_lwip: esp_netif_up_api esp_netif:0x3ffd3f70
D (3195) esp_netif_lwip: check: local, if=0x3ffd3f70 fn=0x40104074
0x40104074: esp_netif_update_default_netif_lwip at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:163


I (3195) wifi:AP's beacon interval = 102400 us, DTIM period = 1
D (3205) esp_netif_lwip: esp_netif_update_default_netif_lwip 0x3ffd3f70
D (3215) esp_netif_lwip: call api in lwip: ret=0x0, give sem
D (3215) esp_netif_lwip: check: remote, if=0x3ffd3f70 fn=0x40103428
0x40103428: esp_netif_dhcpc_start_api at C:/Users/Fred/esp-idf/components/esp_netif/lwip/esp_netif_lwip.c:929


D (3225) esp_netif_lwip: esp_netif_dhcpc_start_api esp_netif:0x3ffd3f70
D (3235) esp_netif_lwip: esp_netif_start_ip_lost_timer esp_netif:0x3ffd3f70
D (3235) esp_netif_lwip: if0x3ffd3f70 start ip lost tmr: no need start because netif=0x3ffd3ff4 interval=120 ip=0
D (3245) esp_netif_lwip: starting dhcp client
D (3265) esp_netif_lwip: call api in lwip: ret=0x0, give sem
D (4695) esp_netif_lwip: esp_netif_dhcpc_cb lwip-netif:0x3ffd3ff4
D (4695) esp_netif_lwip: if0x3ffd3f70 ip changed=1
D (4695) wifi_init_default: Got IP wifi default handler entered
D (4705) esp_netif_handlers: esp_netif action got_ip with netif0x3ffd3f70 from event_id=0
I (4705) esp_netif_handlers: sta ip: 192.168.2.5, mask: 255.255.255.0, gw: 192.168.2.254
W (4715) APP_MAIN: Initialize MQTT connection here
E (4725) APP_MAIN: WAITING A WHILE FOR MQTT INITS TO FINISH
D (4725) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=7
I (4735) mqtt_event_handler_cb: Other event id:7
D (4775) TRANS_TCP: [sock=54] Connecting to server. IP: 145.53.71.16, Port: 1883
D (4795) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=1
I (4795) mqtt_event_handler_cb: MQTT_EVENT_CONNECTED
I (4795) mqtt_event_handler_cb: sent publish successful, msg_id=26720
I (4805) mqtt_event_handler_cb: sent subscribe successful, msg_id=22978
I (4815) mqtt_event_handler_cb: sent subscribe successful, msg_id=12314
I (4815) mqtt_event_handler_cb: sent unsubscribe successful, msg_id=28737
I (4825) mqtt_event_handler_cb: sent subscribe successful, msg_id=46124
I (4835) mqtt_event_handler_cb: sent subscribe successful, msg_id=1299
I (4845) mqtt_event_handler_cb: sent subscribe successful, msg_id=60390
I (4845) mqtt_event_handler_cb: sent subscribe successful, msg_id=46667
D (4855) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=5
I (4855) mqtt_event_handler_cb: MQTT_EVENT_PUBLISHED, msg_id=26720
D (4875) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4875) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=22978
I (4885) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4895) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4895) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=12314
I (4905) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4915) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=4
I (4915) mqtt_event_handler_cb: MQTT_EVENT_UNSUBSCRIBED, msg_id=28737
D (4925) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4935) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=46124
I (4935) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4945) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4955) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=1299
I (4965) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4975) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (4985) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=25375
I (4985) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (4995) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5005) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=60390
I (5005) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (5015) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5025) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=54368
I (5025) mqtt_event_handler_cb: sent publish successful, msg_id=0
D (5035) mqtt_event_handler: Event dispatched from event loop base=MQTT_EVENTS, event_id=3
I (5045) mqtt_event_handler_cb: MQTT_EVENT_SUBSCRIBED, msg_id=46667
I (5055) mqtt_event_handler_cb: sent publish successful, msg_id=0
E (5725) APP_MAIN: CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDER
W (5725) APP_MAIN: ENTERING init_test_recorder()
MyNotifier::NotifyRECEIVED EVENT of type 1  with number 314132345
RECEIVED EVENT OF TYPE 1  WITH NUMBER 314132345
INIT_TEST_RECORDER
Testing Outbuf
31 HEXADECIMAL: 1F
Tested Outbuf
E (5745) FCKX_SEQ: Entering test_recorder of TEST_RECORDER example
E (5745) FCKX_SEQ: CONDITIONAL----------------------------------CONDITIONAL  TEST_RECORDER
W (5765) MIDIMANAGER::INIT: Enter
Executing MIDIManager::Init()
Going to create temp MidiOutNimBLE for detecting nr of ports
W (5775) MIDIOUTNIMBLE :: INITIALIZE: Initialize nimBLEOutdriver MidiOutNimBLE :: initialize
D (5775) BTDM_INIT: Release DRAM [0x3ffb2730] - [0x3ffb6388]
I (5785) BTDM_INIT: BT controller compile version [3723d5b]
D (5785) BTDM_INIT: .data initialise [0x3ffae6e0] <== [0x4000d890]
D (5795) BTDM_INIT: .bss initialise [0x3ffb0000] - [0x3ffb09a8]
D (5795) BTDM_INIT: .bss initialise [0x3ffb09a8] - [0x3ffb1ddc]
D (5805) BTDM_INIT: .bss initialise [0x3ffb1ddc] - [0x3ffb2730]
D (5815) BTDM_INIT: .bss initialise [0x3ffb8000] - [0x3ffb9a20]
D (5815) BTDM_INIT: .bss initialise [0x3ffbdb28] - [0x3ffbdb5c]
D (5835) phy_init: loading PHY init data from application binary
D (5835) nvs: nvs_open_from_partition phy 0
D (5845) efuse: coding scheme 0
D (5835) nvs: nvs_get cal_version 4
D (5845) nvs: nvs_get_str_or_blob cal_mac
D (5845) efuse: In EFUSE_BLK0__DATA2_REG iD (5855) efuse: coding scheme 0
D (5855) efuse: coding scheme 0
s used 8 bits starting with 8 bit
D (5855) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (5865) eD (5875) efuse: coding scheme 0
fuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (5875) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits sD (5885) efuse: coding scheme 0
D (5885) efuse: coding scheme 0
tarting with 16 bit
D (5885) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (5895) efuse: In EFUSED (5905) efuse: coding scheme 0
_BLK0__DATA1_REG is used 8 bits starting with 0 bit
D (5905) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16D (5925) efuse: coding scheme 0
 bit
D (5915) nvs: nvs_get_str_or_blob cal_data
D (5925) nvs: nvs_close 4
D (5925) efuse: In EFUSE_BLK0__DATA2_REG iD (5935) efuse: coding scheme 0
D (5935) efuse: coding scheme 0
s used 8 bits starting with 8 bit
D (5935) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 0 bit
D (5935) eD (5945) efuse: coding scheme 0
fuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 24 bit
D (5955) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits sD (5965) efuse: coding scheme 0
D (5965) efuse: coding scheme 0
tarting with 16 bit
D (5965) efuse: In EFUSE_BLK0__DATA1_REG is used 8 bits starting with 8 bit
D (5975) efuse: In EFUSED (5985) efuse: coding scheme 0
_BLK0__DATA1_REG is used 8 bits starting with 0 bit
D (5985) efuse: In EFUSE_BLK0__DATA2_REG is used 8 bits starting with 16 bit
D (6245) nvs: nvs_open_from_partition nimble_bond 1
D (6245) nvs: nvs_get_str_or_blob our_sec_1
D (6245) nvs: nvs_close 5
D (6245) nvs: nvs_open_from_partition nimble_bond 1
D (6245) nvs: nvs_get_str_or_blob our_sec_2
D (6245) nvs: nvs_close 6
D (6255) nvs: nvs_open_from_partition nimble_bond 1
D (6255) nvs: nvs_get_str_or_blob our_sec_3
D (6255) nvs: nvs_close 7
D (6265) NIMBLE_NVS: ble_store_config_our_secs restored 0 bonds
D (6265) nvs: nvs_open_from_partition nimble_bond 1
D (6275) nvs: nvs_get_str_or_blob peer_sec_1
D (6275) nvs: nvs_close 8
D (6275) nvs: nvs_open_from_partition nimble_bond 1
D (6285) nvs: nvs_get_str_or_blob peer_sec_2
D (6285) nvs: nvs_close 9
D (6285) nvs: nvs_open_from_partition nimble_bond 1
D (6295) nvs: nvs_get_str_or_blob peer_sec_3
D (6295) nvs: nvs_close 10
D (6295) NIMBLE_NVS: ble_store_config_peer_secs restored 0 bonds
D (6315) nvs: nvs_open_from_partition nimble_bond 1
D (6315) nvs: nvs_get_str_or_blob cccd_sec_1
D (6325) nvs: nvs_close 11
D (6325) nvs: nvs_open_from_partition nimble_bond 1
D (6325) nvs: nvs_get_str_or_blob cccd_sec_2
D (6335) nvs: nvs_close 12
D (6335) nvs: nvs_open_from_partition nimble_bond 1
D (6345) nvs: nvs_get_str_or_blob cccd_sec_3
D (6345) nvs: nvs_close 13
D (6345) nvs: nvs_open_from_partition nimble_bond 1
D (6355) nvs: nvs_get_str_or_blob cccd_sec_4
D (6355) nvs: nvs_close 14
D (6355) nvs: nvs_open_from_partition nimble_bond 1
D (6365) nvs: nvs_get_str_or_blob cccd_sec_5
D (6365) nvs: nvs_close 15
D (6365) nvs: nvs_open_from_partition nimble_bond 1
D (6375) nvs: nvs_get_str_or_blob cccd_sec_6
D (6375) nvs: nvs_close 16
D (6375) nvs: nvs_open_from_partition nimble_bond 1
D (6385) nvs: nvs_get_str_or_blob cccd_sec_7
D (6385) nvs: nvs_close 17
D (6385) nvs: nvs_open_from_partition nimble_bond 1
D (6395) nvs: nvs_get_str_or_blob cccd_sec_8
D (6395) nvs: nvs_close 18
D (6395) NIMBLE_NVS: ble_store_config_cccds restored 0 bonds
D (6415) nvs: nvs_open_from_partition nimble_bond 1
D (6415) nvs: nvs_get_str_or_blob p_dev_rec_1
D (6425) nvs: nvs_close 19
D (6425) nvs: nvs_open_from_partition nimble_bond 1
D (6425) nvs: nvs_get_str_or_blob p_dev_rec_2
D (6435) nvs: nvs_close 20
D (6435) nvs: nvs_open_from_partition nimble_bond 1
D (6445) nvs: nvs_get_str_or_blob p_dev_rec_3
D (6445) nvs: nvs_close 21
D (6445) nvs: nvs_open_from_partition nimble_bond 1
D (6455) nvs: nvs_get_str_or_blob p_dev_rec_4
D (6455) nvs: nvs_close 22
D (6455) NIMBLE_NVS: peer_dev_rec restored 0 records
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
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x18 status=0 rand=0x035ac286009fed4a
ble_hs_hci_cmd_send: ogf=0x08 ocf=0x0018 len=0
0x18 0x20 0x00
Command complete: cmd_pkts=5 ogf=0x8 ocf=0x18 status=0 rand=0xe822763b55669e37
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
W (6645) MIDIOUTNIMBLE :: INITIALIZE: NimBLEDevice created
W (6645) MIDIOUTNIMBLE :: INITIALIZE: BLE server created
W (6655) MIDIOUTNIMBLE :: INITIALIZE: BLE server callbacks created
W (6655) MIDIOUTNIMBLE :: INITIALIZE: BLE server service created
W (6665) MIDIOUTNIMBLE :: INITIALIZE: BLE server MIDI characteristic created
W (6675) MIDIOUTNIMBLE :: INITIALIZE: portName[0] MIDIport
W (6675) MIDIOUTNIMBLE :: INITIALIZE: BLE service started
Created temp MidiOutNimBLE for detecting nr of ports
E (6695) NICMIDI DRIVER: start creation of MidiOutNimBLE port
W (6695) MIDIOUTNIMBLE :: INITIALIZE: Initialize nimBLEOutdriver MidiOutNimBLE :: initialize
W (6705) MIDIOUTNIMBLE :: INITIALIZE: NimBLEDevice created
W (6705) MIDIOUTNIMBLE :: INITIALIZE: BLE server created
W (6715) MIDIOUTNIMBLE :: INITIALIZE: BLE server callbacks created
W (6725) MIDIOUTNIMBLE :: INITIALIZE: BLE server service created
W (6725) MIDIOUTNIMBLE :: INITIALIZE: BLE server MIDI characteristic created
W (6735) MIDIOUTNIMBLE :: INITIALIZE: portName[0] MIDIport
W (6735) MIDIOUTNIMBLE :: INITIALIZE: BLE service started
E (6755) NICMIDI DRIVER: executed creation of MidiOutNimBLE port
Going to create temp MQTTMidiIn for detecting nr of ports
W (6765) MQTTDRIVER: MQTTMidiIn instantiation
W (6765) MQTTDRIVER: MQTTMidiIn :: initialize
Created temp MQTTMidiIn for detecting nr of ports
E (6785) NICMIDI DRIVER INPUT: start creation of MQTTMidiIn port
W (6785) MQTTDRIVER: MQTTMidiIn instantiation
W (6795) MQTTDRIVER: MQTTMidiIn :: initialize
E (6795) MQTTDRIVER: MQTTMidiIn::setCallback: STORING CALLBACK IN inputData_
E (6805) NICMIDI DRIVER INPUT: executed creation of MQTTMidiIn port
W (6805) MIDIMANAGER::INIT: Pos1
W (6815) MQTTDRIVER: Closed MQTTMidiIn NO CLEANUP: BEWARE of MEMORY LEAKS
W (6815) MIDIMANAGER::INIT: Pos2
Timer resolution 10
portTICK_PERIOD_MS 10
Calculated periodTicks 1
Timer started with 1 system ticks resolution
Timer open with 10 msecs resolution
Exiting MIDIManager::Init() Found 1 midi out and 1 midi in
W (6845) MIDIMANAGER::INIT: Pos3 (exit)
111 AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
E (6855) SEQUENCER: track_states.size() 17
GUI EVENT: All       GENERAL RESET
E (6865) SEQUENCER: track_states.size() 17
E (6865) SEQUENCER: track_states.size() 17
E (6875) SEQUENCER: track_states.size() 17
GUI EVENT: All       GENERAL RESET
E (6885) SEQUENCER: track_states.size() 17
GUI EVENT: All       GENERAL RESET
11a AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
11b AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
DEBUG MIDIThru constructor
11c AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
11d AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
11e AdvancedSequencer::AdvancedSequencer(MIDISequencerGUINotifier *n)
W (6935) MQTTDRIVER: open MQTTMidiIn port
IN Port MQTT_In open
E (6945) NICMIDI DRIVER: MIDIOutDriver::OpenPort() port_id 0
W (6945) MIDIOUTNIMBLE :: OPENPORT: Initialize nimBLEOutdriver MidiOutNimBLE :: openPort nr 0
OPENPORT2 ENTEREDE (6955) MIDIOUTNIMBLE :: OPENPORT: A valid connection already exists nevertheless start advertising (DIRTYOPEN!)
E (6965) MIDIOUTNIMBLE :: OPENPORT: >>>>>take the opportunity to list all available Characteristics somewhere over here <<<<<<<
I (6975) MIDIOUTNIMBLE :: OPENPORT: Prepare advertising
I (6985) MIDIOUTNIMBLE :: OPENPORT: Start advertising
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
I (7055) MIDIOUTNIMBLE :: OPENPORT: OPENPORT Waiting for a client connection to notify...
LE connection complete. handle=0 role=1 paddrtype=0 addr=84.cc.a8.d.3b.ba local_rpa=0.0.0.0.0.0 peer_rpa=0.0.0.0.0.0 itvl=40 latency=0 spvn_tmo=256 mca=1
W (8625) NIMBLE_ONCONNECT: -----
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
I (12065) MIDIOUTNIMBLE :: OPENPORT: Number of connected peers: 1
OUT Port MIDIport open
WAIT FOR PEER1
WAIT FOR PEER on MIDI out
set Midi Characteristic callbacks
Timer stopped by MIDITimer::HardStop()
Timer resolution 10
portTICK_PERIOD_MS 10
Calculated periodTicks 1
Timer started with 1 system ticks resolution
Timer open with 10 msecs resolution
E (12095) FCKX_SEQ: PROCEEDING with TEST_RECORDER example (a connection should be available here, because tested before)
E (12105) FCKX_SEQ: thru = new MIDIThru;
DEBUG MIDIThru constructor
E (12115) FCKX_SEQ: Entering Idle Loop of test_recorder
E (12115) FCKX_SEQ: USE GUI TO ISSUE COMMANDS