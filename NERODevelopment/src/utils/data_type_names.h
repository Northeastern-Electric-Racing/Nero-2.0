#ifndef DATATYPENAMES_H
#define DATATYPENAMES_H

#define PACKTEMP "BMS/Status/Temp_Average"
#define MOTORTEMP "DTI/Temps/Motor_Temperature"
#define STATEOFCHARGE "BMS/Pack/SOC"
#define CURRENT "DTI/Power/DC_Current"
#define BALANCINGCELLS "BMS/Status/Balancing"
#define PACKVOLTAGE "BMS/Pack/Voltage"
#define MAXCELLTEMP "BMS/Cells/Temp_High_Value"
#define MAXCELLTEMPCHIP "BMS/Cells/Temp_High_Chip"
#define MAXCELLTEMPCELL "BMS/Cells/Temp_High_Cell"
#define MAXCELLVOLTAGE "BMS/Cells/Volts_High_Value"
#define MAXCELLVOLTAGECHIP "BMS/Cells/Volts_High_Chip"
#define MAXCELLVOLTAGECELL "BMS/Cells/Volts_High_Cell"
#define MINCELLTEMP "BMS/Cells/Temp_Low_Value"
#define MINCELLTEMPCHIP "BMS/Cells/Temp_Low_Chip"
#define MINCELLTEMPCELL "BMS/Cells/Temp_Low_Cell"
#define MINCELLVOLTAGE "BMS/Cells/Volts_Low_Value"
#define MINCELLVOLTAGECHIP "BMS/Cells/Volts_Low_Chip"
#define MINCELLVOLTAGECELL "BMS/Cells/Volts_Low_Cell"
#define AVECELLTEMP "BMS/Status/Temp_Average"
#define AVECELLVOLTAGE "BMS/Cells/Volts_Avg_Value"
// #define TRACTIONCONTROL "MPU/State/LaunchControl" // OUTDATED - see
// LAUNCHCONTROL and TRACTIONCONTROL
#define INVERTERTEMP "DTI/Temps/Controller_Temperature"
#define BMSSTATE "BMS/Status/State"
#define BMSFAULT "BMS/Faults/Critical/#"
#define DCL "BMS/Commands/Max_DC_Current_Target"
#define CCL "BMS/Commands/Max_DC_Brake_Current_Target"
// Single wheel-button topic. The int payload is a 0-indexed button ordinal
// (0..9) matching the VCU-side `button_t` enum in
// Cerberus-2.0/Core/Inc/u_buttons.h. See raspberry_model.h for the full layout.
#define BUTTONID "Wheel/Buttons/button_id"
#define SEGMENTTEMP1 "BMS/Segment_Temp/1"
#define SEGMENTTEMP2 "BMS/Segment_Temp/2"
#define SEGMENTTEMP3 "BMS/Segment_Temp/3"
#define SEGMENTTEMP4 "BMS/Segment_Temp/4"
// #define SIDEBRBS "MPU/Fuses/SD_TO_BRB_FUSE_STAT" // OUTDATED - see
// EFUSE_SHUTDOWN_ENABLED and EFUSE_SHUTDOWN_FAULTED #define MPU
// "MPU/Shutdown/MC_STAT" // OUTDATED - see EFUSE_MC_ENABLED and
// EFUSE_MC_FAULTED
#define CRITICALFAULTS "BMS/Faults/Critical/#"
#define NONCRITICALFAULTS "BMS/Faults/Non-Critical/#"
#define LVVOLTAGE "VCU/LV/voltage"

/* VCU Topics */
#define TORQUEPOWER "VCU/CarState/torque_limit_percentage"
#define GFORCE "VCU/IMU/Accelerometer"
#define REGENPOWER "VCU/CarState/regen_limit"
#define MPH "VCU/CarState/speed"
#define STATUS                                                                 \
  "VCU/CarState/tsms" // '1' if TSMS is on, '0' if TSMS is off. Refers to the
                      // "Official" TSMS state (not the raw pin state, but the
                      // state stored in the debounced 'tsms' bool).
