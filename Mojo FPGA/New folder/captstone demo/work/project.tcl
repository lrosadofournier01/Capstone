set projDir "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/planAhead"
set projName "captstone demo"
set topName top
set device xc6slx9-2tqg144
if {[file exists "$projDir/$projName"]} { file delete -force "$projDir/$projName" }
create_project $projName "$projDir/$projName" -part $device
set_property design_mode RTL [get_filesets sources_1]
set verilogSources [list "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/mojo_top_0.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/hdmi_encoder_1.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/reset_conditioner_2.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/dvi_encoder_8.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/serdes_n_to_1_9.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/tmds_encoder_11.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/tmds_encoder_11.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/tmds_encoder_11.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/fifo_2x_reducer_14.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/serdes_n_to_1_9.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/serdes_n_to_1_9.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/serdes_n_to_1_9.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/async_fifo_23.v" "D:/Student drive/Capstone Project_ Sensor Drone/Sensors (Code & Datasheets)/Mojo FPGA/New folder/captstone demo/work/verilog/simple_dual_ram_30.v"]
import_files -fileset [get_filesets sources_1] -force -norecurse $verilogSources
set ucfSources [list  "C:/Program\ Files/Mojo\ IDE/library/components/hdmi.ucf" "C:/Program\ Files/Mojo\ IDE/library/components/mojo.ucf"]
import_files -fileset [get_filesets constrs_1] -force -norecurse $ucfSources
set coreSources [list "D:/Student\ drive/Capstone\ Project_\ Sensor\ Drone/Sensors\ (Code\ &\ Datasheets)/Mojo\ FPGA/New\ folder/captstone\ demo/coreGen/clk_wiz_v3_6.v"]
import_files -fileset [get_filesets sources_1] -force -norecurse $coreSources
set_property -name {steps.bitgen.args.More Options} -value {-g Binary:Yes -g Compress} -objects [get_runs impl_1]
set_property steps.map.args.mt on [get_runs impl_1]
set_property steps.map.args.pr b [get_runs impl_1]
set_property steps.par.args.mt on [get_runs impl_1]
update_compile_order -fileset sources_1
launch_runs -runs synth_1
wait_on_run synth_1
launch_runs -runs impl_1
wait_on_run impl_1
launch_runs impl_1 -to_step Bitgen
wait_on_run impl_1
