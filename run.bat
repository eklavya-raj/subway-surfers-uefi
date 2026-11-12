@echo off
nasm -f bin game.asm -o game.efi || exit /b
if not exist esp\EFI\BOOT mkdir esp\EFI\BOOT
copy /y game.efi esp\EFI\BOOT\BOOTX64.EFI >nul
qemu-system-x86_64 -machine q35 -m 256 -drive if=pflash,format=raw,readonly=on,file="C:\Program Files\qemu\share\edk2-x86_64-code.fd" -drive format=raw,file=fat:rw:esp -net none
