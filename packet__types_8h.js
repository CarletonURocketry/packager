var packet__types_8h =
[
    [ "PacketHeader", "structPacketHeader.html", "structPacketHeader" ],
    [ "BlockHeader", "structBlockHeader.html", "structBlockHeader" ],
    [ "SignalReportBlock", "unionSignalReportBlock.html", "unionSignalReportBlock" ],
    [ "AltitudeDataBlock", "structAltitudeDataBlock.html", "structAltitudeDataBlock" ],
    [ "AngularVelocityBlock", "structAngularVelocityBlock.html", "structAngularVelocityBlock" ],
    [ "AccelerationDataBlock", "structAccelerationDataBlock.html", "structAccelerationDataBlock" ],
    [ "TelemetryRequestBlock", "structTelemetryRequestBlock.html", "structTelemetryRequestBlock" ],
    [ "TIGHTLY_PACKED", "structTIGHTLY__PACKED.html", "structTIGHTLY__PACKED" ],
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
      [ "DATA_STATUS", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa3b39bde0f8878dd16ae1209ede64c18b", null ],
      [ "DATA_STARTUP_MSG", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa43afb8e7ce308b695cedfff7d9f4c179", null ],
      [ "DATA_ALT", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3faacf6096387d07c89e56487b091d83010", null ],
      [ "DATA_ACCEL", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa599fafe621581ddbd0e7a8bddac5a168", null ],
      [ "DATA_ANGULAR_VEL", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa164d2895a374e02e5d1663a48f2679de", null ],
      [ "DATA_GNSS_LOC", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa8508368d05295d04be62133d7bff0288", null ],
      [ "DATA_GNSS_META", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa413e07c416faf5a3ebb4a456776a73de", null ],
      [ "DATA_PWR_INFO", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa47085061bea47e0e43945d79d7cbf73f", null ],
      [ "DATA_TEMP", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa64e6755af0f1208f6470bfa4f937832f", null ],
      [ "DATA_MPU9250_IMU", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fa218a2d1ba32c79c5df7fdd6063fdf7ec", null ],
      [ "DATA_KX134_1211_ACCEL", "packet__types_8h.html#aa2cdad06147808bbec52b49736391d3fab216f5ffeb9ee554f1cd4c1f842731a6", null ]
    ] ],
    [ "DeviceAddress", "packet__types_8h.html#a342f5a9f74cdec1043dd88e774c4c630", [
      [ "GROUNDSTATION", "packet__types_8h.html#a342f5a9f74cdec1043dd88e774c4c630a5e56eb1cf5d04bea91f1bfa04b1b1868", null ],
      [ "ROCKET", "packet__types_8h.html#a342f5a9f74cdec1043dd88e774c4c630afafc201536271f745d3923b7efb6d375", null ],
      [ "MULTICAST", "packet__types_8h.html#a342f5a9f74cdec1043dd88e774c4c630a607d700b2c0a01c54bdadde074a7cb12", null ]
    ] ],
    [ "acceleration_data_block_init", "packet__types_8h.html#a34e642c0ccb44093cda0bf821381fde1", null ],
    [ "altitude_data_block_init", "packet__types_8h.html#a53a0207767b9583ec28be4902f75f690", null ],
    [ "angular_velocity_block_init", "packet__types_8h.html#a32db71d086e85e48ea86a56b79ef5af2", null ],
    [ "block_header_get_length", "packet__types_8h.html#a83e46b37d266d623868283fa3fe29e2a", null ],
    [ "block_header_init", "packet__types_8h.html#aae2e56890427e8cd7f3aef4144fdf3bc", null ],
    [ "block_header_set_length", "packet__types_8h.html#a092456471f9ac5f601a615d65be8c95f", null ],
    [ "memcpy_be", "packet__types_8h.html#aba679592b86f02a1e165463d32f103f8", null ],
    [ "packet_append_block", "packet__types_8h.html#aa0f7d3bde8660c68f0c96dd21da35eec", null ],
    [ "packet_header_get_length", "packet__types_8h.html#a458cb5abef8a8a04b26c5b31cebcdbc9", null ],
    [ "packet_header_init", "packet__types_8h.html#ac5862f04a01edd6d42896f795bd6ef5c", null ],
    [ "packet_header_set_length", "packet__types_8h.html#ae5475025bd142a3d86b9322c98ebd1ae", null ],
    [ "signal_report_init", "packet__types_8h.html#a0b521ef6a3e32e6040a68a6a82d67ada", null ],
    [ "telemetry_request_block", "packet__types_8h.html#adfbccd8055ee85d43c7d2a237f0b4d95", null ]
];