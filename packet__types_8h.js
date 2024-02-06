var packet__types_8h =
[
    [ "PacketHeader", "structPacketHeader.html", "structPacketHeader" ],
    [ "BlockHeader", "structBlockHeader.html", "structBlockHeader" ],
    [ "SignalReportBlock", "structSignalReportBlock.html", "structSignalReportBlock" ],
    [ "AltitudeDB", "structAltitudeDB.html", "structAltitudeDB" ],
    [ "TemperatureDB", "structTemperatureDB.html", "structTemperatureDB" ],
    [ "PressureDB", "structPressureDB.html", "structPressureDB" ],
    [ "AngularVelocityDB", "structAngularVelocityDB.html", "structAngularVelocityDB" ],
    [ "AccelerationDB", "structAccelerationDB.html", "structAccelerationDB" ],
    [ "TelemetryRequestBlock", "structTelemetryRequestBlock.html", "structTelemetryRequestBlock" ],
    [ "GNSSLocationDB", "structGNSSLocationDB.html", "structGNSSLocationDB" ],
    [ "TIGHTLY_PACKED", "structTIGHTLY__PACKED.html", "structTIGHTLY__PACKED" ],
    [ "BLOCK_MAX_SIZE", "packet__types_8h.html#a3ca3b65f4c18d3bd57fd3f42a72bbafc", null ],
    [ "PACKET_MAX_SIZE", "packet__types_8h.html#a35627e27a13bf2b36a3a9cc3e395a312", null ],
    [ "TIGHTLY_PACKED", "packet__types_8h.html#a89837ed22f4c4f6dfdbac0cdbc5de57d", null ],
    [ "BlockSubtype", "packet__types_8h.html#a7c66e316cf9c5bc29a1f8cb8ac599c97", null ],
    [ "BlockType", "packet__types_8h.html#a54420623f26ab6bb61042b41cccf37a3", [
      [ "TYPE_CTRL", "packet__types_8h.html#a54420623f26ab6bb61042b41cccf37a3abf69e32423b4eec57c639b337bca3a6e", null ],
      [ "TYPE_CMD", "packet__types_8h.html#a54420623f26ab6bb61042b41cccf37a3a31c7517410257c2000072034cb89fa0a", null ],
      [ "TYPE_DATA", "packet__types_8h.html#a54420623f26ab6bb61042b41cccf37a3ac731049dbbf7536acbb7f99f6d702b15", null ]
    ] ],
    [ "CmdBlockType", "packet__types_8h.html#a451aabef89d8f76e81fbc4bf439e8e59", [
      [ "CMD_RST_ROCKET_AV", "packet__types_8h.html#a451aabef89d8f76e81fbc4bf439e8e59a63f41707648853b58d19a4261f15c579", null ],
      [ "CMD_RQST_TELEM_DATA", "packet__types_8h.html#a451aabef89d8f76e81fbc4bf439e8e59a9c4962d529fdd0ebdcebe5eb090b70a0", null ],
      [ "CMD_DEPLOY_CHUTE", "packet__types_8h.html#a451aabef89d8f76e81fbc4bf439e8e59aedfb28c7fdc96c286612c059949cb2b2", null ],
      [ "CMD_TARE_SENSORS", "packet__types_8h.html#a451aabef89d8f76e81fbc4bf439e8e59a72ec7c70d5a467c688d26821411b3fc1", null ]
    ] ],
    [ "CtrlBlockType", "packet__types_8h.html#ac588e0028f994a83da3d2dcfef7ee380", [
      [ "CTRL_SIGNAL_REPORT", "packet__types_8h.html#ac588e0028f994a83da3d2dcfef7ee380aef6f1fd8b40a4c425822dacb2f832d27", null ],
      [ "CTRL_CMD_ACK", "packet__types_8h.html#ac588e0028f994a83da3d2dcfef7ee380a524cc1dbd7bfae5d50b73bd04c5e937c", null ],
      [ "CTRL_CMD_NONCE_RQST", "packet__types_8h.html#ac588e0028f994a83da3d2dcfef7ee380a1b668477b2ae5cfcd98f93f6e1af0535", null ],
      [ "CTRL_CMD_NONCE", "packet__types_8h.html#ac588e0028f994a83da3d2dcfef7ee380afb7c42cc2dbcb20d196142ec57fe1687", null ],
      [ "CTRL_BEACON", "packet__types_8h.html#ac588e0028f994a83da3d2dcfef7ee380aeafb398a1ef9f46016eda0dd71d86088", null ],
      [ "CTRL_BEACON_RESPONSE", "packet__types_8h.html#ac588e0028f994a83da3d2dcfef7ee380af38546f54cbffcff8a25c38a182572ec", null ]
    ] ],
    [ "DataBlockType", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3f", [
      [ "DATA_DBG_MSG", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fac672ecfbfe918efc6cbbe74f220542fe", null ],
      [ "DATA_ALT", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3faacf6096387d07c89e56487b091d83010", null ],
      [ "DATA_TEMP", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa64e6755af0f1208f6470bfa4f937832f", null ],
      [ "DATA_PRESSURE", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa2ded6705e57448adceeb30478f1360b4", null ],
      [ "DATA_ACCEL", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa599fafe621581ddbd0e7a8bddac5a168", null ],
      [ "DATA_ANGULAR_VEL", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa164d2895a374e02e5d1663a48f2679de", null ],
      [ "DATA_GNSS_LOC", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa8508368d05295d04be62133d7bff0288", null ],
      [ "DATA_GNSS_META", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa413e07c416faf5a3ebb4a456776a73de", null ]
    ] ],
    [ "DeviceAddress", "packet__types_8h.html#a342f5a9f74cdec1043dd88e774c4c630", [
      [ "GROUNDSTATION", "packet__types_8h.html#a342f5a9f74cdec1043dd88e774c4c630a5e56eb1cf5d04bea91f1bfa04b1b1868", null ],
      [ "ROCKET", "packet__types_8h.html#a342f5a9f74cdec1043dd88e774c4c630afafc201536271f745d3923b7efb6d375", null ],
      [ "MULTICAST", "packet__types_8h.html#a342f5a9f74cdec1043dd88e774c4c630a607d700b2c0a01c54bdadde074a7cb12", null ]
    ] ],
    [ "acceleration_db_init", "packet__types_8h.html#ae07814aeca5224dcc15f87a7c08687cd", null ],
    [ "altitude_db_init", "packet__types_8h.html#ab12cd79378d809441f655438b4a3b831", null ],
    [ "angular_velocity_db_init", "packet__types_8h.html#a543359e6301c9ef8dee54ba2a7c61653", null ],
    [ "block_header_init", "packet__types_8h.html#aae2e56890427e8cd7f3aef4144fdf3bc", null ],
    [ "gnss_location_db_init", "packet__types_8h.html#a900add4f7036507ea27042eed65b7615", null ],
    [ "memcpy_be", "packet__types_8h.html#aba679592b86f02a1e165463d32f103f8", null ],
    [ "packet_append_block", "packet__types_8h.html#aa0f7d3bde8660c68f0c96dd21da35eec", null ],
    [ "packet_header_init", "packet__types_8h.html#ac5862f04a01edd6d42896f795bd6ef5c", null ],
    [ "packet_print_hex", "packet__types_8h.html#af63f80194e050629b872ff50d81a448d", null ],
    [ "pressure_db_init", "packet__types_8h.html#a5bf100277c9d7c2195015de2a8f93fe5", null ],
    [ "signal_report_init", "packet__types_8h.html#a0b521ef6a3e32e6040a68a6a82d67ada", null ],
    [ "telemetry_request_block_init", "packet__types_8h.html#aa32867ba87c9f3ab1dca3b61737969ee", null ],
    [ "temperature_db_init", "packet__types_8h.html#a745f8f08a168990e46dc7b46aeda81c7", null ]
];