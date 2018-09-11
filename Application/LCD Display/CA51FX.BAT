SET C51INC=C:\Keil\C51\Inc;C:\Keil\C51\Inc\Cast
SET C51LIB=C:\Keil\C51\Lib
SET CPU_TYPE=R8051XC2
SET CPU_VENDOR=CAST, Inc.
SET UV2_TARGET=CA51FX
SET CPU_XTAL=0x016E3600
"C:\Keil\C51\BIN\C51.EXE" @.\hex\main.__i
"C:\Keil\C51\BIN\C51.EXE" @.\hex\uart.__i
"C:\Keil\C51\BIN\C51.EXE" @.\hex\LCD_DI~1.__I
"C:\Keil\C51\BIN\C51.EXE" @.\hex\lcd_led.__i
"C:\Keil\C51\BIN\A51.EXE" @.\hex\STARTUP._ia
"C:\Keil\C51\BIN\BL51.EXE" @.\hex\ca51fx.lnp
"C:\Keil\C51\BIN\OH51.EXE" ".\hex\ca51fx" 
