W (157835) NIMBLE_ONDISCONNECT: -----
LE connection complete. handle=0 role=1 paddrtype=0 addr=84.cc.a8.d.3b.ba local_rpa=0.0.0.0.0.0 peer_rpa=0.0.0.0.0.0 itvl=40 latency=0 spvn_tmo=256 mca=1
W (159945) NIMBLE_ONCONNECT: -----
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
host tx hci data; handle=0 length=17
ble_hs_hci_acl_tx(): 0x00 0x00 0x11 0x00 0x0d 0x00 0x04 0x00 0x07 0x0a 0x00 0x10 0x00 0x11 0x00 0x17 0x00 0x18 0x00 0x1e 0x00
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=27 data=0x17 0x00 0x04 0x00 0x06 0x1f 0x00 0xff 0xff 0x00 0x28 0x4b 0x91 0x31 0xc3 0xc9 0xc5 0xcc 0x8f 0x9e 0x45 0xb5 0x1f 0x01 0xc2 0xaf 0x4f
rxed att command: find type value req; conn=0 start_handle=0x001f end_handle=0xffff attr_type=10240
txed att command: error rsp; conn=0 req_op=6 handle=0x001f error_code=10
host tx hci data; handle=0 length=9
ble_hs_hci_acl_tx(): 0x00 0x00 0x09 0x00 0x05 0x00 0x04 0x00 0x01 0x06 0x1f 0x00 0x0a
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=11 data=0x07 0x00 0x04 0x00 0x08 0x18 0x00 0x1e 0x00 0x03 0x28
rxed att command: read type req; conn=0 start_handle=0x0018 end_handle=0x001e
txed att command: read type rsp; conn=0 length=21
host tx hci data; handle=0 length=48
ble_hs_hci_acl_tx(): 0x00 0x00 0x30 0x00 0x2c 0x00 0x04 0x00 0x09 0x15 0x19 0x00 0x1a 0x1a 0x00 0xa8 0x26 0x1b 0x36 0x07 0xea 0xf5 0xb7 0x88 0x46 0xe1 0x36 0x3e 0x48 0xb5 0xbe 0x1c 0x00 0x1a 0x1d 0x00 0x02 0x00 0x12 0xac 0x42 0x02 0x09 0xb9 0xec 0x11 0x22 0x9f 0xfa 0x98 0x9d 0x10

Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=11 data=0x07 0x00 0x04 0x00 0x08 0x1d 0x00 0x1e 0x00 0x03 0x28
rxed att command: read type req; conn=0 start_handle=0x001d end_handle=0x001e
txed att command: error rsp; conn=0 req_op=8 handle=0x001d error_code=10
host tx hci data; handle=0 length=9
ble_hs_hci_acl_tx(): 0x00 0x00 0x09 0x00 0x05 0x00 0x04 0x00 0x01 0x08 0x1d 0x00 0x0a
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=11 data=0x07 0x00 0x04 0x00 0x08 0x18 0x00 0x1e 0x00 0x03 0x28
rxed att command: read type req; conn=0 start_handle=0x0018 end_handle=0x001e
txed att command: read type rsp; conn=0 length=21
host tx hci data; handle=0 length=48
ble_hs_hci_acl_tx(): 0x00 0x00 0x30 0x00 0x2c 0x00 0x04 0x00 0x09 0x15 0x19 0x00 0x1a 0x1a 0x00 0xa8 0x26 0x1b 0x36 0x07 0xea 0xf5 0xb7 0x88 0x46 0xe1 0x36 0x3e 0x48 0xb5 0xbe 0x1c 0x00 0x1a 0x1d 0x00 0x02 0x00 0x12 0xac 0x42 0x02 0x09 0xb9 0xec 0x11 0x22 0x9f 0xfa 0x98 0x9d 0x10

Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=11 data=0x07 0x00 0x04 0x00 0x08 0x1d 0x00 0x1e 0x00 0x03 0x28
rxed att command: read type req; conn=0 start_handle=0x001d end_handle=0x001e
txed att command: error rsp; conn=0 req_op=8 handle=0x001d error_code=10
host tx hci data; handle=0 length=9
ble_hs_hci_acl_tx(): 0x00 0x00 0x09 0x00 0x05 0x00 0x04 0x00 0x01 0x08 0x1d 0x00 0x0a
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=7 data=0x03 0x00 0x04 0x00 0x0a 0x1a 0x00
rxed att command: read req; conn=0 handle=0x001a
host tx hci data; handle=0 length=24
ble_hs_hci_acl_tx(): 0x00 0x00 0x18 0x00 0x14 0x00 0x04 0x00 0x0b 0x73 0x6f 0x6d 0x65 0x20 0x69 0x6e 0x66 0x6f 0x20 0x54 0x45 0x53 0x54 0x5f 0x4d 0x49 0x44 0x49
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=11 data=0x07 0x00 0x04 0x00 0x08 0x1a 0x00 0x1e 0x00 0x03 0x28
rxed att command: read type req; conn=0 start_handle=0x001a end_handle=0x001e
txed att command: read type rsp; conn=0 length=21
host tx hci data; handle=0 length=27
ble_hs_hci_acl_tx(): 0x00 0x00 0x1b 0x00 0x17 0x00 0x04 0x00 0x09 0x15 0x1c 0x00 0x1a 0x1d 0x00 0x02 0x00 0x12 0xac 0x42 0x02 0x09 0xb9 0xec 0x11 0x22 0x9f 0xfa 0x98 0x9d 0x10
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=9 data=0x05 0x00 0x04 0x00 0x04 0x1b 0x00 0x1b 0x00
rxed att command: find info req; conn=0 start_handle=0x001b end_handle=0x001b
txed att command: find info rsp; conn=0 format=1
host tx hci data; handle=0 length=10
ble_hs_hci_acl_tx(): 0x00 0x00 0x0a 0x00 0x06 0x00 0x04 0x00 0x05 0x01 0x1b 0x00 0x02 0x29
Number of Completed Packets: num_handles=1
handle:0 pkts:1
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=9 data=0x05 0x00 0x04 0x00 0x52 0x1b 0x00 0x01 0x00 rxed att command: write cmd; conn=0 handle=0x001b
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x30
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x30 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x31
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x31 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x32
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x32 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x33
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x33 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x34
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x34 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x35
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x35 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x36
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x36 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x37
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x37 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x38
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x38 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x19 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x39
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=3 data=0x20 0x39 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x1b 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x31
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=5 data=0x30 0x20 0x31 0x30 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x1b 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x31
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=5 data=0x31 0x20 0x31 0x31 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x1b 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x31
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=5 data=0x32 0x20 0x31 0x32 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x1b 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x31
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=5 data=0x33 0x20 0x31 0x33 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x1b 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x31
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=5 data=0x34 0x20 0x31 0x34 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x1b 0x00 0x04 0x00 0x52 0x1a 0x00 0x2f 0x66 0x63 0x6b 0x78 0x5f 0x73 0x65 0x71 0x2f 0x63 0x68 0x61 0x6e 0x6e 0x65 0x6c 0x2f 0x31
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=1 len=5 data=0x35 0x20 0x31 0x35 0x46
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=26 data=0x16 0x00 0x04 0x00 0x52 0x1a 0x00 0x73 0x6f 0x6d 0x65 0x20 0x69 0x6e 0x66 0x6f 0x20 0x54 0x45 0x53 0x54 0x5f 0x4d 0x49 0x44 0x49
rxed att command: write cmd; conn=0 handle=0x001a
ble_hs_hci_evt_acl_process(): conn_handle=0 pb=2 len=25 data=0x15 0x00 0x04 0x00 0x52 0x1d 0x00 0x73 0x6f 0x6d 0x65 0x20 0x69 0x6e 0x66 0x6f 0x20 0x54 0x45 0x53 0x54 0x5f 0x47 0x55 0x49
rxed