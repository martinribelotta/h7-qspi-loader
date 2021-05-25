source [find interface/cmsis-dap.cfg]
transport select swd

source [find target/stm32h7x.cfg]

init

reset halt
load_image $firmware
verify_image $firmware
arm semihosting enable
arm semihosting_resexit enable

source [find burn-tools.tcl]

reset halt

doInitTarget

puts "Writing file $binary_file to memory"

set size [file size $binary_file]
echo "$binary_file size: $size"

set addr 0x00000000
set readed 0
while { $readed < $size } {
    set percent [expr ($readed * 100.0) / $size]
    puts [format "Programming at offset %d of %d (%3.02f%%)" $readed $size $percent]
    load_bin $binary_file $readed $MemBuffer $MemBufferSize
    if { [doErasePage $addr] != 0x00000000 } {
        puts "Error erasing at addr $addr"
        break
    }
    if { [doFlashCopy $addr $MemBufferSize] != 0x00000000 } {
        puts "Error erasing at addr $addr"
        break
    }
    if { [doFlashVerify $addr $MemBufferSize] != 0x00000000 } {
        puts "Error erasing at addr $addr"
        break
    }
    set addr [expr $addr + $MemBufferSize]
    set readed [expr $readed + $MemBufferSize]
}
puts "End of program"

if { [env RESET_AT_END false] } {
    puts "Resetting..."
    reset run
}

exit
