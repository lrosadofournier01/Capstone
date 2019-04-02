<?xml version="1.0" encoding="UTF-8"?>
<project name="captstone demo" board="Mojo V3" language="Lucid">
  <files>
    <src top="true">mojo_top.luc</src>
    <ucf lib="true">hdmi.ucf</ucf>
    <ucf lib="true">mojo.ucf</ucf>
    <component>dvi_globals.luc</component>
    <component>dvi_encoder.luc</component>
    <component>fifo_2x_reducer.luc</component>
    <component>tmds_decoder.luc</component>
    <component>hdmi_passthru.luc</component>
    <component>dvi_decoder.luc</component>
    <component>channel_align.luc</component>
    <component>edge_detector.luc</component>
    <component>tmds_encoder.luc</component>
    <component>bit_align.luc</component>
    <component>async_fifo.luc</component>
    <component>serdes_n_to_1.luc</component>
    <component>hdmi_encoder.luc</component>
    <component>reset_conditioner.luc</component>
    <component>simple_dual_ram.v</component>
    <component>pipeline.luc</component>
    <component>serdes_1_to_5.luc</component>
    <core name="clk_wiz_v3_6">
      <src>clk_wiz_v3_6.v</src>
    </core>
  </files>
</project>