#define DIRECTION                                                              \
  "VCU/CarState/not_in_reverse" // '1' means that `cerberus_state.functional !=
                                // F_REVERSE`. '0' means that
                                // 'cerberus_state.functional == F_REVERSE'.
#define LAUNCHCONTROL                                                          \
  "VCU/CarState/launch_control" // '1' indicates that launch control is enabled.
                                // '0' indicates that launch control is
                                // disabled.
#define TRACTIONCONTROL                                                        \
  "VCU/CarState/traction_control" // '1' indicates that traction control is
                                  // enabled. '0' indicates that traction
                                  // control is disabled.
#define VCUFAULT                                                               \
  "VCU/Faults/#" // '1' indicates that the fault is active, '0' indicates that
                 // the fault is inactive.
#define HOMEBUTTON                                                             \
  "VCU/CarState/home_mode" // '1' indicates that VCU is in home mode. '0'
                           // indicates that VCU is not in home mode.
#define MODEINDEX                                                              \
  "VCU/CarState/nero_index" // '0' = OFF, '1' = PIT, '2' = REVERSE, '3' =
                            // PERFORMANCE, '4' = EFFICIENCY, '5' = GAMES, '6' =
                            // THEMES, '7' = EXIT
#define MOTORPOWER                                                             \
  "VCU/eFuses/MC/Current" // Current (A) flowing through motor controller eFuse.
#define FANPOWER                                                               \
  "VCU/eFuses/Fanbatt/Current" // Current (A) flowing through battbox fan eFuse.
#define EFUSE_SHUTDOWN_ENABLED                                                 \
  "VCU/eFuses/Shutdown/Enabled?" // '1' if the Shutdown eFuse is enabled (i.e.,
                                 // current is allowed to flow through it). '0'
                                 // if the Shutdown eFuse is disabled (i.e.,
                                 // current can't flow through it).
#define EFUSE_SHUTDOWN_FAULTED                                                 \
  "VCU/eFuses/Shutdown/Faulted?" // '1' if the Shutdown eFuse has been tripped
                                 // (i.e., it detected overcurrent and is now
                                 // blocking all current from flowing through
                                 // it). '0' if the Shutdown eFuse is not
                                 // faulted.
#define EFUSE_MC_ENABLED                                                       \
  "VCU/eFuses/MC/Enabled?" // '1' if the Motor Controller eFuse is enabled
                           // (i.e., current is allowed to flow through it). '0'
                           // if the Motor Controller eFuse is disabled (i.e.,
                           // current can't flow through it).
#define EFUSE_MC_FAULTED                                                       \
  "VCU/eFuses/MC/Faulted?" // '1' if the Motor Controller eFuse has been tripped
                           // (i.e., it detected overcurrent and is now blocking
                           // all current from flowing through it). '0' if the
                           // Motor Controller eFuse is not faulted.
#define SHUTDOWN_EFUSE "VCU/eFuses/Shutdown"
#define MC_EFUSE "VCU/eFuses/MC"
#define BMS "VCU/Shutdown/BMS_GPIO"
#define BSPD "VCU/Shutdown/BSPD_GPIO"
#define BOTS "VCU/Shutdown/BOTS_GPIO"
#define INERTIA "VCU/Shutdown/INERTIA_SW_GPIO"
#define CPBRB "VCU/Shutdown/CKPT_GPIO"
#define TSMS "VCU/CarState/tsms" // seems to be a duplicate of STATUS
#define IMD "VCU/Shutdown/IMD_GPIO"
#define HVDINTRLK "VCU/Shutdown/HVD_GPIO"
#define HVCNCTR "VCU/Shutdown/HV_C_GPIO"
#define LEDMODE "Control/Mode"

#endif // DATATYPENAMES_H
