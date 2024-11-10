@echo off

cd %~dp0

copy ..\..\script.ver .
copy ..\..\tone.cfg .
copy ..\..\p11_code.bin .
copy ..\..\br28loader.bin .
copy ..\..\ota.bin .
copy ..\..\anc_coeff.bin .
copy ..\..\anc_gains.bin .

..\..\isd_download.exe ..\..\isd_config.ini -tonorflash -dev br28 -boot 0x120000 -div8 -wait 300 -uboot ..\..\uboot.boot -app ..\..\app.bin -res ..\..\cfg_tool.bin tone.cfg p11_code.bin ..\..\eq_cfg_hw.bin -uboot_compress -format all

@REM..\..\isd_download.exe ..\..\isd_config.ini -tonorflash -dev br34 -boot 0x20000 -div8 -wait 300 -uboot ..\..\uboot.boot -app ..\..\app.bin ..\..\cfg_tool.bin -res tone.cfg kws_command.bin p11_code.bin -uboot_compress

:: -format all
::-reboot 2500

@rem 删除临时文件-format all
if exist *.mp3 del *.mp3 
if exist *.PIX del *.PIX
if exist *.TAB del *.TAB
if exist *.res del *.res
if exist *.sty del *.sty

..\..\ufw_maker.exe -fw_to_ufw jl_isd.fw
copy jl_isd.ufw update.ufw
del jl_isd.ufw

@REM 生成配置文件升级文件
::ufw_maker.exe -chip AC800X %ADD_KEY% -output config.ufw -res bt_cfg.cfg

::IF EXIST jl_696x.bin del jl_696x.bin 

if exist script.ver del script.ver
if exist tone.cfg del tone.cfg
if exist p11_code.bin del p11_code.bin
if exist br28loader.bin del br28loader.bin
if exist anc_coeff.bin del anc_coeff.bin
if exist anc_gains.bin del anc_gains.bin

@rem 常用命令说明
@rem -format vm        //擦除VM 区域
@rem -format cfg       //擦除BT CFG 区域
@rem -format 0x3f0-2   //表示从第 0x3f0 个 sector 开始连续擦除 2 个 sector(第一个参数为16进制或10进制都可，第二个参数必须是10进制)

ping /n 2 127.1>null
IF EXIST null del null
