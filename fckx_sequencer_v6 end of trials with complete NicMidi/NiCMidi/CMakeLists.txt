idf_component_register(
  REQUIRED_IDF_TARGETS
    "esp32"
  INCLUDE_DIRS
    "./src" "./include"
  SRCS
    "./src/msg.cpp"
#    ".src/NimBLEAddress.cpp"
#    etc.    
#  REQUIRES
#    bt
#    nvs_flash
#  PRIV_REQUIRES
#    ${ESP_NIMBLE_PRIV_REQUIRES}
)